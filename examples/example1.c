#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  double A_values[] = {
    1, 2, 3,
    0, 2, 4,
    2, 1, 9
  };
  double B_values[] = {
    3, -1, 1,
    2, 0, -5,
    -1, 1, 4
  };

  nml_mat *A = nml_mat_from(3, 3, 9, A_values);
  nml_mat *B = nml_mat_from(3, 3, 9, B_values);

  // printing
  printf("A:\n");                       
  nml_mat_printf(A, "%10.4lf  ");

  printf("B:\n");                       
  nml_mat_printf(B, "%10.4lf  ");

  printf("A+B:\n");                       
  nml_mat_printf(nml_mat_add(A, B), "%10.4lf  ");

  printf("A-B:\n");                       
  nml_mat_printf(nml_mat_sub(A, B), "%10.4lf  ");

  printf("A*B:\n");                       
  nml_mat_printf(nml_mat_dot(A, B), "%10.4lf  ");

  // freeing
  nml_mat_free(A);    
  nml_mat_free(B);    
  return 0;
}
