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
