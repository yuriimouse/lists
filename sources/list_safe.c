#include "list_safe.h"
#include <errno.h>

/**
 * Create list
 *
 * @return list_safe_t*
 */
list_safe_t *list_safe_construct(void)
{
    list_safe_t *list = calloc(1, sizeof(list_safe_t));
    ALLOC_CHECK(list)
    {
        errno = ENOMEM;
        return NULL;
    }
    list_safe_init(list);

    return list;
}

/**
 * Init empty list
 *
 * @param list
 */
void list_safe_init(list_safe_t *list)
{
    if (list)
    {
        list->head = NULL;
        list->last = NULL;
        pthread_mutex_init(&list->mutex, NULL);
        pthread_cond_init(&list->cond, NULL);
    }
}

/**
 * Clear list & destruct values
 *
 * @param list
 * @param destructor
 */
void list_safe_clear(list_safe_t *list, void (*destructor)(void *))
{
    if (list)
    {
        pthread_mutex_lock(&list->mutex);

        list->last = NULL;
        while (list->head != NULL)
        {
            record_safe_t *node = list->head;
            if (destructor)
            {
                destructor(node->value);
            }
            list->head = node->next;
            free(node);
        }

        pthread_mutex_unlock(&list->mutex);
    }
}

/**
 * Clear & destruct list
 *
 * @param list
 * @param destructor
 * @return list_t*
 */
list_safe_t *list_safe_destruct(list_safe_t *list, void (*destructor)(void *))
{
    if (list)
    {
        list_safe_clear(list, destructor);
        pthread_mutex_destroy(&list->mutex);
        pthread_cond_destroy(&list->cond);
        free(list);
    }

    return NULL;
}

/**
 * Add to end of queue
 *
 * @param list
 * @param value
 */
void list_safe_add(list_safe_t *list, void *value)
{
    if (list)
    {
        pthread_mutex_lock(&list->mutex);

        record_safe_t *new_node = malloc(sizeof(record_safe_t));
        if (new_node)
        {
            new_node->next = NULL;
            new_node->value = value;

            if (list->last)
            {
                (list->last)->next = new_node;
            }
            else
            {
                list->head = new_node;
            }
            list->last = new_node;
        }
        pthread_cond_signal(&list->cond);
        pthread_mutex_unlock(&list->mutex);
        CHECK_ALLOC(new_node)
        {
            errno = ENOMEM;
        }
    }
}

/**
 * Add to head of stack
 *
 * @param list
 * @param value
 */
void list_safe_push(list_safe_t *list, void *value)
{
    if (list)
    {
        record_safe_t *new_node = malloc(sizeof(record_safe_t));
        CHECK_ALLOC(new_node)
        {
            errno = ENOMEM;
            return;
        }

        pthread_mutex_lock(&list->mutex);
        new_node->next = list->head;
        new_node->value = value;

        list->head = new_node;
        if (!list->last)
        {
            list->last = new_node;
        }

        pthread_cond_signal(&list->cond);
        pthread_mutex_unlock(&list->mutex);
    }
}

/**
 * Remove from queue head
 *
 * @param list
 * @return void*
 */
void *list_safe_remove(list_safe_t *list)
{
    void *value = NULL;

    if (list)
    {
        pthread_mutex_lock(&list->mutex);

        while (list->head == NULL)
        {
            pthread_cond_wait(&list->cond, &list->mutex);
        }

        record_safe_t *node = list->head;
        if (node)
        {
            value = node->value;
            list->head = node->next;
            free(node);
            if(!list->head)
            {
                list->last = NULL;
            }
        }

        pthread_mutex_unlock(&list->mutex);
    }
    return value;
}
