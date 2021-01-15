#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  nml_mat* m, *mx;

  printf("\nCreating an empty matrix with 2x3\n");
  m = nml_mat_new(2,3);
  nml_mat_print(m);
  nml_mat_free(m);

  printf("\nCreating a square matrix 5x5 \n");
  m = nml_mat_sqr(5);
  nml_mat_print(m);
  nml_mat_free(m);

  printf("\nCreating an ID 7x7 Matrix and copying it into another matrix:\n");
  m = nml_mat_eye(7);
  mx = nml_mat_cp(m);
  nml_mat_print(m);
  nml_mat_print(mx);
  nml_mat_free(m);
  nml_mat_free(mx);

  return 0;
}
