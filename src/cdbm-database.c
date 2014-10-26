

#include "aat-incl.h"
#include "return_code.h"
#include "gfi-print-buffer.h"

#include "cdbm-lib.h"
#include "cdbm-database.h"


T_gfi_list_head* cdbm_db_nlist_creat()
{
    return gfi_list_create2("cdbm_db_node_list");
}

T_global_rc cdbm_db_nlist_from_lined_text(T_gfi_list_head* config_list,
                                          T_gfi_print_buffer_id config_text,
                                          T_gfi_print_buffer_id err_msg)
{
    T_global_rc ret_cod = RC_OK;
    uint32 read_len;
    char* dst, *src, *config_line, *config_node;
    char key_path[CDBM_MAX_KEYPATH_LEN+1];
    char key_token[CDBM_MAX_KEYPATH_LEN+1];
    T_gfi_print_buffer_iterator buf_it;
    T_cdbm_dm_node *dm_node;

    T_cdbm_vtype    val_type;
    T_cdbm_val_opt *val_opt=NULL;
    T_cdbm_value *node_key_path=NULL;
    T_cdbm_value* node_value=NULL;

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

        read_len = gfi_print_buf_it_read_line(config_text, config_line, GFI_PRINT_BUF_NODE_SIZE);
        if (read_len == 0)
            break;

        read_len = cdbm_lib_get_next_token(config_line, key_path, CDBM_MAX_KEYPATH_LEN);
        if (read_len == 0)
            break;

        read_len = cdbm_lib_get_next_token(config_line+read_len, config_node, GFI_PRINT_BUF_NODE_SIZE);
        ret_cod = cdbm_db_nlist_from_key_par(config_list, key_path, config_node, err_msg);
        if (ret_cod != RC_OK)
            goto err_proc;
    }

err_proc:
    if (node_key_path != NULL)
        cdbm_val_free(node_key_path);
    if (config_node != NULL)
        cdbm_val_free(config_node);

end_proc:
    if (config_line != NULL)
        gfi_print_buf_node_free(config_line);
    if (config_node != NULL)
        gfi_print_buf_node_free(config_node);


    return ret_cod;
}


T_global_rc cdbm_db_nlist_from_key_par(T_gfi_list_head* config_list,
                                       const char *keypath, const char* value,
                                       T_gfi_print_buffer_id err_msg)
{
#if 0
    T_global_rc ret_cod = RC_OK;
    va_list ap;
    uint32 read_len;
    char* dst, *src, *config_line, *config_node;
    char key_path[CDBM_MAX_KEYPATH_LEN+1];
    char key_token[CDBM_MAX_KEYPATH_LEN+1];
    T_gfi_print_buffer_iterator buf_it;
    T_cdbm_dm_node *dm_node;

    T_cdbm_vtype    val_type;
    T_cdbm_val_opt *val_opt=NULL;
    T_cdbm_value *node_key_path=NULL;
    T_cdbm_value* node_value=NULL;

    config_line = gfi_print_buf_node_alloc();
    config_node = gfi_print_buf_node_alloc();

    if ((config_line==NULL) || (config_node == NULL)) {
        ret_cod = RC_CDBM_NO_MORE_MEMORY;
        goto end_proc;
    }

    gfi_print_buf_it_init(config_text, &buf_it);
    while (1)
    {
        read_len = gfi_print_buf_it_read_line(config_text, config_line, GFI_PRINT_BUF_NODE_SIZE);
        if (read_len == 0)
            break;

        read_len = cdbm_lib_get_next_token(config_line, key_path, CDBM_MAX_KEYPATH_LEN);
        if (read_len == 0)
            break;

        dm_node = cdbm_dm_get_node_from_keypath(key_path);
        if (dm_node == NULL) {
            gfi_print_buf_add_format(err_msg, "Error: keypath <%s> does not exist!");
            ret_cod = RC_CDBM_KEYPATH_NOT_MATCH;
            goto end_proc;
        }

        node_key_path = cdbm_val_alloc();
        node_value = cdbm_val_alloc();
        if ((node_key_path==NULL) || (node_value==NULL)) {
            ret_cod = RC_CDBM_NO_MORE_MEMORY;
            goto end_proc;
        }

        ret_cod = cdbm_str_to_keypath(NULL, key_path, node_key_path);
        if (ret_cod != RC_OK)
            goto end_proc;

        switch (dm_node->node_type) {
        case CDBM_NODE_TYPE_LEAF:
            
        case CDBM_NODE_TYPE_INVALID:
    CDBM_NODE_TYPE_CONTAINER    = 1,
    CDBM_NODE_TYPE_LIST         = 2,
    CDBM_NODE_TYPE_LEAF_LIST    = 3,
    
        }
        val_type = dm_node->
//        val_opt
//        ret_cod = cdbm_str_to

    }

    dst = buffer;
    src = config_line + read_len;

    // skip the space
    while (*src == ' ' && *src != '\0')
        src++;

    /* use max_len-1 to make sure there is a space to fill terminate '\0'
     * should not copy the last '\n'
     */
    for (; *src && *src!='\n' && (max_len-1>0); max_len--)
        *dst++ = *src++;

    *dst = '\0';

    return (RC_OK);


    gfi_list_insert_last(g_cdbm_data.trans_list, &cdbm_trans_id->list_node);

err_proc:
    if (node_key_path != NULL)
        gfi_print_buf_node_free(node_key_path);
    if (node_value = cdbm_val_alloc();

end_proc:

    return ret_cod;
#endif
}

