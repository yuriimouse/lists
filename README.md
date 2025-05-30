# lists – Simple Pointer-based List Library in C

A minimal and efficient singly-linked list library written in C.  
Designed for low-overhead projects that need simple list management.

---

## Features

- Lightweight and minimal
- Written in pure C (C89/C90/GNU compatible)
- Easy to integrate and extend
- Pointer-based design for flexible data storage
- Thread-safe locking mechanisms (if enabled)
- Supports cloning and safe manipulation of list elements

---

## Building

To build the library:
```sh
make
```

To clean build artifacts:
```sh
make clean
```
By default, the library installs to /usr/local:
```sh
sudo make install
```

To uninstall:
```sh
sudo make uninstall
```

To install to a custom prefix (e.g. /usr/local/custom):
```sh
sudo make install PREFIX=/usr/local/custom
```

## Usage

Include the header in your source code:
```c
#include <lists.h>
```

Compile and link with the static library:
```sh
gcc yourfile.c -L/usr/local/custom/lib -llists
```

Example usage snippet:
```c
#include <stdio.h>
#include <lists.h>

int main() {
    list_t *list = list_construct();
    if (!list) {
        perror("Failed to create list");
        return 1;
    }

    int value = 42;
    list_add(list, &value);

    list_destruct(list);
    return 0;
}
```

## Testing and Continuous Integration

Automated tests are implemented using CUnit and run via make test, which includes memory checks with Valgrind.

This project uses GitHub Actions to:
 - Build the library on every push and pull request
 - Run unit tests with Valgrind for memory safety
 - Verify that the library builds correctly and tests pass

## Documentation

The source code is documented with inline comments suitable for generating Doxygen documentation.
Future releases will include detailed API docs and usage examples.

_Co-authored with assistance from ChatGPT_
