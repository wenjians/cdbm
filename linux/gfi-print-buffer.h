
/*******************************************************************************
**
** Description   : this file is for the GFI print buffer
**                 some times, application have many output to be done, e.g.
**                 loop the whole list, and need print then all, but is hard
**                 to print them to console or UI one time, which need long
**                 time and can not lock those data for that long
**                 so here just print those information to a temp buffer
**                 (output to memory is faster), and then output to console
**                 or UI slow.
**
**                 this node is like the RAM disk, which is linked by many small
**                 block, and it is dynamic increased, till reach the maximum
**                 size, which is defined when create
**
** Reference(s)  : TLD document (e.g. )
**
*****************************************************************************/

#ifndef GFI_PRINT_BUFFER_H
#define GFI_PRINT_BUFFER_H

/*****************************************************************************
**
**  1. HEADER (INCLUDE) SECTION
**
*****************************************************************************/


/*****************************************************************************
**
**  2. MACROS           (internal or external according to .h usage)
**
*****************************************************************************/


/* for length for prefix, including 
 *     cc,ss,time_stamp                     ==> total 31 bytes
 *     e.g.    01,10,FEB 02 04 00:45:05:607083
 *     ,(after time) "" (for log) \n (last) ==> total  4 bytes
 *                                           so total 35 bytes
 *     add level  8(e.g. CRITICAL) + 1 (,)
 */
#define GFI_LOG_PREFIX_FORMAT_LEN   (35+9)

#define GFI_PRINT_BUF_NAME_LEN      12

/* what's the maximum length for calling gfi_print_buf_add_format_short
 * in this case, will not allocate buffer, but use the local variable
 */
#define GFI_PRINT_BUF_SHORT_LEN     256

/* each print node block size, the size include management header size
 * NOTE: !!!!!!!!!   GFI_PRINT_BUF_NODE_SIZE inpact by the following
 *       a) maximum print buffer length supported, 
 *          2K is also used in uiPrint/print/sysprintf
 *       b) each time inter-board sync (used by GFI Log) packet size
 *       c) each log will ONLY in one print buffer node, space impact
 *       ==> so BE CAREFULL if you want to change the size
 */
#define GFI_PRINT_BUF_NODE_SIZE     (2048)  /* each node is 2K, BE CAREFUL when change */
#define GFI_PRINT_BUF_ALLOC_COUNT   (128)   /* each time allcoate 50 node */
#define GFI_PRINT_BUF_REAP_THRESH   (2)     /* when there are 2, reap one */

/* default total size for the print buffer, it can be changed specific
 * when calling the create function
 */
#define GFI_PRINT_BUF_DEFAULT_SIZE  (1024*1024)  /* default is 1 M */

/* define different flag for different applications, each flag in one bit 
 * NOTE: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *       DO NOT change the value, it will impact the GFI Log flag
 *       need change them together if REALLY need
 *
 * GFI_PRITN_BUF_FLAG_OVERWRITE: will overwrite the oldest when maximum size is reached
 * GFI_PRITN_BUF_FLAG_OVERSTOP : will stop write when maximum size is reached
 * GFI_PRINT_BUF_FLAG_ALIGNED  : for gfi print, there are two type exist:
 *      1, aligned, means each log/print only exist in one node, used for non-scm
 *         it is easy for sync to ACT-SCM, because sync is node based
 *      2, non-aligned, one log/print may exist in multiple node, used in SCM, memory save
 */
#define GFI_PRITN_BUF_FLAG_OVERWRITE    (0x0001)
#define GFI_PRITN_BUF_FLAG_OVERSTOP     (0x0002)
#define GFI_PRINT_BUF_FLAG_ALIGNED      (0x0004)

#define GFI_PRINT_BUF_DEFAULT_FLAG      (GFI_PRITN_BUF_FLAG_OVERWRITE)
                                         

/*
CF added @2012-12-5
    put a long string(more than 2K Bytes) to the gfi-print-buffer
*/
#define GFI_PRINT_BUF_STR_SEG_LEN       (GFI_PRINT_BUF_NODE_SIZE-32)
#define GFI_PRINTF_BUF_PRT_LONG_STRING(printBuffer, str, len)                       \
do{                                                                                 \
    int ls = len;                                                                   \
    const char *ss= (str);                                                          \
    int seg_len = 0;                                                                \
    while(ls > 0)                                                                   \
    {                                                                               \
        seg_len = ls > GFI_PRINT_BUF_STR_SEG_LEN ? GFI_PRINT_BUF_STR_SEG_LEN : ls;  \
        gfi_print_buf_add_string(printBuffer,ss,seg_len);                           \
        ss += GFI_PRINT_BUF_STR_SEG_LEN;                                            \
        ls -= seg_len;                                                              \
                                                                                    \
    }                                                                               \
}while(0);
/*****************************************************************************
**
**  3. DATA STRUCTURE DEFINITIONS
**
*****************************************************************************/


