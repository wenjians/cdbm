
/*****************************************************************************
 * this file is used to unit test for CDBM
 ****************************************************************************/
#include "aat-incl.h"
#include "vunit.h"

#include "cdbm-lib.h"


/***************************************************************************
 * local prototype
 **************************************************************************/
static int cdbm_test_set_value() ;


VTestCase cdbm_test_cases[] = {
    DEF_TEST_CASE(cdbm_test_set_value),

    // add one line before here for every test case
    {NULL, NULL} // don't remove, end of array!
};



/***************************************************************************
 * test case definitions
 **************************************************************************/

static int cdbm_test_set_value() {
    int a, b, result;

    // a typical unit test include 4 steps.

    // ================================
    // 1. Setup
    // in this part, you prepare test data, in real situation,
    // some time it might be very complex, and need a lot of effort,
    // you may think about ROI before coding.
    // anyway, make SURE your test code(in PART 2) ONLY based on this setup code,
    // we MUST NOT do special thing to run unit test, for example, setting config
    // using special chassis.
    a = 2;
    b = 1;
    // ================================


    // ================================
    // 2. Execute
    result = foo_max(a, b);
    // ================================


    // ================================
    // 3. Verify
    // in this part, output of step 2 can be asserted, the output might be return value,
    // output parameters, global varibles...
    // check macro VASSERT_EQ in vunit.h you will see a "return -1" once assert fail, that
    // will stop execution of test.
    VASSERT_EQ(a, result);
    // ================================

    // ================================
    // 4. Teardown
    // restore all change in the test, e.g. free memory, restore global variables
    // ================================
    return 0;
}
