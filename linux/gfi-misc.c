
#include "aat-incl.h"
#include "gfi-misc.h"

char *aat_strncpy(char *dst, const char *src, uint32 n)
{
    char           *d;

    if (!dst || !src || (n<=0))
        return (dst);

    /* use n-1 becuase maek sure fill with terminate '\0' */
    for (d = dst; *src && (n-1>0); d++, src++, n--)
        *d = *src;

    *d = '\0';

    return(dst);
}
