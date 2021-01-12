#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  double v1[9] = {
    0.0, 1.0, 2.0,
    1.0, 2.0, 1.0,
    2.0, 7.0, 8.0
  };

  double v2[9] = {
    1.0, 5.0, 1.0,
    2.0, 11.0, 5.0
  };

  double v3[9] = {
    1.0, 2.0, 3.0,
    4.0, 5.0, 6.0,
    7.0, 8.0, 9.0
  };

  nml_mat *m1 = nml_mat_from(3, 3, 9, v1);
  printf("\nm1=\n");
  nml_mat_print(m1);
  nml_mat *refm1 = nml_mat_ref(m1);
  printf("\nrefm1=\n");
  nml_mat_print(refm1);
  nml_mat *rrefm1 = nml_mat_rref(m1);
  printf("\nrrefm1=");
  nml_mat_print(rrefm1);

  nml_mat *m2 = nml_mat_from(2, 3, 6, v2);
  nml_mat_print(m2);
  nml_mat *refm2 = nml_mat_ref(m2);
  nml_mat_print(refm2);

  nml_mat *m3 = nml_mat_from(3, 3, 9, v3);
  nml_mat_print(m3);
  nml_mat *refm3 = nml_mat_ref(m3);
  nml_mat_print(refm3);

  nml_mat *m4 = nml_mat_fromfile("examples/data/matrix1.data");
  nml_mat_print(m4);
  nml_mat *refm4 = nml_mat_ref(m4);
  nml_mat_print(refm4);

  nml_mat *m5 = nml_mat_fromfile("examples/data/matrix2.data");
  nml_mat_print(m5);
  nml_mat *refm5 = nml_mat_ref(m5);
  nml_mat_print(refm5);

  nml_mat_free(m1);
  nml_mat_free(refm1);
  nml_mat_free(m2);
  nml_mat_free(refm2);
  nml_mat_free(m3);
  nml_mat_free(refm3);
  nml_mat_free(m4);
  nml_mat_free(refm4);
  return 0;
}
