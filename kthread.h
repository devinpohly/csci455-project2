#ifndef _KTHREAD_H_
#define _KTHREAD_H_

#define MAX_KTHREADS 64

#include <pthread.h>
#include <semaphore.h>

typedef pid_t kthread_t;
typedef pthread_mutex_t kthread_mutex_t;
typedef pthread_cond_t kthread_cond_t;
typedef sem_t kthread_sem_t;

int kthread_create(kthread_t *ptid, void *(*start_func)(void *), void *arg);
void kthread_exit(void);
kthread_t kthread_self(void);
int kthread_join(kthread_t tid, void **pret);

int kthread_mutex_init(kthread_mutex_t *mutex);
int kthread_mutex_lock(kthread_mutex_t *mutex);
int kthread_mutex_unlock(kthread_mutex_t *mutex);
void kthread_mutex_destroy(kthread_mutex_t *mutex);

int kthread_cond_init(kthread_cond_t *cond);
int kthread_cond_signal(kthread_cond_t *cond);
int kthread_cond_wait(kthread_cond_t *cond, kthread_mutex_t *mutex);
void kthread_cond_destroy(kthread_cond_t *cond);

int kthread_sem_init(kthread_sem_t *sem, int value);
int kthread_sem_post(kthread_sem_t *sem);
int kthread_sem_wait(kthread_sem_t *sem);
void kthread_sem_destroy(kthread_sem_t *sem);

#endif