/****************************************************************************
 * the following is the data structure for the GFI print buffer
 * each print buffer is a list of node, with each of 2K buffer
 * the total buffer size can be set by application
 *
 * each print will only located in ONE print buffer node, which make it easy
 * when sync to ACT/STB SCM, or the log message will be mixed each other 

        create_list (all GFI print buffer list together for beffer management)
             |
             V 
          --------(node) --------        --------       --------      
prt-buf 1 | head |---->  | node | ---->  | node |---->  | node | --> null
          --------       --------        --------       --------
             |(next)
             V 
          --------       --------        --------
prt-buf 2 | head |---->  | node | ---->  | node |----> null
          --------       --------        --------     
             |
             V
          --------       --------        --------       --------     -------- 
prt-buf 3 | head |---->  | node | ---->  | node |---->  | node | --> | node | --> null
          --------       --------        --------       --------     --------
             |
             V
          ......
             |
             V
          --------       -------- 
prt-buf n | head |---->  | node | --> null
          --------       -------- 
             |
             V
            null
           
*******************************************************************************/

/* T_gfi_print_node_idx is used for iterator, when do iterator, 
 * a) the whole print buffer maybe cleared
 * b) current reading node maybe delete because of overwritten
 *
 * we can not check those via address in pointer, because
 * the buffer maybe freed and we can not check the content.
 * so we MUST have some check whether iterator's current node is still 
 * valid or not this idx is used to identify
 *
 * T_gfi_print_node_idx is created when adding to GFI print buffer,
 * and it is unnique for whole lifetime of gfi print buffer, 64bit
 * it is not global unique, because don't want to introduce another
 * lock for this node index update.
 * 
 * because GFI print buffer work in a list, 
 *       insert to last (largest), and remove from header (minimal)
 * when do iterator, we can easily check whether current iterator is in
 * gfi print buffer list or not by compare the index in iterator.
 *
 * iterator.idx < header.idx       
 *        the iterator's node is discard, so reinit
 * header.idx <= iterator.idx <= last.idx
 *        the iterator's node is valid
 * iterator.idx > last.idex
 *        something error, not possible
 *
 */

/* invalid < minimal, so when do iterator, it is easily detected by 
 * read_line or read_node, and will update it easily
 */
#define GFI_PRINT_NODE_IDX_INVALID      0
#define GFI_PRINT_NODE_IDX_MINIMAL      1

#define GFI_PRINT_BUFF_MAX_SIZE         (1024*1024)

/* for each print buffer */
typedef struct _T_gfi_print_buffer 
{
    uint32 write_offset;
    char buffer [GFI_PRINT_BUFF_MAX_SIZE];
} T_gfi_print_buffer;

typedef T_gfi_print_buffer* T_gfi_print_buffer_id;

#define T_GFI_PRINT_BUFF_SIZE   sizeof(T_gfi_print_buffer)

/*******************************************************************************
**
**  4. FUNCTION PROTOTYPES
**
*******************************************************************************/


/*****************************************************************************
**
**  5. EXTERNAL FUNCTION DEFINITIONS
**
*****************************************************************************/

/* init the print buffer module */
void gfi_print_buf_init(int phase);


/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_creat(char*  buf_name,  uint32 module_id,
 *                                  uint32 buf_flags, uint32 max_buf_size)
 *
 * Description: this will create the gfi print buffer, all the gfi print buffer
 *              MUST firstly created before usage.
 *
 * Parameter:
 *              buf_name    : slab name, must be static, no stack local address
 *              module_id   : which module the slab belong to, nomall creator
 *              buf_flags   : which feature the slab need to support, 
 *                            use GFI_PRINT_BUF_DEFAULT_FLAG
 *              max_buf_size: the maximum of the total print buffer size
 *                            actual size is dynamic increased 
 *
 * Return:      created pointer, NULL for failure
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
T_gfi_print_buffer_id gfi_print_buf_creat(char*  buf_name,  uint32 module_id,
                                          uint32 buf_flags, uint32 max_buf_size);


/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_destroy(T_gfi_print_buffer_id print_buffer_id)
 *
 * Description: this will destroy the print buffer, and release related memory
 *
 * Parameter:
 *              T_gfi_print_buffer_id: the print buffer address
 *
 * Return:      
 *         RC_OK                         : successful
 *         RC_GFI_PRINT_BUF_STILL_ENABLED: shoud be disabled before destroy
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
T_global_rc gfi_print_buf_destroy(T_gfi_print_buffer_id print_buffer_id);




