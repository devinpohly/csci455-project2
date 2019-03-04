#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"
#include "kfc.h"

static char stack[4096];

static void *
subthread_main(void *arg)
{
	long x = (long) arg;
	CHECKPOINT(2);

	ASSERT(x == 73, "argument 73 was not passed");
	ASSERT((char *) &x >= stack && (char *) &x < stack + sizeof(stack),
		"local x not in provided stack");

	CHECKPOINT(3);
	return NULL;
}

static void *
thread_main(void *arg)
{
	long x = (long) arg;
	CHECKPOINT(1);

	ASSERT(x == 42, "argument 42 was not passed");

	THREAD_ARG_STACK(subthread_main, (void *) 73, stack, sizeof(stack));

	ASSERT(x == 42, "argument 42 was changed");

	CHECKPOINT(4);
	return NULL;
}

int
main(void)
{
	INIT(1, 0);

	CHECKPOINT(0);

	THREAD_ARG(thread_main, (void *) 42);

	// Preserve correct behavior once thread switching is implemented
	kfc_yield();

	VERIFY(5);
}
