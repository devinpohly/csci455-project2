/*
 * Queue library by Devin J. Pohly
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#include "queue.h"

/**
 * Initializes a newly-allocated queue
 */
int queue_init(queue_t *this)
{
	assert(this != NULL);

	// Set up the list
	this->head.item = NULL;
	this->head.next = NULL;
	this->tail = &this->head;
	this->size = 0;
	return 0;
}

/**
 * Deinitializes an existing queue
 */
void queue_destroy(queue_t *this)
{
	assert(this != NULL);

	queuenode_t *n = this->head.next;
	while (n != NULL)
	{
		queuenode_t *next = n->next;
		free(n);
		n = next;
	}
}

/**
 * Adds an item to the tail of the queue.
 * Returns nonzero error code on error.
 */
int queue_enqueue(queue_t *this, const void *item)
{
	assert(this != NULL);
	// Can't contain NULL
	assert(item != NULL);

	// Initialize node
	queuenode_t *node;
	if ((node = malloc(sizeof *node)) == NULL)
	{
		return errno;
	}
	node->item = (void *) item;
	node->next = NULL;

	// Rearrange the list
	this->tail->next = node;
	this->tail = node;
	this->size++;
	return 0;
}

/**
 * Inserts an item at the head of the queue.
 * Returns nonzero error code on error.
 */
int queue_insert_first(queue_t *this, const void *item)
{
	assert(this != NULL);
	// Can't contain zero
	assert(item != NULL);

	// Initialize node
	queuenode_t *node;
	if ((node = malloc(sizeof *node)) == NULL)
	{
		return errno;
	}
	node->item = (void *) item;

	// Rearrange the list
	if ((node->next = this->head.next) == NULL)
	{
		this->tail = node;
	}
	this->head.next = node;
	this->size++;
	return 0;
}

/**
 * Removes all items from a queue.
 */
void queue_clear(queue_t *this)
{
	assert(this != NULL);

	queuenode_t *n = this->head.next;
	while (n != NULL)
	{
		queuenode_t *next = n->next;
		free(n);
		n = next;
	}
	this->tail = &this->head;
	this->size = 0;
}

/**
 * Returns the first item in a queue without removing it.
 */
void *queue_peek(const queue_t *this)
{
	assert(this != NULL);

	queuenode_t *n = this->head.next;
	return n ? n->item : NULL;
}

/**
 * Removes the first item in a queue and returns it.
 * Returns NULL if queue is empty.
 */
void *queue_dequeue(queue_t *this)
{
	assert(this != NULL);

	// Remove the node from the list
	queuenode_t *n = this->head.next;
	if (!n)
	{
		// Empty list
		return NULL;
	}
	if ((this->head.next = n->next) == NULL)
	{
		this->tail = &this->head;
	}
	this->size--;

	// Deallocate node and return stored item
	void *item = n->item;
	free(n);
	return item;
}

/**
 * Removes a given item from a list. Returns zero if successful.
 */
int queue_remove_item(queue_t *this, const void *item)
{
	assert(this != NULL);

	queuenode_t *prev = &this->head;
	for (queuenode_t *n = prev->next; n != NULL; n = n->next)
	{
		if (n->item == item)
		{
			if (n->next == NULL)
			{
				this->tail = prev;
			}
			prev->next = n->next;
			this->size--;

			free(n);
			return 0;
		}
		prev = n;
		n = n->next;
	}
	return -1;
}

/**
 * Returns the number of items in a list.
 */
int queue_size(const queue_t *this)
{
	assert(this != NULL);

	return this->size;
}
