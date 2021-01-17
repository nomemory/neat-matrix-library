#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    
    FILE *input = fopen("./examples/data/matrix9_lower_triangular.data", "r");

    nml_mat *A = nml_mat_fromfilef(input);
    nml_mat *B = nml_mat_fromfilef(input);
    nml_mat *x = nml_ls_solvefwd(A, B);

    nml_mat_print(A);
    nml_mat_print(B);
    nml_mat_print(x);

    nml_mat_free(A);
    nml_mat_free(B);
    nml_mat_free(x);

    fclose(input);

    return 0;
}