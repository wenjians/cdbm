

#ifndef CDBM_RET_CODE_H
#define CDBM_RET_CODE_H

typedef enum T_global_rc {
    RC_OK                         =  0,
    RC_FAILED                     = -1,
    RC_INVALID_REQUEST            = -2,
    RC_OUT_OF_RANGE               = -6,

    /* the following is the error code for overall CDBM */
    RC_CDBM_NO_MORE_MEMORY     = -10000,

    /* transaction manager related */
    RC_CDBM_TRANS_IS_NULL      = -10100,
    RC_CDBM_TRANS_NOT_IN_LIST  = -10101,

    /* data model related */
    RC_CDBM_DM_KEYPATH_EXIST   = -20000,
    RC_CDBM_DM_ADD_KEYPATH_FAIL= -20001,

    /* data base related */
    RC_CDBM_DB_KPATH_NOT_EXIST    =-20050,
    RC_CDBM_DB_DEL_NODE_WITH_CHILD=-20051,
    RC_CDBM_DB_DEL_ROOT           =-20052,

    /* CDBM value related error */
    RC_CDBM_VAL_INVALID_TYPE   = -21001,
    RC_CDBM_VAL_TYPE_NOT_MATCH = -21002,
    RC_CDBM_VAL_BUF_TOO_SHORT  = -21003,
    RC_CDBM_VAL_INVALID_RANGE  = -21004,
    RC_CDBM_VAL_SPACE_IN_STRWORD = -21005,

    RC_CDBM_NAME_SPACE_INVALID = -11001,
    RC_CDBM_INVALID_IPV4_ADDR  = -11002,
    RC_CDBM_INVALID_IPV6_ADDR  = -11003,
    RC_CDBM_INVALID_IP_ADDR    = -11004,
    RC_CDBM_KEYPATH_NOT_MATCH  = -11005,
    RC_CDBM_READ_EMPTY         = -11006,
    RC_CDBM_SET_CONF_STR_FAIL  = -11007,
    RC_CDBM_INVALID_NODE_TYPE  = -11008,


    RC_CDBM_INVALID_STRING     = -11009,
    RC_CDBM_INVALID_RANGE      = -11010,
    RC_CDBM_INVALID_TYPE       = -11014,
    RC_CDBM_STR_NO_SPACE       = -11011,

    RC_CDBM_SYNTAX_ERROR       = -11012,
    RC_CDBM_VALIDAT_ERROR      = -11013,
    //RC_CDBM_NO_MORE_MEMORY     = -11014,
    RC_CDBM_INVALID_VTYPE      = -11015,
    RC_CDBM_KEYPATH_TOO_LONG   = -11016,

} T_global_rc;

#endif /* CDBM_RET_CODE_H */
