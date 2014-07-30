

/* event log for CDBM */


#define CDBM_EVT_ERROR  0

#define CDBM_LOG(level, _strArgs ...)   \
do {                                    \
        printf(_strArgs);               \
        printf("\n");                   \
}  while (0)