/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_set_enable(T_gfi_print_buffer_id print_buffer_id, 
 *                                       uint32 enable_flag)
 *              gfi_print_buf_get_enable(T_gfi_print_buffer_id print_buffer_id)
 *
 * Description: these two functions enable/disable the print buffer, the next 
 *              is get the print flag
 *              
 *              when print buffer create, it is set to "enable"
 *               "enable" : can write information to print buffer
 *               "disable": can show result and destroy
 *
 * Parameter:
 *              T_gfi_print_buffer_id: the print buffer address
 *              enable_flag          : 1 -- means enable
 *                                     0 -- means disable
 *
 * Return:  NA     
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
T_global_rc gfi_print_buf_set_enable(T_gfi_print_buffer_id print_buffer_id, uint32 enable_flag);
uint32      gfi_print_buf_get_enable(T_gfi_print_buffer_id print_buffer_id);



/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_node_alloc()
 *              gfi_print_buf_node_free(void *tmp_buffer)
 *
 * Description: this is print buffer management supported, application can also
 *              used this interface to allocate buffer used for temp print buffer
 *
 * Parameter:
 *              tmp_buffer  : buffer to be freed
 *
 * Return:      
 *      gfi_print_buf_node_alloc: return pointer of buffercreated pointer, 
 *                                NULL for failure
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
void*  gfi_print_buf_node_alloc();
void   gfi_print_buf_node_free(void *tmp_buffer);



/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_add_format(T_gfi_print_buffer_id print_buffer_id, 
                                         const char* fmt, ...)
 *
 * Description: this function use the "printf" function like format, write to
 *              print buffer
 *
 * Parameter:
 *              T_gfi_print_buffer_id: the print buffer address
 *              fmt                  : the format is same as standard "printf"
 *
 * Return:  
 *              the length added to print buffer (not include terminal '\0')
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
int gfi_print_buf_add_format(T_gfi_print_buffer_id print_buffer_id, const char* fmt, ...);

/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_add_string(T_gfi_print_buffer_id print_buffer_id, 
 *                                       const char* p_string, uint32 str_len)
 *
 * Description: this function write a buffer to print buffer
 *
 * Parameter:
 *              print_buffer: the print buffer address
 *              p_string    : a const string
 *              str_len     : the length of string, *NOT* include terminal '\0'
 *
 * Return:  
 *              the length added to print buffer (not include terminal '\0')
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
int gfi_print_buf_add_string(T_gfi_print_buffer_id print_buffer_id, const char* p_string, uint32 str_len);



/*****************************************************************************
**  the following interface is about GFI print buffer iterator
*****************************************************************************/

typedef struct {
    T_gfi_print_buffer_id  print_buffer;    /* GFI Print buffer ID */
    uint32                 cur_read_off;    /* the read offset of current print node */
} T_gfi_print_buffer_iterator;


/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_it_init(T_gfi_print_buffer_id        print_buffer,  
 *                                    T_gfi_print_buffer_iterator* node_it)
 *
 * Description: init the iterator with print buffer, it MUST do this before
 *              any iterator actions
 *              
 *
 * Parameter:
 *              print_buffer: the print buffer address
 *              node_it     : address for the node interator
 *
 * Return:  NA     
 *
 * Function History:
 * 2012/Feb/3    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
void   gfi_print_buf_it_init(T_gfi_print_buffer_id print_buffer_id, 
                             T_gfi_print_buffer_iterator* it);

/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_it_init(T_gfi_print_buffer_iterator* node_it,  
 *                                    char* buffer, uint32 max_buf_size)
 *
 * Description: read one line from the print buffer, 
 *              in case the iterator is not valid, auto move to start position
 *              in case of not_aligned buffer, it will read string cross the
 *              print node
 *
 * Parameter:
 *              node_it     : node iterator, it MUST be init firstly before read
 *              buffer      : address used for store the result
 *              max_buf_size: maximum length of buffer, this function will
 *                            read at most max_buf_size-1, the last byte
 *                            is used to store terminated char '\0'
 *
 * Return:     string length, not including terminator '\0', like strlen
 *
 * Function History:
 * 2012/Feb/3    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
uint32 gfi_print_buf_it_read_line(T_gfi_print_buffer_iterator* node_it,
                                  char* buffer, uint32 max_buf_size);


#endif /* GFI_PRINT_BUFFER_H */

