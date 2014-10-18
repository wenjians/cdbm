
/*
 * this file include test data for following
 *
 * 1, CDBM configuration parameter tree
 * 2, CDBM running database
 * 3, CDBM data type definition
 */

#include "aat-incl.h"

#include "cdbm-lib.h"
#include "cdbm-db.h"
#include "cdbm-config-model.h"


/**************************************************************************************************
 ******               the following is CDBM configuration type definition                    ******
 *************************************************************************************************/

T_cdbm_type_enum_value test_enum_type_def_1[] = {{"enable", 1, 1}, {"disable", 1, 0}};

T_cdbm_dm_typedef cdbm_test_cm_typedef[] = {
    /*name          builtin defalt   union.type       */
    /*  derived:    typedef_idx     range,      pattern_count,   pattern_list */
    {"uint32",      1,      NULL,   .type.builtin = CDBM_TYPE_UINT32},  /* 0 */
    {"int32",       1,      NULL,   .type.builtin = CDBM_TYPE_INT32 },  /* 1 */
    {"ipv4",        1,      NULL,   .type.builtin = CDBM_TYPE_IPV4  },  /* 2 */
    {"ipv6",        1,      NULL,   .type.builtin = CDBM_TYPE_IPV6  },  /* 3 */
    {"ip-address",  1,      NULL,   .type.builtin = CDBM_TYPE_IPADDR},  /* 4 */
    {"mac-addrres", 1,      NULL,   .type.builtin = CDBM_TYPE_MAC   },  /* 5 */
    {"hex",         1,      NULL,   .type.builtin = CDBM_TYPE_HEX   },  /* 6 */
    {"string",      1,      NULL,   .type.builtin = CDBM_TYPE_STRING},  /* 7 */
    {"str-word",    1,      NULL,   .type.builtin = CDBM_TYPE_STR_WORD},/* 8 */
    {"enum",        1,      NULL,   .type.builtin = CDBM_TYPE_ENUM_HASH}, /* 9 */
    {"iprealm-name",0,      NULL,
        .type.derived = {8, CDBM_TYPE_STRING,   .type.t_string = { "1..51",    3,  {"[A-Za-z0-9\\+-&!_/\\'\\?@\\^`~\\*\\$\\/%\\|\\.]", "(?!none)", "(?!TGW_default_sys_realm)"}}}
    }, /* 10 */

    {"vlan-id",     0,      NULL,
        .type.derived = {0, CDBM_TYPE_UINT32,   .type.t_int = {"0 | 2..4094"}}
    }, /* 11 */

    {"vmg-id",      0,      NULL,
        .type.derived = {0, CDBM_TYPE_UINT32,   .type.t_int = {"0..255"     }}
    }, /* 12 */

    {"admin-state", 0,      NULL,
        .type.derived = {9, CDBM_TYPE_ENUM_HASH, .type.t_enum = {2, test_enum_type_def_1}}
    }, /* 13 */

    {"eth-lpa",     0,      NULL,
        .type.derived = {8, CDBM_TYPE_STR_WORD, .type.t_string = { NULL,    1,  {"en.scm.[0-9]|en.pim[0-32].[0-8]"}}}
    }, /* 14 */

    {"lpa",         0,      NULL,
        .type.derived = {14, CDBM_TYPE_STR_WORD, .type.t_string = { "1..32" , 0,  {NULL}}}
    }, /* 15 */

};

uint32 cdbm_test_get_sizeof_typedef()
{
    return sizeof(cdbm_test_cm_typedef);
}

/**************************************************************************************************
 ******            the following is type declaration which is used for leaf/leaf_list     *********
 *************************************************************************************************/
