/*
 *   FILE: uthread_cond.c
 * AUTHOR: Peter Demoreuille
 *  DESCR: uthreads 条件变量
 *   DATE: Mon Oct  1 01:59:37 2001
 *
 *
 * Modified to handle time slicing by Tom Doeppner
 *   DATE: Sun Jan 10, 2016
 * Modified for SCUT students in July 2021 By Wu Yimin
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"
#include "uthread_mtx.h"
#include "uthread_cond.h"
#include "uthread_queue.h"
#include "uthread_sched.h"
#include "uthread_private.h"


/*
 * uthread_cond_init
 *
 * 初始化指定的条件变量
 */
void uthread_cond_init(uthread_cond_t *cond)
{
    //Function_you_need_to_implement("UTHREADS: uthread_cond_init");
    utqueue_init(&cond->uc_waiters);
}

/*
 * uthread_cond_wait
 * the cond is protected by mtx from multi threads asking for this function
 * before using this method, the mtx has already been locked, 
 * so in this function, we first unlock mtx to prevent deadlock,
 * we wait for the cond to be satisfied, so we make current thread wait, 
 * that is , put the current thread on cond's waiters,  finally we lock mtx again
 * 所作工作：改变当前线程的状态，并将其放入条件变量的等待队列中，切换线程。
 * 
 */
void uthread_cond_wait(uthread_cond_t *cond, uthread_mtx_t *mtx)
{
    //Function_you_need_to_implement("UTHREADS: uthread_cond_wait");
    printf("in cond_wait : ");
    uthread_mtx_unlock(mtx);
    printf(" reaper queue: %d enqueue cond waiter", ut_curthr->ut_id);
    utqueue_enqueue(&cond->uc_waiters, ut_curthr);
    uthread_block(); // change state into wait ans switch
    uthread_mtx_lock(mtx);
    // uthread_switch();
}

/*
 * uthread_cond_broadcast
 *
 * 唤醒等待于此条件变量的艘有线程.
 * 
 */
void uthread_cond_broadcast(uthread_cond_t *cond)
{
    //Function_you_need_to_implement("UTHREADS: uthread_cond_broadcast");
    if (utqueue_empty(&cond->uc_waiters))
    { //no waiters
        return;
    }
    while (!utqueue_empty(&cond->uc_waiters))
    {
        uthread_t *uthread_to_wake = utqueue_dequeue(&cond->uc_waiters);
        printf("broadcast\n");
        uthread_wake(uthread_to_wake);
        
    }
}

/*
 * uthread_cond_signal
 *
 * 唤醒等待于此条件变量的一个线程.
 */
void uthread_cond_signal(uthread_cond_t *cond)
{
    //Function_you_need_to_implement("UTHREADS: uthread_cond_signal");
    if (utqueue_empty(&cond->uc_waiters))
    { //no waiters
    printf("no cond waiters!\n");
        return;
    }
    uthread_t *uthread_to_wake = utqueue_dequeue(&cond->uc_waiters);
    printf("wake cond waiter");
    uthread_wake(uthread_to_wake);
    // uthread_to_wake->ut_state = UT_RUNNABLE;
    // uthread_add_to_runq(uthread_to_wake);
}
