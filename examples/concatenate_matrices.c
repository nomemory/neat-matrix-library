#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  nml_mat *I = nml_mat_id(3);
  nml_mat *Ix2 = nml_mat_scalarmult(I, 2.0);
  // Concatenate using a pointer
  nml_mat **ms = malloc(sizeof(*ms) * 2);
  ms[0] = I;
  ms[1] = Ix2;
  nml_mat *concats1 = nml_mat_concath(2, ms);
  //Concatenates using variable arguments list
  nml_mat *concats2 = nml_mat_concath_va(3, I, Ix2, I);

  printf("I=\n");
  nml_mat_print(I);
  printf("Ix2=\n");
  nml_mat_print(Ix2);
  printf("concats1=\n");
  nml_mat_print(concats1);
  printf("concats2=\n");
  nml_mat_print(concats2);

  free(ms);
  nml_mat_free(I);
  nml_mat_free(Ix2);
  nml_mat_free(concats1);
  nml_mat_free(concats2);

  return 0;
}
