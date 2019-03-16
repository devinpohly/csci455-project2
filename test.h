#ifndef _TEST_H_
#define _TEST_H_

/* Some functions to help write tests more efficiently */

#include <stdio.h>
#include <stdlib.h>

#include "kfc.h"

static int _test_counter = 0;
static int _test_nope = 0;

static inline void
INIT(int nthreads, int quantum_us)
{
	if (kfc_init(nthreads, quantum_us)) {
		perror("kfc_init");
		exit(1);
	}
}

static inline void
CHECKPOINT(int n)
{
	if (_test_counter != n) {
		_test_nope++;
		DPRINTF("nope: ");
	}
	DPRINTF("checkpoint %d\n", n);
	_test_counter++;
}

static inline void
ASSERT(int cond, char *text)
{
	if (!cond) {
		_test_nope++;
		DPRINTF("nope: %s\n", text);
	}
}

static inline void
VERIFY(int n)
{
	kfc_teardown();

	if (_test_counter != n) {
		DPRINTF("nope: hit %d checkpoints instead of %d\n",
				_test_counter, n);
		_test_nope++;
	}
	if (!_test_nope) {
		DPRINTF("success!\n");
		exit(0);
	}

	DPRINTF("try again (%d nopes)\n", _test_nope);
	exit(1);
}

static inline tid_t
THREAD_ARG_STACK(void *(*func)(void *), void *arg, void *stack, size_t stack_sz)
{
	tid_t tid;
	if (kfc_create(&tid, func, arg, stack, stack_sz)) {
		perror("kfc_create");
		exit(1);
	}
	return tid;
}

static inline tid_t
THREAD_ARG(void *(*func)(void *), void *arg)
{
	return THREAD_ARG_STACK(func, arg, NULL, 0);
}

static inline tid_t
THREAD(void *(*func)(void *))
{
	return THREAD_ARG(func, NULL);
}

#endif
