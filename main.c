#include "mythread.h"
#include <stdio.h>
#include<unistd.h>
void threadFunction() {
    for (int i = 0; i < 5; i++) {
        printf("Thread %d: %d\n", myThreadGetId(), i);
        myThreadYield();
    }

    myThreadExit();
}

void start() {
    for (int i = 0; i < 10; i++) {
        printf("Main Program: %d\n", i);
        myThreadYield();
    }
}

int main() {
    myThreadInit();

    MyThread thread1, thread2;
    myThreadCreate(&thread1, threadFunction);
    myThreadCreate(&thread2, threadFunction);

    start();

    myThreadExit();
    return 0;
}
