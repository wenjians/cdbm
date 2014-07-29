
/*
 * this file is used to provide the interface for value getting and setting
 * it is operating with plain text string
 *
 * get/set <==> plain text <==> XML <==> config database
 */

#include <aat-incl.h>
#include <return_codes.h>

#include <cdbm-lib.h>


T_global_rc cdbm_set_uint32(T_cdbm_trans_id cdbm_id, uint32 val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH, fmt, ap);
    va_end (ap);

    gfi_print_buf_add_format(cdbm_id->plain_config, "%s %lu\n", key_path, val);

    return (RC_OK);
}


T_global_rc cdbm_set_int32(T_cdbm_trans_id cdbm_id, int32 val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH, fmt, ap);
    va_end (ap);

    gfi_print_buf_add_format(cdbm_id->plain_config, "%s %ld\n", key_path, val);

    return (RC_OK);
}


T_global_rc cdbm_set_ipv4(T_cdbm_trans_id cdbm_id, T_global_IP_ADDR val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH];
    char ip_addr[IPNG_ADDR_STRING_LENGTH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH, fmt, ap);
    va_end (ap);


    snprintIp4a(ip_addr, IPNG_ADDR_STRING_LENGTH, &val);
    gfi_print_buf_add_format(cdbm_id->plain_config, "%s %s\n", key_path, ip_addr);

    return (RC_OK);
}


T_global_rc cdbm_set_ipv6(T_cdbm_trans_id cdbm_id, T_global_IP6_ADDR val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH];
    char ip_addr[IPNG_ADDR_STRING_LENGTH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH, fmt, ap);
    va_end (ap);

    snprintIp6a(ip_addr, IPNG_ADDR_STRING_LENGTH, &val);

    gfi_print_buf_add_format(cdbm_id->plain_config, "%s %s\n", key_path, ip_addr);

    return (RC_OK);
}


T_global_rc cdbm_set_ipng(T_cdbm_trans_id cdbm_id, T_global_IPNG_ADDR val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH];
    char ip_addr[IPNG_ADDR_STRING_LENGTH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH, fmt, ap);
    va_end (ap);

    snprintIpnga(ip_addr, IPNG_ADDR_STRING_LENGTH, &val);

    gfi_print_buf_add_format(cdbm_id->plain_config, "%s %s\n", key_path, ip_addr);

    return (RC_OK);
}


T_global_rc cdbm_set_string(T_cdbm_trans_id cdbm_id, const char* val, const char* fmt, ...)
{
    va_list ap;
    char key_path[CDBM_MAX_KEYPATH];

    va_start (ap, fmt);
    vsnprintf(key_path, CDBM_MAX_KEYPATH, fmt, ap);
    va_end (ap);

    gfi_print_buf_add_format(cdbm_id->plain_config, "%s %s\n", key_path, val);

    return (RC_OK);
}
