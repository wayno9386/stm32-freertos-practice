#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;              // 互斥锁：同一时刻只有一个线程能拿到

void *task(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);    // 上锁：拿不到就阻塞等待
        counter++;
        pthread_mutex_unlock(&lock);  // 解锁：释放给别的线程
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&lock, NULL);  // 初始化锁

    pthread_t t1, t2;
    pthread_create(&t1, NULL, task, NULL);
    pthread_create(&t2, NULL, task, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);     // 销毁锁
    printf("counter = %d\n", counter);  // 这次是200000
    return 0;
}
