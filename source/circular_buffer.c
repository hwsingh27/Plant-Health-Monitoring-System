#include "common.h"
/*
 * buffer.c
 *
 *  Created on: Dec 10, 2021
 *      Author: hwsin
 */

/**
 * @brief: initialization function that sets front, rear pointers to 0,
 * items to 0 and sets capacity to the maximum capacity of the buffer
 *
 * @param: pointer to the buffer
 * @return: NULL
 */
void cbuff_init(cbuff_t *c)
{
	for(int i=0;i<MAX_CAPACITY;i++)
	{
		c->data[i]='_'; // make all the items as '_' for easy debugging
	}
	c->front=0; // front is used to read and remove items from the buffer
	c->rear=0; // rear is used to write items into the buffer
	c->items=0; // "items" tracks the number of items in the buffer
	c->capacity=MAX_CAPACITY;
}

/**
 * @brief: computes whether the buffer is full or not.
 *
 * @param: pointer to the buffer
 * @return: returns 1 if full and 0 if not
 */
uint8_t ifFull(cbuff_t *c)
{
	if(c->items==MAX_CAPACITY)
		return 1;
	else
		return 0;
}

/**
 * @brief: computes whether the buffer is empty or not
 *
 * @param: pointer to the buffer
 * @return: returns 1 if empty and 0 if not
 */
uint8_t ifEmpty(cbuff_t *c)
{
	if(c->items==0)
		return 1;
	else
		return 0;
}

/**
 * @brief: function to return the capacity of the buffer
 *
 * @param: pointer to the buffer
 * @return: returns the capacity of the buffer
 */
size_t cbuff_capacity(cbuff_t *c)
{
	return (c->capacity);
}

size_t cbuff_length(cbuff_t *c)
{
	return (c->items);
}

/**
 * @brief: function to enqueue the elements into the buffer,
 * interrupts are enabled and disabled accordingly (race conditions)
 * for the operations to be performed correctly.
 *
 * @param: pointer to the buffer, pointer the source from where the items are to be
 * enqueued and the number of bytes to be enqueued
 * @return: returns the number of items/elements enqueued
 */
size_t cbuff_enqueue(cbuff_t *c,void *buffer,size_t nbyte)
{
	size_t flag=0;
	__disable_irq();
	if(ifFull(c))
	{
		__enable_irq();
		return 0;
	}
	else
	{
		while((c->items!=c->capacity) && flag<nbyte)
		{
			(c->data[c->rear])=*(uint8_t *)(buffer++);
			c->items++;
			c->rear=(c->rear+1)%c->capacity;
			flag++;
		}
	}
	__enable_irq();
	return flag;
}

/**
 * @brief: function to dequeue the elements into the buffer,
 * interrupts are enabled and disabled accordingly (race conditions)
 * for the operations to be performed correctly.
 *
 * @param: pointer to the source buffer, destination pointer where the dequeued
 * elements are stored and the number of bytes to be dequeued
 * @return: returns the number of items/elements dequeued
 */
size_t cbuff_dequeue(cbuff_t *c,void *buffer,size_t nbyte)
{
	size_t flag=0;

	__disable_irq();
	if(ifEmpty(c))
	{
		__enable_irq();
		return 0;
	}
	else
	{
		while((c->items!=0) && flag<nbyte)
		{
			*(uint8_t *)(buffer++)=c->data[c->front];
			c->data[c->front]='_';
			c->items--;
			c->front=(c->front+1)%c->capacity;
			flag++;
		}
	}
	__enable_irq();
	return flag;
}

