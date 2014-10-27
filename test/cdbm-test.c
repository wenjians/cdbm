
/*****************************************************************************
 * this file is used to unit test for CDBM
 ****************************************************************************/
#include "aat-incl.h"
#include "return_codes.h"
#include "vunit.h"

#include "gfi-list.h"

#include "cdbm-lib.h"
#include "cdbm-types.h"
#include "cdbm-datamodel.h"
#include "cdbm-global-data.h"


#include "cdbm-test.h"
#include "cdbm-test-client.h"

/***************************************************************************
 * local prototype
 **************************************************************************/
int main();

extern T_cdbm_dm_typedef cdbm_test_cm_typedef[];
extern T_cdbm_dm_node cdbm_test_dm_node[] ;

#if 0
static int cdbm_test_misc_lib();
static int cdbm_test_transaction();
static int cdbm_test_set_value() ;
static int cdbm_test_mo_leaf();
static int cdbm_test_mo_group();
static int cdbm_test_cm_hash_search();
#endif




VTestCase cdbm_test_cases[] = {    

    // the following is about the different value types
    DEF_TEST_CASE(cdbm_test_range),
    DEF_TEST_CASE(cdbm_test_uint32),
    DEF_TEST_CASE(cdbm_test_int32),
    DEF_TEST_CASE(cdbm_test_ipaddress),

    // the following is about the transaction management
    DEF_TEST_CASE(cdbm_test_transaction_manage),
    DEF_TEST_CASE(cdbm_test_transaction_leaf),


    /*
    DEF_TEST_CASE(cdbm_test_misc_lib),
    DEF_TEST_CASE(cdbm_test_transaction),
    DEF_TEST_CASE(cdbm_test_set_value),
    DEF_TEST_CASE(cdbm_test_mo_leaf),
    DEF_TEST_CASE(cdbm_test_mo_group),
    DEF_TEST_CASE(cdbm_test_cm_hash_search),
    */

    // add one line before here for every test case
    {NULL, NULL} // don't remove, end of array!
};



int main()
{
    T_global_rc ret_cod;

    /* init phase 1 */
    gfi_list_mgr_init(1);
    cdbm_lib_init(1);

    /* init phase 2 */
    gfi_list_mgr_init(2);
    cdbm_lib_init(2);
    
    cdbm_dm_attach_data(cdbm_test_dm_node,    cdbm_test_get_dm_node_items(),
                        cdbm_test_cm_typedef, cdbm_test_get_typedef_items());

    //cdbm_test_print_size();

    ret_cod = cdbm_dm_init();
    AAT_STD_ASSERT(ret_cod == RC_OK);


    //cdbm_dm_node_printall();
    runAll();

    return 0;
}

/***************************************************************************
 * test case definitions
 **************************************************************************/

#if 0
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
    VASSERT_TRUE(ip4_is_addr_equal(ipv4_addr, g_test_config.pri_ip));

    memset(&ipv6_addr, 0, sizeof(ipv6_addr));
    ipv6_addr.in6.addr8[0] = 0xff;
    ipv6_addr.in6.addr8[1] = 0x02;
    ipv6_addr.in6.addr8[15] = 0x1;
    VASSERT_TRUE(ip6_is_addr_equal(&ipv6_addr, &g_test_config.sec_ip));

    ipng_addr.ipVer = IP_VERSION_4;
    ipng_addr.ipng_ip4.S_ip_b.s_b1 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b2 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b3 = 0;
    ipng_addr.ipng_ip4.S_ip_b.s_b4 = 6;
    VASSERT_TRUE(ipng_is_addr_equal(&ipng_addr, &g_test_config.ipng_v4));

    memset(&ipng_addr, 0, sizeof(ipng_addr));
    ipng_addr.ipVer = IP_VERSION_6;
    ipng_addr.ipng_ip6.in6.addr8[0] = 0xff;
    ipng_addr.ipng_ip6.in6.addr8[1] = 0x02;
    ipng_addr.ipng_ip6.in6.addr8[15] = 0x6;
    VASSERT_TRUE(ipng_is_addr_equal(&ipng_addr, &g_test_config.ipng_v6));

    VASSERT_EQ_STR("aa:bb:cc:dd:ee:ff", g_test_config.local_mac);

    // 4. Teardown
    ret_cod = cdbm_close_transaction(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);

    return 0;
}

static int cdbm_test_cm_hash_search()
{
    T_cdbm_dm_node *cm_node;
    cm_node = cdbm_dm_get_node_from_keypath("/");
    VASSERT_TRUE(cdbm_dm_is_root(cm_node));

    cm_node = cdbm_dm_get_node_from_keypath("/ip-realm/realm-table/ip-if");
    VASSERT_TRUE(cmbm_dm_is_list(cm_node));

    cm_node = cdbm_dm_get_node_from_keypath("/ip-realm/realm-table/vmg-id");
    VASSERT_TRUE(cmbm_dm_is_leaf_list(cm_node));

    cm_node = cdbm_dm_get_node_from_keypath("/ip-realm/realm-table/cp-monitoring/admin-state");
    VASSERT_TRUE(cmbm_dm_is_leaf(cm_node));

    cm_node = cdbm_dm_get_node_from_keypath("/test");
    VASSERT_TRUE(cm_node==NULL);

    return 0;
}
#endif

