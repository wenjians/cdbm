#ifndef CDBM_LIB_H
#define CDBM_LIB_H


#include "gfi-print-buffer.h"
//#include ""

typedef struct T_cdbm_trans {

    //char *plain_config_string;
    T_gfi_print_buffer_id plain_config;
}T_cdbm_trans;

typedef struct T_cdbm_trans* T_cdbm_trans_id;
#define CDBM_TRANSACTION_SIZE sizeof(T_cdbm_trans)

/* maximum key path for each node, which include all the parents
 *   e.g. /syslog/speed
 */
#define CDBM_MAX_KEYPATH        256

T_global_rc cdbm_set_uint32(T_cdbm_trans_id cdbm_id, uint32 val, const char* fmt, ...);
T_global_rc cdbm_set_int32(T_cdbm_trans_id cdbm_id, int32 val, const char* fmt, ...);
T_global_rc cdbm_set_ipv4(T_cdbm_trans_id cdbm_id, T_global_IP_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_ipv6(T_cdbm_trans_id cdbm_id, T_global_IP6_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_ipng(T_cdbm_trans_id cdbm_id, T_global_IPNG_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_string(T_cdbm_trans_id cdbm_id, const char val, const char* fmt, ...);

#endif /* CDBM_LIB_H */
