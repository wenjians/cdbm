

#include "aat-incl.h"
#include "return_codes.h"
#include "gfi-list.h"
#include "gfi-print-buffer.h"

#include "cdbm-types.h"
#include "cdbm-lib.h"
#include "cdbm-event.h"
#include "cdbm-datamodel.h"
#include "cdbm-database.h"
#include "cdbm-global-data.h"


void cdbm_db_init(int phase) {
    if (phase == 1) {
        g_cdbm_data.running_db = cdbm_db_create();
    }
    else if (phase == 2) {

    }
}

T_global_rc cdbm_db_nlist_from_key_par(T_gfi_list_head* config_list_kv_pair,
                                       const char *keypath, const char* value,
                                       T_gfi_print_buffer_id rsp_msg);

T_cdbm_db_kv_pair* cdbm_db_node_pair_malloc()
{
    return (T_cdbm_db_kv_pair*) malloc(sizeof(T_cdbm_db_kv_pair));
}


void cdbm_db_node_pair_free(T_cdbm_db_kv_pair *db_node)
{
    cdbm_val_free(db_node->key_path);
    cdbm_val_free(db_node->value);

    cdbm_free(db_node);
}

void cdbm_db_node_pair_init(T_cdbm_db_kv_pair *db_node)
{
    memset(db_node, 0, sizeof(T_cdbm_db_kv_pair));
}

T_global_rc cdbm_db_nlist_from_lined_text(T_gfi_list_head* config_list_kv_pair,
        T_gfi_print_buffer_id config_text, T_gfi_print_buffer_id rsp_msg)
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

        read_len = cdbm_lib_get_next_token(config_line+read_len, config_node,
                GFI_PRINT_BUF_NODE_SIZE);
        //printf("cdbm_db_nlist_from_lined_text, keypath=<%s>, val=<%s>\n", key_path, config_node);

        ret_cod = cdbm_db_nlist_from_key_par(config_list_kv_pair, key_path, config_node, rsp_msg);
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


T_global_rc cdbm_db_nlist_from_key_par(T_gfi_list_head* config_list_kv_pair, const char *key_path,
        const char* value, T_gfi_print_buffer_id rsp_msg)
{
    T_global_rc ret_cod = RC_OK;
    T_cdbm_dm_node *dm_node;
    //T_cdbm_val_opt *val_opt=NULL;
    T_cdbm_dm_type *dm_type=NULL;
    T_cdbm_db_kv_pair *node_kv_pair;

    //printf("cdbm_db_nlist_from_key_par with pair <%s, %s>\n", key_path, value);

    dm_node = cdbm_dm_get_node_from_keypath(key_path);
    if (dm_node == NULL) {
        ret_cod = RC_CDBM_KEYPATH_NOT_MATCH;
        gfi_print_buf_add_format(rsp_msg, "Error: %s: key_path <%s> does not exist (%d), error (%d)\n",
                __FUNCTION__, key_path, ret_cod);
        return ret_cod;
    }

    node_kv_pair = cdbm_db_node_pair_malloc();
    if (node_kv_pair == NULL) {
        ret_cod = RC_CDBM_NO_MORE_MEMORY;
        gfi_print_buf_add_format(rsp_msg, "Error: %s: allocate memory failure, error (%d)\n",
                __FUNCTION__, ret_cod);
        return ret_cod;
    }
    cdbm_db_node_pair_init(node_kv_pair);
    gfi_list_init(&node_kv_pair->list_node);

    node_kv_pair->key_path = cdbm_val_malloc();
    cdbm_val_set_type(node_kv_pair->key_path, CDBM_TYPE_KEYPATH);
    ret_cod = cdbm_str_to_val(NULL, key_path, node_kv_pair->key_path);
    if (ret_cod != RC_OK) {
        ret_cod = RC_CDBM_NO_MORE_MEMORY;
        gfi_print_buf_add_format(rsp_msg, "Error: %s: translate key path <%s> to database "
                "node error (%d)\n", __FUNCTION__, key_path, ret_cod);
        cdbm_db_node_pair_free(node_kv_pair);
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
        //printf("[dm_type->base_type = %d\n", dm_type->base_type);
        node_kv_pair->value = cdbm_val_malloc();
        cdbm_val_set_type(node_kv_pair->value, dm_type->base_type);
        cdbm_str_to_val(dm_type, value, node_kv_pair->value);
    }

    gfi_list_insert_last(config_list_kv_pair, &node_kv_pair->list_node);

    return RC_OK;
}

T_global_rc cdbm_db_nlist_print(T_gfi_list_head* config_list)
{
    T_cdbm_db_kv_pair *kv_pair;
    char key_path[CDBM_MAX_KEYPATH_LEN+1];

    gfi_list_foreach_entry(kv_pair, config_list, list_node) {
        printf("%s", cdbm_val_ret_str(kv_pair->key_path, key_path, CDBM_MAX_KEYPATH_LEN));
        printf(": %s\n", cdbm_val_ret_str(kv_pair->value, key_path, CDBM_MAX_KEYPATH_LEN));
    }

    return RC_OK;
}



