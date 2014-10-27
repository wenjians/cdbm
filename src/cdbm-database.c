

#include "aat-incl.h"
#include "return_codes.h"
#include "gfi-print-buffer.h"

#include "cdbm-lib.h"
#include "cdbm-types.h"
#include "cdbm-database.h"
#include "cdbm-datamodel.h"


T_global_rc cdbm_db_nlist_from_key_par(T_gfi_list_head* config_list,
                                       const char *keypath, const char* value,
                                       T_gfi_print_buffer_id err_msg);

T_cdbm_db_kv_pair* cdbm_db_node_pair_malloc()
{
    return (T_cdbm_db_kv_pair*) malloc(sizeof(T_cdbm_db_kv_pair));
}


void cdbm_db_node_pair_free(T_cdbm_db_kv_pair *db_node)
{
    cdbm_val_free(&db_node->key_path);
    cdbm_val_free(&db_node->value);

    free(db_node);
}

void cdbm_db_node_init(T_cdbm_db_kv_pair *db_node)
{
    memset(db_node, 0, sizeof(T_cdbm_db_kv_pair));
}

T_global_rc cdbm_db_nlist_from_lined_text(T_gfi_list_head* config_list,
                                          T_gfi_print_buffer_id config_text,
                                          T_gfi_print_buffer_id err_msg)
{
    T_global_rc ret_cod = RC_OK;
    uint32 read_len;
    char *config_line, *config_node;
    char key_path[CDBM_MAX_KEYPATH_LEN+1];
    T_gfi_print_buffer_iterator buf_it;

    config_line = gfi_print_buf_node_alloc();
    config_node = gfi_print_buf_node_alloc();

    if ((config_line==NULL) || (config_node == NULL)) {
        ret_cod = RC_CDBM_NO_MORE_MEMORY;
        goto err_proc;
    }

    gfi_print_buf_it_init(config_text, &buf_it);
    while (1)
    {
        config_line[0] = '\0';
        config_node[0] = '\0';

        read_len = gfi_print_buf_it_read_line(&buf_it, config_line, GFI_PRINT_BUF_NODE_SIZE);
        if (read_len == 0)
            break;

        read_len = cdbm_lib_get_next_token(config_line, key_path, CDBM_MAX_KEYPATH_LEN);
        if (read_len == 0)
            break;

        read_len = cdbm_lib_get_next_token(config_line+read_len, config_node, GFI_PRINT_BUF_NODE_SIZE);
        printf("cdbm_db_nlist_from_lined_text, keypath=<%s>, val=<%s>\n", key_path, config_node);

        ret_cod = cdbm_db_nlist_from_key_par(config_list, key_path, config_node, err_msg);
        if (ret_cod != RC_OK)
            goto err_proc;
    }

err_proc:
    if (config_line != NULL)
        gfi_print_buf_node_free(config_line);
    if (config_node != NULL)
        gfi_print_buf_node_free(config_node);


    return ret_cod;
}


T_global_rc cdbm_db_nlist_from_key_par(T_gfi_list_head* config_list,
                                       const char *key_path, const char* value,
                                       T_gfi_print_buffer_id err_msg)
{
    T_global_rc ret_cod = RC_OK;
    T_cdbm_dm_node *dm_node;
    T_cdbm_val_opt *val_opt=NULL;
    T_cdbm_dm_type *dm_type=NULL;
    T_cdbm_db_kv_pair *db_node;

    printf("cdbm_db_nlist_from_key_par with pair <%s, %s>\n", key_path, value);

    dm_node = cdbm_dm_get_node_from_keypath(key_path);
    if (dm_node == NULL) {
        ret_cod = RC_CDBM_KEYPATH_NOT_MATCH;
        gfi_print_buf_add_format(err_msg, "Error: %s: key_path <%s> does not exist (%d), error (%d)\n",
                __FUNCTION__, key_path, ret_cod);
        return ret_cod;
    }

    db_node = cdbm_db_node_pair_malloc();
    if (db_node == NULL) {
        ret_cod = RC_CDBM_NO_MORE_MEMORY;
        gfi_print_buf_add_format(err_msg, "Error: %s: allocate memory failure, error (%d)\n",
                __FUNCTION__, ret_cod);
        return ret_cod;
    }
    cdbm_db_node_init(db_node);
    gfi_list_init(&db_node->list_node);


    ret_cod = cdbm_str_to_keypath(NULL, key_path, &db_node->key_path);
    if (ret_cod != RC_OK) {
        ret_cod = RC_CDBM_NO_MORE_MEMORY;
        gfi_print_buf_add_format(err_msg, "Error: %s: translate key path <%s> to database node error (%d)\n",
                __FUNCTION__, key_path, ret_cod);
        cdbm_db_node_pair_free(db_node);
        return ret_cod;

    }

    if (dm_node->node_type == CDBM_NODE_TYPE_LEAF )
    {
        dm_type = dm_node->node.leaf.data_type;
    }
    else if (dm_node->node_type == CDBM_NODE_TYPE_LEAF_LIST)
    {
        dm_type = dm_node->node.leaf_list.data_type;
    }

    /* transfer the value from type definition */
    if (dm_type != NULL)
    {
        printf("[dm_type->base_type = %d\n", dm_type->base_type);
        val_opt = &g_cdbm_val_ops[dm_type->base_type];
        val_opt->str_to_val(dm_type, value, &db_node->value);
    }

    gfi_list_insert_last(config_list, &db_node->list_node);

    return RC_OK;
}





T_global_rc cdbm_db_from_nlist(T_gfi_list_head* config_list,
                              const char *key_path, const char* value,
                              T_gfi_print_buffer_id err_msg)
{
    return RC_OK;
}
