#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  printf("\nInverse of a matrix:\n");
  double m_v[16] = {
    2.0, 7.0, 6.0, 1.0,
    9.0, 5.0, 0.0, 2.0,
    4.0, 3.0, 8.0, 3.0,
    3.0, 5.0, 1.0, 9.0
  };
  nml_mat *m = nml_mat_from(4,4,16, m_v);
  nml_mat_lup *lup = nml_mat_lup_solve(m);
  nml_mat* minv = nml_mat_inverse(lup);
  nml_mat *mdotminv = nml_mat_dot(m, minv);

  printf("m=");
  nml_mat_print(m);
  printf("minv=");
  nml_mat_print(minv);
  printf("(%%e) m * minv=");
  nml_mat_printf(mdotminv, "%e\t");
  printf("(%%f) m * minv=");
  nml_mat_printf(mdotminv, "%f\t");

  nml_mat_free(m);
  nml_mat_free(minv);
  nml_mat_free(mdotminv);
  return 0;
}
