#include "queue.h"

#include <stdlib.h>

#include "xmalloc.h"

struct queue *queue_create(void)
{
    return xcalloc(1, sizeof(struct queue));
}

static void queue_element_destroy(struct queue_element *queue_element)
{
    if (queue_element != NULL)
    {
        queue_element_destroy(queue_element->next);
        free(queue_element);
    }
}

void queue_destroy(struct queue *queue)
{
    if (queue)
    {
        queue_element_destroy(queue->head);
        free(queue);
    }
}

void queue_enqueue(struct queue *queue, void *element)
{
    struct queue_element *queue_element =
        xcalloc(1, sizeof(struct queue_element));
    queue_element->element = element;
    if (queue->head == NULL)
    {
        queue->head = queue_element;
        queue->tail = queue_element;
    }
    else
    {
        queue->tail->next = queue_element;
        queue->tail = queue_element;
    }
}

void *queue_dequeue(struct queue *queue)
{
    if (queue != NULL && queue->head != NULL)
    {
        struct queue_element *queue_head = queue->head;
        queue->head = queue_head->next;
        void *head_element = queue_head->element;
        free(queue_head);
        return head_element;
    }
    return NULL;
}

void *queue_peek(struct queue *queue)
{
    if (queue != NULL && queue->head != NULL)
        return queue->head->element;
    return NULL;
}
