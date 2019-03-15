#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"
#include "kfc.h"

static int parent_first = -1;

static void *
subthread_main(void *arg)
{
	CHECKPOINT(2);

	return NULL;
}

static void *
thread2_main(void *arg)
{
	CHECKPOINT(4);

	return NULL;
}

static void *
thread_main(void *arg)
{
	CHECKPOINT(1);

	THREAD(subthread_main);
	if (parent_first) {
		kfc_yield();
		kfc_yield();
		kfc_yield();
	}

	CHECKPOINT(5);

	return NULL;
}

int
main(void)
{
	INIT(1, 0);

	CHECKPOINT(0);

	THREAD(thread_main);
	if (parent_first < 0) {
		parent_first = 1;
		kfc_yield();
		kfc_yield();
	}

	CHECKPOINT(3);

	THREAD(thread2_main);
	if (parent_first) {
		kfc_yield();
		kfc_yield();
	}

	VERIFY(6);
}
