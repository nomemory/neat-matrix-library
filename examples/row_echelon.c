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
  nml_mat_print(m1);
  nml_mat *refm1 = nml_mat_ref(m1);
  nml_mat_print(refm1);

  nml_mat *m2 = nml_mat_from(2, 3, 6, v2);
  nml_mat_print(m2);
  nml_mat *refm2 = nml_mat_ref(m2);
  nml_mat_print(refm2);

  nml_mat *m3 = nml_mat_from(3, 3, 9, v3);
  nml_mat_print(m3);
  nml_mat *refm3 = nml_mat_ref(m3);
  nml_mat_print(refm3);

  nml_mat_free(m1);
  nml_mat_free(refm1);
  nml_mat_free(m2);
  nml_mat_free(refm2);
  return 0;
}
