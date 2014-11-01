

/******************************************************************************
 *                CDBM configuration model definition
 *****************************************************************************/

#include "uthash.h"


#include "aat-incl.h"


#include "cdbm-types.h"
#include "cdbm-lib.h"
#include "cdbm-event.h"
#include "cdbm-global-data.h"
#include "cdbm-datamodel.h"


/* TODO init phase, validate the parameter tree
 *      - init, make sure all dm node in tree, and tree node count == array items count
 *      - (done) xpath is correct
 *      - default value follow rules
 *      - leaf no children
 *      - self is available in parent's children list
 *      - build hash
 *      - following rules for each field: container, leaf, leaf-list, list
 *
 */

void cdbm_dm_attach_data(T_cdbm_dm_node *_node,       uint32 node_cnt,
                         T_cdbm_dm_typedef *_typedef, uint32 typedef_cnt)
{
    AAT_STD_ASSERT(_typedef != NULL);
    AAT_STD_ASSERT(_node);

    g_cdbm_data.dm_type_cnt = typedef_cnt;
    g_cdbm_data.dm_node_cnt = node_cnt;
    g_cdbm_data.dm_node = _node;
    g_cdbm_data.dm_typedef = _typedef;
}



static inline bool cdbm_dm_node_is_id_valid(T_cdbm_dm_node_idx node_idx)
{
    return node_idx != CDBM_CM_INVALID_IDX;
}

static inline T_cdbm_dm_node* cdbm_dm_get_node_from_id(T_cdbm_dm_node_idx node_idx)
{
    if (!cdbm_dm_node_is_id_valid(node_idx)) {
        return NULL;
    }

    return  g_cdbm_data.dm_node + node_idx;
}


T_cdbm_dm_node* cdbm_dm_get_node_from_keypath(const char* key_path)
{
    T_cdbm_dm_node *dm_node=NULL;
    HASH_FIND_STR(g_cdbm_data.dm_node_hash, key_path, dm_node);

    return dm_node;
}


bool cdbm_dm_node_is_root(T_cdbm_dm_node *cm_node) {
    return (cm_node->parent_idx == -1);
}


bool cmbm_dm_node_is_container(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_CONTAINER);
}

bool cmbm_dm_node_is_list(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_LIST);
}

bool cmbm_dm_node_is_leaf(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_LEAF);
}

bool cmbm_dm_node_is_leaf_list(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_LEAF_LIST);
}


#define CDBM_CM_NODE_VISIT(cur_node, node_op)   \
do {                                            \
    T_global_rc ret_cod;                        \
    if (node_op != NULL) {                      \
        ret_cod = node_op(cur_node);            \
        if (ret_cod != RC_OK) {                 \
            return ret_cod;                     \
        }                                       \
    }                                           \
} while (0)

/*
#define cdbm_cm_node_foreach(node_idx, first_idx, next) \
        for (child_idx = cur_node->children_idx;
             cdbm_cm_node_idx_is_valid(child_idx);
             child_idx = child->sibling_idx)
        {
            cdbm_cm_node_walk_one_node(child_idx, node_ops);
        }
*/

T_global_rc cdbm_dm_node_walk_one_node(T_cdbm_dm_node *cur_node, T_cdbm_dm_node_ops *node_ops)
{
    T_cdbm_dm_node *child;
    T_cdbm_dm_node_idx child_idx;

    if (cur_node == NULL) {
        return RC_FAILED;
    }

    if (cmbm_dm_node_is_leaf(cur_node)) {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_leaf);
        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_leaf);
    }
    else if (cmbm_dm_node_is_container(cur_node))
    {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_container);

        child_idx = cur_node->children_idx;
        while (cdbm_dm_node_is_id_valid(child_idx))
        {
            child = cdbm_dm_get_node_from_id(child_idx);
            cdbm_dm_node_walk_one_node(child, node_ops);
            child_idx = child->sibling_idx;
        }

        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_container);
    }
    else if (cmbm_dm_node_is_list(cur_node))
    {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_list);

        child_idx = cur_node->children_idx;
        while (cdbm_dm_node_is_id_valid(child_idx))
        {
            child = cdbm_dm_get_node_from_id(child_idx);
            cdbm_dm_node_walk_one_node(child, node_ops);
            child_idx = child->sibling_idx;
        }

        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_list);
    }
    else if (cmbm_dm_node_is_leaf_list(cur_node))
    {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_leaf_list);
        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_leaf_list);
    }
    else
    {
        CDBM_LOG(CDBM_EVT_ERROR, "Unknown node type (%u), key path (%s)", cur_node->node_type, cur_node->key_path);
        return RC_CDBM_INVALID_NODE_TYPE;
    }

    return RC_OK;
}

