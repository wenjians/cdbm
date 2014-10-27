
#ifndef CDBM_CONFIG_DATA_MODEL_H
#define CDBM_CONFIG_DATA_MODEL_H

#include "uthash.h"

#include "cdbm-types.h"

#define CDBM_CM_INVALID_IDX -1
typedef int32   T_cdbm_dm_node_idx;



typedef enum T_cdbm_vtype {
    CDBM_TYPE_EMPTY         =  0,
    CDBM_TYPE_UINT32        =  1,
    CDBM_TYPE_INT32         =  2,
    CDBM_TYPE_IPV4          =  3,
    CDBM_TYPE_IPV6          =  4,
    CDBM_TYPE_IPADDR        =  5,
    CDBM_TYPE_MAC           =  6,
    CDBM_TYPE_HEX           =  7,
    CDBM_TYPE_STRING        =  8,
    CDBM_TYPE_STR_WORD      =  9,
    CDBM_TYPE_BOOL          = 10,
    CDBM_TYPE_ENUM_HASH     = 11,
    CDBM_TYPE_BUF           = 12,
    CDBM_TYPE_KEYPATH       = 13,
    CDBM_TYPE_MAX,                  /* MAX type value definition */
} T_cdbm_vtype;


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


typedef enum T_cdbm_vtype_category {
    CDBM_CATEGORY_INT   =1,
    CDBM_CATEGORY_STR   =2,
    CDBM_CATEGORY_ENUM  =3,
    CDBM_CATEGORY_BOOL  =4,
    CDBM_CATEGORY_IP    =5,
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

typedef int32  T_cdbm_dm_typedef_idx;

typedef struct T_cdbm_dm_type {
    T_cdbm_dm_typedef_idx type_def_id;
    T_cdbm_vtype base_type;      /* the base builtin type */
    union _type {
        T_cdbm_type_int t_int;
        T_cdbm_type_string t_string;
        T_cdbm_type_enum  t_enum;
        T_cdbm_type_empty t_empty;
    } type;
} T_cdbm_dm_type;


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
