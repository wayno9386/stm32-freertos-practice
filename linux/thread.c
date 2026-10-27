#include <stdio.h>
#include <pthread.h>   // 线程库

int counter = 0;

void *task(void *arg) {          // 线程函数，返回void*
    for (int i = 0; i < 100000; i++) {
        counter++;               // 两个线程同时改，会竞争
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;            // 线程ID

    pthread_create(&t1, NULL, task, NULL);   // 创建线程1，跑task
    pthread_create(&t2, NULL, task, NULL);   // 创建线程2

    pthread_join(t1, NULL);      // 等线程1结束（类比FreeRTOS的xTaskJoin）
    pthread_join(t2, NULL);      // 等线程2结束

    printf("counter = %d\n", counter);   // 期望200000
    return 0;
}
