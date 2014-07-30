
/*******************************************************************************
**
** Description   : it is a common lock adaption for Linux and other OS
**                 
**                 in Linux, all source code running in user space, and the
**                 lock is reuse the pthread mtex, can not protect the
**                 critical section, except disable interrupt, because of
**                 preeptive schedule
**
*****************************************************************************/

#ifndef _GFI_LOCK_H_
#define _GFI_LOCK_H_

#include "aat-incl.h"
#include "gfi-cpu-al.h"
#include <pthread.h>


typedef pthread_mutex_t  T_gfi_lock;


#define GFI_LOCK_DEFINE(_lock)  T_gfi_lock _lock = PTHREAD_MUTEX_INITIALIZER



static inline void gfi_lock_init(T_gfi_lock *lock)
{
    pthread_mutex_init(lock, NULL);
}

static inline void gfi_lock(T_gfi_lock *lock)
{
    pthread_mutex_lock(lock);
}

static inline void gfi_unlock(T_gfi_lock *lock)
{
    pthread_mutex_unlock(lock);
}


#endif /* _GFI_LOCK_H_ */
