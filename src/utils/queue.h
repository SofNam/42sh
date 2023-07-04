#ifndef QUEUE_H
#define QUEUE_H

/// @file queue.h
/// @brief Contains implementation of queue and definition of related functions.

/// Element of the queue and pointer to the next element.
struct queue_element
{
    void *element;
    struct queue_element *next;
};

/// Dynamic implementation of a queue.
struct queue
{
    struct queue_element *head;
    struct queue_element *tail;
};

/// Create an empty queue.
struct queue *queue_create(void);

/// Deallocate the space allocated to the given queue
void queue_destroy(struct queue *queue);

/// Add the given element at the tail of the queue.
void queue_enqueue(struct queue *queue, void *element);

/// Returns and removes the head of the queue.
void *queue_dequeue(struct queue *queue);

/// Returns the head of the queue without removing it.
void *queue_peek(struct queue *queue);

#endif /* ! QUEUE_H */
