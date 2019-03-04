#include <assert.h>
#include <sys/types.h>

#include "kfc.h"

static int inited = 0;

/**
 * Initializes the kfc library.  Programs are required to call this function
 * before they may use anything else in the library's public interface.
 *
 * @param kthreads    Number of kernel threads (pthreads) to allocate
 * @param quantum_us  Preemption timeslice in microseconds, or 0 for cooperative
 *                    scheduling
 *
 * @return 0 if successful, nonzero on failure
 */
int
kfc_init(int kthreads, int quantum_us)
{
	assert(!inited);

	inited = 1;
	return 0;
}

/**
 * Creates a new user thread which executes the provided function concurrently.
 * It is left up to the implementation to decide whether the calling thread
 * continues to execute or the new thread takes over immediately.
 *
 * @param ptid[out]   Pointer to a tid_t variable in which to store the new
 *                    thread's ID
 * @param start_func  Thread main function
 * @param arg         Argument to be passed to the thread main function
 * @param stack_base  Location of the thread's stack if already allocated, or
 *                    NULL if requesting that the library allocate it
 *                    dynamically
 * @param stack_size  Size (in bytes) of the thread's stack, or 0 to use the
 *                    default thread stack size KFC_DEF_STACK_SIZE
 *
 * @return 0 if successful, nonzero on failure
 */
int
kfc_create(tid_t *ptid, void *(*start_func)(void *), void *arg,
		caddr_t stack_base, size_t stack_size)
{
	assert(inited);

	return 0;
}

void
kfc_exit(void *ret)
{
	assert(inited);
}

int
kfc_join(tid_t tid, void **pret)
{
	assert(inited);

	return 0;
}

/**
 * Returns a small integer which identifies the calling thread.
 *
 * @return Thread ID of the currently executing thread
 */
tid_t
kfc_self()
{
	assert(inited);

	return 0;
}

/**
 * Causes the calling thread to yield the processor voluntarily.  This may often
 * result in another thread being scheduled, but it does not preclude the
 * possibility of the same caller continuing if re-chosen by the scheduler.
 */
void
kfc_yield()
{
	assert(inited);
}

int
kfc_sem_init(kfc_sem_t *sem, int value)
{
	assert(inited);
	return 0;
}

int
kfc_sem_post(kfc_sem_t *sem)
{
	assert(inited);
	return 0;
}

int
kfc_sem_wait(kfc_sem_t *sem)
{
	assert(inited);
	return 0;
}

void
kfc_sem_destroy(kfc_sem_t *sem)
{
	assert(inited);
}
