#include "test.h"
#include "kfc.h"

kfc_sem_t sem;

void *
thread(void *arg)
{
	CHECKPOINT(2);
	kfc_sem_post(&sem);
	CHECKPOINT(3);
	kfc_yield();

	CHECKPOINT(5);
	kfc_yield();
	CHECKPOINT(6);
	kfc_sem_post(&sem);
	CHECKPOINT(7);

	kfc_yield();
	CHECKPOINT(9);
	return NULL;
}

int
main(void)
{
	INIT(1, 0);

	kfc_sem_init(&sem, 0);

	CHECKPOINT(0);
	tid_t thr = THREAD(thread);
	CHECKPOINT(1);

	kfc_sem_wait(&sem);
	CHECKPOINT(4);

	kfc_sem_wait(&sem);
	CHECKPOINT(8);

	void *retval;
	kfc_join(thr, &retval);
	kfc_sem_destroy(&sem);

	VERIFY(10);
}
