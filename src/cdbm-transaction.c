

#include "aat-incl.h"
#include "return_codes.h"

#include "gfi-print-buffer.h"

#include "cdbm-lib.h"
#include "cdbm-types.h"
#include "cdbm-database.h"
#include "cdbm-datamodel.h"
#include "cdbm-event.h"
#include "cdbm-global-data.h"

/* one configuration 10M should be enough */
#define CDBM_CONFIG_MAX_SIZE    10*1024*1024



void cdbm_trans_init()
{
    g_cdbm_data.trans_list = gfi_list_create("cdbm_trans");
    AAT_STD_ASSERT (g_cdbm_data.trans_list != NULL);

    gfi_lock_init(&g_cdbm_data.trans_list_lock);
}


T_cdbm_trans_id cdbm_trans_create(char* trans_name)
{
    T_cdbm_trans_id trans_id = NULL;

    trans_id = cdbm_malloc(CDBM_TRANSACTION_SIZE);
    AAT_STD_ASSERT(trans_id != NULL) ;

    memset(trans_id, 0, CDBM_TRANSACTION_SIZE);

    trans_id->config_txt = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
            GFI_PRITN_BUF_FLAG_OVERSTOP | GFI_PRINT_BUF_FLAG_ALIGNED, CDBM_CONFIG_MAX_SIZE);
    AAT_STD_ASSERT(trans_id->config_txt != NULL);

    trans_id->config_xml = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
            GFI_PRITN_BUF_FLAG_OVERSTOP | GFI_PRINT_BUF_FLAG_ALIGNED, CDBM_CONFIG_MAX_SIZE);
    AAT_STD_ASSERT(trans_id->config_txt != NULL);

    trans_id->err_msg = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
            GFI_PRITN_BUF_FLAG_OVERSTOP | GFI_PRINT_BUF_FLAG_ALIGNED, CDBM_CONFIG_MAX_SIZE);
    AAT_STD_ASSERT(trans_id->config_txt != NULL);

    gfi_list_create2(&trans_id->config_node_list, "cdbm_transaction");

    /*
    cdbm_trans_id->xml_config = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
                                  GFI_PRITN_BUF_FLAG_OVERSTOP, CDBM_CONFIG_MAX_SIZE);
    AAT_STD_ASSERT(cdbm_trans_id->xml_config != NULL);
    */

    /* add to transaction session list */
    gfi_list_init(&trans_id->list_node);
    gfi_lock(&g_cdbm_data.trans_list_lock);
    gfi_list_insert_last(g_cdbm_data.trans_list, &trans_id->list_node);
    gfi_unlock(&g_cdbm_data.trans_list_lock);

    return trans_id;
}


T_global_rc cdbm_trans_delete(T_cdbm_trans_id trans_id)
{
    uint32 ret_addr = gfi_return_address();

    if (trans_id == NULL) {
        CDBM_LOG(CDBM_EVT_ERROR, "ERROR: CDBM delete a NULL transaction, caller (0x%x)", ret_addr);
        return RC_CDBM_TRANS_IS_NULL;
    }
    if (!gfi_list_is_entry_in_list(g_cdbm_data.trans_list, &trans_id->list_node))
    {
        CDBM_LOG(CDBM_EVT_ERROR, "ERROR: CDBM close transaction(0x%p) which is not exist, "
                "caller (0x%x)", trans_id, ret_addr);
        return RC_CDBM_TRANS_NOT_IN_LIST;
    }

    gfi_print_buf_destroy(trans_id->config_txt);
    //gfi_print_buf_destroy(cdbm_trans_id->xml_config);

    /* remove from transaction session list */
    gfi_lock(&g_cdbm_data.trans_list_lock);
    gfi_list_remove(g_cdbm_data.trans_list, &trans_id->list_node);
    gfi_unlock(&g_cdbm_data.trans_list_lock);

    cdbm_free (trans_id);

    return RC_OK;
}


T_global_rc cdbm_trans_commit(T_cdbm_trans_id trans_id)
{
    T_global_rc ret_cod;

    ret_cod = cdbm_db_nlist_from_lined_text(&trans_id->config_node_list,
            trans_id->config_txt, trans_id->err_msg);
    trans_id->config_result = ret_cod;
    CDBM_RET_IF_FAIL(ret_cod);

    return RC_OK;
}

T_global_rc cdbm_trans_get_result(T_cdbm_trans_id trans_id)
{
    return trans_id->config_result;
}



/* the following is used to set the configuration data to system
 * it support different data types
 */

T_global_rc cdbm_set_uint32(T_cdbm_trans_id trans_id, uint32 val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    gfi_print_buf_add_format(trans_id->config_txt, "%s %lu\n", key_path, val);

    return (RC_OK);
}


T_global_rc cdbm_set_int32(T_cdbm_trans_id trans_id, int32 val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    gfi_print_buf_add_format(trans_id->config_txt, "%s %ld\n", key_path, val);

    return (RC_OK);
}


T_global_rc cdbm_set_ipv4(T_cdbm_trans_id trans_id, T_global_IP_ADDR val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char ip_addr[IPNG_ADDR_STRING_LENGTH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);


    snprintIp4a(ip_addr, IPNG_ADDR_STRING_LENGTH, &val);
    gfi_print_buf_add_format(trans_id->config_txt, "%s %s\n", key_path, ip_addr);

    return (RC_OK);
}


T_global_rc cdbm_set_ipv6(T_cdbm_trans_id trans_id, T_global_IP6_ADDR val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char ip_addr[IPNG_ADDR_STRING_LENGTH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    snprintIp6a(ip_addr, IPNG_ADDR_STRING_LENGTH, &val);

    gfi_print_buf_add_format(trans_id->config_txt, "%s %s\n", key_path, ip_addr);

    return (RC_OK);
}


T_global_rc cdbm_set_ipng(T_cdbm_trans_id trans_id, T_global_IPNG_ADDR val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char ip_addr[IPNG_ADDR_STRING_LENGTH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    snprintIpnga(ip_addr, IPNG_ADDR_STRING_LENGTH, &val);

    gfi_print_buf_add_format(trans_id->config_txt, "%s %s\n", key_path, ip_addr);

    return (RC_OK);
}


T_global_rc cdbm_set_string(T_cdbm_trans_id trans_id, const char* val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    gfi_print_buf_add_format(trans_id->config_txt, "%s %s\n", key_path, val);

    return (RC_OK);
}


/* the conf_str include both key and value pair, it might include multiple line
 * which could be a total configuration files
 */
T_global_rc cdbm_set_conf_string(T_cdbm_trans_id trans_id, const char* conf_str)
{
    uint32 len;

    len = gfi_print_buf_add_string(trans_id->config_txt, conf_str, strlen(conf_str));
    if (len == 0) {
        return RC_CDBM_SET_CONF_STR_FAIL;
    }

    return (RC_OK);
}
