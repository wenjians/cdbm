

#ifndef CDBM_DATABASE_H
#define CDBM_DATABASE_H

#include "aat-incl.h"
#include "cdbm-datamodel.h"


// TODO: should based on gfi slab pool
T_cdbm_db_kv_pair* cdbm_db_node_pair_malloc();
void cdbm_db_node_pair_free(T_cdbm_db_kv_pair *db_node);


T_global_rc cdbm_db_nlist_from_lined_text(T_gfi_list_head* config_list,
        T_gfi_print_buffer_id config_text, T_gfi_print_buffer_id err_msg);



T_global_rc cdbm_db_nlist_print(T_gfi_list_head* config_list);



T_cdbm_db_node* cdbm_db_node_malloc();
void cdbm_db_node_free(T_cdbm_db_node *db_node);
void cdbm_db_node_print(T_cdbm_db_node *db_node);



T_cdbm_db_node* cdbm_db_get_child(T_cdbm_db_node *parent, const char* node_name);
T_cdbm_db_node* cdbm_db_add_child(T_cdbm_dbase* dbase, T_cdbm_db_node* parent,
                                  const char* node_name, const T_cdbm_node_type node_type);
T_cdbm_db_node* cdbm_db_get_node(T_cdbm_dbase* dbase, T_cdbm_value* kpath);
T_global_rc     cdbm_db_add_node(T_cdbm_dbase* dbase, T_cdbm_value* key_path, T_cdbm_value *value);
T_global_rc     cdbm_db_mod_node(T_cdbm_dbase* dbase, T_cdbm_value* key_path, T_cdbm_value *value);

T_global_rc cdbm_db_del_node_kpath(T_cdbm_dbase* dbase, T_cdbm_value* key_path);
T_global_rc cdbm_db_del_node_ptr(T_cdbm_dbase* dbase, T_cdbm_db_node* db_node);
T_global_rc cdbm_db_del_node_recursive(T_cdbm_dbase* dbase, T_cdbm_value* key_path);



T_cdbm_dbase* cdbm_db_create();
void cdbm_db_print(T_cdbm_dbase* dbase);
void cdbm_db_walk_print(T_cdbm_dbase* dbase);
void cdbm_db_walk_find(T_cdbm_dbase* dbase);

T_global_rc cdbm_db_import_from_nlist(T_cdbm_dbase* dbase, T_gfi_list_head* config_list,
                                      T_gfi_print_buffer_id err_msg);


void cdbm_db_init(int phase);




#endif /* CDBM_DATABASE_H */
