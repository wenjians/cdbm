
/*******************************************************************************
**
** Description   : this file is used for implement the double list
**                 NOTES: there is NO multi-core or multi-thread protect in 
**                        this double list, application must make sure the
**                        usage of them are correct.
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
#include "aat-assert.h"

#include "gfi-list.h"


/*******************************************************************************
** 
**  2. MACROS           (file scope only)
**
*******************************************************************************/

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

/* use array to store all the list, which can easy to show all list
 * why not use list, but use array, 
 * array is more safe, which can scan in any case 
 */
static T_gfi_list_head gfi_list_head_array[GFI_LIST_MAX_COUNT];


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
/*
 *===========================================================================
 *                         T_gfi_list_assert
 *===========================================================================
 * Description:   Assert a list header and/or list entry.
 * Parameters:    head  - A pointer to the list header or NULL if unknown.
 *                entry - A pointer to the list entry or NULL if none.
 * Returns:       .
 *
 */
#ifdef GFI_LIST_DEBUG_ASSERT
static void gfi_list_assert(T_gfi_list_head *head, T_gfi_list *entry)
{
    /* Assert the list entry. */
    if(entry != NULL)
    {
        T_gfi_list *__head;
        /* If no head supplied, pick this entry's head. */
        if(head == NULL)
            __head = entry->head;
        else
            __head = __gfi_list_head(head);
        
        AAT_STD_ASSERT(entry->head != NULL);
        AAT_STD_ASSERT(entry->next != NULL);
        AAT_STD_ASSERT(entry->prev != NULL);
        AAT_STD_ASSERT(entry->head == __head);
        
        /* unlink_head() results in an entry being the head.
           if lone head-entry ignore below checks. */
        if(__head->size > 0)
        {
            AAT_STD_ASSERT(entry->head != entry);
            AAT_STD_ASSERT(entry->next != entry);
            AAT_STD_ASSERT(entry->prev != entry);
            AAT_STD_ASSERT(entry->prev->next == entry);
            AAT_STD_ASSERT(entry->next->prev == entry);
        }
    }

    /* Assert the list header. */
    if(head != NULL)
    {
        T_gfi_list *__head = __gfi_list_head(head);;
        AAT_STD_ASSERT(__head->head == __head);
        AAT_STD_ASSERT(__head->next != NULL);
        AAT_STD_ASSERT(__head->prev != NULL);
        AAT_STD_ASSERT(__head->size != 0 || (__head->next == __head && __head->prev == __head));

        if(__head->size > 0)
        {
            uint32     size;
            T_gfi_list *tmp, *prev;
            bool       found;

            size  = 0;
            found = entry ? FALSE : TRUE;

            for(prev = __head, tmp = __head->next; tmp != __head; prev = tmp, tmp = tmp->next)
            {
                AAT_STD_ASSERT(tmp->head == __head);
                AAT_STD_ASSERT(tmp->prev == prev);
                AAT_STD_ASSERT(tmp->prev->next == tmp);
                AAT_STD_ASSERT(tmp->next != tmp);
                size++;
                if(entry == tmp)
                    found = TRUE;
            }
            AAT_STD_ASSERT(size == __head->size);
            AAT_STD_ASSERT(found == TRUE);
        }
    }
}
#else
#define gfi_list_assert(xxhead, xxentry)   do { } while((0))
#endif


/*******************************************************************************
**
**  7. EXPORTED FUNCTIONS
**
*******************************************************************************/

/* this function is used for init the GFI shaping queue */
void gfi_list_mgr_init(int phase)
{
    uint32 i;
    
    if (1 == phase)
    {
        for (i = 0; i < GFI_LIST_MAX_COUNT; ++i)
            memset(&gfi_list_head_array[i], 0, sizeof(T_gfi_list_head));
    }
    else
    {
        /* do nothing for phase 2 */
    }
}

static inline void gfi_list_head_init(T_gfi_list_head *list_head, 
        char *name, uint32 ret_addr)
{
#define GFI_ANONY_LIST_NAME "anony"
    if (NULL == name)
        name = GFI_ANONY_LIST_NAME;

    AAT_STD_ASSERT(NULL != list_head);
    AAT_STD_ASSERT(0 == list_head->list_create_flag);
    AAT_STD_ASSERT(NULL != name);
    AAT_STD_ASSERT(strlen(name) + 1 < GFI_LIST_NAME_LEN);

    //AAT_ASYNC_CALL_ARG_ASSERT_RA(list_head->list_name, ret_addr);

    memset(list_head, 0, sizeof(T_gfi_list_head));

    gfi_list_init(&list_head->list);
    list_head->list_name             = name;
    list_head->list_index            = 0; /* 0 by default */
    list_head->list_create_flag      = 1;
    list_head->list_magic            = (uint32)list_head;
    list_head->list_create_ra        = ret_addr;

#ifdef GFI_LIST_DEBUG
    list_head->list_max_size;        = 0;
#endif
}

