/*
 * Queue library by Devin J. Pohly
 */

#ifndef _QUEUE_H
#define _QUEUE_H


typedef struct _queuenode
{
	void *item;
	struct _queuenode *next;
} queuenode_t;

typedef struct _queue
{
	queuenode_t head;
	queuenode_t *tail;
	int size;
} queue_t;

int queue_init(queue_t *this);
void queue_destroy(queue_t *this);

int queue_enqueue(queue_t *this, const void *item);
int queue_insert_first(queue_t *this, const void *item);
void queue_clear(queue_t *this);
void *queue_peek(const queue_t *this);
void *queue_dequeue(queue_t *this);
int queue_remove_item(queue_t *this, const void *item);

int queue_size(const queue_t *this);


#endif
