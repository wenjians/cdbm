
#include "aat-incl.h"
#include "vunit.h"

#include "cdbm-types.h"
#include "cdbm-data-model.h"
#include "cdbm-test.h"



int cdbm_test_range()
{
    // a typical unit test include 4 steps.
    // 1. Setup

    // 2. Execute

    /* test the range of uint32 */
    VASSERT_TRUE(cdbm_val_uint32_in_range(10, "0..10"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(0, "0..10"));
    VASSERT_FALSE(cdbm_val_uint32_in_range(-1, "0..10"));
    VASSERT_FALSE(cdbm_val_uint32_in_range(11, "0..10"));

    VASSERT_TRUE(cdbm_val_uint32_in_range(0x50, "0x10..0X100"));

    VASSERT_TRUE(cdbm_val_uint32_in_range(5, "0..10|20|30..100"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(20, "0..10|20|30..100"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(30, "0..10|20 | 30..100"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(40, "0..10|20|30  ..  100"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(100, "0..10|20|30..100"));
    VASSERT_FALSE(cdbm_val_uint32_in_range(21, "0..10|20|30..100"));
    VASSERT_FALSE(cdbm_val_uint32_in_range(15, "0..10|20|30..100"));


    VASSERT_TRUE(cdbm_val_uint32_in_range(321, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(600, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_FALSE(cdbm_val_uint32_in_range(1001, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_FALSE(cdbm_val_uint32_in_range(490, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_TRUE(cdbm_val_uint32_in_range(0xf000000f, "0..0xff000000"));


    

    /* test the range of int32 */
    VASSERT_TRUE(cdbm_val_int32_in_range(10, "-10..10"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-10, "-10..10"));
    VASSERT_TRUE(cdbm_val_int32_in_range(0, "-10..10"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-5, "-10..10"));
    VASSERT_TRUE(cdbm_val_int32_in_range(5, "-10..10"));
    VASSERT_FALSE(cdbm_val_int32_in_range(-11, "-10..10"));
    VASSERT_FALSE(cdbm_val_int32_in_range(20, "-10..10"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-50, "-100..-10"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-20, "-100..0X100"));

    VASSERT_TRUE(cdbm_val_int32_in_range(-935, "-1000..-110|-20|-10..100|500..10000"));
    VASSERT_FALSE(cdbm_val_int32_in_range(-50, "-1000..-110| -20  | -10  ..100  |500..10000"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-90, "-1000..-110|-90|-30..100|500..10000"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-5, "-1000..-110|-20|-10..100|500..10000"));
    VASSERT_TRUE(cdbm_val_int32_in_range(501, "-1000..-110|-20|-10..100|500..10000"));
    VASSERT_FALSE(cdbm_val_int32_in_range(-30, "-1000..-110|-20|-10..100|500..10000"));
    VASSERT_FALSE(cdbm_val_int32_in_range(499, "-1000..-110|-20|-10..100|500..10000"));

    VASSERT_TRUE(cdbm_val_int32_in_range(321, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_TRUE(cdbm_val_int32_in_range(600, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_FALSE(cdbm_val_int32_in_range(1001, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_FALSE(cdbm_val_int32_in_range(490, "0..10|20|30..100|320..331|500..1000"));
    VASSERT_TRUE(cdbm_val_int32_in_range(-0xfffffff0, "-0xfffffff0..0xf000000"));
    VASSERT_FALSE(cdbm_val_int32_in_range(0xf000001, "-0xfffffff0..0xf000000"));

    // 3. Verify

    // 4. Teardown

    return 0;    
}


int cdbm_test_uint32()
{
#define CDBM_TEST_UINT32_BUF_LEN 128    
    char buffer[CDBM_TEST_UINT32_BUF_LEN];
    T_cdbm_value v1, v2;
    T_global_rc  ret_cod;
    T_cdbm_val_opt *val_opt;
    T_cdbm_dm_type dm_type = {
        .type_def_id = 0,
        .base_type   = CDBM_TYPE_UINT32,
        .type.t_int.range = "0..1000"
    };

    // a typical unit test include 4 steps.
    // 1. Setup
    val_opt = &g_cdbm_val_ops[CDBM_TYPE_UINT32];
    

    
    // 2. Execute

    /* the following is the sunny case */
    ret_cod = val_opt->str_to_val(&dm_type, "10", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(CDBM_TYPE_UINT32, v1.type);
    VASSERT_EQ(10, v1.val.u32);

    ret_cod = val_opt->str_to_val(&dm_type, "10 ", &v2);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(CDBM_TYPE_UINT32, v2.type);
    VASSERT_EQ(10, v2.val.u32);
    
    ret_cod = val_opt->val_to_str(&v1, buffer, CDBM_TEST_UINT32_BUF_LEN);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("10", buffer);

    VASSERT_TRUE(val_opt->val_eq(&v1, &v2));


    /* the follwoing is the runny case */
    ret_cod = val_opt->str_to_val(&dm_type, "10a", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "0x10", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "-10", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_RANGE, ret_cod);
    VASSERT_EQ(-10, v1.val.u32);
    ret_cod = val_opt->str_to_val(&dm_type, "10000", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_RANGE, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "x10", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "0xa0", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "0xe1", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "1a", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "a1", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);


    v1.type = CDBM_TYPE_INT32;
    VASSERT_FALSE(val_opt->val_eq(&v1, &v2));
    v1.type = CDBM_TYPE_UINT32;
    v2.type = CDBM_TYPE_INT32;
    VASSERT_FALSE(val_opt->val_eq(&v1, &v2));
    
    v1.type = CDBM_TYPE_UINT32;
    v1.val.u32 = 1000;
    v2.type = CDBM_TYPE_UINT32;
    v2.val.u32 = 1000;
    VASSERT_TRUE(val_opt->val_eq(&v1, &v2));
    v2.val.u32 = 1001;
    VASSERT_FALSE(val_opt->val_eq(&v1, &v2));

    v1.type = CDBM_TYPE_UINT32;
    v1.val.u32 = 1001;
    ret_cod = val_opt->val_valid(&dm_type, &v1);
    VASSERT_EQ(RC_CDBM_INVALID_RANGE, ret_cod);
    v1.val.u32 = -1;
    ret_cod = val_opt->val_valid(&dm_type, &v1);
    VASSERT_EQ(RC_CDBM_INVALID_RANGE, ret_cod);
    v1.val.u32 = 100;
    ret_cod = val_opt->val_valid(&dm_type, &v1);
    VASSERT_EQ(RC_OK, ret_cod);

    v1.type = CDBM_TYPE_UINT32;
    v1.val.u32 = 1000;
    ret_cod = val_opt->val_to_str(&v1, buffer, CDBM_TEST_UINT32_BUF_LEN);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("1000", buffer);
    ret_cod = val_opt->val_to_str(&v1, buffer, 3);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("10", buffer);
    ret_cod = val_opt->val_to_str(&v1, buffer, 2);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("1", buffer);
    
    // 3. Verify

    // 4. Teardown

    return 0;    
}



int cdbm_test_int32()
{
#define CDBM_TEST_UINT32_BUF_LEN 128
    char buffer[CDBM_TEST_UINT32_BUF_LEN];
    T_cdbm_value v1, v2;
    T_global_rc  ret_cod;
    T_cdbm_val_opt *val_opt;
    T_cdbm_dm_type dm_type = {
        .type_def_id = 1,
        .base_type   = CDBM_TYPE_INT32,
        .type.t_int.range = "-1000..1000"
    };

    // a typical unit test include 4 steps.
    // 1. Setup
    val_opt = &g_cdbm_val_ops[CDBM_TYPE_INT32];

    // 2. Execute

    /* the following is the sunny case */

    ret_cod = val_opt->str_to_val(&dm_type, "-10", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(CDBM_TYPE_INT32, v1.type);
    VASSERT_EQ(-10, v1.val.i32);

    ret_cod = val_opt->str_to_val(&dm_type, "10 ", &v2);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(CDBM_TYPE_INT32, v2.type);
    VASSERT_EQ(10, v2.val.i32);

    ret_cod = val_opt->val_to_str(&v1, buffer, CDBM_TEST_UINT32_BUF_LEN);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("-10", buffer);

    v2.val.i32 = -10;
    VASSERT_TRUE(val_opt->val_eq(&v1, &v2));


    /* the follwoing is the runny case */
    ret_cod = val_opt->str_to_val(&dm_type, "10a", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "0x10", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "-10", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ(-10, v1.val.u32);
    ret_cod = val_opt->str_to_val(&dm_type, "10000", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_RANGE, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "x10", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "0xa0", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "0xe1", &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "1a", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);
    ret_cod = val_opt->str_to_val(&dm_type, "a1", &v1);
    VASSERT_EQ(RC_CDBM_INVALID_STRING, ret_cod);


    v1.type = CDBM_TYPE_UINT32;
    VASSERT_FALSE(val_opt->val_eq(&v1, &v2));
    v1.type = CDBM_TYPE_UINT32;
    v2.type = CDBM_TYPE_INT32;
    VASSERT_FALSE(val_opt->val_eq(&v1, &v2));

    v1.type = CDBM_TYPE_INT32;
    v1.val.u32 = -1000;
    v2.type = CDBM_TYPE_INT32;
    v2.val.u32 = -1000;
    VASSERT_TRUE(val_opt->val_eq(&v1, &v2));
    v2.val.u32 = 1001;
    VASSERT_FALSE(val_opt->val_eq(&v1, &v2));

    v1.type = CDBM_TYPE_INT32;
    v1.val.u32 = 1001;
    ret_cod = val_opt->val_valid(&dm_type, &v1);
    VASSERT_EQ(RC_CDBM_INVALID_RANGE, ret_cod);
    v1.val.u32 = -1;
    ret_cod = val_opt->val_valid(&dm_type, &v1);
    VASSERT_EQ(RC_OK, ret_cod);
    v1.val.u32 = 100;
    ret_cod = val_opt->val_valid(&dm_type, &v1);
    VASSERT_EQ(RC_OK, ret_cod);

    v1.type = CDBM_TYPE_INT32;
    v1.val.u32 = 1000;
    ret_cod = val_opt->val_to_str(&v1, buffer, CDBM_TEST_UINT32_BUF_LEN);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("1000", buffer);
    ret_cod = val_opt->val_to_str(&v1, buffer, 3);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("10", buffer);
    ret_cod = val_opt->val_to_str(&v1, buffer, 2);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("1", buffer);

    v1.type = CDBM_TYPE_INT32;
    v1.val.u32 = -1000;
    ret_cod = val_opt->val_to_str(&v1, buffer, CDBM_TEST_UINT32_BUF_LEN);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("-1000", buffer);
    ret_cod = val_opt->val_to_str(&v1, buffer, 4);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("-10", buffer);
    ret_cod = val_opt->val_to_str(&v1, buffer, 2);
    VASSERT_EQ(RC_OK, ret_cod);
    VASSERT_EQ_STR("-", buffer);

    // 3. Verify

    // 4. Teardown

    return 0;
}
