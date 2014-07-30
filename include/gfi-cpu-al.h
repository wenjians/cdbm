
/*******************************************************************************
**
** Description   : this file is used adapt all the CPU related operation
**                 all application should include this file for the CPU related
**                 operation, but not any other header files
**
** Reference(s)  : TLD document (e.g. )
**
*****************************************************************************/

#ifndef _GFI_CPU_AL_H_
#define _GFI_CPU_AL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**
**  1. HEADER (INCLUDE) SECTION
**
*****************************************************************************/
#include "aat-incl.h"

/*****************************************************************************
**
**  2. MACROS           (internal or external according to .h usage)
**     those macro are common for all chips, if they are not common, need
**     move to different architecture
**
*****************************************************************************/


#define SMP_CACHE_BYTES         64
#define ____cacheline_aligned   __attribute__((__aligned__(SMP_CACHE_BYTES)))


/* for the memory barrier, used for SMP share same memory */
#define gfi_memory_barrier()    __asm__ __volatile__("": : :"memory")


/* get the return address for the caller "program counter",
 * it is widely used by application for debug usage, for traciability
 * the return is the pointer, it is uint32 because application use uint32
 * Sun Wenjian, 2010/1/11
 */
#define gfi_return_address()    (uint32)__builtin_return_address(0)
#define gfi_stack_pointer()    (0)

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
static inline void* gfi_current_text_addr(void)
{
    void *pc;

    asm volatile("mov $1f, %0; 1:":"=r" (pc));

    return pc;
}


/* get the cpu cycles for x86 system, it is 32 bit version, same as rdtscl in Linux */
#define   _x86_gfi_cpu_get_cycle(val)   \
        __asm__ __volatile__("rdtsc" : "=a" (val) : : "edx")

/* following is 64bit version, not used currently */
/*#define   _gfi_cpu_get_cycle(val)   __asm__ __volatile__("rdtsc" : "=A" (val))*/


static inline uint32 gfi_cpu_get_cycles()
{
    uint32 ret_val=0;
    _x86_gfi_cpu_get_cycle(ret_val);
    return ret_val;
}



/*****************************************************************************
**
**  3. DATA STRUCTURE DEFINITIONS
**
*****************************************************************************/


/*******************************************************************************
**
**  4. FUNCTION PROTOTYPES
**
*******************************************************************************/


/*****************************************************************************
**
**  5. EXTERNAL FUNCTION DEFINITIONS
**
*****************************************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif /* _GFI_CPU_AL_H_ */

