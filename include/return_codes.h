

#ifndef CDBM_RET_CODE_H
#define CDBM_RET_CODE_H

typedef enum T_global_rc {
    RC_OK                         =  0,
    RC_FAILED                     = -1,
    RC_INVALID_REQUEST            = -2,
    RC_OUT_OF_RANGE               = -6,

    /* the follwoing is the error code for CDBM */

    /* transaction manager related */
    RC_CDBM_TRANS_IS_NULL      = -10000,
    RC_CDBM_TRANS_NOT_IN_LIST  = -10001,

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
    RC_CDBM_NO_MORE_MEMORY     = -11014,
    RC_CDBM_INVALID_VTYPE      = -11015,
    RC_CDBM_KEYPATH_TOO_LONG   = -11016,

} T_global_rc;

#endif /* CDBM_RET_CODE_H */
