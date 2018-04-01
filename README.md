# bk - Base library for C99

[![License](https://img.shields.io/badge/license-BSD-blue.svg)](LICENSE)

## Modules

### [macro.h](include/bk/macro.h)

Common macros that I have copied and pasted across way too many projects.

### [allocator.h](include/bk/allocator.h)

Common allocator interface, inspired by [bkaradzic/bx](https://github.com/bkaradzic/bx/blob/master/include/bx/allocator.h).

An implementation using C runtime is provided in [default\_allocator.h](include/bk/default_allocator.h).

### [assert.h](include/bk/assert.h)

Assertion with an explanation message.
It also provides "critical assertions" which cannot be disabled.

### [dlist.h](include/bk/dlist.h)

Intrusive doubly-linked list.

### [array.h](include/bk/array.h)

Dynamically sized array, inspired by [stretchy\_buffer](https://github.com/nothings/stb/blob/master/stretchy_buffer.h) but with support for non-default alignments.

### [fs.h](include/bk/fs.h)

Common filesystem abstraction.
The goal is to remove direct file access from libraries.

There are several implementations:

- [crt](include/bk/fs/crt.h): provides direct access to the filesystem through C runtime's functions.
- [ro](include/bk/fs/ro.h): wraps an underlying filesystem and make it read-only.
- [vfs](include/bk/fs/vfs.h): a "filesystem manager".
  Other filesystems can be mounted at different paths of this filesystem.
  In other words, it's a very primitive version of [PhysicsFS](https://icculus.org/physfs) without any archive codec.
- [prefix](include/bk/fs/prefix.h): adds a prefix to all paths.
  It can be used together with [vfs](include/bk/fs/vfs.h) to create a "bind mount".
- [mem](include/bk/fs/mem.h): Use a memory buffer to store file content.

### [printf.h](inclue/bk/printf.h)

printf-like functions which are compatible with [fs](include/bk/fs.h) streams.
