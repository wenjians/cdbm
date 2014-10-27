

#ifndef CDBM_DATABASE_H
#define CDBM_DATABASE_H

#include "aat-incl.h"
#include "cdbm-datamodel.h"

typedef struct T_cdbm_db_kv_pair {
    T_gfi_list   list_node;
    T_cdbm_value key_path;
    T_cdbm_value value;
} T_cdbm_db_kv_pair;



typedef struct T_cdbm_db_node
{
    const char* key_name;
    T_cdbm_value key_path;
    T_cdbm_value value;

    T_cdbm_db_node  *parent;
    T_cdbm_db_node  *children;
    T_cdbm_db_node  *sibling;

    UT_hash_handle hh;

} T_cdbm_db_node;


// TODO: should based on gfi slab pool
T_cdbm_db_kv_pair* cdbm_db_node_pair_malloc();
void cdbm_db_node_pair_free(T_cdbm_db_kv_pair *db_node);


T_global_rc cdbm_db_nlist_from_lined_text(T_gfi_list_head* config_list,
                                          T_gfi_print_buffer_id config_text,
                                          T_gfi_print_buffer_id err_msg);

#endif /* CDBM_DATABASE_H */
