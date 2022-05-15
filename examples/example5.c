#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  double A_values[] = {
    1, 3, 4,
    1, 1, -6,
    1, -1, 2
  };

  nml_mat *A = nml_mat_from(3, 3, 9, A_values);

  // obtaining the QR factorization
  nml_mat_qr *A_qr = nml_mat_qr_solve(A);

  // printing
  printf("A:\n");
  nml_mat_printf(A, "%10.4lf  ");
  printf("Q:\n");
  nml_mat_printf(A_qr->Q, "%10.4lf  ");
  printf("R:\n");
  nml_mat_printf(A_qr->R, "%10.4lf  ");

  // freeing
  nml_mat_free(A); 
  nml_mat_qr_free(A_qr);
  return 0;
}
