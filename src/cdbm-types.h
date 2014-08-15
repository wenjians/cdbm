
#ifndef CDBM_TYPES_H
#define CDBM_TYPES_H

#include "aat-types.h"

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
    CDBM_TYPE_MAX,                  /* MAX type value definition */
} T_cdbm_vtype;


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



#endif /* CDBM_TYPES_H */
