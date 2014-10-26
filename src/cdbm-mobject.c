
/* it is interface/implementation with managed object
 *
 * NOTE: name space is not support in first release
 */

#include "aat-incl.h"
#include "return_codes.h"
#include "gfi-print-buffer.h"

#include "cdbm-lib.h"

#if 0

T_global_rc cdbm_subscribe(const char* name_space, const char *fmt, ...)
{
    return RC_OK;
}


T_global_rc cdbm_unsubscribe(const char* name_space, const char *fmt, ...)
{
    return RC_OK;
}


T_global_rc cdbm_read_config_done()
{
    return RC_OK;
}



T_global_rc cdbm_check_namespace(const char* name_space)
{
    return RC_OK;
}


T_global_rc cdbm_start_read_config(T_cdbm_trans_id trans_id)
{
    gfi_print_buf_it_init(trans_id->plain_config, &trans_id->conf_read_it);

    return RC_OK;
}


T_global_rc cdbm_get_uint32(T_cdbm_trans_id trans_id, uint32 *val, const char* fmt, ...)
{
    va_list ap;
    T_global_rc ret_cod;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char int_val[CDBM_MAX_NONSTR_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);
    //printf("keypath=<%s>\n", key_path);

    ret_cod = cdbm_get_string(trans_id, int_val, CDBM_MAX_NONSTR_LEN, key_path);
    if (ret_cod != RC_OK) {
        return ret_cod;
    }

    //printf("cdbm_get_uint32: buffer=<%s>", int_val);
    *val = atoi(int_val);

    return (RC_OK);
}

T_global_rc cdbm_get_int32(T_cdbm_trans_id trans_id, int32 *val, const char* fmt, ...)
{
    va_list ap;
    T_global_rc ret_cod;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char int_val[CDBM_MAX_NONSTR_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    ret_cod = cdbm_get_string(trans_id, int_val, CDBM_MAX_NONSTR_LEN, key_path);
    if (ret_cod != RC_OK) {
        return ret_cod;
    }

    *val = atoi(int_val);

    return (RC_OK);
}

T_global_rc cdbm_get_ipv4(T_cdbm_trans_id trans_id, T_global_IP_ADDR *ipv4_addr, const char* fmt, ...)
{
    va_list ap;
    T_global_rc ret_cod;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char ip_val[CDBM_MAX_NONSTR_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    ret_cod = cdbm_get_string(trans_id, ip_val, CDBM_MAX_NONSTR_LEN, key_path);
    if (ret_cod != RC_OK) {
        return ret_cod;
    }

    ret_cod = ip4_get_addr_from_str(ip_val, ipv4_addr);
    if (ret_cod != RC_OK) {
        return RC_CDBM_INVALID_IPV4_ADDR;
    }

    return (RC_OK);
}

T_global_rc cdbm_get_ipv6(T_cdbm_trans_id trans_id, T_global_IP6_ADDR *ipv6_addr, const char* fmt, ...)
{
    va_list ap;
    T_global_rc ret_cod;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char ip_val[CDBM_MAX_NONSTR_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    ret_cod = cdbm_get_string(trans_id, ip_val, CDBM_MAX_NONSTR_LEN, key_path);
    if (ret_cod != RC_OK) {
        return ret_cod;
    }

    ret_cod = ip6_get_addr_from_str(ip_val, ipv6_addr);
    if (ret_cod != RC_OK)
        return RC_CDBM_INVALID_IPV6_ADDR;

    return (RC_OK);
}


T_global_rc cdbm_get_ipaddress(T_cdbm_trans_id trans_id, T_global_IPNG_ADDR *ipng_addr, const char* fmt, ...)
{
    va_list ap;
    T_global_rc ret_cod;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char ip_val[CDBM_MAX_NONSTR_LEN];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    ret_cod = cdbm_get_string(trans_id, ip_val, CDBM_MAX_NONSTR_LEN, key_path);
    if (ret_cod != RC_OK) {
        return ret_cod;
    }

    ret_cod = ipng_get_addr_from_str(ip_val, ipng_addr);
    if (ret_cod != RC_OK)
        return RC_CDBM_INVALID_IP_ADDR;

    return (RC_OK);
}


T_global_rc cdbm_get_string(T_cdbm_trans_id trans_id, char*buffer, uint32 max_len, const char* fmt, ...)
{
    va_list ap;
    uint32 read_len;
    char* dst, *src, *conf_line;
    char key_path[CDBM_MAX_KEYPATH_LEN];
    char key_token[CDBM_MAX_KEYPATH_LEN+1];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    /*
    printf("cdbm_get_string: keypath=<%s>\n", key_path);
    printf("cdbm_get_string: config value=<%s>\n",
            &trans_id->conf_read_it.print_buffer->buffer[trans_id->conf_read_it.cur_read_off]);
    */
    
    conf_line = gfi_print_buf_node_alloc();
    read_len = gfi_print_buf_it_read_line(&trans_id->conf_read_it,
                                          conf_line, GFI_PRINT_BUF_NODE_SIZE);
    if (read_len == 0) {
        return RC_CDBM_READ_EMPTY;
    }
    //printf("conf_line=<%s>\n", conf_line);

    /* use CDBM_MAX_KEYPATH_LEN+1 to make sure that in case there are too longer
     * key in configuration could get enough string, which will make sure
     * strcmp failure in that case
     */

    read_len = cdbm_lib_get_next_token(conf_line, key_token, CDBM_MAX_KEYPATH_LEN+1);
    if (strcmp(key_token, key_path) != 0) {
        return RC_CDBM_KEYPATH_NOT_MATCH;
    }

    dst = buffer;
    src = conf_line + read_len;

    // skip the space
    while (*src == ' ' && *src != '\0')
        src++;

    /* use max_len-1 to make sure there is a space to fill terminate '\0'
     * should not copy the last '\n'
     */
    for (; *src && *src!='\n' && (max_len-1>0); max_len--)
        *dst++ = *src++;

    *dst = '\0';

    return (RC_OK);
}

#endif
