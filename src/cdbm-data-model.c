

/******************************************************************************
 *                CDBM configuration model definition
 *****************************************************************************/

#include "aat-incl.h"

#include "uthash.h"

#include "cdbm-lib.h"
#include "cdbm-event.h"
#include "cdbm-types.h"
#include "cdbm-data-model.h"

#include "cdbm-global-data.h"

/* TODO init phase, validate the parameter tree
 *      - (done) xpath is correct
 *      - default value follow rules
 *      - leaf no children
 *      - self is available in parent's children list
 *      - build hash
 *      - following rules for each field: container, leaf, leaf-list, list
 *
 */

void cdbm_cm_attach_data(T_cdbm_dm_node *_node, T_cdbm_dm_typedef *_typedef)
{
    AAT_STD_ASSERT(_typedef != NULL);
    AAT_STD_ASSERT(_node);

    g_cdbm_data.dm_node = _node;
    g_cdbm_data.dm_typedef = _typedef;
}

static inline bool cdbm_cm_node_idx_is_valid(T_cdbm_dm_node_idx node_idx)
{
    return node_idx != CDBM_CM_INVALID_IDX;
}


static inline T_cdbm_dm_node* cdbm_cm_get_node_from_id(T_cdbm_dm_node_idx node_idx)
{
    if (!cdbm_cm_node_idx_is_valid(node_idx)) {
        return NULL;
    }

    //if (!cdbm_cm_node_idx)

    return  g_cdbm_data.dm_node + node_idx;
}

T_cdbm_dm_node* cdbm_dm_get_node_from_keypath(const char* key_path)
{
    T_cdbm_dm_node *cm_node;
    HASH_FIND_STR(g_cdbm_data.dm_node_hash, key_path, cm_node);

    return cm_node;
}

bool cdbm_dm_is_root(T_cdbm_dm_node *cm_node) {
    return (cm_node->parent_idx == -1);
}


bool cmbm_dm_is_container(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_CONTAINER);
}

bool cmbm_dm_is_list(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_LIST);
}

bool cmbm_dm_is_leaf(T_cdbm_dm_node *cm_node) {
    return (cm_node->node_type == CDBM_NODE_TYPE_LEAF);
}

bool cmbm_dm_is_leaf_list(T_cdbm_dm_node *cm_node) {
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

T_global_rc cdbm_cm_node_walk_one_node(T_cdbm_dm_node *cur_node, T_cdbm_dm_node_ops *node_ops)
{
    T_cdbm_dm_node *child;
    T_cdbm_dm_node_idx child_idx;
    //T_global_rc ret_cod = RC_OK;

    if (cur_node == NULL) {
        return RC_FAILED;
    }

    if (cmbm_dm_is_leaf(cur_node)) {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_leaf);
        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_leaf);
    }
    else if (cmbm_dm_is_container(cur_node))
    {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_container);

        child_idx = cur_node->children_idx;
        while (cdbm_cm_node_idx_is_valid(child_idx))
        {
            child = cdbm_cm_get_node_from_id(child_idx);
            cdbm_cm_node_walk_one_node(child, node_ops);
            child_idx = child->sibling_idx;
        }

        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_container);
    }
    else if (cmbm_dm_is_list(cur_node))
    {
        CDBM_CM_NODE_VISIT(cur_node, node_ops->enter_list);

        child_idx = cur_node->children_idx;
        while (cdbm_cm_node_idx_is_valid(child_idx))
        {
            child = cdbm_cm_get_node_from_id(child_idx);
            cdbm_cm_node_walk_one_node(child, node_ops);
            child_idx = child->sibling_idx;
        }

        CDBM_CM_NODE_VISIT(cur_node, node_ops->exit_list);
    }
    else if (cmbm_dm_is_leaf_list(cur_node))
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

T_global_rc cdbm_dm_node_walk(T_cdbm_dm_node_ops *node_ops)
{
    //T_cdbm_cm_node *cur_node, *parent_node;

    return cdbm_cm_node_walk_one_node(&g_cdbm_data.dm_node[0], node_ops);

    //return RC_OK;
}


const char* cdbm_cm_get_key_path(T_cdbm_dm_node_idx node_idx) {
    T_cdbm_dm_node* cm_node = cdbm_cm_get_node_from_id(node_idx);
    if (cm_node == NULL) {
        return "";
    }

    return cm_node->key_path;
}

T_global_rc cdbm_cm_node_init(T_cdbm_dm_node* cm_node)
{
    char key_path[CDBM_MAX_KEYPATH_LEN];

    memset(&cm_node->hh, 0, sizeof(UT_hash_handle));
    HASH_ADD_KEYPTR(hh, g_cdbm_data.dm_node_hash, cm_node->key_path, strlen(cm_node->key_path), cm_node);

    if (strlen(cm_node->key_path) >= CDBM_MAX_KEYPATH_LEN) {
        AAT_FMT_ASSERT(0, ("key_path too long(%d)(%s)", strlen(cm_node->key_path), cm_node->key_path));
    }

    /* verify that the key path is correct for each configuration model node */
    if (cdbm_dm_is_root(cm_node))
    {
        AAT_FMT_ASSERT(strcmp(cm_node->key_path, "/")==0,
                       ("key_path (%s) of root is not correct!", cm_node->key_path));
    }
    else if (cdbm_dm_is_root(cdbm_cm_get_node_from_id(cm_node->parent_idx)))
    {
        snprintf(key_path, CDBM_MAX_KEYPATH_LEN, "/%s", cm_node->key_name);
        AAT_FMT_ASSERT(strcmp(cm_node->key_path, key_path)==0,
                       ("key_path (%s)(%s) of module not correct!", cm_node->key_path, key_path));
    }
    else
    {
        snprintf(key_path, CDBM_MAX_KEYPATH_LEN, "%s/%s",
                 cdbm_cm_get_key_path(cm_node->parent_idx), cm_node->key_name);
        AAT_FMT_ASSERT(strcmp(cm_node->key_path, key_path)==0,
                       ("key_path (%s)(%s) is not correct!", cm_node->key_path, key_path));
    }

    /* key_name should be trim of space,  key_path don't need check
     * because it will verified by key name
     */
    if (!cdbm_dm_is_root(cm_node))
    {
        AAT_FMT_ASSERT(strlen(cm_node->key_name),
                       ("key name nust be not empty, parent (%s)",
                        cdbm_cm_get_key_path(cm_node->parent_idx)));
        AAT_FMT_ASSERT(!isspace(cm_node->key_name[0]),
                       ("configure node (%s) start with space!", cm_node->key_path));
        AAT_FMT_ASSERT(!isspace(cm_node->key_name[strlen(cm_node->key_name)-1]),
                       ("configure node (%s) end with space!", cm_node->key_path));
    }

    return RC_OK;
}

T_global_rc cdbm_dm_init()
{
    T_global_rc ret_cod;
    T_cdbm_dm_node_ops node_ops = {
        cdbm_cm_node_init,        NULL,
        cdbm_cm_node_init,        NULL,
        cdbm_cm_node_init,        NULL,
        cdbm_cm_node_init,        NULL
    };

    g_cdbm_data.dm_node_hash = NULL;
    ret_cod = cdbm_dm_node_walk(&node_ops);
    CDBM_RET_IF_FAIL(ret_cod);

    return RC_OK;
}

T_global_rc cdbm_cm_validate(const char* line)
{

    return RC_OK;
}
