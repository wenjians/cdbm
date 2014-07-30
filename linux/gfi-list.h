
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

#ifndef GFI_LIST_H
#define GFI_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

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

/* for some debug check */
/*#define GFI_LIST_DEBUG_ASSERT   1*/


#define GFI_LIST_MAX_COUNT      32  /* Maximum list count */
#define GFI_LIST_NAME_LEN       32

/*****************************************************************************
**
**  3. DATA STRUCTURE DEFINITIONS
**
*****************************************************************************/

/* list header and list node data structure definition 
 * when use them as node, they should define in the header for the node define
 *   e.g. 
 *   typedef struct _node_define
 *       T_gfi_list list_node;
 *       ... // other data field
 *   } T_node_define.
 *   
 *   for the header definition, it is free to declar in any place
 */
typedef struct gfi_list_struct
{
    struct gfi_list_struct      *next;  /* point to the next node */
    struct gfi_list_struct      *prev;  /* point to the previous node */
    struct gfi_list_struct      *head;  /* point to the header of this list */
    uint32          size;   /* how many nodes exist in this list */
} T_gfi_list;


/* all the created list are managed in one array, which make it easy
 * to management, e.g. show all list info
 */
typedef struct gfi_list_head_struct {
    T_gfi_list  list;           /* point to the first list entry */
    char*       list_name;
    uint32      list_index;
    uint32      list_create_flag;
    uint32      list_create_ra; 
    uint32      list_magic;     /* magic for validate, pointer */
#ifdef GFI_LIST_DEBUG
    uint32      list_max_size;  /* how much maximum the list ever reach */
#endif
} T_gfi_list_head;


void gfi_list_mgr_init(int phase);


/**
 * __gfi_list_for_each  -       iterate over a list
 * @pos:        the &struct list_head to use as a loop cursor.
 * @xxhead:       the head for your list.
 *
 * This variant differs from list_for_each() in that it's the
 * simplest possible list iteration code, no prefetching is done.
 * Use this for code that knows the list to be very short (empty
 * or 1 entry) most of the time.
 */
#define __gfi_list_for_each(pos, xxhead) \
        for (pos = (xxhead)->next; pos != (xxhead); pos = pos->next)


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
    
void gfi_list_mgr_init(int phase);

/*
 *===========================================================================
 *                    gfi_list_create
 *===========================================================================
 * Description:   Create a list header. 
 *                The created list is globally managed, 
 *                e.g. can be found via "diag view gfi lib list"
 * Parameters:    name - list name.
 * Returns:       .
 *
 */
T_gfi_list_head *gfi_list_create(char *name);

/*
 *===========================================================================
 *                    gfi_list_create_annoy
 *===========================================================================
 * Description:   Init a list header. 
 *                The created list is managed by application itself.
 *                They can not be found via "diag view gfi lib list".
 * Parameters:    list_head, name - list name
 * Returns:       0 : success; -1 : failure
 *
 */
int gfi_list_create2(T_gfi_list_head *list_head, char *name);

void gfi_list_destroy(T_gfi_list_head *head);

/*
 *===========================================================================
 *                    gfi_list_init
 *===========================================================================
 * Description:   Init a list header.
 * Parameters:    entry - list entry.
 * Returns:       .
 *
 */
void gfi_list_init(T_gfi_list *entry);

static inline void gfi_list_init_head(T_gfi_list_head *list_head)
{
    gfi_list_init(&list_head->list);
}

static inline T_gfi_list_head* gfi_list_get_head(T_gfi_list *entry)
{
    return   (T_gfi_list_head*)(entry->head);
}


    
/*
 *===========================================================================
 *                         gfi_list_first
 *===========================================================================
 * Description:   Get the first list entry in list 'head'.
 * Parameters:    head - Pointer to the list header.
 * Returns:       Pointer to the first entry in the list or NULL if list
 *                is empty.
 *
 */
T_gfi_list* gfi_list_first(T_gfi_list_head *head);

#define GFI_LIST_FIRST(xxhead)   (void *)gfi_list_first(xxhead)


/*
 *===========================================================================
 *                         gfi_list_last
 *===========================================================================
 * Description:   Get the last list entry in list 'head'.
 * Parameters:    head - Pointer to the list header.
 * Returns:       Pointer to the last entry in the list or NULL if list
 *                is empty.
 *
 */
T_gfi_list *gfi_list_last(T_gfi_list_head *head);

