#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "valgrind.h"
#include "test.h"
#include "kfc.h"

#define TEST_STACK_SZ 16384

static int parent_first = -1;
static char stack[TEST_STACK_SZ];

static void *
subthread_main(void *arg)
{
	long x = (long) arg;
	CHECKPOINT(2);

	ASSERT(x == 73, "argument 73 was not passed");
	ASSERT((char *) &x >= stack && (char *) &x < stack + TEST_STACK_SZ,
		"local x not in provided stack");

	CHECKPOINT(3);
	return NULL;
}

static void *
thread_main(void *arg)
{
	if (parent_first < 0)
		parent_first = 0;

	long x = (long) arg;
	CHECKPOINT(1);

	ASSERT(x == 42, "argument 42 was not passed");

	THREAD_ARG_STACK(subthread_main, (void *) 73, stack, TEST_STACK_SZ);
	if (parent_first)
		kfc_yield();

	ASSERT(x == 42, "argument 42 was changed");

	CHECKPOINT(4);
	return NULL;
}

int
main(void)
{
	VALGRIND_STACK_REGISTER(stack, TEST_STACK_SZ);

	INIT(1, 0);

	CHECKPOINT(0);

	THREAD_ARG(thread_main, (void *) 42);
	if (parent_first < 0) {
		parent_first = 1;
		kfc_yield();
	}

	// Preserve correct behavior once thread switching is implemented
	kfc_yield();

	VERIFY(5);
}
