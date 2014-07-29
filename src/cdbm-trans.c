

#include "aat-incl.h"
#include "return_codes.h"

#include "cdbm-lib.h"
#include "gfi-print-buffer.h"

/* one configuration 10M should be enough */
#define CDBM_CONFIG_MAX_SIZE    10*1024*1024

T_cdbm_trans_id cdbm_create_transaction(char* trans_name)
{
    T_cdbm_trans_id cdbm_trans = NULL;

    cdbm_trans = malloc(CDBM_TRANSACTION_SIZE);
    AAT_STD_ASSERT(cdbm_trans != NULL) ;

    cdbm_trans->plain_config = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
                                  GFI_PRITN_BUF_FLAG_OVERSTOP, CDBM_CONFIG_MAX_SIZE);

    return cdbm_trans;
}

T_global_rc cdbm_close_transaction(T_cdbm_trans_id cdbm_trans)
{
    free (cdbm_trans);

    return RC_OK;
}
