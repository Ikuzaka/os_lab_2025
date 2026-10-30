#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void check(int error) {
    if (error) {
        fprintf(stderr, "pthread: %s\n", strerror(error));
        exit(EXIT_FAILURE);
    }
}

void *thread1_work(void *arg) {
    (void)arg;
    check(pthread_mutex_lock(&mutex1));
    puts("Thread 1: locked mutex1");
    sleep(1); // Даёт второму потоку время захватить mutex2. 
    puts("Thread 1: waiting for mutex2");
    check(pthread_mutex_lock(&mutex2));

    check(pthread_mutex_unlock(&mutex2));
    check(pthread_mutex_unlock(&mutex1));
    return NULL;
}

void *thread2_work(void *arg) {
    (void)arg;
    check(pthread_mutex_lock(&mutex2));
    puts("Thread 2: locked mutex2");
    sleep(1); // Даёт первому потоку время захватить mutex1. 
    puts("Thread 2: waiting for mutex1");
    check(pthread_mutex_lock(&mutex1));

   
    check(pthread_mutex_unlock(&mutex1));
    check(pthread_mutex_unlock(&mutex2));
    return NULL;
}

int main(void) {
    pthread_t thread1, thread2;
    setbuf(stdout, NULL);
    check(pthread_create(&thread1, NULL, thread1_work, NULL));
    check(pthread_create(&thread2, NULL, thread2_work, NULL));
    check(pthread_join(thread1, NULL));
    check(pthread_join(thread2, NULL));
    check(pthread_mutex_destroy(&mutex1));
    check(pthread_mutex_destroy(&mutex2));
    return 0;
}
