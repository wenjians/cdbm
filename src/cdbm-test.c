
/*****************************************************************************
 * this file is used to unit test for CDBM
 ****************************************************************************/
#include "aat-incl.h"
#include "return_codes.h"
#include "vunit.h"

#include "cdbm-lib.h"


/***************************************************************************
 * local prototype
 **************************************************************************/
int main();
static int cdbm_test_transaction();
static int cdbm_test_set_value() ;


VTestCase cdbm_test_cases[] = {
    DEF_TEST_CASE(cdbm_test_transaction),
    DEF_TEST_CASE(cdbm_test_set_value),

    // add one line before here for every test case
    {NULL, NULL} // don't remove, end of array!
};



int main()
{
    runAll();

    return 0;
}

/***************************************************************************
 * test case definitions
 **************************************************************************/

static int cdbm_test_transaction() 
{
    T_cdbm_trans_id trans_id;
    T_global_rc ret_cod;

    // a typical unit test include 4 steps.
    // 1. Setup
    
    // 2. Execute
    trans_id = cdbm_create_transaction("test_trans");
    VASSERT_TRUE(trans_id!=NULL);

    ret_cod = cdbm_close_transaction(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);

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
    

    ret_cod = cdbm_set_string(trans_id, "aa:bb:cc:dd:ee:ff:01", "/syslog/local-mac");
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
        "/syslog/local-mac aa:bb:cc:dd:ee:ff:01\n" ;
        
    VASSERT_EQ_STR(expect, trans_id->plain_config->buffer);
    
    // 4. Teardown
    ret_cod = cdbm_close_transaction(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);
    
    return 0;
}
