#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <ucontext.h>

#define MAX_THREADS 100

typedef void (*ThreadFunction)(void);

typedef struct {
    ucontext_t context;
    int state; // 0: DEAD, 1: RUN, 2: SUS, 3: SLP
} MyThread;

void myThreadInit();
int myThreadCreate(MyThread *thread, ThreadFunction func);
void myThreadYield();
void myThreadSleep(int seconds);
void myThreadExit();
int myThreadGetId();  // Added declaration for myThreadGetId

#endif
