#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    // Creates a matrix of zeros of size = 5
    nml_mat *pi_mat = nml_mat_sqr(5);

    // Sets the first diagonal to PI
    nml_mat_diag_set(pi_mat, M_PI);

    nml_mat_print(pi_mat);
    nml_mat_free(pi_mat);
    return 0;
}