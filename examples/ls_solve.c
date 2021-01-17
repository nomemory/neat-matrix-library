#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *A = nml_mat_sqr_rnd(4, 1.0, 10.0);
    nml_mat *B = nml_mat_new_rnd(4, 1, 1.0, 10.0);
    nml_mat_lup *LUP = nml_mat_lup_solve(A);

    nml_mat *x = nml_ls_solve(LUP, B);
    nml_mat_print(x);

    nml_mat_free(A);
    nml_mat_free(B);
    nml_mat_free(x);
    nml_mat_lup_free(LUP);
}