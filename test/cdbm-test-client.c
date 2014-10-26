
#include "aat-incl.h"

#include "cdbm-lib.h"
#include "cdbm-test-client.h"

#if 0

#define CDBM_TEST_NAME_SPACE   "http://www.cdbm-test.com/test-data"

T_test_config g_test_config;

/* register one "syslog/speed-uint" */
T_global_rc test_read_config_leaf(const T_cdbm_trans_id trans_id)
{
    T_global_rc ret_cod;

    ret_cod = cdbm_check_namespace(CDBM_TEST_NAME_SPACE);
    if (ret_cod != RC_OK)
        return ret_cod;

    ret_cod = cdbm_start_read_config(trans_id);
    if (ret_cod != RC_OK)
        return ret_cod;

    //printf("test_read_config_leaf, config=<%s>\n", trans_id->plain_config->buffer);
    ret_cod = cdbm_get_uint32(trans_id, &g_test_config.speed_uint, "/syslog/speed-uint");

    //cdbm_read_config_done(trans_id, )
    return ret_cod;
}

T_global_rc test_read_config_group(const T_cdbm_trans_id trans_id)
{
    T_global_rc ret_cod;
    //char ip_addr[IPNG_ADDR_STRING_LENGTH];

    ret_cod = cdbm_check_namespace(CDBM_TEST_NAME_SPACE);
    if (ret_cod != RC_OK)
        return ret_cod;

    ret_cod = cdbm_start_read_config(trans_id);
    if (ret_cod != RC_OK)
        return ret_cod;
        

    cdbm_get_string(trans_id, g_test_config.save_mode, 16, "/syslog/save-mode");
    //printf("g_test_config.save_mode=<%s>\n", g_test_config.save_mode);
    
    cdbm_get_uint32(trans_id, &g_test_config.speed_uint, "/syslog/speed-uint");
    //printf("g_test_config.speed_uint=<%d>\n", g_test_config.speed_uint);
    
    cdbm_get_int32(trans_id, &g_test_config.speed_int, "/syslog/speed-int");
    //printf("g_test_config.speed_int=<%d>\n", g_test_config.speed_int);
    
    ret_cod = cdbm_get_ipv4(trans_id, &g_test_config.pri_ip, "/syslog/primary-ip");
    //printf("g_test_config.pri_ipv4=<%s>\n",
    //    snprintIp4a(ip_addr, IPNG_ADDR_STRING_LENGTH, &g_test_config.pri_ip));

    cdbm_get_ipv6(trans_id, &g_test_config.sec_ip, "/syslog/secondary-ip");
    //printf("g_test_config.sec_ipv6=<%s>\n",
    //       snprintIp6a(ip_addr, IPNG_ADDR_STRING_LENGTH, &g_test_config.sec_ip));
    
    cdbm_get_ipaddress(trans_id, &g_test_config.ipng_v4, "/syslog/ipng-v%d", 4);
    //printf("g_test_config.ipng_v4=<%s>\n",
    //       snprintIpnga(ip_addr, IPNG_ADDR_STRING_LENGTH, &g_test_config.ipng_v4));
    
    cdbm_get_ipaddress(trans_id, &g_test_config.ipng_v6, "/%s/ipng-v%d", "syslog", 6);
    //printf("g_test_config.ipng_v6=<%s>\n",
    //       snprintIpnga(ip_addr, IPNG_ADDR_STRING_LENGTH, &g_test_config.ipng_v6));
    
    cdbm_get_string(trans_id, g_test_config.local_mac, MAC_STRING_LENGTH, "/syslog/local-mac");
    //printf("g_test_config.local_mac=<%s>\n", g_test_config.local_mac);

    //cdbm_read_config_done(trans_id, )
    return RC_OK;
}

#endif
