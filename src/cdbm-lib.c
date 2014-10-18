

/*
 * this is the main of CDBM module
 */

#include "aat-incl.h"
#include "cdbm-lib.h"
#include "cdbm-db.h"

T_cdbm_db_data g_cdbm_db;

void cdbm_lib_init(int phase)
{
    if (phase == 1)
    {
        cdbm_trans_init();
    }
    else if (phase == 2)
    {

    }
    else /* should NEVER come here */
    {
        AAT_FMT_ASSERT(0, ("invalid phase <%d>", phase));
    }
}

/* get one token from buffer,
 * one token means next keyword which is terminated with space ()
 *
 * return how much chars read, NOT include the length of terminal '\0'
 */
uint32 cdbm_lib_get_next_token(const char* buffer, char* token, uint32 max_len)
{
    const char* src = buffer;
    char* dst = token;

    // skip the leading space
    while (*src && isspace(*src))
        src++;

    for (; *src && (!isspace(*src)) && (max_len-1>0); max_len--) {
        *dst++ = *src++;
    }

    *dst = '\0';
    return dst-token;
}

/*
 * int GetNumberType(char* token)
{
    if (strlen(token)<2)
        return 10 ;
    if (token[0] == '0' && tolower(token[1])== 'x')
        return 16 ;

    return 10 ;
}
 *  case PARAM_TYPE_INT4:       // int
        pVariable->value.i4Param = strtol(token, &pend, GetNumberType(token)) ;
        if (!isspace(*pend) && (*pend) ||   // there is something error with number
            (pParam->u4Define1 != 0 || pParam->u4Define2 != 0) &&
            (pVariable->value.i4Param < (INT4)pParam->u4Define1 ||  // too small
             pVariable->value.i4Param > (INT4)pParam->u4Define2))   // too big
                return PARSE_PARAM_WRONG ;
        break ;

    case PARAM_TYPE_UINT4:      // int
        pVariable->value.u4Param = strtoul(token, &pend, GetNumberType(token)) ;
        if (!isspace(*pend) && (*pend) ||   // there is something error with number
            (pParam->u4Define1 != 0 || pParam->u4Define2 != 0) &&   // need compare
            (pVariable->value.u4Param < pParam->u4Define1 ||    // too small
             pVariable->value.u4Param > pParam->u4Define2))     // too big
                return PARSE_PARAM_WRONG ;
        break ;
 */
