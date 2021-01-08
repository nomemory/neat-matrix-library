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

#ifndef nml_UTIL_H
#define nml_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define NP_CHECK(ptr) \
        if (!(ptr)) { \
            fprintf(stderr, "%s:%d NULL POINTER: %s n", \
                __FILE__, __LINE__, (#ptr)); \
            exit(-1); \
        } \

// -----------------------------------------------------------------------------

// Enable this to allow debugging messages
#define DEBUG_TRUE 1

double nml_rand_interval(double min, double max);

void nml_log(FILE *stream, const char *file_name,
  unsigned int line, const char *format, ...);

void nml_vlog(FILE* stream, const char *file_name,
  unsigned int line, const char *format, va_list argp);

#define NML_FLOG(stream, fmt, ...) \
      if (DEBUG_TRUE) { \
        nml_log(stream, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define NML_FINFO(fmt, ...) \
      if (DEBUG_TRUE) { \
          nml_log(stdout, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define NML_FERROR(fmt, ...) \
      if (DEBUG_TRUE) { \
          nml_log(stderr, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define NML_ERROR(fmt) \
      if (DEBUG_TRUE) { \
        nml_log(stderr, __FILE__, __LINE__, fmt); \
      } \

#endif
