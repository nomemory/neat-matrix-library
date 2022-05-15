#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  double A_values[] = {
    1, 2, 3,
    0, 2, 4,
    2, 1, 9
  };
  double b_values[] = {
    0, -1, 3.0/2
  };

  nml_mat *A = nml_mat_from(3, 3, 9, A_values);
  nml_mat *b = nml_mat_from(3, 1, 3, b_values);

  nml_mat_lup *A_lup = nml_mat_lup_solve(A);

  // solving via LUP decomposition
  nml_mat *x = nml_ls_solve(A_lup, b);

  // printing
  printf("A:\n");
  nml_mat_printf(A, "%10.4lf  ");
  printf("b:\n");
  nml_mat_printf(b, "%10.4lf  ");
  printf("x:\n");
  nml_mat_printf(x, "%10.4lf  ");

  // freeing
  nml_mat_free(A); 
  nml_mat_lup_free(A_lup);
  return 0;
}
