
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
******************************************************************************/


/***************************** COMPILE DIRECTIVES ******************************
**
** 0. if have please describe
**  
*******************************************************************************/

/*******************************************************************************
**
**  1. HEADER (INCLUDE) SECTION
**
*******************************************************************************/
#include "aat-incl.h"
#include "return_codes.h"

#include "gfi-print-buffer.h"


/*******************************************************************************
** 
**  2. MACROS           (file scope only)
**
*******************************************************************************/
#if !defined(MIN)
#define    MIN(a,b) (((a)<(b))?(a):(b))
#endif

/*#define GFI_PRINT_BUF_DEBUG(x)  uiPrintf x */

#define GFI_PRINT_BUF_DEBUG(x)  do {} while(0)



/*******************************************************************************
**
**  3. TYPE DEFINITIONS  (file scope only)
**
*******************************************************************************/

/*******************************************************************************
**
**  4. GLOBAL DATA 
**
*******************************************************************************/




/*******************************************************************************
**
**  5. LOCAL FUNCTION PROTOTYPES (file scope only)
**
*******************************************************************************/

/*******************************************************************************
**
**  6. LOCAL FUNCTIONS   (file scope only, must be static)
**
*******************************************************************************/



void gfi_print_buf_pre_check(T_gfi_print_buffer_id print_buffer)
{

}


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
void* gfi_print_buf_node_alloc()
{
    return malloc (GFI_PRINT_BUF_NODE_SIZE);
}

void gfi_print_buf_node_free(void *tmp_buffer)
{
    free (tmp_buffer);
}



/*******************************************************************************
**
**  7. EXPORTED FUNCTIONS
**
*******************************************************************************/

/* init the print buffer module */
void gfi_print_buf_init(int phase)
{

}



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
                                          uint32 buf_flags, uint32 max_buf_size)
{

    T_gfi_print_buffer_id print_buffer = malloc(T_GFI_PRINT_BUFF_SIZE);
    print_buffer->write_offset = 0;

    return print_buffer;
}


/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_destroy(T_gfi_print_buffer_id print_buffer)
 *
 * Description: this will destroy the print buffer, and release related memory
 *
 * Parameter:
 *              print_buffer: the print buffer address
 *
 * Return:      
 *         RC_OK                         : successful
 *         RC_GFI_PRINT_BUF_STILL_ENABLED: shoud be disabled before destroy
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
T_global_rc gfi_print_buf_destroy(T_gfi_print_buffer_id print_buffer)
{

    free (print_buffer);
    return RC_OK;
}




/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_set_enable(T_gfi_print_buffer_id print_buffer, 
 *                                       uint32 enable_flag)
 *              gfi_print_buf_get_enable(T_gfi_print_buffer_id print_buffer)
 *
 * Description: these two functions enable/disable the print buffer, the next 
 *              is get the print flag
 *              
 *              when print buffer create, it is set to "enable"
 *               "enable" : can write information to print buffer
 *               "disable": can show result and destroy
 *
 * Parameter:
 *              print_buffer: the print buffer address
 *              enable_flag : 1 -- means enable
 *                            0 -- means disable
 *
 * Return:  NA     
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/
T_global_rc gfi_print_buf_set_enable(T_gfi_print_buffer_id print_buffer, uint32 enable_flag)
{


    return RC_OK;
}


static inline int gfi_print_buf_is_enabled(T_gfi_print_buffer *print_buffer)
{
    return 1; //(1 == print_buffer->pb_enable_flag);
}





/*----------------------------------------------------------------------------
 * Function:    gfi_print_buf_add_format(T_gfi_print_buffer_id print_buffer, 
 *                                       const char* fmt, ...)
 *              gfi_print_buf_add_format_short(T_gfi_print_buffer_id print_buffer, 
 *                                             const char* fmt, ...)
 *
 * Description: this function use the "printf" function like format, write to
 *              print buffer
 *
 *              gfi_print_buf_add_format_short is used for short (<256)
 *              in this case, use local variable for output (high performance)
 *
 * Parameter:
 *              print_buffer: the print buffer address
 *              fmt         : the format is same as standard "printf"
 *
 * Return:  
 *              the length added to print buffer (not include terminal '\0')
 *
 * Function History:
 * 2010/7/27    Created      Sun Wenjian
 *
 *--------------------------------------------------------------------------*/

