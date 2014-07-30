
/*******************************************************************************
**
** Description   : this file manage all test case and provide UIs to run it.
**                 
*******************************************************************************/

#include "aat-incl.h"

#include "vunit.h"


extern VTestCase cdbm_test_cases[];


VTestSuite VTEST_SUITE_LIST[] = {
    {"cdbm_test_cases", cdbm_test_cases},

    // add one line before this line for every new test suite,
    // usually the test suite name is the c file name without ".c"
    {NULL, NULL} // indicate the end of array
};




void runAll()
{
    VTestSuite *tsuite;
    VTestCase *tcase;
    VunitTestCaseFunc func;
    int result;
    int total;
    
    total = 0;
    tsuite = &VTEST_SUITE_LIST[0];
    while(tsuite->name != NULL) {
        tcase = tsuite->cases;
        while(tcase->name != NULL) {
            total++;

            func = tcase->func;
            result = (*func)();
            if(result != 0) {
                printf("%s failed, result:0x%08x\n", tcase->name, result);
                printf("test stopped!\n");
                return;
            }
            tcase++;
        }
        tsuite++;
    }

    printf("All test PASSED! total cases: %d\n", total);
}

void runOneSuite(VTestSuite *tsuite)
{
    VTestCase *tcase;
    VunitTestCaseFunc func;
    int result;
    int total;
    
    total = 0;
    tcase = tsuite->cases;
    while(tcase->name != NULL) {
        total++;

        func = tcase->func;
        result = (*func)();
        if(result != 0) {
            printf("%s failed, result:0x%08x\n", tcase->name, result);
            printf("test stopped!\n");
            return;
        }
        tcase++;
    }

    printf("All test PASSED! total cases: %d\n", total);
}

void runOneCase(VTestCase *tcase)
{
    VunitTestCaseFunc func;
    int result;

    func = tcase->func;
    result = (*func)();
    if(result != 0) {
        printf("%s failed, result:0x%08x\n", tcase->name, result);
    }
    printf("Test PASS!\n");
}



    