#define GFI_LIST_LAST(xxhead)   (void *)gfi_list_last(xxhead)


/*
 *===========================================================================
 *                         gfi_list_next
 *===========================================================================
 * Description:   Get the next list entry following 'entry'.
 * Parameters:    head - Pointer to the list entry.
 * Returns:       Pointer to the next entry in the list or NULL if none.
 *
 */
T_gfi_list *gfi_list_next(T_gfi_list *entry);

#define GFI_LIST_NEXT(xxentry)   (void *)gfi_list_next(xxentry)


/*
 *===========================================================================
 *                         gfi_list_prev
 *===========================================================================
 * Description:   Get the previous list entry to 'entry'.
 * Parameters:    head - Pointer to the list entry.
 * Returns:       Pointer to the previous entry or NULL if none.
 *
 */
T_gfi_list *gfi_list_prev(T_gfi_list *entry);

#define GFI_LIST_PREV(xxentry)   (void *)gfi_list_prev(xxentry)


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
void gfi_list_insert_first(T_gfi_list_head *head, T_gfi_list *entry);


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
void gfi_list_insert_last(T_gfi_list_head *head, T_gfi_list *entry);


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
void gfi_list_insert_before(T_gfi_list_head *head, T_gfi_list *inlist, T_gfi_list *entry);


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
void gfi_list_insert_after(T_gfi_list_head *head, T_gfi_list *inlist, T_gfi_list *entry);


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
void gfi_list_remove(T_gfi_list_head *head, T_gfi_list *entry);

/*
 *===========================================================================
 *                    gfi_list_remove_node
 *===========================================================================
 * Description:   Remove list entry pointer to by 'entry' from its list.
 * Parameters:    entry - Pointer to the list entry to remove.
 * Returns:       .
 *
 */
void gfi_list_remove_node(T_gfi_list *entry);

#if 0
/*
 *===========================================================================
 *                    gfi_list_unlink_head
 *===========================================================================
 * Description:   Unlink 'head' and make the first entry the new 'head'.
 * Parameters:    head  - Pointer to the list header to unlink.
 * Returns:       Pointer to the new head or NULL if list was empty.
 *
 */
T_gfi_list *gfi_list_unlink_head(T_gfi_list *head);
#endif
    
/*
 *===========================================================================
 *                       gfi_list helpers
 *===========================================================================
 */

/* We currently do not have prefetch. */
#ifndef prefetch
#define prefetch(addr) ((void) addr)
#endif

/* Since T_gfi_list_head also contains management data, this macro is used to
 * retrieve pure list head data (T_gfi_list *) from T_gfi_list_head
 * Note: <xxhead> should be valid
 *       This macro is only used internally */
#define __gfi_list_head(xxhead) (&(xxhead)->list)

static inline int gfi_list_is_entry_in_list(T_gfi_list_head*  head, T_gfi_list* entry)
{
    return (entry->head == __gfi_list_head(head));
}

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/**
 * gfi_list_entry - get the struct for this entry
 * @ptr:        the &T_gfi_list pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the T_gfi_list within the struct.
 */
#define gfi_list_entry(ptr, type, member) \
        container_of(ptr, type, member)

/**
 * gfi_list_prepare_entry - prepare a pos entry for use in gfi_list_foreach_entry_continue()
 * @pos:        the type * to use as a start point
 * @xxhead:     the head of the list
 * @member:     the name of the T_gfi_list within the struct.
 *
 * Prepares a pos entry for use as a start point in gfi_list_foreach_entry_continue().
 */
#define gfi_list_prepare_entry(pos, xxhead, member) \
        ((pos) ? : gfi_list_entry(__gfi_list_head(xxhead), typeof(*pos), member))

/**
 * gfi_list_foreach - iterate over a list
 * @pos:        the &T_gfi_list to use as a loop cursor.
 * @xxhead:     the head for your list.
 */
#define gfi_list_foreach(pos, xxhead) \
        for (pos = __gfi_list_head(xxhead)->next;                       \
                prefetch(pos->next), pos != __gfi_list_head(xxhead);    \
                pos = pos->next)

/**
 * gfi_list_foreach_entry - iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @xxhead:     the head for your list.
 * @member:     the name of the T_gfi_list within the struct.
 */
#define gfi_list_foreach_entry(pos, xxhead, member)                                 \
    for (pos = gfi_list_entry(__gfi_list_head(xxhead)->next, typeof(*pos), member); \
         prefetch(pos->member.next), &pos->member != __gfi_list_head(xxhead);       \
         pos = gfi_list_entry(pos->member.next, typeof(*pos), member))

