
/*
 * this file is used to provide the interface for value getting and setting
 * it is operating with plain text string
 *
 * get/set <==> plain text <==> XML <==> config database
 */

#include <aat-incl.h>
#include <return_codes.h>

#include <cdbm-lib.h>


//TODO make sure that key path is less than CDBM_MAX_KEYPATH_LEN


