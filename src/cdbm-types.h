
#ifndef CDBM_TYPES_H
#define CDBM_TYPES_H

#include "aat-types.h"

#include "cdbm-datamodel.h"



/* maximum key path for each node, which include all the parents
 *   e.g. /syslog/speed
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


T_cdbm_value* cdbm_val_alloc();
void cdbm_val_free(T_cdbm_value *p_val);

bool cdbm_val_uint32_in_range(uint32 val, const char* range_string);
bool cdbm_val_int32_in_range(int32 val, const char* range_string);

extern bool cdbm_uint32_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
extern T_global_rc cdbm_uint32_to_str(const T_cdbm_value *val, char *buf, uint32 len);
extern T_global_rc cdbm_str_to_uint32(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
extern T_global_rc cdbm_uint32_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);

extern bool cdbm_int32_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
extern T_global_rc cdbm_int32_to_str(const T_cdbm_value *val, char *buf, uint32 len);
extern T_global_rc cdbm_str_to_int32(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
extern T_global_rc cdbm_int32_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);



bool cdbm_ipv4_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
T_global_rc cdbm_str_to_ipv4(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
T_global_rc cdbm_ipv4_to_str(const T_cdbm_value *val, char *str, uint32 len);
T_global_rc cdbm_ipv4_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);



bool cdbm_ipv6_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
T_global_rc cdbm_str_to_ipv6(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
T_global_rc cdbm_ipv6_to_str(const T_cdbm_value *val, char *str, uint32 len);
T_global_rc cdbm_ipv6_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);


bool cdbm_ipng_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
T_global_rc cdbm_str_to_ipng(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
T_global_rc cdbm_ipng_to_str(const T_cdbm_value *val, char *str, uint32 len);
T_global_rc cdbm_ipng_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);



bool cdbm_keypath_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2);
T_global_rc cdbm_str_to_keypath(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
T_global_rc cdbm_keypath_to_str(const T_cdbm_value *val, char *str, uint32 len);
T_global_rc cdbm_keypath_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val);

typedef bool (*T_cmdb_val_eq)(const T_cdbm_value* v1, const T_cdbm_value* v2);
typedef T_global_rc (*T_cdbm_val_to_str)(const T_cdbm_value *val, char *buf, uint32 len);
typedef T_global_rc (*T_cdbm_str_to_val)(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val);
typedef T_global_rc (*T_cdbm_val_valid)(const T_cdbm_dm_type *type, const T_cdbm_value *val);


typedef struct T_cdbm_val_opt {
    T_cdbm_vtype  val_type;
    T_cmdb_val_eq val_eq;
    T_cdbm_val_to_str val_to_str;
    T_cdbm_str_to_val str_to_val;
    T_cdbm_val_valid  val_valid;
} T_cdbm_val_opt;


extern T_cdbm_val_opt g_cdbm_val_ops[CDBM_TYPE_MAX];




#endif /* CDBM_TYPES_H */
