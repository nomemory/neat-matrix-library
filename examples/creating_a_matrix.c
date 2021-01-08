#include <stdlib.h>
#include <stdio.h>

#include "nml.h"

int main(int argc, char *argv[]) {

  // Creating an empty matrix:
  nml_mat* empty_mat = nml_mat_new(2,3);
  nml_mat_print(empty_mat);
  nml_mat_free(empty_mat);

  double A_v[25] = {
    1.0, 2.0, 5.0, 0.0, 5.0,
    3.0, 4.0, 9.0, 7.0, 8.0,
    9.0, 8.0, 2.0, 8.0, 0.0,
    1.0, 2.5, 3.7, 3.0, 2.5,
    4.0, 8.0, 3.0, 1.0, 0.0
  };

  return 0;
}
