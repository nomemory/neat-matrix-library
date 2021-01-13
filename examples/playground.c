#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  nml_mat *m = nml_mat_fromfile("examples/data/matrix3.data");
  nml_mat_print(m);
  nml_mat *rrefm = nml_mat_rref(m);
  nml_mat_print(rrefm);
  nml_mat *frefm_expected = nml_mat_fromfile("examples/data/matrix4.data");
  // nml_mat_print(frefm_expected);
  nml_mat_free(m);
  nml_mat_free(rrefm);
  nml_mat_free(frefm_expected);
  return 0;
}
