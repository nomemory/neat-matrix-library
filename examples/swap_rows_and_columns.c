#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  srand(time(NULL));

  nml_mat *m = nml_mat_sqr_rnd(8, 0.0, 10.0);

  printf("m=");
  nml_mat_print(m);

  printf("m= (...after swapping col1=%d with col2=%d):\n", 1, 2);
  nml_mat_swapcols_r(m, 1, 2);
  nml_mat_print(m);

  printf("newm= (...after swapping col1=%d with col2=%d and creating a new matrix):\n", 0, 1);
  nml_mat *newm = nml_mat_swapcols(m, 0, 1);
  nml_mat_print(newm);

  printf("m= (...after swapping row1=%d with row2=%d)\n", 0, 2);
  nml_mat_swaprows_r(m, 0, 2);
  nml_mat_print(m);

  nml_mat_free(m);
  nml_mat_free(newm);

  return 0;
}
