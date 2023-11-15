#include "mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include<unistd.h>

static MyThread threads[MAX_THREADS];
static int currentThread = -1;
static int totalThreads = 0;

void myThreadSchedule();

void myThreadExit() {
    // Cleanup resources
    for (int i = 0; i < totalThreads; i++) {
        if (threads[i].context.uc_stack.ss_sp != NULL) {
            free(threads[i].context.uc_stack.ss_sp);
        }
    }
    exit(EXIT_SUCCESS);
}

void myThreadHandler(int signo) {
    myThreadSchedule();
}

void myThreadSchedule() {
    int nextThread = (currentThread + 1) % totalThreads;

    while (threads[nextThread].state == 0 || threads[nextThread].state == 2) {
        nextThread = (nextThread + 1) % totalThreads;
    }

    if (threads[nextThread].state == 3 && alarm(0) > 0) {
        // The next thread is in sleep state, and the sleep time is not finished
        return;
    }

    threads[currentThread].state = 1; // Set current thread state to RUN
    currentThread = nextThread;
    threads[currentThread].state = 1; // Set next thread state to RUN

    if (threads[nextThread].state == 3) {
        // The next thread was in sleep state, and the sleep time has elapsed
        alarm(0);
    }

    if (swapcontext(&threads[currentThread].context, &threads[nextThread].context) == -1) {
        fprintf(stderr, "Error: swapcontext failed.\n");
        exit(EXIT_FAILURE);
    }
}


void myThreadInit() {
    signal(SIGALRM, myThreadHandler);
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10000; // 10 milliseconds
    timer.it_interval = timer.it_value;
    setitimer(ITIMER_REAL, &timer, NULL);
}

int myThreadCreate(MyThread *thread, ThreadFunction func) {
    if (totalThreads >= MAX_THREADS) {
        fprintf(stderr, "Error: Maximum number of threads reached.\n");
        return -1;
    }

    int threadId = totalThreads;
    getcontext(&thread->context);
    thread->context.uc_stack.ss_sp = malloc(8192);
    thread->context.uc_stack.ss_size = 8192;
    thread->context.uc_link = NULL;
    makecontext(&thread->context, func, 0);

    thread->state = 1; // Set thread state to RUN
    threads[threadId] = *thread;
    totalThreads++;

    if (currentThread == -1) {
        currentThread = threadId;
    }

    return threadId;
}

void myThreadYield() {
    threads[currentThread].state = 1; // Set current thread state to RUN
    myThreadSchedule();
}

void myThreadSleep(int seconds) {
    threads[currentThread].state = 3; // Set current thread state to SLP
    alarm(seconds);
    myThreadSchedule();
}

// Added implementation for myThreadGetId
int myThreadGetId() {
    return currentThread;
}
