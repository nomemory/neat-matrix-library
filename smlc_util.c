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
