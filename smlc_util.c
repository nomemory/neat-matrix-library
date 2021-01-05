#include <stdio.h>
#include <stdarg.h>
#include "smlc_util.h"

#define BUFFER_SIZE 4096

void smlc_log(
  FILE* stream,
  const char* file_name,
  unsigned int line,
  const char* format,
  ...
) {
#if DEBUG_TRUE
  va_list argp;
  va_start(argp, format);
  smlc_vlog(stream, file_name, line, format, argp);
  va_end(argp);
#endif
}

void smlc_vlog(
  FILE* stream,
  const char *file_name,
  unsigned int line,
  const char *format,
  va_list argp
){
#if DEBUG_TRUE
  char buffer[BUFFER_SIZE];
  char* level;
  int stop;

  if (stderr == stream) {
      level = "ERROR";
  } else if (stdout == stream) {
      level = "INFO";
  }

  // Formating string and
  // Check if the the string has been completly written and
  // no buffer overflow occured
  stop = vsnprintf(buffer, BUFFER_SIZE, format, argp);
  if (stop < BUFFER_SIZE && stop > 0) {
    fprintf(stream, "[%s:%d] [%s] %s\n", file_name, line, level, buffer);
  }
#endif
}
