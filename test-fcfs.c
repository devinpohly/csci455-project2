#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"
#include "kfc.h"

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

	CHECKPOINT(5);

	return NULL;
}

int
main(void)
{
	INIT(1, 0);

	CHECKPOINT(0);

	THREAD(thread_main);

	CHECKPOINT(3);

	THREAD(thread2_main);

	VERIFY(6);
}
