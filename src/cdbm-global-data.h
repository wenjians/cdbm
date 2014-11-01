
/*
 * CDBM database internal type definition and API definition
 */

#ifndef _CDBM_GLOBAL_DATA_H_
#define _CDBM_GLOBAL_DATA_H_

#include "gfi-list.h"
#include "gfi-lock.h"
#include "cdbm-datamodel.h"

/* it is the global data definition for CDBM module */
typedef struct T_cdbm_global_data
{
    /* the following is for CDBM transaction related data structure */
    T_gfi_list_head *trans_list;
    T_gfi_lock       trans_list_lock;


    /* the following is global data for data model */
    uint32 dm_type_cnt;
    T_cdbm_dm_typedef *dm_typedef;
    uint32 dm_node_cnt;
    T_cdbm_dm_node *dm_node;
    T_cdbm_dm_node *dm_node_hash;

    /* the following is the global data for running DB */
    T_cdbm_dbase* running_db;

} T_cdbm_global_data;

extern T_cdbm_global_data g_cdbm_data;





void cdbm_lib_init(int phase);




#endif /* _CDBM_GLOBAL_DATA_H_ */
