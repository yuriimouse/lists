// sources/lists.c
// Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>
#define _GNU_SOURCE

#include "lists.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/**
 * Create list
 *
 * @param is_safe
 * @return list_t*
 */
list_t *list_construct(int is_safe)
{
    list_t *list = calloc(1, sizeof(list_t));
    IF_NULL(list)
    {
        errno = ENOMEM;
        return NULL;
    }

    if ((list->is_safe = is_safe))
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&list->mutex, &attr);
        pthread_mutexattr_destroy(&attr);
        pthread_cond_init(&list->cond, NULL);
    }

    list->head = NULL;
    list->last = NULL;

    errno = 0;
    return list;
}

/**
 * Destroy and free list
 *
 * @param list
 * @param destructor
 * @return list_t*
 */
list_t *list_destruct(list_t *list, void (*destructor)(void *))
{
    if (list)
    {
        list_clear(list, destructor);
        if (list->is_safe)
        {
            pthread_mutex_destroy(&list->mutex);
            pthread_cond_destroy(&list->cond);
        }
        free(list);

        errno = 0;
        return NULL;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * Clear list
 *
 * @param list
 * @return list_t*
 */
list_t *list_clear(list_t *list, void (*destructor)(void *))
{
    if (list)
    {
        list_lock(list);
        while (list->head)
        {
            void *value = list_remove(list);
            if (destructor && value)
            {
                destructor(value);
            }
        }
        list_unlock(list);

        errno = 0;
        return list;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * Add to end of queue
 *
 * @param list
 * @param value
 * @return void*
 */
void *list_add(list_t *list, void *value)
{
    if (list)
    {
        record_t *new_node = malloc(sizeof(record_t));
        IF_NULL(new_node)
        {
            errno = ENOMEM;
            return NULL;
        }

        list_lock(list);
        new_node->next = NULL;
        new_node->value = value;

        if (list->last)
        {
            list->last->next = new_node;
        }
        else
        {
            list->head = new_node;
        }
        list->last = new_node;
        list_unlock(list);

        errno = 0;
        return value;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * Push to head of stack
 *
 * @param list
 * @param value
 * @return void*
 */
void *list_push(list_t *list, void *value)
{
    if (list)
    {
        record_t *new_node = malloc(sizeof(record_t));
        IF_NULL(new_node)
        {
            errno = ENOMEM;
            return NULL;
        }

        list_lock(list);
        new_node->next = list->head;
        new_node->value = value;

        list->head = new_node;
        if (!list->last)
        {
            list->last = new_node;
        }
        list_unlock(list);

        errno = 0;
        return value;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * Remove from queue head
 *
 * @param list
 * @return void*
 */
void *list_remove(list_t *list)
{
    if (list)
    {
        void *value = NULL;

        list_lock(list);
        record_t *node = list->head;
        if (node)
        {
            value = node->value;
            list->head = node->next;
            free(node);
            if (!list->head)
            {
                list->last = NULL;
            }
            errno = 0;
        }
        else
        {
            errno = ENODATA;
        }
        list_unlock(list);

        return value;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * Wait until list is non-empty, then unlocks the thread.
 * Only works for thread-safe lists (is_safe == true).
 *
 * @param list
 */
void list_wait(list_t *list)
{
    if (!list || !list->is_safe)
        return;

    pthread_mutex_lock(&list->mutex);
    while (!list->head)
    {
        pthread_cond_wait(&list->cond, &list->mutex);
    }
    pthread_mutex_unlock(&list->mutex);
}

/**
 * Get head
 *
 * WARNING!
 * You must use the list_lock() / list_unlock() wrapper
 * to retrieve and handle an element as atomic
 * for safe lists.
 *
 * @param list
 * @return void*
 */
void *list_get(list_t *list)
{
    IF_NULL(list)
    {
        errno = EINVAL;
        return NULL;
    }
    return (list->head) ? list->head->value : NULL;
}
