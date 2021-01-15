#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  nml_mat *I = nml_mat_eye(3);
  nml_mat *Ix2 = nml_mat_scalarmult(I, 2.0);
  nml_mat *rndm = nml_mat_new_rnd(3, 4, 1.0, 5.0);
  // Concatenate using a pointer
  nml_mat **ms = malloc(sizeof(*ms) * 2);
  ms[0] = I;
  ms[1] = Ix2;
  nml_mat *concats1 = nml_mat_concath(2, ms);

  printf("\nConcatenate horizontally\n");
  printf("I=\n");
  nml_mat_print(I);
  printf("Ix2=\n");
  nml_mat_print(Ix2);
  printf("rndm=\n");
  nml_mat_print(rndm);
  printf("concats1=\n");
  nml_mat_print(concats1);
  printf("concats2=\n");

  free(ms);
  nml_mat_free(I);
  nml_mat_free(Ix2);
  nml_mat_free(concats1);
  nml_mat_free(rndm);

  // -------------------------------------
  // Vertical concatenation
  // -------------------------------------

  nml_mat *A = nml_mat_new_rnd(3, 4, 1.0, 4.0);
  nml_mat *B = nml_mat_new_rnd(5, 4, 10.0, 20.0);
  nml_mat *C = nml_mat_eye(4);

  nml_mat **ABarr = malloc(sizeof(*ABarr) * 2);
  ABarr[0] = A;
  ABarr[1] = B;
  nml_mat *ABCat = nml_mat_concatv(2, ABarr);

  printf("\nA=\n");
  nml_mat_print(A);
  printf("\nB=\n");
  nml_mat_print(B);
  printf("\nC=\n");
  nml_mat_print(C);
  printf("\nA concat B =\n");
  nml_mat_print(ABCat);

  free(ABarr);
  nml_mat_free(A);
  nml_mat_free(B);
  nml_mat_free(C);

  return 0;
}
