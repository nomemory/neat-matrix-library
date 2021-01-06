/**

Copyright 2021

Andrei N. Ciobanu
http://www.andreinc.net/
https://github.com/nomemory/

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef SMLC_UTIL_H
#define SMLC_UTIL_H

#include <stdio.h>
#include <stdarg.h>

#define NP_CHECK(ptr) \
        if (!(ptr)) { \
            fprintf(stderr, "%s:%d NULL POINTER: %s n", \
                __FILE__, __LINE__, (#ptr)); \
            exit(-1); \
        } \

// -----------------------------------------------------------------------------

// Enable this to allow debugging messages
#define DEBUG_TRUE 1

void smlc_log(FILE *stream, const char *file_name,
  unsigned int line, const char *format, ...);

void smlc_vlog(FILE* stream, const char *file_name,
  unsigned int line, const char *format, va_list argp);

#define SMLC_FLOG(stream, fmt, ...) \
      if (DEBUG_TRUE) { \
        smlc_log(stream, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define SMLC_FINFO(fmt, ...) \
      if (DEBUG_TRUE) { \
          smlc_log(stdout, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define SMLC_FERROR(fmt, ...) \
      if (DEBUG_TRUE) { \
          smlc_log(stderr, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define SMLC_ERROR(fmt) \
      if (DEBUG_TRUE) { \
        smlc_log(stderr, __FILE__, __LINE__, fmt); \
      } \

#endif
