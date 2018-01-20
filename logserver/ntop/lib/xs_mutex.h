#ifndef XS_MUTEX_H
#define XS_MUTEX_H
#include <stdlib.h>
#include <string.h>
#include <sched.h>

typedef struct _xs_mutex
{
    int mutex;// = 0;
    int lock;// = 0;
    int unlock;// = 1
}xs_mutex,*pxs_mutex;


#define xs_mutex_init(p)    {memset((p),0,sizeof(xs_mutex)); (p)->unlock=1; (p)->lock=0; (p)->mutex=0; }

#define xs_mutex_lock(p)    { while (!(__sync_bool_compare_and_swap (&(p)->mutex,(p)->lock, 1) )) {sched_yield();} }


#define xs_mutex_unlock(p)  { __sync_bool_compare_and_swap(&(p)->mutex,(p)->unlock,0); }

#endif // XS_MUTEX_H

