

#include "aat-incl.h"
#include "return_codes.h"

#include "gfi-print-buffer.h"

#include "cdbm-lib.h"
#include "cdbm-types.h"
#include "cdbm-data-model.h"
#include "cdbm-event.h"
#include "cdbm-global-data.h"

/* one configuration 10M should be enough */
#define CDBM_CONFIG_MAX_SIZE    10*1024*1024



void cdbm_trans_init()
{
    g_cdbm_data.trans_list = gfi_list_create("cdbm_trans");
    AAT_STD_ASSERT (g_cdbm_data.trans_list != NULL);

    gfi_lock_init(&g_cdbm_data.trans_list_lock);
}


T_cdbm_trans_id cdbm_create_transaction(char* trans_name)
{
    T_cdbm_trans_id cdbm_trans_id = NULL;

    cdbm_trans_id = malloc(CDBM_TRANSACTION_SIZE);
    AAT_STD_ASSERT(cdbm_trans_id != NULL) ;

    memset(cdbm_trans_id, 0, CDBM_TRANSACTION_SIZE);

    cdbm_trans_id->plain_config = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
                                  GFI_PRITN_BUF_FLAG_OVERSTOP, CDBM_CONFIG_MAX_SIZE);
    AAT_STD_ASSERT(cdbm_trans_id->plain_config != NULL);

    /*
    cdbm_trans_id->xml_config = gfi_print_buf_creat(trans_name, AAT_MOD_CDBM,
                                  GFI_PRITN_BUF_FLAG_OVERSTOP, CDBM_CONFIG_MAX_SIZE);
    AAT_STD_ASSERT(cdbm_trans_id->xml_config != NULL);
    */

    /* add to transaction session list */
    gfi_list_init(&cdbm_trans_id->list_node);
    gfi_lock(&g_cdbm_data.trans_list_lock);
    gfi_list_insert_last(g_cdbm_data.trans_list, &cdbm_trans_id->list_node);
    gfi_unlock(&g_cdbm_data.trans_list_lock);

    return cdbm_trans_id;
}

T_global_rc cdbm_close_transaction(T_cdbm_trans_id cdbm_trans_id)
{
    uint32 ret_addr = gfi_return_address();
    if (!gfi_list_is_entry_in_list(g_cdbm_data.trans_list, &cdbm_trans_id->list_node))
    {
        CDBM_LOG(CDBM_EVT_ERROR, "cdbm close transaction which is not exist, caller (0x%x)", ret_addr);
        return RC_CDBM_TRANS_NOT_IN_LIST;
    }

    gfi_print_buf_destroy(cdbm_trans_id->plain_config);
    //gfi_print_buf_destroy(cdbm_trans_id->xml_config);

    /* remove from transaction session list */
    gfi_lock(&g_cdbm_data.trans_list_lock);
    gfi_list_remove(g_cdbm_data.trans_list, &cdbm_trans_id->list_node);
    gfi_unlock(&g_cdbm_data.trans_list_lock);

    free (cdbm_trans_id);

    return RC_OK;
}
