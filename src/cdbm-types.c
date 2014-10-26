
#include "aat-incl.h"

#include <arpa/inet.h>

#include "cdbm-lib.h"
#include "cdbm-types.h"
#include "cdbm-event.h"

/* the following is operation for different types, for each type, the following
 * operation are mandatory:
 *  - check whether two value are equal
 *  - translate string to value
 *  - translate value to string
 *      does not print more than len bytes, including trailing NUL.
 *      return value as snprintf (different OS may have different return value)
 *      Linux: if the value is correct for the type, it returns the length of the string form regardless
 *             of the len limit
 *             otherwise it returns a negative number. thus, the NUL terminated
 *             output has been completely written if and only if the returned
 *             value is nonnegative and less than len.
 *             If strp is non-NULL and the string form is constant (i.e. C_ENUM_HASH),
 *             a pointer to the string is stored in *strp.
 *  - validate whether one value is according to type specification
 */

static int cdbm_type_get_base(const char* token)
{
    /* skip the preceding space or minus (-) */
    while (*token && isspace(*token))
        token++;
    if (*token == '-')
        token++;

    if (strlen(token)<2)
        return 10 ;
    if (token[0] == '0' && tolower(token[1])== 'x')
        return 16 ;

    return 10 ;
}


T_cdbm_val_opt g_cdbm_val_ops[CDBM_TYPE_MAX] = {

    /* val_type         val_eq,             val_to_str,         str_to_val,         val_valid           */
    {CDBM_TYPE_EMPTY,   NULL,               NULL,               NULL,               NULL                },
    {CDBM_TYPE_UINT32,  cdbm_uint32_val_eq, cdbm_uint32_to_str, cdbm_str_to_uint32, cdbm_uint32_validate},
    {CDBM_TYPE_INT32,   cdbm_int32_val_eq,  cdbm_int32_to_str,  cdbm_str_to_int32,  cdbm_int32_validate },
    {CDBM_TYPE_IPV4,    cdbm_ipv4_val_eq,   cdbm_ipv4_to_str,   cdbm_str_to_ipv4,   cdbm_ipv4_validate  },
    {CDBM_TYPE_IPV6,    cdbm_ipv6_val_eq,   cdbm_ipv6_to_str,   cdbm_str_to_ipv6,   cdbm_ipv6_validate  },
    {CDBM_TYPE_IPADDR,  cdbm_ipng_val_eq,   cdbm_ipng_to_str,   cdbm_str_to_ipng,   cdbm_ipng_validate  },
    {CDBM_TYPE_MAC,   NULL,     NULL,               NULL,               NULL  },
    {CDBM_TYPE_HEX,     cdbm_uint32_val_eq, cdbm_uint32_to_str, cdbm_str_to_uint32, cdbm_uint32_validate},
    {CDBM_TYPE_STRING,   NULL,  NULL,               NULL,               NULL   },
    {CDBM_TYPE_STR_WORD,   NULL, NULL,               NULL,               NULL},
    {CDBM_TYPE_BOOL,   NULL,     NULL,               NULL,               NULL  },
    {CDBM_TYPE_ENUM_HASH,   NULL, NULL,               NULL,               NULL },
    {CDBM_TYPE_BUF,   NULL,      NULL,               NULL,               NULL  },
    {CDBM_TYPE_KEYPATH, cdbm_keypath_val_eq,cdbm_keypath_to_str,cdbm_str_to_keypath,cdbm_keypath_validate},
};


//T_gfi_slab_pool* cdbm_val_pool = NULL;

T_cdbm_value* cdbm_val_alloc()
{
    return (T_cdbm_value*)malloc(sizeof(T_cdbm_value));
#if 0
    uint32 ret_addr = gfi_return_address();
    T_cdbm_value *p_val;

    p_val = gfi_slab_pool_alloc_with_ra(cdbm_val_pool, ret_addr);
    if (p_val != NULL) {
        p_val->type = CDBM_TYPE_EMPTY;
    }

    return p_val;
#endif
}

