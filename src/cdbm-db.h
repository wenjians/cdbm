
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
    const char*            key_name;
    T_cdbm_cm_node_idx  parent_idx;
    T_cdbm_cm_node_idx  children_idx;
    T_cdbm_cm_node_idx  sibling_idx;
    const char*  must_state;
    const char*  xpath;
    
    union node_data {
        T_cdbm_cm_container container;
        T_cdbm_cm_list      list;
        T_cdbm_cm_leaf_list leaf_list;
        T_cdbm_cm_leaf      leaf;
    } node;
    
} T_cdbm_cm_node;











typedef enum T_cdbm_vtype {
    CDBM_TYPE_EMPTY        =0,
    CDBM_TYPE_UINT32        =1,
    CDBM_TYPE_INT32         =2,
    CDBM_TYPE_IPV4          =3,
    CDBM_TYPE_IPV6          =4,
    CDBM_TYPE_IPADDR          =5,
    CDBM_TYPE_MAC           =6,
    CDBM_TYPE_HEX           =7,
    CDBM_TYPE_STRING        =8,
    CDBM_TYPE_STR_WORD      =9,
    CDBM_TYPE_ENUM_HASH     =10,
    CDBM_TYPE_MAX,                  /* MAX type value definition */
} T_cdbm_builtin;




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
    T_cdbm_builtin base_type;      /* the base builtin type */
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
        T_cdbm_builtin builtin;
    } type;
} T_cdbm_cm_typedef;


void cdbm_lib_init(int phase);




#endif /* _CDBM_DB_H_ */
