

#include "aat-incl.h"
#include "cdbm-lib.h"


T_cdbm_trans_id cdbm_create_transaction()
{
    T_cdbm_trans_id cdbm_trans = NULL;

    cdbm_trans = malloc(CDBM_TRANSACTION_SIZE);
    AAT_STD_ASSERT(cdbm_trans!=NULL);


    return RC_OK;
}
