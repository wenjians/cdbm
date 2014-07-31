
/*****************************************************************************
 * this file is used to unit test for CDBM
 ****************************************************************************/
#include "aat-incl.h"
#include "return_codes.h"
#include "vunit.h"

#include "gfi-list.h"
#include "cdbm-lib.h"
#include "cdbm-db.h"

#include "cdbm-test-client.h"

/***************************************************************************
 * local prototype
 **************************************************************************/
int main();

static int cdbm_test_misc_lib();
static int cdbm_test_transaction();
static int cdbm_test_set_value() ;
static int cdbm_test_mo_leaf();
static int cdbm_test_mo_group();


VTestCase cdbm_test_cases[] = {
    DEF_TEST_CASE(cdbm_test_misc_lib),
    DEF_TEST_CASE(cdbm_test_transaction),
    DEF_TEST_CASE(cdbm_test_set_value),
    DEF_TEST_CASE(cdbm_test_mo_leaf),
    DEF_TEST_CASE(cdbm_test_mo_group),


    // add one line before here for every test case
    {NULL, NULL} // don't remove, end of array!
};



int main()
{
    /* init phase 1 */
    gfi_list_mgr_init(1);
    cdbm_lib_init(1);

    /* init phase 2 */
    gfi_list_mgr_init(2);
    cdbm_lib_init(2);
    
    runAll();

    return 0;
}

/***************************************************************************
 * test case definitions
 **************************************************************************/

