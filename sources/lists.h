// sources/lists.h
#ifndef LISTS_H
#define LISTS_H

#include <pthread.h>

// versioning
#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)
#define VERSION_LIST_MAJOR 3
#define VERSION_LIST_MINOR 1
#define VERSION_LIST_PATCH 1
#define VERSION_LIST STR(VERSION_LIST_MAJOR) "." STR(VERSION_LIST_MINOR) "." STR(VERSION_LIST_PATCH)

#define IF_NULL(ptr) \
    if (!(ptr))

typedef struct record
{
    struct record *next;
    void *value;
} record_t;

typedef struct list
{
    int is_safe;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    record_t *head;
    record_t *last;
} list_t;

list_t *list_construct(int is_safe);
list_t *list_clear(list_t *list, void (*destructor)(void *));
list_t *list_destruct(list_t *list, void (*destructor)(void *));

void *list_add(list_t *list, void *value); // to end
void *list_remove(list_t *list);
void *list_push(list_t *list, void *value); // to head
#define list_pop(list) list_remove(list)
void *list_get(list_t *list);
#define list_peek(list) list_get(list)

#ifdef __GLIBC__
void *list_clone(list_t *list);
#endif

void list_wait(list_t *list);

#define list_lock(list)                         \
    do                                          \
    {                                           \
        if ((list) && (list)->is_safe)          \
        {                                       \
            pthread_mutex_lock(&(list)->mutex); \
        }                                       \
    } while (0)
#define list_unlock(list)                         \
    do                                            \
    {                                             \
        if ((list) && (list)->is_safe)            \
        {                                         \
            pthread_cond_broadcast(&(list)->cond);   \
            pthread_mutex_unlock(&(list)->mutex); \
        }                                         \
    } while (0)

#define list_foreach(list) for (record_t *node = list->head; node; node = node->next)
#define list_foreach_lock(list)                    \
    do                                             \
    {                                              \
        list_t *_stored_list_ = list;              \
        list_lock(_stored_list_);                  \
        for (record_t *node = _stored_list_->head; \
             node;                                 \
             node = node->next)
#define list_foreach_unlock    \
    list_unlock(_stored_list); \
    }                          \
    while (0)

#endif // LISTS_H
