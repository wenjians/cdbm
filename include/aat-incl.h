
#ifndef AAT_INCL_H
#define AAT_INCL_H

#include "aat-types.h"
#include "aat-assert.h"
#include "stdio.h"

#ifdef LINUX_MT
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
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


#endif /* AAT_INCL_H */