void cdbm_val_free(T_cdbm_value *p_val)
{
    uint32 ret_addr = gfi_return_address();

    switch (p_val->type) {
    case CDBM_TYPE_STRING:
    case CDBM_TYPE_STR_WORD:
    case CDBM_TYPE_KEYPATH:
        cdbm_free(p_val->val.str);
        break;

    case CDBM_TYPE_ENUM_HASH:
        cdbm_free(p_val->val.enum_val.str_val);
        break;
    case CDBM_TYPE_BUF:
        cdbm_free(p_val->val.buf.ptr);
        break;
    default:
        CDBM_LOG(CDBM_EVT_ERROR, "Error: cdbm_val_free unsupported type (%u), caller (0x%x)",
                p_val->type, ret_addr);
        break;
    }


    free(p_val);
#if 0


    gfi_slab_pool_free_with_ra(cdbm_val_pool, p_val, ret_addr);
#endif
}


/******************************************************************************
 *  the following is types for "uint32"
 *****************************************************************************/
bool cdbm_uint32_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_UINT32) || (v2->type != CDBM_TYPE_UINT32))
        return false;

    return (v1->val.u32 == v2->val.u32);
}

T_global_rc cdbm_uint32_to_str(const T_cdbm_value *val, char *buf, uint32 len)
{
    AAT_STD_ASSERT(buf != NULL);

    snprintf(buf, len, "%u", val->val.u32);

    return RC_OK;
}

T_global_rc cdbm_str_to_uint32(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    char* pend=NULL;
    T_global_rc ret_code;

    uint32 u32 = strtoul(str, &pend, cdbm_type_get_base(str));
    if (!isspace(*pend) && (*pend)) {
        return RC_CDBM_INVALID_STRING;
    }

    val->type = CDBM_TYPE_UINT32;
    val->val.u32 = u32;

    ret_code = cdbm_uint32_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    return RC_OK;
}


T_global_rc cdbm_uint32_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    if (val->type != CDBM_TYPE_UINT32)
        return RC_CDBM_INVALID_TYPE ;

    if (type->base_type != CDBM_TYPE_UINT32)
        return RC_CDBM_INVALID_TYPE ;

    if (!cdbm_val_uint32_in_range(val->val.u32, type->type.t_int.range))
        return RC_CDBM_INVALID_RANGE;

    return RC_OK;
}



/******************************************************************************
 *  the following is types for "int32"
 *****************************************************************************/
bool cdbm_int32_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_INT32) || (v2->type != CDBM_TYPE_INT32))
        return false;

    return (v1->val.i32 == v2->val.i32);
}

T_global_rc cdbm_int32_to_str(const T_cdbm_value *val, char *buf, uint32 len)
{
    AAT_STD_ASSERT(buf != NULL);

    snprintf(buf, len, "%d", val->val.i32);

    return RC_OK;
}

T_global_rc cdbm_str_to_int32(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    char* pend=NULL;
    T_global_rc ret_code;

    int32 i32 = strtol(str, &pend, cdbm_type_get_base(str));
    if (!isspace(*pend) && (*pend)) {
        return RC_CDBM_INVALID_STRING;
    }

    val->type = CDBM_TYPE_INT32;
    val->val.i32 = i32;

    ret_code = cdbm_int32_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    return RC_OK;
}



T_global_rc cdbm_int32_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    if (val->type != CDBM_TYPE_INT32)
        return RC_CDBM_INVALID_TYPE ;

    if (type->base_type != CDBM_TYPE_INT32)
        return RC_CDBM_INVALID_TYPE ;

    if (!cdbm_val_int32_in_range(val->val.i32, type->type.t_int.range))
        return RC_CDBM_INVALID_RANGE;

    return RC_OK;
}




/******************************************************************************
 *  the following is types for "ipv4"
 *****************************************************************************/

