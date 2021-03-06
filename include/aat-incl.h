
#ifndef AAT_INCL_H
#define AAT_INCL_H

#include "aat-types.h"
#include "aat-assert.h"
#include "gfi-cpu-al.h"
#include "gfi-misc.h"


#ifdef LINUX_MT
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <ctype.h>
//#include <linux/compiler.h>
#include <stdlib.h>



/* the follwoing is for Linux likely definition */

#if __GNUC__ == 2 && __GNUC_MINOR__ < 96
#define __builtin_expect(x, expected_value) (x)
Oooooooooooooooops
#endif

#undef  likely
#undef  unlikely

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#endif


#ifndef NULL
#define NULL   ((void *) 0)
#endif

#define MemAlloc(size,type,callerID,qos,bufpp)  malloc(size)


#endif /* AAT_INCL_H */