T_cdbm_db_node* cdbm_db_node_malloc()
{
    T_cdbm_db_node* db_node;
    db_node = (T_cdbm_db_node*) malloc(sizeof(T_cdbm_db_node));

    if (db_node != NULL) {
        memset(db_node, 0, sizeof(T_cdbm_db_node));
    }

    return db_node;
}


void cdbm_db_node_free(T_cdbm_db_node *db_node)
{
    if (db_node->name != NULL) {
        cdbm_val_free(db_node->name);
        db_node->name = NULL;
    }

    if (db_node->kpath != NULL) {
        cdbm_val_free(db_node->kpath);
        db_node->kpath = NULL;
    }

    if (db_node->value != NULL) {
        cdbm_val_free(db_node->value);
        db_node->value = NULL;
    }

    free(db_node);
}

bool cdbm_node_is_root(const T_cdbm_db_node *db_node)
{
    return (db_node->parent == NULL);
}

T_cdbm_value* cdbm_db_node_create_kpath(const T_cdbm_db_node *parent, const char* node_name)
{
    T_cdbm_value* kpath;

    if (cdbm_node_is_root(parent)) {
        kpath = cdbm_keypath_create("/%s", node_name);
    } else {
        kpath = cdbm_keypath_create("%s/%s", cdbm_kpath_get_string(parent->kpath), node_name);
    }

    return kpath;
}

void cdbm_db_node_print(T_cdbm_db_node *db_node)
{
    char name_buffer[CDBM_MAX_KEYPATH_LEN];

    cdbm_val_to_str(db_node->kpath, name_buffer, CDBM_MAX_KEYPATH_LEN);
    printf("%-40s", name_buffer);

    cdbm_val_to_str(db_node->name, name_buffer, CDBM_MAX_KEYPATH_LEN);
    printf("%-20s", name_buffer);

    cdbm_val_to_str(db_node->value, name_buffer, CDBM_MAX_KEYPATH_LEN);
    printf("%-20s", name_buffer);

    printf("\n");
}

T_cdbm_dbase* cdbm_db_create()
{
    T_cdbm_dbase* dbase;
    T_cdbm_db_node *db_root;

    dbase = cdbm_malloc(sizeof(T_cdbm_dbase));
    if (dbase == NULL)
        return NULL;

    db_root = cdbm_db_node_malloc();
    if (db_root == NULL) {
        cdbm_free(dbase);
        return NULL;
    }

    db_root->name = cdbm_val_malloc();
    cdbm_val_set_type(db_root->name, CDBM_TYPE_STRING);
    cdbm_str_to_val(NULL, "", db_root->name);

    db_root->kpath = cdbm_val_malloc();
    cdbm_val_set_type(db_root->kpath, CDBM_TYPE_KEYPATH);
    cdbm_str_to_val(cdbm_val_get_kpath_type(), "/", db_root->kpath);

    db_root->value = NULL;

    dbase->db_root = db_root;
    dbase->node_hash = NULL;

    HASH_ADD_KEYPTR(hh_keypath, dbase->node_hash, "/", strlen("/"), db_root);

    //printf("cdbm_db_create: name=<%s>, keypath=<%s>\n", 
    //    db_root->name->val.str, db_root->kpath->val.str);

    return dbase;
}


T_cdbm_db_node* cdbm_db_get_child(T_cdbm_db_node *parent, const char* node_name)
{
    T_cdbm_db_node* db_node=NULL;

/*
    #define HASH_FIND_STR(head,findstr,out)                                          \
        HASH_FIND(hh,head,findstr,strlen(findstr),out)
*/
    HASH_FIND(hh_name,parent->children, node_name, strlen(node_name), db_node);

    return db_node;
}