/**
 * gfi_list_foreach_entry_reverse - iterate backwards over list of given type.
 * @pos:        the type * to use as a loop cursor.
 * @xxhead:     the head for your list.
 * @member:     the name of the T_gfi_list within the struct.
 */
#define gfi_list_foreach_entry_reverse(pos, xxhead, member)                             \
        for (pos = gfi_list_entry(__gfi_list_head(xxhead)->prev, typeof(*pos), member); \
             prefetch(pos->member.prev), &pos->member != __gfi_list_head(xxhead);       \
             pos = gfi_list_entry(pos->member.prev, typeof(*pos), member))

/**
 * gfi_list_foreach_entry_from - iterate over list of given type from the current point
 * @pos:        the type * to use as a loop cursor.
 * @xxhead:     the head for your list.
 * @member:     the name of the T_gfi_list within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define gfi_list_foreach_entry_from(pos, xxhead, member)      \
        for (; &pos->member != __gfi_list_head(xxhead);       \
             pos = gfi_list_entry(pos->member.next, typeof(*pos), member))

/**
 * gfi_list_foreach_entry_continue - continue iteration over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @xxhead:     the head for your list.
 * @member:     the name of the T_gfi_list within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define gfi_list_foreach_entry_continue(pos, xxhead, member)                        \
        for (pos = gfi_list_entry(pos->member.next, typeof(*pos), member);          \
             prefetch(pos->member.next), &pos->member != __gfi_list_head(xxhead);   \
             pos = gfi_list_entry(pos->member.next, typeof(*pos), member))

/**
 * gfi_list_foreach_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @xxhead:     the head for your list.
 * @member:     the name of the T_gfi_list within the struct.
 */
#define gfi_list_foreach_entry_safe(pos, n, xxhead, member)                             \
        for (pos = gfi_list_entry(__gfi_list_head(xxhead)->next, typeof(*pos), member), \
                n = gfi_list_entry(pos->member.next, typeof(*pos), member);             \
             &pos->member != __gfi_list_head(xxhead);                                   \
             pos = n, n = gfi_list_entry(n->member.next, typeof(*n), member))

/**
 * gfi_list_foreach_entry_safe_reverse
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @xxhead:     the head for your list.
 * @member:     the name of the T_gfi_list within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define gfi_list_foreach_entry_safe_reverse(pos, n, xxhead, member)                     \
        for (pos = gfi_list_entry(__gfi_list_head(xxhead)->prev, typeof(*pos), member), \
                n = gfi_list_entry(pos->member.prev, typeof(*pos), member);             \
             &pos->member != __gfi_list_head(xxhead);                                   \
             pos = n, n = gfi_list_entry(n->member.prev, typeof(*n), member))

/**
 * gfi_list_foreach_entry_safe_from
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @xxhead:     the head for your list.
 * @member:     the name of the list_struct within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define gfi_list_foreach_entry_safe_from(pos, n, xxhead, member)                \
        for (n = gfi_list_entry(pos->member.next, typeof(*pos), member);        \
             &pos->member != __gfi_list_head(xxhead);                           \
             pos = n, n = gfi_list_entry(n->member.next, typeof(*n), member))

/**
 * gfi_list_foreach_entry_safe_continue
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @xxhead:     the xxhead for your list.
 * @member:     the name of the list_struct within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define gfi_list_foreach_entry_safe_continue(pos, n, xxhead, member)            \
        for (pos = gfi_list_entry(pos->member.next, typeof(*pos), member),      \
                n = gfi_list_entry(pos->member.next, typeof(*pos), member);     \
             &pos->member != __gfi_list_head(xxhead);                           \
             pos = n, n = gfi_list_entry(n->member.next, typeof(*n), member))

/**
 * gfi_list_size - get size of the list
 * @xxhead:     the head for your list.
 */
#define gfi_list_size(xxhead) (__gfi_list_head(xxhead)->head->size)

/**
 * gfi_list_is_empty - check if the list is empty
 * @xxhead:     the head for your list.
 */
#define gfi_list_is_empty(xxhead) (gfi_list_size((xxhead)) == 0)



/*
****************************************************************************
* 6                    FUNCTIONS
****************************************************************************
*/


#ifdef __cplusplus
}
#endif

#endif /* GFI_LIST_H */
/* vi: set number ts=8 sw=4 expandtab sts=4 noet: */

