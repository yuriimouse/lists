// tests/unit/defines.h
#ifndef UNIT_TEST_DEFS_H
#define UNIT_TEST_DEFS_H

#define FREE_AND_NULL(name) \
    do                      \
    {                       \
        if (name)           \
            free(name);     \
        name = NULL;        \
    } while (0)

#define START_USING_TEST_DATA(path)                        \
    char *_str = NULL;                                     \
    FILE *_fp;                                             \
    asprintf(&_str, "%s%s", (path), __func__);             \
    if ((_fp = fopen(_str, "r")))                          \
    {                                                      \
        free(_str);                                        \
        _str = NULL;                                       \
        size_t _len = 0;                                   \
        int _read;                                         \
        while ((_read = getline(&_str, &_len, _fp)) != -1) \
        {                                                  \
            if (strlen(_str) && _str[0] != '#')

#define USE_OF_THE_TEST_DATA(...) sscanf(_str, __VA_ARGS__)

#define FINISH_USING_TEST_DATA                      \
    }                                               \
    fclose(_fp);                                    \
    }                                               \
    else                                            \
    {                                               \
        fprintf(stderr, "'%s' not found...", _str); \
        CU_ASSERT_TRUE_FATAL(0);                    \
    }                                               \
    FREE_AND_NULL(_str)

#endif // UNIT_TEST_DEFS_H
