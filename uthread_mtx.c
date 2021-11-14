/*
 *   FILE: uthread_mtx.c
 * AUTHOR: Peter Demoreuille
 *  DESCR: userland mutexes
 *   DATE: Sat Sep  8 12:40:00 2001
 *
 *
 * Modified to handle time slicing by Tom Doeppner
 *   DATE: Sun Jan 10, 2016
 * Modified for SCUT students in July 2021 By Wu Yimin
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "uthread.h"
#include "uthread_mtx.h"
#include "uthread_sched.h"

/*
 * uthread_mtx_init
 *
 * 初始化指定的 mutex.
 */
void uthread_mtx_init(uthread_mtx_t *mtx) {
    //Function_you_need_to_implement("UTHREADS: uthread_mtx_init");
    mtx->m_owner = NULL; //unlocked
    utqueue_init(&mtx->m_waiters);
}

/*
 * uthread_mtx_lock
 *
 * 如果没有线程占有该锁，则将锁的拥有着改为当前线程，否则，当前阻塞.  
 */
void uthread_mtx_lock(uthread_mtx_t *mtx) {
    //Function_you_need_to_implement("UTHREADS: uthread_mtx_lock");
    if(mtx->m_owner == NULL){
        mtx->m_owner = ut_curthr;
        printf("mtx lock: mtx owner is %d\n", ut_curthr->ut_id);
    }else {
        if(mtx->m_owner->ut_id == ut_curthr->ut_id) {
            printf("cur id == mtx owner id\n");
            return;
        }
        printf(" block, enqueue mtx waiter, mtx owner is %d\n", mtx->m_owner->ut_id);
        utqueue_enqueue(&mtx->m_waiters, ut_curthr);//wait for the mtx
        uthread_block();
    }
    
}

/*
 * uthread_mtx_trylock
 *
 * 试图上锁 mutex, 得到锁时返回 1 , 否则返回 0 .
 */
int uthread_mtx_trylock(uthread_mtx_t *mtx) {
    //Function_you_need_to_implement("UTHREADS: uthread_mtx_trylock");
    if(mtx->m_owner == NULL) {
        mtx->m_owner = ut_curthr;
        return 1;
    }else {
        return 0;
    }
}

/*
 * uthread_mtx_unlock
 *
 * 释放锁.  如果有其他线程在等待该锁，则唤醒它.
 */
void uthread_mtx_unlock(uthread_mtx_t *mtx) {
    //Function_you_need_to_implement("UTHREADS: uthread_mtx_unlock");
    //if someone is waiting to get this mutex,get this thread, rid it off the waiters, give this to it, and wake it up
    if(!utqueue_empty(&mtx->m_waiters)) {
        uthread_t *waiter = utqueue_dequeue(&mtx->m_waiters);
        mtx->m_owner = waiter;
        printf("unlock mtx, wake mtx waiter\n");
        uthread_wake(waiter);
    }else {
        mtx->m_owner = NULL;
    }
    
}