bool cdbm_ipv4_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_IPV4) || (v2->type != CDBM_TYPE_IPV4))
        return false;

    return ip4_is_addr_equal(v1->val.ipv4, v2->val.ipv4);
}

T_global_rc cdbm_str_to_ipv4(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    T_global_IP_ADDR ipv4;
    T_global_rc ret_code;

    ret_code = ip4_get_addr_from_str(str, &ipv4);
    if (ret_code != RC_OK)
        return RC_CDBM_INVALID_IP_ADDR;

    val->type = CDBM_TYPE_IPV4;
    val->val.ipv4 = ipv4;

    /* NOTE: IPv4 don't need further validation, e.g. range */

    return RC_OK;
}

T_global_rc cdbm_ipv4_to_str(const T_cdbm_value *val, char *str, uint32 len)
{
    str[0] = '\0';
    if (inet_ntop (AF_INET, &val->val.ipv4, str, len) == NULL)
        return RC_CDBM_STR_NO_SPACE;

    return RC_OK;
}


T_global_rc cdbm_ipv4_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    /* for IPv4, no range check is need */

    if (val->type != CDBM_TYPE_IPV4)
        return RC_CDBM_INVALID_TYPE;

    return RC_OK;
}




/******************************************************************************
 *  the following is types for "ipv6"
 *****************************************************************************/

bool cdbm_ipv6_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_IPV6) || (v2->type != CDBM_TYPE_IPV6))
        return false;

    return ip6_is_addr_equal(&v1->val.ipv6, &v2->val.ipv6);
}

T_global_rc cdbm_str_to_ipv6(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    T_global_IP6_ADDR ipv6;
    T_global_rc ret_code;

    ret_code = ip6_get_addr_from_str(str, &ipv6);
    if (ret_code != RC_OK)
        return RC_CDBM_INVALID_IP_ADDR;

    val->type = CDBM_TYPE_IPV6;
    val->val.ipv6 = ipv6;

    /* NOTE: IPv4 don't need further validation, e.g. range */

    return RC_OK;
}

T_global_rc cdbm_ipv6_to_str(const T_cdbm_value *val, char *str, uint32 len)
{
    //char* snprintIp6a(char *buf, uint32 len, const T_global_IP6_ADDR *ip6_addr)

    str[0] = '\0';
    if (inet_ntop (AF_INET6, &val->val.ipv6, str, len) == NULL)
        return RC_CDBM_STR_NO_SPACE;

    return RC_OK;
}


T_global_rc cdbm_ipv6_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    /* for IPv6, no range check is need */
    if (val->type != CDBM_TYPE_IPV6)
        return RC_CDBM_INVALID_TYPE;

    return RC_OK;
}



/******************************************************************************
 *  the following is types for "ipng"
 *****************************************************************************/

bool cdbm_ipng_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_IPADDR) || (v2->type != CDBM_TYPE_IPADDR))
        return false;

    return ipng_is_addr_equal(&v1->val.ip_addr, &v2->val.ip_addr);
}

T_global_rc cdbm_str_to_ipng(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    T_global_IPNG_ADDR ip_addr;
    T_global_rc ret_code;

    ret_code = ipng_get_addr_from_str(str, &ip_addr);
    if (ret_code != RC_OK)
        return ret_code;

    val->type = CDBM_TYPE_IPADDR;
    val->val.ip_addr = ip_addr;

    /* NOTE: IPv4 don't need further validation, e.g. range */

    return RC_OK;
}

T_global_rc cdbm_ipng_to_str(const T_cdbm_value *val, char *str, uint32 len)
{
    snprintIpnga(str, len, &val->val.ip_addr);

    return RC_OK;
}


T_global_rc cdbm_ipng_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    /* for IP address, no range check is need */
    if (val->type != CDBM_TYPE_IPADDR)
        return RC_CDBM_INVALID_TYPE;

    return RC_OK;
}



