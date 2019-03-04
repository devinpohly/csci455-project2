#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"
#include "kfc.h"

static void *
thread2_main(void *arg)
{
	CHECKPOINT(2);
	kfc_exit(NULL);

	ASSERT(0, "thread2 exit didn't");
	return NULL;
}

static void *
thread_main(void *arg)
{
	CHECKPOINT(1);

	THREAD(thread2_main);

	CHECKPOINT(4);
	kfc_exit(NULL);

	ASSERT(0, "thread exit didn't");
	return NULL;
}

int
main(void)
{
	INIT(1, 0);

	CHECKPOINT(0);

	THREAD(thread_main);

	CHECKPOINT(3);
	kfc_yield();

	CHECKPOINT(5);
	kfc_yield();

	CHECKPOINT(6);
	kfc_yield();

	CHECKPOINT(7);
	kfc_yield();

	CHECKPOINT(8);
	kfc_yield();

	VERIFY(9);
	return 0;
}
