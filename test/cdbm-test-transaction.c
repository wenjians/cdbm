
#include "aat-incl.h"
#include "vunit.h"

#include "cdbm-types.h"
#include "cdbm-datamodel.h"
#include "cdbm-lib.h"
#include "cdbm-test.h"

#include "cdbm-global-data.h"

int cdbm_test_transaction_manage()
{
    // a typical unit test include 4 steps.
    // 1. Setup
    uint32 cur_trans_size;
    T_global_rc ret_code;
    //T_cdbm_trans_id trans_id;
    T_cdbm_trans_id trans_id_1, trans_id_2;


    // 2. Execute

    ret_code = cdbm_trans_delete(NULL);
    VASSERT_EQ(RC_CDBM_TRANS_IS_NULL, ret_code);

    cur_trans_size = gfi_list_size(g_cdbm_data.trans_list);
    trans_id_1 = cdbm_trans_create("test_trans_test1");
    VASSERT_TRUE(trans_id_1!=NULL);
    VASSERT_EQ(cur_trans_size+1, gfi_list_size(g_cdbm_data.trans_list));

    trans_id_2 = cdbm_trans_create("test_trans_test2");
    VASSERT_TRUE(trans_id_2!=NULL);
    VASSERT_EQ(cur_trans_size+2, gfi_list_size(g_cdbm_data.trans_list));

    ret_code = cdbm_trans_delete(trans_id_1);
    VASSERT_EQ(RC_OK, ret_code);
    VASSERT_EQ(cur_trans_size+1, gfi_list_size(g_cdbm_data.trans_list));

    ret_code = cdbm_trans_delete(trans_id_1);
    VASSERT_EQ(RC_CDBM_TRANS_NOT_IN_LIST, ret_code);
    VASSERT_EQ(cur_trans_size+1, gfi_list_size(g_cdbm_data.trans_list));

    ret_code = cdbm_trans_delete(trans_id_2);
    VASSERT_EQ(RC_OK, ret_code);
    VASSERT_TRUE(gfi_list_is_empty(g_cdbm_data.trans_list));

    // 3. Verify

    // 4. Teardown

    return 0;
}

int cdbm_test_transaction_leaf()
{
    // a typical unit test include 4 steps.
    // 1. Setup

    //char *expect;
    T_cdbm_trans_id trans_id;
    T_global_rc ret_cod;
    /*
    T_global_IP_ADDR ipv4_addr;
    T_global_IP6_ADDR ipv6_addr;
    T_global_IPNG_ADDR ipng_addr;
    */

    // a typical unit test include 4 steps.

    // 1. Setup
    trans_id = cdbm_trans_create("cdbm_test_trans");
    VASSERT_TRUE(trans_id!=NULL);

    // ================================
    // 2. Execute
    ret_cod = cdbm_set_uint32(trans_id, 10, "/test-types/speed-uint");
    VASSERT_EQ(RC_OK, ret_cod);

    /*
    ret_cod = cdbm_set_string(trans_id, "local", "/test-types/save-mode");
    VASSERT_EQ(RC_OK, ret_cod);


    ret_cod = cdbm_set_int32(trans_id, -10, "/test-types/speed-int");
    VASSERT_EQ(RC_OK, ret_cod);

    ipv4_addr.S_ip_b.s_b1 = 10;
    ipv4_addr.S_ip_b.s_b2 = 10;
    ipv4_addr.S_ip_b.s_b3 = 0;
    ipv4_addr.S_ip_b.s_b4 = 4;
    ret_cod = cdbm_set_ipv4(trans_id, ipv4_addr, "/test-types/%s", "primary-ip");
    VASSERT_EQ(RC_OK, ret_cod);

    memset(&ipv6_addr, 0, sizeof(ipv6_addr));
    ipv6_addr.in6.addr8[0] = 0xff;
    ipv6_addr.in6.addr8[1] = 0x02;
    ipv6_addr.in6.addr8[15] = 0x1;
    ret_cod = cdbm_set_ipv6(trans_id, ipv6_addr, "/test-types/secondary-ip");
    VASSERT_EQ(RC_OK, ret_cod);

    ipng_addr.ipVer = IP_VERSION_4;
    ipng_addr.ipng_ip4.S_ip_b.s_b1 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b2 = 10;
    ipng_addr.ipng_ip4.S_ip_b.s_b3 = 0;
    ipng_addr.ipng_ip4.S_ip_b.s_b4 = 6;
    ret_cod = cdbm_set_ipng(trans_id, ipng_addr, "/%s/ipng-v%d", "test-types", 4);
    VASSERT_EQ(RC_OK, ret_cod);

    memset(&ipng_addr, 0, sizeof(ipng_addr));
    ipng_addr.ipVer = IP_VERSION_6;
    ipng_addr.ipng_ip6.in6.addr8[0] = 0xff;
    ipng_addr.ipng_ip6.in6.addr8[1] = 0x02;
    ipng_addr.ipng_ip6.in6.addr8[15] = 0x6;
    ret_cod = cdbm_set_ipng(trans_id, ipng_addr, "/%s/ipng-v%d", "test-types", 6);
    VASSERT_EQ(RC_OK, ret_cod);


    ret_cod = cdbm_set_string(trans_id, "aa:bb:cc:dd:ee:ff", "/test-types/local-mac");
    VASSERT_EQ(RC_OK, ret_cod);

    ret_cod = cdbm_trans_commit(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);

    ret_cod = cdbm_trans_get_result(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);
    */

    // 3. Verify
#if 0
    expect =
     "/test-types/save-mode local\n"
     "/test-types/speed-uint 10\n"
     "/test-types/speed-int -10\n"
     "/test-types/primary-ip 10.10.0.4\n"
     "/test-types/secondary-ip ff02::1\n"
     "/test-types/ipng-v4 10.10.0.6\n"
     "/test-types/ipng-v6 ff02::6\n"
     "/test-types/local-mac aa:bb:cc:dd:ee:ff\n" ;

    VASSERT_EQ_STR(expect, trans_id->plain_config->buffer);
#endif

    // 4. Teardown
    ret_cod = cdbm_trans_delete(trans_id);
    VASSERT_EQ(RC_OK, ret_cod);

    return 0;
}
