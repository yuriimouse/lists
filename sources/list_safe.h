// Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>

#ifndef LIST_SAFE_H
#define LIST_SAFE_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <pthread.h>
#include "lists_macros.h"

typedef struct record_safe
{
    struct record_safe *next;
    void *value;
} record_safe_t;

typedef struct list_safe
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    record_safe_t *head;
    record_safe_t *last;
} list_safe_t;

list_safe_t *list_safe_construct(void);
void list_safe_init(list_safe_t *list);
void list_safe_clear(list_safe_t *list, void (*destructor)(void *));
list_safe_t *list_safe_destruct(list_safe_t *list, void (*destructor)(void *));

void list_safe_add(list_safe_t *list, void *value); // to end
void list_safe_push(list_safe_t *list, void *value); // to head 
void *list_safe_remove(list_safe_t *list);
#define list_safe_pop(list) list_safe_remove(list);

#define list_safe_foreach(list) for (record_safe_t *node = list->head; node; node = node->next)

#endif // LIST_SAFE_H
