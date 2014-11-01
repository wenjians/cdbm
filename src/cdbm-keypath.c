

/*
 * this is the main of CDBM module
 */

#include "aat-incl.h"

#include "cdbm-types.h"
#include "cdbm-lib.h"
#include "cdbm-event.h"
#include "cdbm-datamodel.h"
#include "cdbm-global-data.h"

//CDBM_MAX_KEYPATH_LEN
T_cdbm_dm_type cdbm_val_kpath_def_type = {
    .type_def_id = 1,
    .base_type   = CDBM_TYPE_KEYPATH,
    .type.t_int.range = "1..255"
};

const T_cdbm_dm_type* cdbm_val_get_kpath_type() {
    return &cdbm_val_kpath_def_type;
}


bool cdbm_keypath_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_KEYPATH) || (v2->type != CDBM_TYPE_KEYPATH))
        return false;

    return (strcmp(v1->val.str, v2->val.str)==0);
}

T_global_rc cdbm_str_to_keypath(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    uint32 keypath_len;
    T_global_rc ret_code;
    char* temp_addr=NULL;

    keypath_len = strlen(str);
    temp_addr = cdbm_malloc(keypath_len+1);
    if (temp_addr == NULL)
        return RC_CDBM_NO_MORE_MEMORY;

    memcpy(temp_addr, str, keypath_len+1);  // it will copy both string and terminal '\0'

    if (val->val.str != NULL)
        cdbm_free(val->val.str);

    val->type = CDBM_TYPE_KEYPATH;
    val->val.str = temp_addr;

    ret_code = cdbm_keypath_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    /* NOTE: KEYPATH don't need further validation */

    return RC_OK;
}

T_cdbm_value* cdbm_keypath_create(const char* fmt, ...)
{
    T_cdbm_value* kpath;
    va_list ap;
    T_global_rc ret_cod;
    char kpath_str[CDBM_MAX_KEYPATH_LEN];

    kpath = cdbm_val_malloc();

    va_start (ap, fmt);
    vsnprintf(kpath_str, CDBM_MAX_KEYPATH_LEN, fmt, ap);
    va_end (ap);

    cdbm_val_set_type(kpath, CDBM_TYPE_KEYPATH);
    ret_cod = cdbm_str_to_keypath(NULL, kpath_str, kpath);
    if (ret_cod != RC_OK)
        return NULL;

    return kpath;
}

T_global_rc cdbm_keypath_to_str(const T_cdbm_value *val, char *str, uint32 len)
{
    uint32 copy_len;
    if (len <= 1) {
        return RC_CDBM_STR_NO_SPACE;
    }

    if (val->type != CDBM_TYPE_KEYPATH)
        return RC_CDBM_INVALID_VTYPE;

    copy_len = MIN(len-1, strlen(val->val.str));
    memcpy(str, val->val.str, copy_len);
    str[copy_len] = '\0';

    return RC_OK;
}


T_global_rc cdbm_keypath_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    /* for IP address, no range check is need */
    if (val->type != CDBM_TYPE_KEYPATH)
        return RC_CDBM_INVALID_TYPE;

    if (strlen(val->val.str) >= CDBM_MAX_KEYPATH_LEN)
        return RC_CDBM_KEYPATH_TOO_LONG;

    return RC_OK;
}


/* get one token from keypath ,
 * one token means next keyword which is terminated with space ()
 *
 * return :
 *      1: it is not ending
 *      2: it is now ending
 */
bool cdbm_kpath_get_next_token(const T_cdbm_value* kpath, uint32 *offset, char* token, uint32 max_len)
{
    const char* src;
    char* dst = token;
    
    if (kpath->type != CDBM_TYPE_KEYPATH)
        return true;
    
    src = cdbm_kpath_get_string(kpath) + (*offset);

    // skip the leading space
    while (*src && (isspace(*src) || (*src == '/')))
        src++;

    // skip the hierarchy level char
    while (*src == '/')
        src++;

    for (; *src && (!isspace(*src)) && (*src!='/') && (max_len-1>0); max_len--) {
        *dst++ = *src++;
    }

    /* skip ending space */
    while (*src && (isspace(*src) || (*src == '/')))
        src++;

    *dst = '\0';
    *offset = src-cdbm_kpath_get_string(kpath);
    
    return (*src == '\0');
}

const char* cdbm_kpath_get_string(const T_cdbm_value *kpath)
{
    return kpath->val.str;
}