/* some predefined IP definition */
T_cdbm_dm_type test_cm_type_uint32 = { 0, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_int32  = { 1, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_ipv4   = { 2, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_ipv6   = { 3, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_ipaddr = { 4, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_mac    = { 5, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_hex    = { 6, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_string = { 7, CDBM_TYPE_EMPTY, .type.t_empty   = {0}};
T_cdbm_dm_type test_cm_type_str_word = { 8, CDBM_TYPE_EMPTY,.type.t_empty  = {0}};

T_cdbm_type_enum_value test_enum_type_2[] = {{"remote", 1, 1}, {"local", 1, 2}, {"none", 1, 3}};
T_cdbm_dm_type test_cm_type_2 = { 9, CDBM_TYPE_ENUM_HASH,   .type.t_enum = {3, test_enum_type_def_1}};
T_cdbm_dm_type test_cm_type_7 = { 0, CDBM_TYPE_UINT32,      .type.t_int = {"10..500"}};
T_cdbm_dm_type test_cm_type_8 = { 1, CDBM_TYPE_INT32,       .type.t_int  = {"-50..50 | 100 | 200..300"}};
T_cdbm_dm_type test_cm_type_10= { 7, CDBM_TYPE_STRING,      .type.t_string  = {"1..20"}};
T_cdbm_dm_type test_cm_type_11= { 8, CDBM_TYPE_STR_WORD,    .type.t_string  = {"1..10"}};


T_cdbm_dm_type test_cm_type_16 = {10, CDBM_TYPE_STR_WORD,    .type.t_string  = {NULL}};
T_cdbm_dm_type test_cm_type_19 = {11, CDBM_TYPE_UINT32,      .type.t_int  = {NULL}};
T_cdbm_type_enum_value test_enum_type_20[] = {{"system", 0, 0}, {"vmg-specific", 0, 0}};
T_cdbm_dm_type test_cm_type_20 = { 9, CDBM_TYPE_ENUM_HASH,   .type.t_enum = {2, test_enum_type_20}};
T_cdbm_dm_type test_cm_type_21 = { 8, CDBM_TYPE_STR_WORD,    .type.t_string = {"1..20"}};
T_cdbm_dm_type test_cm_type_22 = {13, CDBM_TYPE_EMPTY,       .type.t_empty = {0}};
T_cdbm_type_enum_value test_enum_type_23[] = { {"on-first", 1, 0}, {"on-last", 1, 1}};
T_cdbm_dm_type test_cm_type_23 = { 9, CDBM_TYPE_ENUM_HASH,   .type.t_enum = {2, test_enum_type_23}};
T_cdbm_type_enum_value test_enum_type_25[] = { {"specific", 1, 0}, {"root", 1, 1}, {"none", 1, 2}};
T_cdbm_dm_type test_cm_type_25 = { 9, CDBM_TYPE_ENUM_HASH,   .type.t_enum = {3, test_enum_type_25}};
T_cdbm_dm_type test_cm_type_26 = {12, CDBM_TYPE_UINT32,      .type.t_empty= {0}};
T_cdbm_dm_type test_cm_type_30 = {15, CDBM_TYPE_STR_WORD,    .type.t_empty= {0}};
T_cdbm_dm_type test_cm_type_33 = { 0, CDBM_TYPE_UINT32,      .type.t_int  = {"10..1000"}};
T_cdbm_dm_type test_cm_type_34 = { 0, CDBM_TYPE_UINT32,      .type.t_int  = {"10..1000"}};
T_cdbm_dm_type test_cm_type_35 = { 0, CDBM_TYPE_UINT32,      .type.t_int  = {"0..3600000"}};


/**************************************************************************************************
 ******               the following is CDBM configuration parameter model                    ******
 *************************************************************************************************/

T_cdbm_dm_node cdbm_test_cm_node[] = {
    /* node type                config, name,           parent  child,  sibling must,       xpath */
    /*  leaf:   default, mandatory, type    */
    {CDBM_NODE_TYPE_CONTAINER,  false,  NULL,           -1,     1,      -1,     NULL,       "/",
        .node.container = {0} /*,
        .hh = {NULL}*/
    }, /* 0 : reserved for root */
    
    {CDBM_NODE_TYPE_CONTAINER,  true,   "test-types",   0,      2,      13,     NULL,       "/test-types",
        .node.container = {0} /*,
        .hh = {NULL} */
    }, /* 1 */

    {CDBM_NODE_TYPE_LEAF,       true,   "save-mode",    1,      -1,     3,      NULL,       "/test-types/save-mode",
        .node.leaf = {NULL,      false,     &test_cm_type_2} /*,
        .hh = {NULL} */
    }, /* 2 */

    {CDBM_NODE_TYPE_LEAF,       true,   "primary-ip",   1,      -1,     4,      NULL,       "/test-types/primary-ip",
        .node.leaf = {"0.0.0.0", false,     &test_cm_type_ipv4} /*,
        .hh = {NULL}*/
    }, /* 3 */

    {CDBM_NODE_TYPE_LEAF,       true,   "second-ip",    1,      -1,     5,      "/test-types/primary-ip != 0.0.0.0",       "/test-types/second-ip",
        .node.leaf = {"::",      false,     &test_cm_type_ipv6} /*,
        .hh = {NULL} */
    }, /* 4 */

    {CDBM_NODE_TYPE_LEAF,       true,   "ipng-v4",      1,      -1,     6,      NULL,       "/test-types/ipng-v4",
        .node.leaf = {NULL,      false,     &test_cm_type_ipaddr} /*,
        .hh = {NULL} */
    }, /* 5 */

    {CDBM_NODE_TYPE_LEAF,       true,   "ipng-v6",      1,      -1,     7,      NULL,       "/test-types/ipng-v6",
        .node.leaf = {NULL,      false,     &test_cm_type_ipaddr} /*,
        .hh = {NULL} */
    }, /* 6 */

    {CDBM_NODE_TYPE_LEAF,       true,   "speed-uint",   1,      -1,     8,      NULL,       "/test-types/speed-uint",
        .node.leaf = {"50",     false,      &test_cm_type_7}
    }, /* 7 */

    {CDBM_NODE_TYPE_LEAF,       true,   "speed-int",    1,      -1,     9,      NULL,       "/test-types/speed-int",
        .node.leaf = {"-10",    false,     &test_cm_type_8}
    }, /* 8 */

    {CDBM_NODE_TYPE_LEAF,       true,   "local-mac",    1,      -1,     10,     NULL,       "/test-types/local-mac",
        .node.leaf = {NULL,      false,     &test_cm_type_mac}
    }, /* 9 */

    {CDBM_NODE_TYPE_LEAF,       true,   "string",       1,      -1,     11,     NULL,       "/test-types/string",
        .node.leaf = {NULL,      false,     &test_cm_type_10}
    }, /* 10 */

    {CDBM_NODE_TYPE_LEAF,       true,   "string-word",  1,      -1,     12,     NULL,       "/test-types/string-word",
        .node.leaf = {NULL,      false,     &test_cm_type_11}
    }, /* 11 */

    {CDBM_NODE_TYPE_LEAF,       true,   "memory-addr",  1,      -1,     -1,     NULL,       "/test-types/memory-addr",
        .node.leaf = {NULL,      false,     &test_cm_type_hex}
    }, /* 12 */




    /* node type                config, name,           parent  child   sibling must        xpath */
    /*  leaf:   default, mandatory, type    */
    /* the follwoing is realm related test data */
    {CDBM_NODE_TYPE_CONTAINER,  true,   "ip-realm",      0,      14,     -1,     NULL,       "/ip-realm",
        .node.container = {0}
    }, /* 13 */
    
    {CDBM_NODE_TYPE_LEAF,       false,  "version",      13,     -1,     15,     NULL,       "/ip-realm/version",
        .node.leaf = {0,         false,      NULL}
    }, /* 14 */
    
    {CDBM_NODE_TYPE_LIST,       true,   "realm-table",  13,     16,     33,     NULL,       "/ip-realm/realm-table",
        .node.list = {0,     10,     0,      16}
    }, /* 15 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "name",         15,     -1,     17,     NULL,       "/ip-realm/realm-table/name",
        .node.leaf = {NULL,      true,       &test_cm_type_16}
    }, /* 16 */
    
    {CDBM_NODE_TYPE_LIST,       true,   "ip-if",        15,     18,     20,     NULL,       "/ip-realm/realm-table/ip-if",
        .node.list = {0,     2,     0,       18}
    }, /* 17 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "ip",           17,     -1,     19,     NULL,       "/ip-realm/realm-table/ip-if/ip",
        .node.leaf = {NULL,      false,      &test_cm_type_ipaddr}
    }, /* 18 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "vlan",         17,     -1,     -1,     NULL,       "/ip-realm/realm-table/ip-if/vlan",
        .node.leaf = {NULL,      false,      &test_cm_type_19}
    }, /* 19 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "type",         15,     -1,     21,     NULL,       "/ip-realm/realm-table/type",
        .node.leaf = {"vmg-specific",   false,      &test_cm_type_20}
    }, /* 20 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "media-profile",15,     -1,     22,     NULL,       "/ip-realm/realm-table/media-profile",
        .node.leaf = {"default",        false,      &test_cm_type_21}
    }, /* 21 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "admin-state",  15,     -1,     23,     NULL,       "/ip-realm/realm-table/admin-state",
        .node.leaf = {"enable",         false,      &test_cm_type_22}
    }, /* 22 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "oper-down",    15,     -1,     24,     NULL,       "/ip-realm/realm-table/oper-down",
        .node.leaf = {"on-last",        false,      &test_cm_type_23}
    }, /* 23 */
    
    {CDBM_NODE_TYPE_LEAF,       false,  "oper-state",   15,     -1,     25,     NULL,       "/ip-realm/realm-table/oper-state",
        .node.leaf = {NULL,             false,      &test_cm_type_str_word}
    }, /* 24 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "service-change",15,    -1,     26,     NULL,       "/ip-realm/realm-table/service-change",
        .node.leaf = {"specific",       false,      &test_cm_type_25}
    }, /* 25 */
    
    {CDBM_NODE_TYPE_LEAF_LIST,  true,   "vmg-id",       15,     -1,     27,     NULL,       "/ip-realm/realm-table/vmg-id",
        .node.leaf_list = {0,      10,    0,        &test_cm_type_26}
    }, /* 26 */
    
    {CDBM_NODE_TYPE_CONTAINER,  true,   "cp-monitoring",15,     28,     33,     NULL,       "/ip-realm/realm-table/cp-monitoring",
        .node.container = {0}
    }, /* 27 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "ip-address",   27,     -1,     29,     NULL,       "/ip-realm/realm-table/cp-monitoring/ip-address",
        .node.leaf = {"0.0.0.0",        false,      &test_cm_type_ipaddr}
    }, /* 28 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "admin-state",  27,     -1,     30,     NULL,       "/ip-realm/realm-table/cp-monitoring/admin-state",
        .node.leaf = {"disable",        false,      &test_cm_type_22}
    }, /* 29 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "depend-port",  27,     -1,     31,     NULL,       "/ip-realm/realm-table/cp-monitoring/depend-port",
        .node.leaf = {NULL,             false,      &test_cm_type_30}
    }, /* 30 */
    
    {CDBM_NODE_TYPE_LEAF,       false,  "oper-state",   27,     -1,     32,     NULL,       "/ip-realm/realm-table/cp-monitoring/oper-state",
        .node.leaf = {NULL,             false,      &test_cm_type_str_word}
    }, /* 31 */
    
    {CDBM_NODE_TYPE_LEAF,       false,  "suspend-state",27,     -1,     -1,     NULL,       "/ip-realm/realm-table/cp-monitoring/suspend-state",
        .node.leaf = {NULL,             false,      &test_cm_type_str_word}
    }, /* 32 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "cp-rx-interval",13,    -1,     34,     NULL,       "/ip-realm/cp-rx-interval",
        .node.leaf = {"25",             false,      &test_cm_type_33}
    }, /* 33 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "cp-tx-interval",13,    -1,     35,     NULL,       "/ip-realm/cp-tx-interval",
        .node.leaf = {"25",             false,      &test_cm_type_34}
    }, /* 34 */
    
    {CDBM_NODE_TYPE_LEAF,       true,   "cp-delay",     13,     -1,     -1,     NULL,       "/ip-realm/cp-delay",
        .node.leaf = {"8000",           false,      &test_cm_type_34}
    }, /* 35 */

} ;

void cdbm_test_print_size()
{
    printf("size of data model  : items(%3d), each item(%3d), total (%5d)\n",
            sizeof(cdbm_test_cm_node)/sizeof(T_cdbm_dm_node),
            sizeof(T_cdbm_dm_node), sizeof(cdbm_test_cm_node));
    printf("size of data typedef: items(%3d), each item(%3d), total (%5d)\n",
            sizeof(cdbm_test_cm_typedef)/sizeof(T_cdbm_dm_typedef),
            sizeof(T_cdbm_dm_typedef), sizeof(cdbm_test_cm_typedef));
}
