
#ifndef CDBM_CONFIG_MODEL_H
#define CDBM_CONFIG_MODEL_H

#include "uthash.h"

#include "cdbm-db.h"
#include "cdbm-types.h"

#define CDBM_CM_INVALID_IDX -1
typedef int32   T_cdbm_cm_node_idx;



/* it is definition of CDBM configuration paraemter model */
typedef enum T_cdbm_node_type
{
    CDBM_NODE_TYPE_INVALID      = 0,
    CDBM_NODE_TYPE_CONTAINER    = 1,
    CDBM_NODE_TYPE_LIST         = 2,
    CDBM_NODE_TYPE_LEAF_LIST    = 3,
    CDBM_NODE_TYPE_LEAF         = 4,
} T_cdbm_node_type;

/* no self-defined data available for container */
typedef struct T_cdbm_cm_container
{
    uint32 reserved;
} T_cdbm_cm_container;


/* list specified data */
typedef struct T_cdbm_cm_list
{
    uint32 min_element;
    uint32 max_element;
    uint32 ordered_by;
    T_cdbm_cm_node_idx  key_idx;
} T_cdbm_cm_list;


typedef struct T_cdbm_cm_leaf_list
{
    uint32 min_element;
    uint32 max_element;
    uint32 ordered_by;
    struct T_cdbm_cm_type *data_type;
} T_cdbm_cm_leaf_list;

typedef struct T_cdbm_cm_leaf
{
    const char* def_value;
    bool mandatory;
    struct T_cdbm_cm_type *data_type;
} T_cdbm_cm_leaf;




typedef struct T_cdbm_cm_node
{
    T_cdbm_node_type node_type;
    bool             configurable;
    const char*      key_name;
    T_cdbm_cm_node_idx  parent_idx;
    T_cdbm_cm_node_idx  children_idx;
    T_cdbm_cm_node_idx  sibling_idx;
    const char*  must_state;
    const char*  key_path;

    union node_data {
        T_cdbm_cm_container container;
        T_cdbm_cm_list      list;
        T_cdbm_cm_leaf_list leaf_list;
        T_cdbm_cm_leaf      leaf;
    } node;

    UT_hash_handle hh;

} T_cdbm_cm_node;


typedef enum T_cdbm_vtype_category {
    CDBM_CATEGORY_INT   =1,
    CDBM_CATEGORY_STR   =2,
    // TODO ADD more categories
}T_cdbm_vtype_category;

typedef struct T_cdbm_type_int {
    const char* range;
} T_cdbm_type_int;

#define CDBM_MAX_PATTERN_COUNT  5
typedef struct T_cdbm_type_string {
    const char* lengh;
    uint32 pattern_count;
    const char* pattern_list[CDBM_MAX_PATTERN_COUNT];
} T_cdbm_type_string;

typedef struct T_cdbm_type_empty {
    uint32 reserved;
} T_cdbm_type_empty;

typedef struct T_cdbm_type_enum_value {
    const char* keyword;
    bool value_defined;
    int32 value;
} T_cdbm_type_enum_value;

typedef struct T_cdbm_type_enum {
    uint32 enum_count;
    T_cdbm_type_enum_value *enum_list;
} T_cdbm_type_enum;

typedef int32  T_cdbm_cm_typedef_idx;

typedef struct T_cdbm_cm_type {
    T_cdbm_cm_typedef_idx type_def_id;
    T_cdbm_vtype base_type;      /* the base builtin type */
    union _type {
        T_cdbm_type_int t_int;
        T_cdbm_type_string t_string;
        T_cdbm_type_enum  t_enum;
        T_cdbm_type_empty t_empty;
    } type;
} T_cdbm_cm_type;


typedef struct T_cdbm_cm_typedef {
    const char *name;
    bool is_builtin;
    const char *def_value;
    union data_type {
        T_cdbm_cm_type derived;
        T_cdbm_vtype builtin;
    } type;
} T_cdbm_cm_typedef;


typedef T_global_rc (*T_cmdb_node_op)(T_cdbm_cm_node*);

typedef struct T_cdbm_cm_node_ops {
    T_cmdb_node_op enter_container;
    T_cmdb_node_op exit_container;
    T_cmdb_node_op enter_list;
    T_cmdb_node_op exit_list;
    T_cmdb_node_op enter_leaf;
    T_cmdb_node_op exit_leaf;
    T_cmdb_node_op enter_leaf_list;
    T_cmdb_node_op exit_leaf_list;
} T_cdbm_cm_node_ops;


void cdbm_cm_attach_data(T_cdbm_cm_node *_node, T_cdbm_cm_typedef *_typedef);
T_global_rc cdbm_cm_node_walk(T_cdbm_cm_node_ops *node_ops);
T_global_rc cdbm_cm_init();

T_cdbm_cm_node* cdbm_cm_get_node_from_keypath(const char* key_path);
bool cdbm_cm_is_root(T_cdbm_cm_node *cm_node) ;
bool cmbm_cm_is_container(T_cdbm_cm_node *cm_node);
bool cmbm_cm_is_list(T_cdbm_cm_node *cm_node);
bool cmbm_cm_is_leaf(T_cdbm_cm_node *cm_node);
bool cmbm_cm_is_leaf_list(T_cdbm_cm_node *cm_node);

#endif /* CDBM_CONFIG_MODEL_H */
