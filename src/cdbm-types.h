
#ifndef CDBM_TYPES_H
#define CDBM_TYPES_H

#include "aat-types.h"

//#include "cdbm-datamodel.h"




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

/* maximum key path for each node, which include all the parents
 *   e.g. /syslog/speed
 * NOTE: when change the value of CDBM_MAX_KEYPATH_LEN, must also change
 *       the definition of cdbm_val_kpath_def_type in cdbm-key-path.c
 */
#define CDBM_MAX_KEYPATH_LEN    256
#define CDBM_MAX_NONSTR_LEN      64


typedef struct T_cdbm_buf {
    uint32  len;    /* does not include the len of terminal '\0' */
    uchar8* ptr;
} T_cdbm_buf;

typedef struct T_cdbm_enum {
    int32 int_val;
    uchar8 *str_val;
} T_cdbm_enum;

typedef struct T_cdbm_value {
    T_cdbm_vtype type;    /* the data types */
    union {
        uint32 u32;
        int32  i32;
        T_global_IP_ADDR ipv4;
        T_global_IP6_ADDR ipv6;
        T_global_IPNG_ADDR ip_addr;
        uchar8 mac[MAC_STRING_LENGTH];
        char *str;
        bool   b;
        T_cdbm_buf buf;
        T_cdbm_enum enum_val;
    } val;
} T_cdbm_value;





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


T_cdbm_value* cdbm_val_malloc();
void cdbm_val_free(T_cdbm_value *p_val);


T_cdbm_vtype cdbm_val_get_type(T_cdbm_value *val);
T_global_rc  cdbm_val_set_type(T_cdbm_value *v1, T_cdbm_vtype vtype);
bool         cdbm_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
T_global_rc  cdbm_val_to_str(const T_cdbm_value *val, char *buf, uint32 len);
T_global_rc  cdbm_str_to_val(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
char*        cdbm_val_ret_str(const T_cdbm_value *val, char *buf, uint32 len);
T_global_rc  cdbm_val_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);
T_cdbm_value*cdbm_val_duplicate(const T_cdbm_value *val);

bool cdbm_val_uint32_in_range(uint32 val, const char* range_string);
bool cdbm_val_int32_in_range(int32 val, const char* range_string);

const char* cdbm_kpath_get_string(const T_cdbm_value *kpath);
const T_cdbm_dm_type* cdbm_val_get_kpath_type();

bool cdbm_keypath_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
T_global_rc cdbm_str_to_keypath(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
T_global_rc cdbm_keypath_to_str(const T_cdbm_value *val, char *str, uint32 len);
T_global_rc cdbm_keypath_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);
T_cdbm_value* cdbm_keypath_create(const char* fmt, ...);
const char* cdbm_kpath_get_string(const T_cdbm_value *kpath);



void cdbm_val_lib_init();

//extern T_cdbm_val_opt g_cdbm_val_ops[CDBM_TYPE_MAX];




#endif /* CDBM_TYPES_H */
