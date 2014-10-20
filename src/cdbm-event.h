

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
