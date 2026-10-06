# tinyalloc

- Small exprimental dynamic memory allocator in C

## Features
- Allocate, free memory, free all memory
- Usable for very basic memory allocation tasks
- Best fit block finding
- Joining blocks when freed
- Block splitting when large enough space remains

## How to use
- clone the repository into your system
- include the talloc.h header file in your source file: ``#include <talloc.h>``
- compile along with talloc.c. Example: ``gcc yourfile.c talloc.c -o test``

## Built With
- mmap() for getting the full allocation space
- Metadata for blocks in a minimal struct

## Limitations
- __Warning: Do not try to use this for large/serious projects__
- Very limited total allocation space, and it is fixed
- Not really optimised block finding policies
- Allocated pointers need to be handle carefully (dynamic memory in general actually)

