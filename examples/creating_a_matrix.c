#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  nml_mat* m, *mx, *my;

  printf("\nCreating an empty matrix with 2x3\n");
  m = nml_mat_new(2,3);
  nml_mat_print(m);
  nml_mat_free(m);

  printf("\nCreating a square matrix 5x5 \n");
  m = nml_mat_sqr(5);
  nml_mat_print(m);
  nml_mat_free(m);

  printf("\nCreating an ID 7x7 Matrix and copying it into another matrix:\n");
  m = nml_mat_id(7);
  mx = nml_mat_cp(m);
  nml_mat_print(m);
  nml_mat_print(mx);
  nml_mat_free(m);
  nml_mat_free(mx);

  printf("\nCreating a Matrix 3x2 from an array:\n");
  double array[6] = { 1.0, 0.0, 3.0, 4.0, 5.0, 3.0 };
  my = nml_mat_from(3,2,6,array);
  nml_mat_print(my);
  nml_mat_free(my);

  return 0;
}