/*
T_global_rc cdbm_dm_node_walk(T_cdbm_dm_node_ops *node_ops)
{
    //T_cdbm_cm_node *cur_node, *parent_node;

    return cdbm_cm_node_walk_one_node(&g_cdbm_data.dm_node[0], node_ops);

    //return RC_OK;
}
*/

const char* cdbm_dm_get_key_path(T_cdbm_dm_node_idx node_idx) {
    T_cdbm_dm_node* cm_node = cdbm_dm_get_node_from_id(node_idx);
    if (cm_node == NULL) {
        return "";
    }

    return cm_node->key_path;
}

T_global_rc cdbm_dm_node_init(T_cdbm_dm_node* dm_node)
{
    char key_path[CDBM_MAX_KEYPATH_LEN];
    T_cdbm_dm_node *temp_node=NULL;

    /*
    printf("cdbm_dm_node_init: adding data model node <%s>, parent(%d), child(%d), sibling(%d)\n",
            dm_node->key_path, dm_node->parent_idx, dm_node->children_idx, dm_node->sibling_idx);
    */

    memset(&dm_node->hh, 0, sizeof(UT_hash_handle));

    HASH_FIND_STR(g_cdbm_data.dm_node_hash, dm_node->key_path, temp_node);
    if (temp_node != NULL) {
        //printf("cdbm_dm_node_init: <%s> is already in tree!\n", dm_node->key_path);
        return RC_CDBM_DM_KEYPATH_EXIST;
    }

    if (strlen(dm_node->key_path) >= CDBM_MAX_KEYPATH_LEN) {
        AAT_FMT_ASSERT(0, ("key_path too long(%d)(%s)", strlen(dm_node->key_path), dm_node->key_path));
    }

    /* verify that the key path is correct for each configuration model node */
    if (cdbm_dm_node_is_root(dm_node))
    {
        AAT_FMT_ASSERT(strcmp(dm_node->key_path, "/")==0,
                       ("key_path (%s) of root is not correct!", dm_node->key_path));
    }
    else if (cdbm_dm_node_is_root(cdbm_dm_get_node_from_id(dm_node->parent_idx)))
    {
        snprintf(key_path, CDBM_MAX_KEYPATH_LEN, "/%s", dm_node->key_name);
        AAT_FMT_ASSERT(strcmp(dm_node->key_path, key_path)==0,
                       ("key_path (%s)(%s) of module not correct!", dm_node->key_path, key_path));
    }
    else
    {
        snprintf(key_path, CDBM_MAX_KEYPATH_LEN, "%s/%s",
                 cdbm_dm_get_key_path(dm_node->parent_idx), dm_node->key_name);
        AAT_FMT_ASSERT(strcmp(dm_node->key_path, key_path)==0,
                       ("key_path (%s)(%s) is not correct!", dm_node->key_path, key_path));
    }

    /* key_name should be trim of space,  key_path don't need check
     * because it will verified by key name
     */
    if (!cdbm_dm_node_is_root(dm_node))
    {
        AAT_FMT_ASSERT(strlen(dm_node->key_name),
                       ("key name nust be not empty, parent (%s)",
                        cdbm_dm_get_key_path(dm_node->parent_idx)));
        AAT_FMT_ASSERT(!isspace(dm_node->key_name[0]),
                       ("configure node (%s) start with space!", dm_node->key_path));
        AAT_FMT_ASSERT(!isspace(dm_node->key_name[strlen(dm_node->key_name)-1]),
                       ("configure node (%s) end with space!", dm_node->key_path));
    }

    HASH_ADD_KEYPTR(hh, g_cdbm_data.dm_node_hash, dm_node->key_path, strlen(dm_node->key_path), dm_node);

    temp_node = NULL;
    HASH_FIND_STR(g_cdbm_data.dm_node_hash, dm_node->key_path, temp_node);
    if (temp_node == NULL) {
        //printf("cdbm_dm_node_init: <%s> add to hash failure!\n", dm_node->key_path);
        return RC_CDBM_DM_ADD_KEYPATH_FAIL;
    }

    return RC_OK;
}

