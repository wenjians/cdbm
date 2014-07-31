

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
