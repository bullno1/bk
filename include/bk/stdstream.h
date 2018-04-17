#ifndef BK_STDSTREAM_H
#define BK_STDSTREAM_H

#include "decl.h"

struct bk_file_s;

BK_DECL struct bk_file_s* bk_stdin;
BK_DECL struct bk_file_s* bk_stdout;
BK_DECL struct bk_file_s* bk_stderr;

#ifdef BK_IMPLEMENTATION
#include "../../src/stdstream.c"
#endif

#endif
