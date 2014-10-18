
#include "aat-incl.h"


#include "cdbm-types.h"


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
    if (strlen(token)<2)
        return 10 ;
    if (token[0] == '0' && tolower(token[1])== 'x')
        return 16 ;

    return 10 ;
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

    snprintf(buf, len, "%lu", val->val.u32);

    return RC_OK;
}

T_global_rc cdbm_str_to_uint32(const T_cdbm_cm_type *type, const char *str, T_cdbm_value *val)
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


// TODO uint32 validation
T_global_rc cdbm_uint32_validate(const T_cdbm_cm_type *type, const T_cdbm_value *val)
{
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

    snprintf(buf, len, "%ld", val->val.i32);

    return RC_OK;
}

T_global_rc cdbm_str_to_int32(const T_cdbm_cm_type *type, const char *str, T_cdbm_value *val)
{
    char* pend=NULL;
    T_global_rc ret_code;

    uint32 i32 = strtol(str, &pend, cdbm_type_get_base(str));
    if (!isspace(*pend) && (*pend)) {
        return RC_CDBM_INVALID_STRING;
    }

    ret_code = cdbm_int32_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    val->type = CDBM_TYPE_INT32;
    val->val.i32 = i32;

    ret_code = cdbm_int32_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    return RC_OK;
}


// TODO  int32 validation
T_global_rc cdbm_int32_validate(const T_cdbm_cm_type *type, const T_cdbm_value *val)
{
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

T_global_rc cdbm_str_to_ipv4(const T_cdbm_cm_type *type, const char *str, T_cdbm_value *val)
{
    T_global_IP_ADDR ipv4;
    T_global_rc ret_code;

    ret_code = ip4_get_addr_from_str(str, &ipv4);
    if (ret_code != RC_OK)
        return RC_CDBM_SYNTAX_ERROR;

    val->type = CDBM_TYPE_IPV4;
    val->val.ipv4 = ipv4;

    ret_code = cdbm_ipv4_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    return RC_OK;
}

T_global_rc cdbm_ipv4_to_str(const T_cdbm_value *val, char *str, uint32 len)
{
    str[0] = '\0';
    if (inet_ntop (AF_INET, ip4_addr, str, len) == NULL)
        return RC_CDBM_STR_NO_SPACE;

    return RC_OK;
}


// TODO  ipv4 validation
T_global_rc cdbm_ipv4_validate(const T_cdbm_cm_type *type, const T_cdbm_value *val)
{
    return RC_OK;
}




/******************************************************************************
 *  the following is types for "ipv6"
 *****************************************************************************/

bool cdbm_ipv6_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_IPV6) || (v2->type != CDBM_TYPE_IPV6))
        return false;

    return ip6_is_addr_equal(v1->val.ipv6, v2->val.ipv6);
}

T_global_rc cdbm_str_to_ipv6(const T_cdbm_cm_type *type, const char *str, T_cdbm_value *val)
{
    T_global_IP6_ADDR ipv6;
    T_global_rc ret_code;

    ret_code = ip6_get_addr_from_str(str, ipv6);
    if (ret_code != RC_OK)
        return RC_CDBM_SYNTAX_ERROR;

    val->type = CDBM_TYPE_IPV6;
    val->val.ipv6 = ipv6;

    ret_code = cdbm_ipv6_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

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


// TODO  ipv6 validation
T_global_rc cdbm_ipv6_validate(const T_cdbm_cm_type *type, const T_cdbm_value *val)
{
    return RC_OK;
}



/******************************************************************************
 *  the following is types for "ipng"
 *****************************************************************************/

bool cdbm_ipng_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{
    if ((v1->type != CDBM_TYPE_IPADDR) || (v2->type != CDBM_TYPE_IPADDR))
        return false;

    return ipng_is_addr_equal(v1->val.ip_addr, v2->val.ip_addr);
}

T_global_rc cdbm_str_to_ipng(const T_cdbm_cm_type *type, const char *str, T_cdbm_value *val)
{
    T_global_IPNG_ADDR ip_addr;
    T_global_rc ret_code;

    ret_code = ipng_get_addr_from_str(str, &ip_addr);
    if (ret_code != RC_OK)
        return RC_CDBM_SYNTAX_ERROR;

    val->type = CDBM_TYPE_IPADDR;
    val->val.ip_addr = ip_addr;

    ret_code = cdbm_ipng_validate(type, val);
    CDBM_RET_IF_FAIL(ret_code);

    return RC_OK;
}

T_global_rc cdbm_ipng_to_str(const T_cdbm_value *val, char *str, uint32 len)
{
    snprintIpnga(str, len, &val->val.ip_addr);

    return RC_OK;
}


// TODO  ipng validation
T_global_rc cdbm_ipng_validate(const T_cdbm_cm_type *type, const T_cdbm_value *val)
{
    return RC_OK;
}






/* the following is common CDBM types translation, it will call different types of calculation */


bool cdbm_val_eq(const T_cdbm_value *v1, const T_cdbm_value *v2)
{

    return false;
}



/* parse value located in str, and validate.
* returns RC_OK if value is syntactically correct, and others otherwise.
*/
T_global_rc cdbm_str_to_val(const T_cdbm_cm_type *type, const char *str, T_cdbm_value *val)
{
    return RC_OK;
}



int cdbm_val_to_str(const T_cdbm_cm_type *type, const T_cdbm_value *val, char *str, uint32 len, const char **strp)
{
    return 0;
}

/* returns RC_OK if value is correct, otherwise RC_FAIL
*/
T_global_rc cdbm_validate(const T_cdbm_cm_type *type, const T_cdbm_value *val)
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




