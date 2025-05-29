// lists_macros.h
#ifndef LISTS_MACROS_H
#define LISTS_MACROS_H

// versioning
#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)
#define VERSION_LIST_MAJOR 2
#define VERSION_LIST_MINOR 2
#define VERSION_LIST_PATCH 1
#define VERSION_LIST STR(VERSION_LIST_MAJOR) "." STR(VERSION_LIST_MINOR) "." STR(VERSION_LIST_PATCH)

#define IF_NULL(ptr) \
    if (!(ptr))

#endif // LISTS_MACROS_H
