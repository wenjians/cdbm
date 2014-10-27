

#ifndef _CDBM_TEST_H_
#define _CDBM_TEST_H_


T_global_rc cdbm_dm_node_printall();


/* the following will test different value types */
extern int cdbm_test_range();
extern int cdbm_test_uint32();
extern int cdbm_test_int32();
extern int cdbm_test_ipaddress();

extern int cdbm_test_transaction_manage();
extern int cdbm_test_transaction_leaf();


uint32 cdbm_test_get_typedef_items();
uint32 cdbm_test_get_dm_node_items();

#endif /* _CDBM_TEST_H_ */