int gfi_print_buf_add_format(T_gfi_print_buffer_id print_buffer, const char* fmt, ...)
{
    char    *ptemp_str;
    va_list ap = NULL;
    uint32  len;

    gfi_print_buf_pre_check(print_buffer);

    /* must enable first */
    if (!gfi_print_buf_is_enabled(print_buffer))
        return 0;

    ptemp_str = gfi_print_buf_node_alloc();
    if (ptemp_str == NULL)
        return 0;

    va_start(ap, fmt);
    len = vsprintf(ptemp_str, fmt, ap);
    va_end (ap);
    
    /* each slab or node, the maximum is GFI_PRINT_BUF_NODE_SIZE,
     * each time should NEVER larger than this size
     */
    AAT_STD_ASSERT(len < GFI_PRINT_BUF_NODE_SIZE);

    len = gfi_print_buf_add_string(print_buffer, ptemp_str, len);

    gfi_print_buf_node_free(ptemp_str);

    /* LYP add for R52 Klockwork fix GW1_00241582 */
    ptemp_str = NULL;
    
    return (len);
}

int gfi_print_buf_add_string(T_gfi_print_buffer_id print_buffer, const char* p_string, uint32 str_len)
{
    AAT_STD_ASSERT(p_string != NULL);
    AAT_STD_ASSERT((print_buffer->write_offset + str_len) < (T_GFI_PRINT_BUFF_SIZE-1));

    gfi_print_buf_pre_check(print_buffer);

    /* must enable first */
    if (!gfi_print_buf_is_enabled(print_buffer))
        return 0;

    memcpy(&print_buffer->buffer[print_buffer->write_offset], p_string, str_len);
    print_buffer->write_offset += str_len;
    print_buffer->buffer[print_buffer->write_offset] = '\0';

    GFI_PRINT_BUF_DEBUG(("gfi_print_buf_add_string: str_len=%u\n", str_len));


    return str_len;
}

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
void   gfi_print_buf_it_init(T_gfi_print_buffer_id        print_buffer, 
                             T_gfi_print_buffer_iterator* node_it)
{
    AAT_STD_ASSERT(node_it != NULL);
    gfi_print_buf_pre_check(print_buffer);
    
    memset(node_it, 0, sizeof(T_gfi_print_buffer_iterator));

    node_it->print_buffer = print_buffer;
    node_it->cur_read_off = 0;
}



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
                                  char* buffer, uint32 max_buf_size)
{
    uint32             cur_read_len=0, max_read_size;
    char               *p_src, *p_dst;
    T_gfi_print_buffer *print_buffer;

    AAT_STD_ASSERT(node_it != NULL);
    AAT_STD_ASSERT(buffer !=NULL);
    AAT_STD_ASSERT(max_buf_size >= 1);
    
    print_buffer = node_it->print_buffer;   
    gfi_print_buf_pre_check(print_buffer);


    /* MUST quit if there is nothing exist, in case of NO node exist,
     * first/last node will return NULL, and gfi_print_buf_it_update
     * will failure, add check in gfi_lock, because make sure GFI
     * print buffer operation are atomic
     */
    GFI_PRINT_BUF_DEBUG(("gfi_print_buf_it_read_line: read=%u, write=%u\n",
            node_it->cur_read_off, print_buffer->write_offset));

    if (node_it->cur_read_off >= print_buffer->write_offset)
        return 0;

    p_src = node_it->print_buffer->buffer + node_it->cur_read_off;
    p_dst = buffer;
    max_read_size = print_buffer->write_offset - node_it->cur_read_off;
    if (max_read_size > max_buf_size-1)
        max_read_size = max_buf_size-1;


    cur_read_len = 0;
    while ((*p_src != '\n') && (*p_src != '\0') && (cur_read_len < max_read_size))
    {
        *p_dst++ = *p_src++;
        cur_read_len++;
    }
    
    if ((*p_src == '\n') && (cur_read_len < max_read_size))
    {
        *p_dst++ = *p_src++;
        cur_read_len++;
    }
    *p_dst = '\0';
    node_it->cur_read_off += cur_read_len;
    
    
    return cur_read_len;
}



int gfi_print_buf_output(T_gfi_print_buffer_id print_buffer, char* outbuffer, uint32 max_len)
{
    uint32   line_len=0, total_write_len=0;
    char     *ptemp_str;
    T_gfi_print_buffer_iterator buf_it;

    AAT_STD_ASSERT(print_buffer!=NULL);

    ptemp_str = gfi_print_buf_node_alloc();
    if (ptemp_str == NULL)
        return 0;

    gfi_print_buf_it_init(print_buffer, &buf_it);
    for (total_write_len=0; total_write_len<max_len-1; total_write_len += line_len)
    {
        line_len = gfi_print_buf_it_read_line(&buf_it, ptemp_str, GFI_PRINT_BUF_NODE_SIZE);
        if (line_len == 0)
            break;

        if (total_write_len + line_len >= max_len-1)
            break;

        memcpy(outbuffer, ptemp_str, line_len);
        outbuffer += line_len;

    }

    *ptemp_str = '\0';
    gfi_print_buf_node_free(ptemp_str);

    return total_write_len;
}



/*******************************************************************************
**  END OF FILE
*******************************************************************************/