static int cdbm_test_misc_lib()
{
#define CDBM_TEST_TOKEN_LEN 64
    char token[CDBM_TEST_TOKEN_LEN];
    uint32 read_len;


    // a typical unit test include 4 steps.
    // 1. Setup

    // 2. Execute
    read_len = cdbm_lib_get_next_token("abcdefg abc", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(strlen("abcdefg"), read_len);
    VASSERT_EQ_STR("abcdefg", token);

    read_len = cdbm_lib_get_next_token("abcdefg", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(strlen("abcdefg"), read_len);
    VASSERT_EQ_STR("abcdefg", token);

    read_len = cdbm_lib_get_next_token("abc\0defg", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(strlen("abc"), read_len);
    VASSERT_EQ_STR("abc", token);

    read_len = cdbm_lib_get_next_token("abcdefg abc", token, 2);
    VASSERT_EQ(strlen("a"), read_len);
    VASSERT_EQ_STR("a", token);

    read_len = cdbm_lib_get_next_token("abcdefg\tabc", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(strlen("abcdefg"), read_len);
    VASSERT_EQ_STR("abcdefg", token);

    read_len = cdbm_lib_get_next_token("abcdefg\nabc", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(strlen("abcdefg"), read_len);
    VASSERT_EQ_STR("abcdefg", token);

    read_len = cdbm_lib_get_next_token("\tabcdefg\nabc", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(strlen("abcdefg"), read_len);
    VASSERT_EQ_STR("abcdefg", token);

    read_len = cdbm_lib_get_next_token("", token, CDBM_TEST_TOKEN_LEN);
    VASSERT_EQ(0, read_len);
    VASSERT_EQ_STR("", token);

    // 3. Verify

    // 4. Teardown

    return 0;
}
static int cdbm_test_transaction() 
{
    T_cdbm_trans_id trans_id_1, trans_id_2;
    T_global_rc ret_cod;

    // a typical unit test include 4 steps.
    // 1. Setup
    VASSERT_TRUE(gfi_list_is_empty(g_cdbm_db.trans_list));
    
    // 2. Execute
    trans_id_1 = cdbm_create_transaction("test_trans_test1");
    VASSERT_TRUE(trans_id_1!=NULL);
    VASSERT_EQ(1, gfi_list_size(g_cdbm_db.trans_list));

    trans_id_2 = cdbm_create_transaction("test_trans_test2");
    VASSERT_TRUE(trans_id_2!=NULL);
    VASSERT_EQ(2, gfi_list_size(g_cdbm_db.trans_list));

    ret_cod = cdbm_close_transaction(trans_id_1);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(1, gfi_list_size(g_cdbm_db.trans_list));

    ret_cod = cdbm_close_transaction(trans_id_1);
    VASSERT_EQ(RC_CDBM_TRANS_NOT_IN_LIST, ret_cod);
    VASSERT_EQ(1, gfi_list_size(g_cdbm_db.trans_list));

    ret_cod = cdbm_close_transaction(trans_id_2);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_TRUE(gfi_list_is_empty(g_cdbm_db.trans_list));

    // 3. Verify

    // 4. Teardown

    return 0;
}



static int cdbm_test_set_value() 
{
    char *expect;
    T_cdbm_trans_id trans_id;
    T_global_rc ret_cod;
    T_global_IP_ADDR ipv4_addr;
    T_global_IP6_ADDR ipv6_addr;
    T_global_IPNG_ADDR ipng_addr;

    // a typical unit test include 4 steps.

    // 1. Setup
    trans_id = cdbm_create_transaction("test_trans");
    VASSERT_TRUE(trans_id!=NULL);

    // ================================
    // 2. Execute

    ret_cod = cdbm_set_string(trans_id, "local", "/syslog/save-mode");
    VASSERT_EQ(RC_OK, ret_cod);

    ret_cod = cdbm_set_uint32(trans_id, 10, "/syslog/speed-uint");
    VASSERT_EQ(RC_OK, ret_cod);

    ret_cod = cdbm_set_int32(trans_id, -10, "/syslog/speed-int");
    VASSERT_EQ(RC_OK, ret_cod);

    ipv4_addr.S_ip_b.s_b1 = 10;
    ipv4_addr.S_ip_b.s_b2 = 10;
    ipv4_addr.S_ip_b.s_b3 = 0;
    ipv4_addr.S_ip_b.s_b4 = 4;
    ret_cod = cdbm_set_ipv4(trans_id, ipv4_addr, "/syslog/%s", "primary-ip");
    VASSERT_EQ(RC_OK, ret_cod);

    memset(&ipv6_addr, 0, sizeof(ipv6_addr));
    ipv6_addr.in6.addr8[0] = 0xff;
    ipv6_addr.in6.addr8[1] = 0x02;
    ipv6_addr.in6.addr8[15] = 0x1;
    ret_cod = cdbm_set_ipv6(trans_id, ipv6_addr, "/syslog/secondary-ip");
    VASSERT_EQ(RC_OK, ret_cod);

    ipng_addr.ipVer = IP_VERSION_4;
    ipng_addr.ipng_ip4.S_ip_b.s_b1 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b2 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b3 = 0;
    ipng_addr.ipng_ip4.S_ip_b.s_b4 = 6;
    ret_cod = cdbm_set_ipng(trans_id, ipng_addr, "/%s/ipng-v%d", "syslog", 4);
    VASSERT_EQ(RC_OK, ret_cod);

    memset(&ipng_addr, 0, sizeof(ipng_addr));
    ipng_addr.ipVer = IP_VERSION_6;
    ipng_addr.ipng_ip6.in6.addr8[0] = 0xff;
    ipng_addr.ipng_ip6.in6.addr8[1] = 0x02;
    ipng_addr.ipng_ip6.in6.addr8[15] = 0x6;
    ret_cod = cdbm_set_ipng(trans_id, ipng_addr, "/%s/ipng-v%d", "syslog", 6);
    VASSERT_EQ(RC_OK, ret_cod);
    

    ret_cod = cdbm_set_string(trans_id, "aa:bb:cc:dd:ee:ff", "/syslog/local-mac");
    VASSERT_EQ(RC_OK, ret_cod);

        
    // 3. Verify
    expect = 
        "/syslog/save-mode local\n"
        "/syslog/speed-uint 10\n"
        "/syslog/speed-int -10\n"
        "/syslog/primary-ip 10.10.0.4\n"
        "/syslog/secondary-ip ff02::1\n"
        "/syslog/ipng-v4 10.10.0.6\n"
        "/syslog/ipng-v6 ff02::6\n"
        "/syslog/local-mac aa:bb:cc:dd:ee:ff\n" ;
        
    VASSERT_EQ_STR(expect, trans_id->plain_config->buffer);
    
    // 4. Teardown
    ret_cod = cdbm_close_transaction(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);
    
    return 0;
}


static int cdbm_test_mo_leaf()
{
    const char *config_string = "/syslog/speed-uint 10";

    extern T_test_config g_test_config;

    T_cdbm_trans_id trans_id;
    T_global_rc ret_cod;

    // a typical unit test include 4 steps.
    // 1. Setup
    memset(&g_test_config, 0, sizeof(g_test_config));

    trans_id = cdbm_create_transaction("test_trans");
    VASSERT_TRUE(trans_id!=NULL);
    cdbm_set_conf_string(trans_id, config_string);
    VASSERT_EQ_STR(config_string, trans_id->plain_config->buffer);

    // 2. Execute
    ret_cod = test_read_config_leaf(trans_id);
    
    // 3. Verify
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(10, g_test_config.speed_uint);

    // 4. Teardown
    ret_cod = cdbm_close_transaction(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);

    return 0;
}


static int cdbm_test_mo_group()
{
    const char *config_string =
        "/syslog/save-mode local\n"
        "/syslog/speed-uint 10\n"
        "/syslog/speed-int -10\n"
        "/syslog/primary-ip 10.10.0.4\n"
        "/syslog/secondary-ip ff02::1\n"
        "/syslog/ipng-v4 10.10.0.6\n"
        "/syslog/ipng-v6 ff02::6\n"
        "/syslog/local-mac aa:bb:cc:dd:ee:ff\n";

    extern T_test_config g_test_config;

    T_cdbm_trans_id trans_id;
    T_global_rc ret_cod;
    T_global_IP_ADDR ipv4_addr;
    T_global_IP6_ADDR ipv6_addr;
    T_global_IPNG_ADDR ipng_addr;

    // a typical unit test include 4 steps.

    // 1. Setup
    memset(&g_test_config, 0, sizeof(g_test_config));

    trans_id = cdbm_create_transaction("test_trans");
    VASSERT_TRUE(trans_id!=NULL);

    cdbm_set_conf_string(trans_id, config_string);
    VASSERT_EQ_STR(config_string, trans_id->plain_config->buffer);

    // 2. Execute
    ret_cod= test_read_config_group(trans_id);

    // 3. Verify
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("local", g_test_config.save_mode);
    VASSERT_EQ(10, g_test_config.speed_uint);
    VASSERT_EQ(-10, g_test_config.speed_int);

    ipv4_addr.S_ip_b.s_b1 = 10;
    ipv4_addr.S_ip_b.s_b2 = 10;
    ipv4_addr.S_ip_b.s_b3 = 0;
    ipv4_addr.S_ip_b.s_b4 = 4;
    VASSERT_EQ(ipv4_addr.S_addr, g_test_config.pri_ip.S_addr);

    memset(&ipv6_addr, 0, sizeof(ipv6_addr));
    ipv6_addr.in6.addr8[0] = 0xff;
    ipv6_addr.in6.addr8[1] = 0x02;
    ipv6_addr.in6.addr8[15] = 0x1;
    VASSERT_TRUE(IP6_ADDR_EQUAL(ipv6_addr, g_test_config.sec_ip));

    ipng_addr.ipVer = IP_VERSION_4;
    ipng_addr.ipng_ip4.S_ip_b.s_b1 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b2 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b3 = 0;
    ipng_addr.ipng_ip4.S_ip_b.s_b4 = 6;
    VASSERT_TRUE(IPNG_ADDR_EQUAL(ipng_addr, g_test_config.ipng_v4));

    memset(&ipng_addr, 0, sizeof(ipng_addr));
    ipng_addr.ipVer = IP_VERSION_6;
    ipng_addr.ipng_ip6.in6.addr8[0] = 0xff;
    ipng_addr.ipng_ip6.in6.addr8[1] = 0x02;
    ipng_addr.ipng_ip6.in6.addr8[15] = 0x6;
    VASSERT_TRUE(IPNG_ADDR_EQUAL(ipng_addr, g_test_config.ipng_v6));

    VASSERT_EQ_STR("aa:bb:cc:dd:ee:ff", g_test_config.local_mac);

    // 4. Teardown
    ret_cod = cdbm_close_transaction(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);

    return 0;
}
