#include <unistd.h>
#include <time.h>

#include "test.h"
#include "kfc.h"

#define KTHREADS 4
#define EACH_COUNT 1000000
#define EACH_SLEEP 3000

int count = 0;

void *
thread0(void *arg)
{
	int i;
	for (i = 0; i < EACH_COUNT; i++)
		count++;

	return NULL;
}

void *
thread1(void *arg)
{
	int i;
	for (i = 0; i < EACH_COUNT; i++)
		count++;

	return NULL;
}

void *
sleep0(void *arg)
{
	usleep(EACH_SLEEP);
	return NULL;
}

void *
sleep1(void *arg)
{
	usleep(EACH_SLEEP);
	return NULL;
}

int
main(void)
{
	tid_t tid0, tid1;
	void *dummy;

	INIT(KTHREADS, 0);

	tid0 = THREAD(thread0);
	tid1 = THREAD(thread1);

	kfc_join(tid0, &dummy);
	kfc_join(tid1, &dummy);

	ASSERT(count < (EACH_COUNT * 2), "threads not running in parallel?");

	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	tid0 = THREAD(sleep0);
	tid1 = THREAD(sleep1);

	kfc_join(tid0, &dummy);
	kfc_join(tid1, &dummy);
	clock_gettime(CLOCK_REALTIME, &end);

	if (end.tv_nsec < start.tv_nsec) {
		end.tv_sec--;
		end.tv_nsec += 1000000000;
	}
	end.tv_nsec -= start.tv_nsec;
	end.tv_sec -= start.tv_sec;

	ASSERT(end.tv_sec < 1 && end.tv_nsec <= 1500 * EACH_SLEEP,
			"threads not sleeping in parallel?");

	VERIFY(0);
}