T_cdbm_db_node* cdbm_db_add_child(T_cdbm_dbase* dbase, T_cdbm_db_node* parent,
        const char* node_name, const T_cdbm_node_type node_type)
{
    T_cdbm_db_node* db_node=NULL;
    T_cdbm_db_node* tmp_node=NULL;
    //char kpath_name[CDBM_MAX_KEYPATH_LEN];

    db_node = cdbm_db_node_malloc();
    if (parent == NULL)
        return NULL;

    db_node->node_type = node_type;
    db_node->name = NULL;
    db_node->parent = parent;
    db_node->children = NULL;
    db_node->kpath = cdbm_db_node_create_kpath(parent, node_name);

    db_node->name = cdbm_val_malloc();
    cdbm_val_set_type(db_node->name, CDBM_TYPE_STRING);
    cdbm_str_to_val(NULL, node_name, db_node->name);

    //printf("%s: add to hash, keypath=<%s>, name=<%s>\n", __FUNCTION__, cdbm_kpath_get_string(db_node->kpath), node_name);
    
    HASH_ADD_KEYPTR(hh_name, parent->children, node_name, strlen(node_name), db_node);
    HASH_FIND(hh_name, parent->children, node_name, strlen(node_name), tmp_node);
    if (tmp_node == NULL)
        printf("%s, search child failure\n", __FUNCTION__);
    
    HASH_ADD_KEYPTR(hh_keypath, dbase->node_hash, cdbm_kpath_get_string(db_node->kpath), strlen(cdbm_kpath_get_string(db_node->kpath)), db_node);
    HASH_FIND(hh_keypath, dbase->node_hash, cdbm_kpath_get_string(db_node->kpath), strlen(cdbm_kpath_get_string(db_node->kpath)), tmp_node);
    if (tmp_node == NULL)
        printf("%s, search keypath failure\n", __FUNCTION__);

    return db_node;

}


T_cdbm_db_node* cdbm_db_get_node(T_cdbm_dbase* dbase, T_cdbm_value* kpath)
{
    T_cdbm_db_node* db_node=NULL;

/*
    #define HASH_FIND_STR(head,findstr,out)                                          \
        HASH_FIND(hh,head,findstr,strlen(findstr),out)
    HASH_FIND(hh_name,parent->children, node_name, strlen(node_name), db_node);
*/

    //printf("%s: keypath:<%s>\n", __FUNCTION__, cdbm_kpath_get_string(kpath));

    HASH_FIND(hh_keypath, dbase->node_hash, cdbm_kpath_get_string(kpath),
            strlen(cdbm_kpath_get_string(kpath)), db_node);

    return db_node;
}

T_global_rc cdbm_db_add_node(T_cdbm_dbase* dbase, T_cdbm_value* key_path, T_cdbm_value *value)
{
    bool is_ending=false;
    uint32 key_offset=0;
    T_cdbm_node_type node_type;

    char node_name[CDBM_MAX_KEYPATH_LEN];
    T_cdbm_db_node* db_parent=dbase->db_root;
    T_cdbm_db_node* db_node=NULL;

    //printf("cdbm_db_add_node: root name=<%s>, keypath=<%s>\n", 
    //    db_parent->name->val.str, db_parent->kpath->val.str);

    while (!is_ending) {

        is_ending = cdbm_kpath_get_next_token(key_path, &key_offset, node_name, CDBM_MAX_KEYPATH_LEN);
        if (is_ending)
            node_type = CDBM_NODE_TYPE_LEAF;
        else
            node_type = CDBM_NODE_TYPE_CONTAINER;

        //printf("current node_name=%s\n", node_name);

        // TODO need more different types
        db_node = cdbm_db_get_child(db_parent, node_name);
        if (db_node == NULL) {
            db_node = cdbm_db_add_child(dbase, db_parent, node_name, node_type);
            if (db_node == NULL)
                return RC_CDBM_NO_MORE_MEMORY;

            if (!is_ending) {
                db_parent = db_node;
                continue;
            }
            
            // now it is ending leaf, copy the value
            if (is_ending) {
                db_node->value = cdbm_val_duplicate(value);
                break;
            }
        }
    }

    return RC_OK;
}


T_global_rc cdbm_db_del_node_ptr(T_cdbm_dbase* dbase, T_cdbm_db_node* db_node)
{
    T_cdbm_db_node* db_parent=NULL;

    if (db_node->children != NULL) {
        return RC_CDBM_DB_DEL_NODE_WITH_CHILD;
    }

    if (cdbm_node_is_root(db_node)) {
        return RC_CDBM_DB_DEL_ROOT;
    }
        
    db_parent = db_node->parent;

    HASH_DELETE(hh_name, db_parent->children, db_node);
    HASH_DELETE(hh_keypath, dbase->node_hash, db_node);

    cdbm_db_node_free(db_node);

    return RC_OK;
}


T_global_rc cdbm_db_del_node_kpath(T_cdbm_dbase* dbase, T_cdbm_value* key_path)
{
    T_cdbm_db_node* db_node=NULL;


    db_node = cdbm_db_get_node(dbase, key_path);
    if (db_node == NULL) {
        //printf("%s: cdbm_db_get_node return NULL\n", __FUNCTION__);
        return RC_CDBM_DB_KPATH_NOT_EXIST;
    }

    return cdbm_db_del_node_ptr(dbase, db_node);
}

/* this function will delete the following nodes:
 *   the current node of key_path
 *   all the parents if the children is already deleted
 *
 * NOTE: the database root will NEVER be deleted
 */

