#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  srand(time(NULL)); // Should be called once per program
  nml_mat *m = nml_mat_new_rnd(5, 5, -10.0, 10.0);
  nml_mat_print(m);
  nml_mat_free(m);
  return 0;
}