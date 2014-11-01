#ifndef CDBM_LIB_H
#define CDBM_LIB_H


#include "uthash.h"

#include "aat-incl.h"
#include "return_codes.h"
#include "gfi-print-buffer.h"
#include "gfi-list.h"
#include "gfi-lock.h"
#include "cdbm-types.h"

/* it is definition of CDBM configuration paraemter model */
typedef enum T_cdbm_node_type
{
    CDBM_NODE_TYPE_INVALID      = 0,
    CDBM_NODE_TYPE_CONTAINER    = 1,
    CDBM_NODE_TYPE_LIST         = 2,
    CDBM_NODE_TYPE_LEAF_LIST    = 3,
    CDBM_NODE_TYPE_LEAF         = 4,
} T_cdbm_node_type;


typedef enum T_cdbm_operation {
    CDBM_OP_REPLACE = 1,
    CDBM_OP_MERGE   = 2,
    CDBM_OP_CREATE  = 3,
    CDBM_OP_DELETE  = 4
} T_cdbm_operation;


typedef struct T_cdbm_db_kv_pair {
    T_gfi_list   list_node;
    T_cdbm_value *key_path;
    T_cdbm_value *value;
} T_cdbm_db_kv_pair;



typedef struct T_cdbm_db_node
{
    T_cdbm_node_type node_type;

    T_cdbm_value *name;
    T_cdbm_value *kpath;
    T_cdbm_value *value;

    struct T_cdbm_db_node *parent;
    //T_cdbm_db_node *sibling;

    // the following is for children management, only valid for list and container
    struct T_cdbm_db_node *children;   /* list of the child, organized in hash with name */
    UT_hash_handle hh_name;     /* hash items used for same level with same parent */



    //T_cdbm_db_node *child_hash; /* hash searching for child, with fast searching */


    UT_hash_handle hh_keypath;  /* hash for the whole data base */

} T_cdbm_db_node;


typedef struct T_cdbm_dbase {
    T_gfi_lock     db_lock;
    T_cdbm_db_node *node_hash;
    T_cdbm_db_node *db_root;
} T_cdbm_dbase;


typedef struct T_cdbm_trans {
    T_gfi_list   list_node;   // all the transaction items will be in one list

    /* the follwoing is the source of configuration data */
    T_cdbm_operation config_op;
    T_gfi_print_buffer_id config_xml;
    T_gfi_print_buffer_id config_txt;

    /* the following is the meta-data (internal data) of the session */
    T_gfi_list_head config_list_kv_pair;
    T_cdbm_dbase   *config_dbase;

    /* the following is the the result of the configuration transaction */
    T_global_rc config_result;
    T_gfi_print_buffer_id rsp_msg;


    /* managed object read configuration */
    T_gfi_print_buffer_iterator conf_read_it;
}T_cdbm_trans;


#define CDBM_TRANS_GET_DBASE(trans_id)      (trans_id->config_dbase)
#define CDBM_TRANS_GET_KV_PAIR(trans_id)    (&trans_id->config_list_kv_pair)
#define CDBM_TRANS_GET_MESSAGE(trans_id)    (trans_id->rsp_msg)


typedef struct T_cdbm_trans* T_cdbm_trans_id;
#define CDBM_TRANSACTION_SIZE sizeof(T_cdbm_trans)




T_global_rc cdbm_set_uint32(T_cdbm_trans_id cdbm_id, const uint32 val, const char* fmt, ...);
T_global_rc cdbm_set_int32(T_cdbm_trans_id cdbm_id, const int32 val, const char* fmt, ...);
T_global_rc cdbm_set_ipv4(T_cdbm_trans_id cdbm_id, const T_global_IP_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_ipv6(T_cdbm_trans_id cdbm_id, const T_global_IP6_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_ipng(T_cdbm_trans_id cdbm_id, const T_global_IPNG_ADDR val, const char* fmt, ...);
T_global_rc cdbm_set_string(T_cdbm_trans_id cdbm_id, const char *val, const char* fmt, ...);
T_global_rc cdbm_set_conf_string(T_cdbm_trans_id cdbm_id, const char* conf_str);


T_global_rc cdbm_get_uint32(T_cdbm_trans_id trans_id, uint32 *val, const char* fmt, ...);
T_global_rc cdbm_get_int32(T_cdbm_trans_id trans_id, int32 *val, const char* fmt, ...);
T_global_rc cdbm_get_ipv4(T_cdbm_trans_id trans_id, T_global_IP_ADDR *ipv4_addr, const char* fmt, ...);
T_global_rc cdbm_get_ipv6(T_cdbm_trans_id trans_id, T_global_IP6_ADDR *ipv6_addr, const char* fmt, ...);
T_global_rc cdbm_get_ipaddress(T_cdbm_trans_id trans_id, T_global_IPNG_ADDR *ipng_addr, const char* fmt, ...);
T_global_rc cdbm_get_string(T_cdbm_trans_id trans_id, char*buffer, uint32 max_len, const char* fmt, ...);

/***************************************************************************
 *  CDBM transaction related function
 */
void cdbm_trans_init();
T_cdbm_trans_id cdbm_trans_create(char* trans_name);
T_global_rc cdbm_trans_delete(T_cdbm_trans_id cdbm_trans);

T_global_rc cdbm_trans_commit(T_cdbm_trans_id trans_id);
T_global_rc cdbm_trans_get_result(T_cdbm_trans_id trans_id);

T_global_rc cdbm_subscribe(const char* name_space, const char *fmt, ...);
T_global_rc cdbm_unsubscribe(const char* name_space, const char *fmt, ...);
T_global_rc cdbm_read_config_done();
T_global_rc cdbm_check_namespace(const char* name_space);
T_global_rc cdbm_start_read_config(T_cdbm_trans_id trans_id);



/* memory related */
void*cdbm_malloc(uint32 size);
void cdbm_free(void *pbuffer);


/* some common libs privided */
uint32 cdbm_lib_get_next_token(const char* buffer, char* token, uint32 max_len);



/* the library of key path */
uint32 cdbm_kpath_get_next_token(const T_cdbm_value* kpath, uint32 *offset, char* token, uint32 max_len);



#endif /* CDBM_LIB_H */
