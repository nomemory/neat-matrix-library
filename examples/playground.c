#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  nml_mat *A = nml_mat_fromfile("examples/data/matrix7.data");
  nml_mat_lup *LUP = nml_mat_lup_solve(A);

  nml_mat_print(A);
  nml_mat_lup_print(LUP);

  printf("---\n");

  nml_mat_print(nml_mat_dot(LUP->P, A));
  nml_mat_print(nml_mat_dot(LUP->L, LUP->U));

  return 0;
}