T_global_rc cdbm_dm_init()
{
    T_global_rc ret_cod;
    //T_cdbm_dm_node* dm_node, *tmp;
    T_cdbm_dm_node_ops node_ops = {
        cdbm_dm_node_init,        NULL,
        cdbm_dm_node_init,        NULL,
        cdbm_dm_node_init,        NULL,
        cdbm_dm_node_init,        NULL
    };

    g_cdbm_data.dm_node_hash = NULL;
    ret_cod = cdbm_dm_node_walk_one_node(&g_cdbm_data.dm_node[0], &node_ops);
    CDBM_RET_IF_FAIL(ret_cod);

    /* free the hash table contents */
#if 0
    printf("cdbm_dm_init: iterate all the data model tree:\n");
    HASH_ITER(hh, g_cdbm_data.dm_node_hash, dm_node, tmp) {
        printf("%s: %s\n", dm_node->key_name, dm_node->key_path);
      //HASH_DEL(users, s);
      //free(s);
    }
#endif

    /*
    cdbm_dm_node_printall();

    printf("not test the search...\n");
    cdbm_dm_get_node_from_keypath("/");
    cdbm_dm_get_node_from_keypath("/test-types");
    cdbm_dm_get_node_from_keypath("/test-types/string");
    cdbm_dm_get_node_from_keypath("/test-types/string-word");
    cdbm_dm_get_node_from_keypath("/test-types/string-ttt");
    cdbm_dm_get_node_from_keypath("/test-types/speed-uint");
    cdbm_dm_get_node_from_keypath("/test-types/speed-int");

    cdbm_dm_get_node_from_keypath("/test-types/secondary-ip");
    cdbm_dm_get_node_from_keypath("/test-types/ipng-v4");
    cdbm_dm_get_node_from_keypath("/test-types/ipng-v6");
    cdbm_dm_get_node_from_keypath("/test-types/local-mac");
    cdbm_dm_get_node_from_keypath("/test-types/memory-addr");
    cdbm_dm_get_node_from_keypath("/test-types/save-mo");
    cdbm_dm_node_printall();
    cdbm_dm_get_node_from_keypath("/test-types/primary-ip");
    cdbm_dm_get_node_from_keypath("/test-types/save-mode");
    */

    return RC_OK;
}

T_global_rc cdbm_cm_validate(const char* line)
{

    return RC_OK;
}





uint32 prefix_space_cnt=0;
T_global_rc cdbm_dm_node_print(T_cdbm_dm_node* cm_node)
{
    int space;
    char fmt[80];

    for (space=0; space<prefix_space_cnt; space++) {
        printf(" ");
    }

    snprintf(fmt, 80, "%%%ds      %%s\n", 40-prefix_space_cnt);

    printf(fmt, cm_node->key_name, cm_node->key_path);
    return RC_OK;
}

T_global_rc cdbm_dm_print_enter_container(T_cdbm_dm_node* cur_node)
{
    prefix_space_cnt+=4;
    cdbm_dm_node_print(cur_node);

    return RC_OK;
}

T_global_rc cdbm_dm_print_exit_container(T_cdbm_dm_node* cm_node)
{
    prefix_space_cnt -= 4;

    return RC_OK;
}


T_global_rc cdbm_dm_node_printall()
{
    T_cdbm_dm_node_ops node_ops = {
        cdbm_dm_print_enter_container,
        cdbm_dm_print_exit_container,
        cdbm_dm_print_enter_container,
        cdbm_dm_print_exit_container,
        cdbm_dm_node_print,
        NULL,
        cdbm_dm_node_print,
        NULL
    };

    //cdbm_dm_node_walk(&node_ops);
    cdbm_dm_node_walk_one_node(&g_cdbm_data.dm_node[0], &node_ops);

    return RC_OK;
}