int gfi_list_create2(T_gfi_list_head *list_head, char *name)
{
    uint32 ret_addr = gfi_return_address();

    gfi_list_head_init(list_head, name, ret_addr);

    return 0;
}

T_gfi_list_head *gfi_list_create(char *name)
{
    uint32  ret_addr = gfi_return_address();
    int     index    = -1;

    T_gfi_list_head* list_head;

    AAT_STD_ASSERT(name!=NULL);
    AAT_STD_ASSERT(strlen(name) + 1 < GFI_LIST_NAME_LEN);
    
    for (index = 0; index < GFI_LIST_MAX_COUNT; ++index)
    {
        if (gfi_list_head_array[index].list_create_flag == 0)
            break;
    }
    /* if creat failure, can be found in start up, NOT leave to onsite
     * so assert is better choice
     */
    AAT_STD_ASSERT(index < GFI_LIST_MAX_COUNT);

    /* not need actually */
    if (index >= GFI_LIST_MAX_COUNT)
        return NULL;

    list_head = &(gfi_list_head_array[index]);

    gfi_list_head_init(list_head, name, ret_addr);
    list_head->list_index            = index;

    return list_head;
}

void gfi_list_destroy(T_gfi_list_head *head)
{
    gfi_list_assert(head, NULL);
   
    head->list_create_flag = 0;
}


/*
 *===========================================================================
 *                    T_gfi_list_init
 *===========================================================================
 * Description:   Initialize a list entry.
 * Parameters:    entry - The uninitialized list entry pointer.
 * Returns:       .
 *
 */
void gfi_list_init(T_gfi_list *entry)
{
    entry->next = entry;
    entry->prev = entry;
    entry->head = entry;
    entry->size = 0;
}


/*
 *===========================================================================
 *                         gfi_list_first
 *===========================================================================
 * Description:   Get the first list entry in list 'head'.
 * Parameters:    head - Pointer to the list head.
 * Returns:       Pointer to the first entry in the list or NULL if list
 *                is empty.
 *
 */
T_gfi_list* gfi_list_first(T_gfi_list_head *head)
{
    T_gfi_list *__head;
    gfi_list_assert(head, NULL);
    __head = __gfi_list_head(head);

    if(__head->next != __head)
        return __head->next;
    else
        return NULL;
}


/*
 *===========================================================================
 *                         gfi_list_last
 *===========================================================================
 * Description:   Get the last list entry in list 'head'.
 * Parameters:    head - Pointer to the list head.
 * Returns:       Pointer to the last entry in the list or NULL if list
 *                is empty.
 *
 */
T_gfi_list* gfi_list_last(T_gfi_list_head *head)
{
    T_gfi_list *__head;
    gfi_list_assert(head, NULL);
    __head = __gfi_list_head(head);

    if(__head->prev != __head)
        return __head->prev;
    else
        return NULL;
}


/*
 *===========================================================================
 *                         gfi_list_next
 *===========================================================================
 * Description:   Get the next list entry following 'entry'.
 * Parameters:    entry - Pointer to the list entry.
 * Returns:       Pointer to the next entry in the list or NULL if none.
 *
 */
T_gfi_list* gfi_list_next(T_gfi_list *entry)
{
    gfi_list_assert(NULL, entry);

    if(entry->next != entry->head)
        return entry->next;
    else
        return NULL;
}


/*
 *===========================================================================
 *                         gfi_list_prev
 *===========================================================================
 * Description:   Get the previous list entry to 'entry'.
 * Parameters:    entry - Pointer to the list entry.
 * Returns:       Pointer to the previous entry or NULL if none.
 *
 */
T_gfi_list* gfi_list_prev(T_gfi_list *entry)
{
    gfi_list_assert(NULL, entry);

    if(entry->prev != entry->head)
        return entry->prev;
    else
        return NULL;
}


/*
 *===========================================================================
 *                    gfi_list_insert_first
 *===========================================================================
 * Description:   Insert list 'entry' first in the list 'head'.
 * Parameters:    head  - Pointer to the list header to insert 'entry' in.
 *                entry - Pointer to the list entry to insert first.
 * Returns:       .
 *
 */
void gfi_list_insert_first(T_gfi_list_head *head, T_gfi_list *entry)
{
    T_gfi_list *__head = __gfi_list_head(head);
#ifdef GFI_LIST_DEBUG_ASSERT
    {
        T_gfi_list *tmp;
        for(tmp = __head->next; tmp != __head; tmp = tmp->next)
            AAT_FMT_ASSERT(entry != tmp, ("%s(): entry(%08x) already in list\n", __func__, entry));
    }
#endif

    entry->next = __head->next;
    entry->prev = __head;
    entry->next->prev = entry;
    entry->prev->next = entry;

    entry->head = __head;
    __head->size++;

    gfi_list_assert(head, entry);
}


