#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

  double A_values[] = {
    0, 1, 2,
    1, 2, 1,
    2, 7, 8
  };

  nml_mat *A = nml_mat_from(3, 3, 9, A_values);
  nml_mat *Aref  = nml_mat_ref(A);
  nml_mat *Arref = nml_mat_rref(A);

  // printing
  printf("A:\n");                       
  nml_mat_printf(A, "%10.4lf  ");

  printf("ref(A):\n");                       
  nml_mat_printf(Aref, "%10.4lf  ");

  printf("rref(A)\n");                       
  nml_mat_printf(Arref, "%10.4lf  ");

  // freeing
  nml_mat_free(A); 
  nml_mat_free(Aref);
  nml_mat_free(Arref);
  return 0;
}
