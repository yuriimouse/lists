# lists – Simple Pointer-based List Library in C

A minimal and efficient singly-linked list library written in C.  
Designed for low-overhead projects that need simple list management.

---

## Features

- Lightweight and minimal
- Written in pure C (C89/C90 compatible)
- Easy to integrate and extend
- Uses pointer-based design for flexibility

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

---

## Usage
Include the header:
```c
#include <lists.h>
```
Link with the static library:
```sh
gcc yourfile.c -L/path/to/lib -llists
```

---

## GitHub Actions CI
This project uses GitHub Actions to automatically build and verify each commit:
- On push and pull request, it compiles the library and checks that the output file liblists.a is produced.
