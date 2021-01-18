#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  printf("\nExtract all matrix rows from a Matrix as matrices\n");
  srand(time(NULL));
  nml_mat *m = nml_mat_rnd(5, 5, -10.0, 10.0);
  nml_mat *row;
  nml_mat_print(m);
  int i = 0;
  srand(time(NULL));
  for(i = 0; i < m->num_rows; i++) {
    row = nml_mat_row_get(m, i);
    nml_mat_print(row);
    nml_mat_free(row);
  }
  nml_mat_free(m);
  return 0;
}