/*
 *===========================================================================
 *                    gfi_list_insert_last
 *===========================================================================
 * Description:   Insert list 'entry' last in the list 'head'.
 * Parameters:    head  - Pointer to the list header to insert 'entry' in.
 *                entry - Pointer to the list entry to insert last.
 * Returns:       .
 *
 */
void gfi_list_insert_last(T_gfi_list_head *head, T_gfi_list *entry)
{
    T_gfi_list *__head = __gfi_list_head(head);
#ifdef GFI_LIST_DEBUG_ASSERT
    {
        T_gfi_list *tmp;
        for(tmp = __head->next; tmp != __head; tmp = tmp->next)
            AAT_FMT_ASSERT(entry != tmp, ("%s(): entry(%08x) already in list\n", __func__, entry));
    }
#endif

    entry->prev = __head->prev;
    entry->next = __head;
    entry->next->prev = entry;
    entry->prev->next = entry;

    entry->head = __head;
    __head->size++;

    gfi_list_assert(head, entry);
}


/*
 *===========================================================================
 *                    gfi_list_insert_before
 *===========================================================================
 * Description:   Insert the new list 'entry' before the entry 'inlist' which
 *                is already in a list.
 * Parameters:    head  - Pointer to the list header to insert 'entry' in.
 *                inlist - Pointer to the list entry already in a list.
 *                entry  - Pointer to the new list entry to insert before
 *                         'inlist'.
 * Returns:       .
 *
 */
void gfi_list_insert_before(T_gfi_list_head *head, T_gfi_list *inlist, T_gfi_list *entry)
{
    T_gfi_list *__head = __gfi_list_head(head);
#ifdef GFI_LIST_DEBUG_ASSERT
    {
        T_gfi_list *tmp;
        for(tmp = __head->next; tmp != __head; tmp = tmp->next)
            AAT_FMT_ASSERT(entry != tmp, ("%s(): entry(%08x) already in list\n", __func__, entry));
    }
#endif

    entry->next = inlist;
    entry->prev = inlist->prev;

    entry->next->prev = entry;
    entry->prev->next = entry;

    entry->head = __head;
    entry->head->size++;

    gfi_list_assert(NULL, entry);
}


/*
 *===========================================================================
 *                    gfi_list_insert_after
 *===========================================================================
 * Description:   Insert the new list 'entry' after the entry 'inlist' which
 *                is already in a list.
 * Parameters:    head  - Pointer to the list header to insert 'entry' in.
 *                inlist - Pointer to the list entry already in a list.
 *                entry  - Pointer to the new list entry to insert after
 *                         'inlist'.
 * Returns:       .
 *
 */
void gfi_list_insert_after(T_gfi_list_head *head, T_gfi_list *inlist, T_gfi_list *entry)
{
    T_gfi_list *__head = __gfi_list_head(head);
#ifdef GFI_LIST_DEBUG_ASSERT
    {
        T_gfi_list *tmp;
        for(tmp = __head->next; tmp != __head; tmp = tmp->next)
            AAT_FMT_ASSERT(entry != tmp, ("%s(): entry(%08x) already in list\n", __func__, entry));
    }
#endif

    entry->prev = inlist;
    entry->next = inlist->next;

    entry->next->prev = entry;
    entry->prev->next = entry;

    entry->head = __head;
    entry->head->size++;

    gfi_list_assert(NULL, entry);
}


/*
 *===========================================================================
 *                    gfi_list_remove
 *===========================================================================
 * Description:   Remove list entry pointer to by 'entry' from its list.
 * Parameters:    head  - Pointer to the list header where 'entry' is removed.
 *                entry - Pointer to the list entry to remove.
 * Returns:       .
 *
 */
void gfi_list_remove(T_gfi_list_head *head, T_gfi_list *entry)
{
    T_gfi_list *__head = __gfi_list_head(head);
    gfi_list_assert(NULL, entry);
    AAT_STD_ASSERT(__head->size > 0);
    AAT_STD_ASSERT(__head == entry->head);

    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;

    __head->size--;
    entry->head = NULL;
}


/*
 *===========================================================================
 *                    gfi_list_remove_node
 *===========================================================================
 * Description:   Remove list entry pointer to by 'entry' from its list.
 * Parameters:    entry - Pointer to the list entry to remove.
 * Returns:       .
 *
 */
void gfi_list_remove_node(T_gfi_list *entry)
{
    gfi_list_assert(NULL, entry);
    AAT_STD_ASSERT(entry->head->size > 0);

    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;

    entry->head->size--;
    entry->head = NULL;
}



/*******************************************************************************
**  END OF FILE
*******************************************************************************/
/* vi: set number ts=8 sw=4 expandtab sts=4 noet: */

