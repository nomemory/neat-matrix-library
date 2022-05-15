#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  double A_values[] = {
    2, 1, 5,
    4, 4, -4,
    1, 3, 1
  };

  nml_mat *A = nml_mat_from(3, 3, 9, A_values);
  nml_mat_lup *A_lup = nml_mat_lup_solve(A);

  // printing
  printf("L, U, P:\n");
  nml_mat_lup_printf(A_lup, "%10.4lf  ");

  // freeing
  nml_mat_free(A); 
  nml_mat_lup_free(A_lup);
  return 0;
}
