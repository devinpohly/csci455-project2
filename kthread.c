#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#include "kthread.h"

struct kthread_info {
	kthread_t tid;
	pthread_t thread;
};

static struct kthread_info kthreads[MAX_KTHREADS];
static pthread_mutex_t kthreads_lock = PTHREAD_MUTEX_INITIALIZER;

static int inited;

static struct kthread_info *
tidmap_find_tid(kthread_t tid)
{
	int i;
	for (i = 0; i < MAX_KTHREADS; i++)
		if (kthreads[i].tid == tid)
			return &kthreads[i];
	return NULL;
}

static kthread_t
tidmap_add_self(void)
{
	pthread_mutex_lock(&kthreads_lock);
	struct kthread_info *entry = tidmap_find_tid(0);
	if (!entry) {
		pthread_mutex_unlock(&kthreads_lock);
		return -1;
	}
	entry->tid = kthread_self();
	entry->thread = pthread_self();
	pthread_mutex_unlock(&kthreads_lock);

	return entry->tid;
}

static void
tidmap_del(struct kthread_info *entry)
{
	entry->tid = 0;
}

struct trampoline_args {
	sem_t sem;
	void *(*start)(void *);
	void *start_arg;
	kthread_t *ptid;
	int error;
};

static void *
kthread_trampoline(void *arg)
{
	struct trampoline_args *args = arg;
	void *(*start)(void *) = args->start;
	void *start_arg = args->start_arg;

	kthread_t id = tidmap_add_self();
	if (id < 0) {
		args->error = EAGAIN;
		sem_post(&args->sem);
		return NULL;
	}

	*args->ptid = id;

	sem_post(&args->sem);
	return start(start_arg);
}

int
kthread_create(kthread_t *ptid, void *(*start)(void *), void *arg)
{
	if (!inited) {
		tidmap_add_self();
		inited = 1;
	}

	struct trampoline_args args;
	sem_init(&args.sem, 0, 0);
	args.start = start;
	args.start_arg = arg;
	args.ptid = ptid;
	args.error = 0;

	pthread_t thr;
	int rv = pthread_create(&thr, NULL, kthread_trampoline, &args);
	if (rv) {
		sem_destroy(&args.sem);
		return rv;
	}

	sem_wait(&args.sem);
	sem_destroy(&args.sem);
	if (args.error) {
		errno = args.error;
		return -1;
	}
	return 0;
}

void
kthread_exit()
{
	pthread_exit(NULL);
}

int
kthread_join(kthread_t tid, void **pret) {
	int rv;
	pthread_t thread;

	pthread_mutex_lock(&kthreads_lock);
	struct kthread_info *entry = tidmap_find_tid(tid);
	if (!entry) {
		pthread_mutex_unlock(&kthreads_lock);
		errno = ESRCH;
		return -1;
	}
	thread = entry->thread;
	pthread_mutex_unlock(&kthreads_lock);

	rv = pthread_join(thread, pret);
	if (rv)
		return rv;

	pthread_mutex_lock(&kthreads_lock);
	tidmap_del(entry);
	pthread_mutex_unlock(&kthreads_lock);
	return 0;
}

kthread_t kthread_self(void) {
	return (kthread_t) syscall(SYS_gettid);
}

int kthread_mutex_init(kthread_mutex_t *mutex) {
	return pthread_mutex_init(mutex, NULL);
}

int kthread_mutex_lock(kthread_mutex_t *mutex) {
	return pthread_mutex_lock(mutex);
}

int kthread_mutex_unlock(kthread_mutex_t *mutex) {
	return pthread_mutex_unlock(mutex);
}

void kthread_mutex_destroy(kthread_mutex_t *mutex) {
	pthread_mutex_destroy(mutex);
}

int kthread_cond_init(kthread_cond_t *cond) {
	return pthread_cond_init(cond, NULL);
}

int kthread_cond_signal(kthread_cond_t *cond) {
	return pthread_cond_signal(cond);
}

int kthread_cond_wait(kthread_cond_t *cond, kthread_mutex_t *mutex) {
	return pthread_cond_wait(cond, mutex);
}

void kthread_cond_destroy(kthread_cond_t *cond) {
	pthread_cond_destroy(cond);
}

int kthread_sem_init(kthread_sem_t *sem, int value) {
	return sem_init(sem, 0, value);
}

int kthread_sem_post(kthread_sem_t *sem) {
	return sem_post(sem);
}

int kthread_sem_wait(kthread_sem_t *sem) {
	return sem_wait(sem);
}

void kthread_sem_destroy(kthread_sem_t *sem) {
	sem_destroy(sem);
}
