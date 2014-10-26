

/* event log for CDBM */


#define CDBM_EVT_ERROR  0


#define CDBM_DEBUG(x)  printf x
//#define CDBM_DEBUG(x)  do {} while(0)


#define CDBM_LOG(level, _strArgs ...)   \
do {                                    \
        printf(_strArgs);               \
        printf("\n");                   \
}  while (0)


#define CDBM_RET_IF_FAIL(ret_cod)  \
do {                            \
    if (ret_cod != RC_OK)       \
        return ret_cod;         \
} while (0)


#ifdef MIN
#undef MIN
#endif

#define MIN(x, y) ((x) > (y) ? (y) : (x))            /* get minimum value */

#ifdef MAX
#undef MAX
#endif

#define MAX(x, y) ((x) > (y) ? (x) : (y))            /* get maximum value */
