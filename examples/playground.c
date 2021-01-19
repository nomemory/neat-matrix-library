#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  nml_mat *A = nml_mat_fromfile("examples/data/matrix11_qr.data");
  nml_mat_qr *QR = nml_mat_qr_solve(A);
  nml_mat_print(QR->Q);
  nml_mat_print(QR->R);
  return 0;
}
