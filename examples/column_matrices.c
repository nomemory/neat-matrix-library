#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  printf("\nExtract all matrix columns from a Matrix as matrices\n");
  nml_mat *m = nml_mat_new_rnd(5, 5, -10.0, 10.0);
  nml_mat *col;
  nml_mat_print(m);
  int i = 0;
  srand(time(NULL));
  for(i = 0; i < m->num_cols; i++) {
    col = nml_mat_getcol(m, i);
    nml_mat_print(col);
    nml_mat_free(col);
  }
  nml_mat_free(m);
  return 0;
}
