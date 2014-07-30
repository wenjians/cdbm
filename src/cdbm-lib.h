#ifndef CDBM_LIB_H
#define CDBM_LIB_H

#include "aat-incl.h"
#include "return_codes.h"
#include "gfi-print-buffer.h"
#include "gfi-list.h"

typedef struct T_cdbm_trans {
    T_gfi_list   list_node;   // all the transaction items will be in one list


    T_gfi_print_buffer_id xml_config;
    T_gfi_print_buffer_id plain_config;
}T_cdbm_trans;

typedef struct T_cdbm_trans* T_cdbm_trans_id;
#define CDBM_TRANSACTION_SIZE sizeof(T_cdbm_trans)

/* maximum key path for each node, which include all the parents
 *   e.g. /syslog/speed
 */
#define CDBM_MAX_KEYPATH        256

T_global_rc cdbm_set_uint32(T_cdbm_trans_id cdbm_id, const uint32 val, const char* fmt, ...);
T_global_rc cdbm_set_int32(T_cdbm_trans_id cdbm_id, const int32 val, const char* fmt, ...);
T_global_rc cdbm_set_ipv4(T_cdbm_trans_id cdbm_id, const T_global_IP_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_ipv6(T_cdbm_trans_id cdbm_id, const T_global_IP6_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_ipng(T_cdbm_trans_id cdbm_id, const T_global_IPNG_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_string(T_cdbm_trans_id cdbm_id, const char *val, const char* fmt, ...);


/***************************************************************************
 *  CDBM transaction related function
 */
void cdbm_trans_init();
T_cdbm_trans_id cdbm_create_transaction(char* trans_name);
T_global_rc cdbm_close_transaction(T_cdbm_trans_id cdbm_trans);


#endif /* CDBM_LIB_H */