/******************************************************************************
 *  the following is types for "key path"
 *****************************************************************************/

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
    temp_addr = MemAlloc(keypath_len+1, MEM_LONGTERM, AAT_MOD_CDBM, 0, NULL);
    if (temp_addr == NULL)
        return RC_CDBM_NO_MORE_MEMORY;

    memcpy(temp_addr, str, keypath_len+1);  // it will copy both string and terminal '\0'

    val->type = CDBM_TYPE_KEYPATH;
    val->val.str = temp_addr;

    ret_code = cdbm_keypath_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    /* NOTE: KEYPATH don't need further validation */

    return RC_OK;
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
    if (val->type != CDBM_TYPE_IPADDR)
        return RC_CDBM_INVALID_TYPE;

    if (strlen(val->val.str) >= CDBM_MAX_KEYPATH_LEN)
        return RC_CDBM_KEYPATH_TOO_LONG;

    return RC_OK;
}

/******************************************************************************
 *  the following is types for "MAC address"
 *  the MAC address support two formats
 *     1) aa-bb-cc-dd-ee-ff
 *     2) aa:bb:cc:dd:ee:ff
 *  it is okay to support both upper or lower case
 *****************************************************************************/
#if 0
bool cdbm_int32_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_INT32) || (v2->type != CDBM_TYPE_INT32))
        return false;

    return (v1->val.i32 == v2->val.i32);
}

T_global_rc cdbm_int32_to_str(const T_cdbm_value *val, char *buf, uint32 len)
{
    AAT_STD_ASSERT(buf != NULL);

    snprintf(buf, len, "%d", val->val.i32);

    return RC_OK;
}

T_global_rc cdbm_str_to_int32(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    char* pend=NULL;
    T_global_rc ret_code;

    int32 i32 = strtol(str, &pend, cdbm_type_get_base(str));
    if (!isspace(*pend) && (*pend)) {
        return RC_CDBM_INVALID_STRING;
    }

    val->type = CDBM_TYPE_INT32;
    val->val.i32 = i32;

    ret_code = cdbm_int32_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    return RC_OK;
}



T_global_rc cdbm_int32_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    if (val->type != CDBM_TYPE_INT32)
        return RC_CDBM_INVALID_TYPE ;

    if (type->base_type != CDBM_TYPE_INT32)
        return RC_CDBM_INVALID_TYPE ;

    if (!cdbm_val_int32_in_range(val->val.i32, type->type.t_int.range))
        return RC_CDBM_INVALID_RANGE;

    return RC_OK;
}
#endif


#if 0
/* the following is common CDBM types translation, it will call different types of calculation */


bool cdbm_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{

    return false;
}



/* parse value located in str, and validate.
* returns RC_OK if value is syntactically correct, and others otherwise.
*/
T_global_rc cdbm_str_to_val(const T_cdbm_dm_type *type, const char *str, T_cdbm_value *val)
{
    return RC_OK;
}



int cdbm_val_to_str(const T_cdbm_dm_type *type, const T_cdbm_value *val, char *str, uint32 len, const char **strp)
{
    return 0;
}

/* returns RC_OK if value is correct, otherwise RC_FAIL
*/
T_global_rc cdbm_validate(const T_cdbm_dm_type *type, const T_cdbm_value *val)
{
    return RC_OK;
}

/*
 * This function copies the contents of *val to *new_val, allocating memory for
 * the actual value for the types that need it. It returns new_val, or NULL if
 * allocation failed. The allocated memory (if any) can be freed with cdbm_free_value().
 */
T_cdbm_value *cdbm_value_dup_to(const T_cdbm_value *val, T_cdbm_value *new_val)
{
    return NULL;
}

/*
 * This function will free the nessary memory allocated by cdbm_value_dup_to
 * and it will NOT free val
 */
