
/*
 * CDBM database internal type definition and API definition
 */

#ifndef _CDBM_DB_H_
#define _CDBM_DB_H_

#include "gfi-list.h"
#include "gfi-lock.h"

/* it is the global data definition for CDBM module */
typedef struct T_cdbm_db_data
{
    /* the following is for CDBM transaction related data structure */
    T_gfi_list_head *trans_list;
    T_gfi_lock       trans_list_lock;

} T_cdbm_db_data;

extern T_cdbm_db_data g_cdbm_db;


#if 0
/* it is definition of CDBM configuration paraemter model */
typedef enum T_cdbm_cm_node_type
{
    CDBM_NODE_TYPE_INVALID      = 0,
    CDBM_NODE_TYPE_CONTAINER    = 1,
    CDBM_NODE_TYPE_LIST         = 2,
    CDBM_NODE_TYPE_LEAF_LIST    = 3,
    CDBM_NODE_TYPE_LEAF         = 4
} T_cdbm_cm_node_type;

typedef struct T_cdbm_cm_node
{
    T_cdbm_cm_node_type node_type;
};
#endif

void cdbm_lib_init(int phase);




#endif /* _CDBM_DB_H_ */
