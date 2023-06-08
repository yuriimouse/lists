/**
 * @brief Test
 * @author Yurii Prudius
 **/
#define _GNU_SOURCE

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <CUnit/Basic.h>

#include <lists.h>

#include "defines.h"

#define HOME_PATH "tests/unit/data/"

// list_t *list_construct(void);
// void list_init(list_t *list);
// void list_clear(list_t *list, void (*destructor)(void *));
// list_t *list_destruct(list_t *list, void (*destructor)(void *));
static void test_construct(void)
{
    list_t *list = list_construct();
    CU_ASSERT_PTR_NOT_NULL(list);

    list = list_destruct(list, NULL);
    CU_ASSERT_PTR_NULL(list);
}

// void list_add(list_t *list, void *value);  // to end
static void test_add(void)
{
    list_t *list = list_construct();

    list_add(list, "First");
    list_add(list, "Second");
    list_add(list, "Third");

    char *actual = NULL;

    actual = list_get(list);
    
    CU_ASSERT_STRING_EQUAL("First", actual);

    actual = list_remove(list);
    CU_ASSERT_STRING_EQUAL("First", actual);

    actual = list_pop(list);
    CU_ASSERT_STRING_EQUAL("Second", actual);

    list_push(list, "Head");
    actual = list_get(list);
    CU_ASSERT_STRING_EQUAL("Head", actual);

    printf("\n");

    list_foreach(list)
    {
        printf("%s\n", (char *)node->value);
    }

    list = list_destruct(list, NULL);
}

// void list_push(list_t *list, void *value); // to head
// void *list_remove(list_t *list);
// #define list_pop(list) list_remove(list)
// void *list_get(list_t *list);

// #define list_foreach(list) for (record_t *node = list->head; node; node = node->next)

// char *uniconf_substitute(cJSON *root, const char *str)
// static void test_substitute(void)
// {
//     char *str = NULL;
//     char *expect = NULL;
//     cJSON *root = cJSON_CreateObject();
//     uniconf_set(root, "foo", "FOOVAL");
//     uniconf_set(root, "bar", "BARVAL");
//     uniconf_set(root, "baz", "BAZVAL");
//     char *actual = cJSON_PrintUnformatted(root);
//     printf("===\n%s\n===\n", actual);
//     free(actual);

//     START_USING_TEST_DATA(HOME_PATH)
//     {
//         USE_OF_THE_TEST_DATA("'%m[^']' '%m[^']'", &str, &expect);
//         printf("[%s]->[%s]", str, expect);
//         actual = uniconf_substitute(root, str);
//         printf("<-[%s]\n", actual);
//         CU_ASSERT_STRING_EQUAL(expect, actual);
//         free(actual);
//     }
//     FINISH_USING_TEST_DATA;
//     cJSON_Delete(root);
//     FREE_TEST_DATA(str);
//     FREE_TEST_DATA(expect);
// }

CU_TestInfo test_list[] =
    {
        {"(construct)", test_construct},
        {"(add)", test_add},
        // {"(node)", test_node},
        // {"(is commented)", test_is_commented},
        // {"(trim)", test_trim},
        // {"(unquote)", test_unquote},
        // {"(set)", test_set},
        // {"(vardata)", test_vardata},
        // {"(substitute)", test_substitute},

        CU_TEST_INFO_NULL,
};
