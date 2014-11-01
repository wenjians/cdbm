
#ifndef CDBM_CONFIG_DATA_MODEL_H
#define CDBM_CONFIG_DATA_MODEL_H

#include "uthash.h"

#include "cdbm-types.h"

#define CDBM_CM_INVALID_IDX -1
typedef int32   T_cdbm_dm_node_idx;







/* no self-defined data available for container */
typedef struct T_cdbm_cm_container
{
    uint32 reserved;
} T_cdbm_dm_container;


/* list specified data */
typedef struct T_cdbm_dm_list
{
    uint32 min_element;
    uint32 max_element;
    uint32 ordered_by;
    T_cdbm_dm_node_idx  key_idx;
} T_cdbm_dm_list;


typedef struct T_cdbm_dm_leaf_list
{
    uint32 min_element;
    uint32 max_element;
    uint32 ordered_by;
    struct T_cdbm_dm_type *data_type;
} T_cdbm_dm_leaf_list;

typedef struct T_cdbm_dm_leaf
{
    const char* def_value;
    bool mandatory;
    struct T_cdbm_dm_type *data_type;
} T_cdbm_dm_leaf;





typedef struct T_cdbm_dm_node
{
    T_cdbm_node_type node_type;
    bool             configurable;
    const char*      key_name;
    T_cdbm_dm_node_idx  parent_idx;
    T_cdbm_dm_node_idx  children_idx;
    T_cdbm_dm_node_idx  sibling_idx;
    const char*  must_state;
    const char*  key_path;

    union node_data {
        T_cdbm_dm_container container;
        T_cdbm_dm_list      list;
        T_cdbm_dm_leaf_list leaf_list;
        T_cdbm_dm_leaf      leaf;
    } node;

    UT_hash_handle hh;

} T_cdbm_dm_node;





typedef struct T_cdbm_dm_typedef {
    const char *name;
    bool is_builtin;
    const char *def_value;
    union data_type {
        T_cdbm_dm_type derived;
        T_cdbm_vtype builtin;
    } type;
} T_cdbm_dm_typedef;


typedef T_global_rc (*T_cmdb_node_op)(T_cdbm_dm_node*);

typedef struct T_cdbm_dm_node_ops {
    T_cmdb_node_op enter_container;
    T_cmdb_node_op exit_container;
    T_cmdb_node_op enter_list;
    T_cmdb_node_op exit_list;
    T_cmdb_node_op enter_leaf;
    T_cmdb_node_op exit_leaf;
    T_cmdb_node_op enter_leaf_list;
    T_cmdb_node_op exit_leaf_list;
} T_cdbm_dm_node_ops;


void cdbm_dm_attach_data(T_cdbm_dm_node *_node,       uint32 node_cnt,
                         T_cdbm_dm_typedef *_typedef, uint32 typedef_cnt);
//T_global_rc cdbm_dm_node_walk(T_cdbm_dm_node_ops *node_ops);
T_global_rc cdbm_dm_node_walk_one_node(T_cdbm_dm_node *cur_node, T_cdbm_dm_node_ops *node_ops);
T_global_rc cdbm_dm_init();
T_global_rc cdbm_dm_node_printall();

T_cdbm_dm_node* cdbm_dm_get_node_from_keypath(const char* key_path);
bool cdbm_dm_node_is_root(T_cdbm_dm_node *cm_node) ;
bool cmbm_dm_node_is_container(T_cdbm_dm_node *cm_node);
bool cmbm_dm_node_is_list(T_cdbm_dm_node *cm_node);
bool cmbm_dm_node_is_leaf(T_cdbm_dm_node *cm_node);
bool cmbm_dm_node_is_leaf_list(T_cdbm_dm_node *cm_node);

#endif /* CDBM_CONFIG_DATA_MODEL_H */
