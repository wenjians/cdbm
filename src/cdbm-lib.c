

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
