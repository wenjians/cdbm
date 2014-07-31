

#ifndef CDBM_TEST_CLIENT_H
#define CDBM_TEST_CLIENT_H


typedef struct T_test_config {
    //uint32 save_mode;
    char save_mode[16];
    uint32 speed_uint;
    int32  speed_int;
    T_global_IP_ADDR pri_ip;
    T_global_IP6_ADDR sec_ip;
    T_global_IPNG_ADDR ipng_v4;
    T_global_IPNG_ADDR ipng_v6;
    char local_mac[IPNG_MAC_STRING_LENGTH];
} T_test_config;

int test_read_config_leaf(const T_cdbm_trans_id trans_id);
int test_read_config_group(const T_cdbm_trans_id trans_id);

#endif /* CDBM_TEST_CLIENT_H */