T_global_rc cdbm_db_del_node_recursive(T_cdbm_dbase* dbase, T_cdbm_value* key_path)
{
    T_global_rc ret_cod=RC_OK;
    T_cdbm_db_node* db_parent=NULL;
    T_cdbm_db_node* db_node=NULL;
    bool is_leaf_delete = true;

    db_node = cdbm_db_get_node(dbase, key_path);
    if (db_node == NULL) {
        //printf("%s: cdbm_db_get_node return NULL\n", __FUNCTION__);
        return RC_CDBM_DB_KPATH_NOT_EXIST;
    }

    while ((ret_cod == RC_OK) && (!cdbm_node_is_root(db_node))) {
        db_parent = db_node->parent;

        ret_cod = cdbm_db_del_node_ptr(dbase, db_node);
        /* if something error while delete parents, then return RC_OK
         * because it is common when error in delete parents, e.g. 
         * there are other children available
         */
        if (!is_leaf_delete && ret_cod != RC_OK)
            return RC_OK;

        db_node = db_parent;
        is_leaf_delete = false;
    }

    return ret_cod;
}



T_global_rc cdbm_db_mod_node(T_cdbm_dbase* dbase, T_cdbm_value* key_path, T_cdbm_value *value)
{
    //bool is_ending=false;
    //uint32 key_offset=0;
    //T_cdbm_node_type node_type;

    //char node_name[CDBM_MAX_KEYPATH_LEN];
    //T_cdbm_db_node* db_parent=NULL;
    T_cdbm_db_node* db_node=NULL;


    db_node = cdbm_db_get_node(dbase, key_path);
    if (db_node == NULL)
        return RC_CDBM_DB_KPATH_NOT_EXIST;

    cdbm_val_free(db_node->value);
    db_node->value = cdbm_val_duplicate(value);

    return RC_OK;
}


T_global_rc cdbm_db_import_from_nlist(T_cdbm_dbase* dbase, T_gfi_list_head* config_list,
        T_gfi_print_buffer_id err_msg)
{
    T_global_rc ret_cod;
    T_cdbm_db_kv_pair *kv_pair;
    //char name_buffer[CDBM_MAX_KEYPATH_LEN];

    gfi_list_foreach_entry(kv_pair, config_list, list_node) {
        //printf("cdbm_db_import_from_nlist: <%s, %s>\n", cdbm_kpath_get_string(kv_pair->key_path),
        //        cdbm_val_ret_str(kv_pair->value, name_buffer, CDBM_MAX_KEYPATH_LEN));
        ret_cod = cdbm_db_add_node(dbase, kv_pair->key_path, kv_pair->value);
        CDBM_RET_IF_FAIL(ret_cod);
    }

    return RC_OK;
}


void cdbm_db_print(T_cdbm_dbase* dbase)
{
    T_cdbm_db_node* db_node=NULL;
    T_cdbm_db_node* tmp=NULL;

    printf("%s\n", __FUNCTION__);
    printf("\n%-40s%-20s%-20s\n", "key_path", "name", "value");
    
    HASH_ITER(hh_keypath, dbase->node_hash, db_node, tmp) {
        cdbm_db_node_print(db_node);
    }
}

typedef void (*T_cmdb_db_node_op)(const T_cdbm_db_node* v1);

void cdbm_db_walk_one_node(T_cdbm_db_node *cur_node, T_cmdb_db_node_op node_op)
{
    T_cdbm_db_node *child = NULL;
    T_cdbm_db_node *tmp = NULL;
    
    node_op(cur_node);

    HASH_ITER(hh_name, cur_node->children, child, tmp) {
        cdbm_db_walk_one_node(child, node_op);
    }
}

void cdbm_db_walk_through(T_cdbm_dbase* dbase, T_cmdb_db_node_op node_op)
{
    cdbm_db_walk_one_node(dbase->db_root, node_op);
}


void cdbm_db_walk_print(T_cdbm_dbase* dbase)
{
    printf("%s:\n", __FUNCTION__);
    cdbm_db_walk_through(dbase, (T_cmdb_db_node_op)cdbm_db_node_print);
}


T_cdbm_dbase* g_test_dbase;
    

void cdbm_db_check_node(T_cdbm_db_node *cur_node)
{
    T_cdbm_db_node* node;
    T_cdbm_value *kpath = cdbm_keypath_create(cdbm_kpath_get_string(cur_node->kpath));

    node = cdbm_db_get_node(g_test_dbase, kpath);

    printf("%s: key path <%s> node is <0x%p>\n", __FUNCTION__, 
        cdbm_kpath_get_string(cur_node->kpath), node);
    
}
    

void cdbm_db_walk_find(T_cdbm_dbase* dbase)
{
    printf("%s:\n", __FUNCTION__);
    g_test_dbase = dbase;
    
    cdbm_db_walk_through(dbase, (T_cmdb_db_node_op)cdbm_db_check_node);
}

