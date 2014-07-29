

#ifndef CDBM_RET_CODE_H
#define CDBM_RET_CODE_H

typedef enum T_global_rc {
    RC_OK                         =  0,
    RC_FAILED                     = -1,
    RC_INVALID_REQUEST            = -2,
    RC_OUT_OF_RANGE               = -6,

} T_global_rc;

#endif /* CDBM_RET_CODE_H */
