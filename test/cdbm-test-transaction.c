
#include "aat-incl.h"
#include "vunit.h"

#include "cdbm-types.h"
#include "cdbm-data-model.h"
#include "cdbm-lib.h"
#include "cdbm-test.h"


int cdbm_test_transaction_manage()
{
    // a typical unit test include 4 steps.
    // 1. Setup
    T_global_rc ret_code;
    T_cdbm_trans_id trans_id;

    trans_id = cdbm_malloc(CDBM_TRANSACTION_SIZE);
    AAT_STD_ASSERT(trans_id != NULL) ;

    memset(trans_id, 0, CDBM_TRANSACTION_SIZE);
    gfi_list_init(&trans_id->list_node);

    // 2. Execute
    ret_code = cdbm_trans_delete(trans_id);
    VASSERT_EQ(RC_CDBM_TRANS_NOT_IN_LIST, ret_code);

    ret_code = cdbm_trans_delete(NULL);
    VASSERT_EQ(RC_CDBM_TRANS_IS_NULL, ret_code);

    trans_id = cdbm_trans_create("cdbm_trans_test");
    ret_code = cdbm_trans_delete(trans_id);
    VASSERT_EQ(RC_OK, ret_code);

    // 3. Verify

    // 4. Teardown

    return 0;
}

int cdbm_test_transaction_local()
{
    // a typical unit test include 4 steps.
    // 1. Setup

    // 2. Execute

    /* test the range of uint32 */


    // 3. Verify

    // 4. Teardown

    return 0;
}
