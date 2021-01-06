/**
Copyright 20201 Andrei N. Ciobanu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
