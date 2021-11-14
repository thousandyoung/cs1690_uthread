/*
 *   FILE: test.c
 * AUTHOR: Sean Andrew Cannella (scannell)
 *  DESCR: a simple test program for uthreads
 *   DATE: Mon Sep 23 14:11:48 2002
 *  modify by wuyimin,aug 15,2021
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "uthread.h"
#include "uthread_mtx.h"
#include "uthread_cond.h"
#include "uthread_sched.h"

#define NUM_THREADS 16

#define SBUFSZ 256

uthread_id_t    thr[NUM_THREADS];

static void tester(long a0, char *a1[]) {
    int i = 0;

    while (i < 10)    {

        printf("第 %i 个线程，第%i 次循环\n", uthread_self(), i++);
        uthread_yield();
        int j, k = 100000000;
        for (j = 0; j < 1000000; j++) {
            k /= 3;
        }
    }
    printf("第 %i 个线程结束.\n", uthread_self());

    uthread_exit((void *)a0);
    printf("success exit %d\n", ut_curthr->ut_id);
}

int main(int ac, char **av) {
    int i;
  
    uthread_init(); //create main thread and reaper

    for (i = 0; i < NUM_THREADS; i++)     {
        uthread_create(&thr[i], tester, i, NULL, 0);//最后一个参数是优先级
        // int index = thr[i];
        //assert(uthreads[index].ut_link.l_next != NULL);
    }
    printf("create done\n");
    uthread_setprio(thr[3], 2);
    printf("setprio done\n");
    for (i = 0; i < NUM_THREADS; i++)     {
        char pbuffer[SBUFSZ];
        int ret;
        void *tmp;

        uthread_join(thr[i], &tmp);
        
        sprintf(pbuffer, "joined with thread %i, exited %li.\n", thr[i], (long)tmp);
        ret = write(STDOUT_FILENO, pbuffer, strlen(pbuffer));
        if (ret < 0)         {
            perror("uthreads_test");
            return EXIT_FAILURE;
        }
    }
    
    uthread_exit(0);
    printf("MAIN thread exit\n");
    return 0;
}