void cdbm_free_value(T_cdbm_value *val)
{

}
/*
 * This function allocates memory and duplicates *val, i.e. a T_cdbm_value struct
 * is always allocated, memory for the actual value is also allocated for the
 * types that need it. Returns a pointer to the new T_cdbm_value, or NULL if
 * allocation failed. The allocated memory can be freed with cdbm_free_dup_value().
 */
T_cdbm_value *cdbm_value_dup(const T_cdbm_value *val)
{
    return NULL;
}

/*
 * Frees memory allocated by cdbm_value_dup(). Note this is not the same as
 * cdbm_free_value() since, cdbm_value_dup() also allocates the actual T_cdbm_value
 * struct.
 */
void cdbm_free_dup_value(T_cdbm_value *val)
{

}

#endif





/* the following is used for range check */

const char* cdbm_val_range_get_next_number(const char* buffer, char* token, uint32 max_len)
{
    const char* src = buffer;
    char* dst = token;

    // skip the leading space
    while (*src && isspace(*src))
        src++;

    for (; *src && (!isspace(*src)) && (*src!='.') && (*src!='|') && (max_len-1>0); max_len--) {
        *dst++ = *src++;
    }

    while (*src && isspace(*src))
        src++;

    *dst = '\0';
    return src;
}

const char* cdbm_val_get_next_one_range(const char* range_string, char* min, char*max, uint32 max_len)
{

    const char* cur_pos = range_string;

    cur_pos = cdbm_val_range_get_next_number(cur_pos, min, max_len);
    if (*cur_pos == '.')
    {
        while (*cur_pos == '.')
            cur_pos++;

        cur_pos = cdbm_val_range_get_next_number(cur_pos, max, max_len);
    }
    else
    {
        strcpy(max, min);
    }

    if (*cur_pos == '|')
        cur_pos++;

    return cur_pos;
}


bool cdbm_val_uint32_in_range(uint32 val, const char* range_string)
{
    char min_val_str[128], max_val_str[128];
    uint32 max_val, min_val;
    const char* cur_pos = range_string;
    char* pend=NULL;

    //CDBM_DEBUG(("range_list=<%s>\n", range_list));
    while (*cur_pos) {
        cur_pos = cdbm_val_get_next_one_range(cur_pos, min_val_str, max_val_str, 127);
        min_val = strtoul(min_val_str, &pend, cdbm_type_get_base(min_val_str));
        max_val = strtoul(max_val_str, &pend, cdbm_type_get_base(max_val_str));

        /* the range list is sorted from small to big, if it already smaller than
         * minimal value, it must be failure
         */
        if (val < min_val)
            return false;

        if ((val >= min_val) && (val <= max_val))
        {
            /*
            CDBM_DEBUG(("source value<%u>, range_list=<%s>, hit range<%s, %s>\n",
                    val, range_string, min_val_str, max_val_str));
            */
            return true;
        }
    }

    return false;
}


bool cdbm_val_int32_in_range(int32 val, const char* range_string)
{
    char min_val_str[128], max_val_str[128];
    int32 max_val, min_val;
    const char* cur_pos = range_string;
    char* pend=NULL;

    //CDBM_DEBUG(("range_list=<%s>\n", range_list));
    while (*cur_pos) {
        cur_pos = cdbm_val_get_next_one_range(cur_pos, min_val_str, max_val_str, 127);
        min_val = strtol(min_val_str, &pend, cdbm_type_get_base(min_val_str));
        max_val = strtol(max_val_str, &pend, cdbm_type_get_base(max_val_str));

        /* the range list is sorted from small to big, if it already smaller than
         * minimal value, it must be failure
         */
        if (val < min_val)
            return false;

        if ((val >= min_val) && (val <= max_val))
        {
            /*
            CDBM_DEBUG(("source value<%d>, range_list=<%s>, hit range<%s, %s>\n",
                    val, range_string, min_val_str, max_val_str));
            */
            return true;
        }
    }

    return false;
}



