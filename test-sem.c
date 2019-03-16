#include <stdio.h>

#include "valgrind.h"
#include "test.h"
#include "kfc.h"

int
main(int argc, char **argv)
{
	if (!RUNNING_ON_VALGRIND)
		DPRINTF("Run `valgrind %s' to check for errors\n", argv[0]);

	INIT(1, 0);

	CHECKPOINT(0);

	kfc_sem_t sems[51];
	int i, j;
	for (i = 0; i < 50; i++) {
		kfc_sem_init(&sems[50], 0);
		kfc_sem_post(&sems[50]);
		kfc_sem_init(&sems[i], i);
		kfc_sem_post(&sems[i]);
		kfc_sem_wait(&sems[50]);
		kfc_sem_destroy(&sems[50]);
	}
	for (i = 0; i < 50; i++) {
		for (j = 0; j < i + 1; j++)
			kfc_sem_wait(&sems[i]);
		kfc_sem_destroy(&sems[i]);
	}

	VERIFY(1);
}
