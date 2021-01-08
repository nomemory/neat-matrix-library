#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  // Creating an empty matrix:
  nml_mat* empty_mat = nml_mat_new(2,3);
  nml_mat_print(empty_mat);
  nml_mat_free(empty_mat);

  return 0;
}
