// tests/unit/test_list.c
// Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>
/**
 * General code coverage tests
 * @authors https://github.com/yuriimouse
 **/
#define _GNU_SOURCE

#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <CUnit/Basic.h>

#include <lists.h>

#include "defines.h"

#define THREADS_COUNT 4
#define OPS_PER_THREAD 1000

#define HOME_PATH "tests/unit/data/"

// list_t *list_construct(int is_safe);
// list_t *list_clear(list_t *list, void (*destructor)(void *));
// list_t *list_destruct(list_t *list, void (*destructor)(void *));
static void test_construct(void)
{
    list_t *list = list_construct(0);
    CU_ASSERT_PTR_NOT_NULL(list);

    // generate
    srand(time(NULL));
    for (size_t i = 0; i < 5; i++)
    {
        char *test_core_string = NULL;
        asprintf(&test_core_string, "Generated %d", rand());
        list_add(list, test_core_string);
    }
    char *top_value = list_get(list);
    CU_ASSERT_PTR_NOT_NULL(top_value);
    CU_ASSERT_TRUE(strncmp(top_value, "Generated ", strlen("Generated ")) == 0);

    list_t *clean = list_clear(list, free);
    CU_ASSERT_PTR_NOT_NULL(clean);
    top_value = list_get(list);
    CU_ASSERT_PTR_NULL(top_value);

    list = list_destruct(list, free);
    CU_ASSERT_PTR_NULL(list);
}

// void *list_add(list_t *list, void *value); // to end
// void *list_remove(list_t *list);
// void *list_push(list_t *list, void *value); // to head
// #define list_pop(list) list_remove(list)
// void *list_get(list_t *list);
// #define list_peek(list) list_get(list)
// #ifdef __GLIBC__
// void *list_clone(list_t *list);
// #endif
static void test_all(void)
{
    list_t *list = list_construct(0);

    list_add(list, "First");
    list_add(list, "Second");
    list_add(list, "Third");

    char *actual = NULL;

    actual = list_get(list);
    CU_ASSERT_PTR_NOT_NULL_FATAL(actual);
    CU_ASSERT_STRING_EQUAL("First", actual);

    actual = list_remove(list);
    CU_ASSERT_PTR_NOT_NULL_FATAL(actual);
    CU_ASSERT_STRING_EQUAL("First", actual);

    actual = list_pop(list);
    CU_ASSERT_PTR_NOT_NULL_FATAL(actual);
    CU_ASSERT_STRING_EQUAL("Second", actual);

    list_push(list, "Head");
    actual = list_get(list);
    CU_ASSERT_PTR_NOT_NULL_FATAL(actual);
    CU_ASSERT_STRING_EQUAL("Head", actual);
    actual = list_peek(list);
    CU_ASSERT_PTR_NOT_NULL_FATAL(actual);
    CU_ASSERT_STRING_EQUAL("Head", actual);

// hack for redirect output
// #define DEBUG
// or
// #define DEBUG stderr
#ifdef DEBUG
    fprintf(!strcmp("" DEBUG "", "") ? stdout : DEBUG, "\n");
    list_foreach(list)
    {
        fprintf(!strcmp("" DEBUG "", "") ? stdout : DEBUG, "%s\n", (char *)node->value);
    }
#endif

    list = list_destruct(list, NULL);
    CU_ASSERT_PTR_NULL(list);
}

// thread safe list test
typedef struct
{
    list_t *list;
    int thread_id;
} thread_arg_t;

void *thread_func(void *arg)
{
    thread_arg_t *targ = (thread_arg_t *)arg;

    for (int i = 0; i < OPS_PER_THREAD; i++)
    {
        char *val = NULL;
        //
        asprintf(&val, "Thread %d - %d", targ->thread_id, i);
        list_add(targ->list, val);

        //
        char *removed = list_remove(targ->list);
        if (removed)
        {
            FREE_AND_NULL(removed);
        }

        //
        if (i % 10 == 0)
        {
            char *head_val = NULL;
            asprintf(&head_val, "Head %d - %d", targ->thread_id, i);
            list_push(targ->list, head_val);
        }
    }
    return NULL;
}

static void test_safe_list_multithreaded(void)
{
    list_t *safe_list = list_construct(1);
    CU_ASSERT_PTR_NOT_NULL(safe_list);

    pthread_t threads[THREADS_COUNT];
    thread_arg_t args[THREADS_COUNT];

    for (int i = 0; i < THREADS_COUNT; i++)
    {
        args[i].list = safe_list;
        args[i].thread_id = i;
        int rc = pthread_create(&threads[i], NULL, thread_func, &args[i]);
        CU_ASSERT_EQUAL(rc, 0);
    }

    for (int i = 0; i < THREADS_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    //
    CU_ASSERT_PTR_NOT_NULL(safe_list);

    //
    safe_list = list_clear(safe_list, free);
    CU_ASSERT_PTR_NOT_NULL(safe_list);

    //
    CU_ASSERT_PTR_NULL(list_get(safe_list));

    safe_list = list_destruct(safe_list, free);
    CU_ASSERT_PTR_NULL(safe_list);
}

CU_TestInfo test_list[] =
    {
        {"(construct)", test_construct},
        {"(all)", test_all},
        {"(safe multithreaded)", test_safe_list_multithreaded},

        CU_TEST_INFO_NULL,
};
