/*******************************************************************************
**
** Description   : this file is for the Venus Unit Test Framework. 
**                 source file(.c) that contain the test cases  should include 
**                 this header file to use series of ASSERT_XX micros
**
**                 Vunit is a very simple unit test framework designated 
**                 to run in 7510 runtime mode.
**                 It assumes that the test case run in the UI context.
*******************************************************************************/

#ifndef __VUNIT_HEADER
#define __VUNIT_HEADER

// vunit depend on a printf function, currently we assume vunit run in 7510 UI,
// so uiPrintf is being used, if you are porting vunit to other system, should
// redfine it properly.

#ifndef VUNIT_PRINTF
#define VUNIT_PRINTF printf
#endif

// assert equal for integer values(char, short, int32, uint32, pointer...)
#define VASSERT_EQ(expect, actual) \
do {\
    if((expect) != (actual)) {\
        VUNIT_PRINTF("%s(%d): expect<0x%x:%d:%u>, but was<0x%x:%d:%u>\n", \
            __FUNCTION__, __LINE__, (unsigned int)(expect), expect, expect, \
            (unsigned int)(actual), actual, actual);\
        return -1;\
    }\
}while(0)

// assert equal for long long values(long long, unsigned long long)
#define VASSERT_LL_EQ(expect, actual) \
do {\
    if((expect) != (actual)) {\
        VUNIT_PRINTF("%s(%d): expect<0x%llx>, but was<0x%llx>\n", \
            __FUNCTION__, __LINE__, (unsigned long long)(expect), (unsigned long long)(actual));\
        return -1;\
    }\
}while(0)

// assert true, definition of "true" is assumed to comply with C language
#define VASSERT_TRUE(actual) \
do {\
    if(!(actual)) {\
        VUNIT_PRINTF("%s(%d): expect true, but was<0x%x>\n", \
            __FUNCTION__, __LINE__, (unsigned int)(actual));\
        return -1;\
    }\
}while(0)

// assert false, definition of "false" is assumed to comply with C language
#define VASSERT_FALSE(actual) \
do {\
    if(actual) {\
        VUNIT_PRINTF("%s(%d): expect false, but was<0x%x>\n", \
            __FUNCTION__, __LINE__, (unsigned int)(actual));\
        return -1;\
    }\
}while(0)


// assert string comparation
#define VASSERT_EQ_STR(expect, actual) \
do {\
    if(strcmp(expect, actual)!=0) {\
        VUNIT_PRINTF("%s(%d): expect<%s>, but was<%s>\n", \
            __FUNCTION__, __LINE__, expect, actual);\
        return -1;\
    }\
}while(0)


// assert string comparation
#define VASSERT_IEQ_STR(expect, actual) \
do {\
    if(stricmp(expect, actual)!=0) {\
        VUNIT_PRINTF("%s(%d): expect<%s>, but was<%s>\n", \
            __FUNCTION__, __LINE__, expect, actual);\
        return -1;\
    }\
}while(0)


// maybe you need more assert, for example assert_str_equal, but it make vunit dependent on 
// other library, sometimes it is not always available, but we can overcome it by a simple trick,
// for example, use
//   ASSERT_EQ(strcmp(expect, actual), 0);
// to assert string compare
    
// this macro is used to simplify the definition of test case.    
// DEF_TEST_CASE(foo_test_case1) => {"foo_test_case1", foo_test_case1}
#define DEF_TEST_CASE(func_name)  {#func_name, func_name}


// function proto type of test case function
// return value:
//     0:     test pass
//     other: not pass
typedef int (*VunitTestCaseFunc)(void);

typedef struct _VTestCase {
    char *name;  // unique name of test case in test suite, usually is the test function name
                 // usually is the test function name
    VunitTestCaseFunc func;
} VTestCase;

typedef struct _VTestSuite {
    char *name; // unique name of test suite, usually is the test c file name without ".c"
    VTestCase *cases; // start address of array of TestCase(should be null-terminated)
} VTestSuite;


void runAll();
void runOneSuite(VTestSuite *tsuite);
void runOneCase(VTestCase *tcase);

#endif /* __VUNIT_HEADER */

