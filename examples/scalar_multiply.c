#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *m = nml_mat_eye(5);

    // Multiply all elements of m with 2.0
    // and return a new matrix

    nml_mat *new_m = nml_mat_scalarmult(m, 2.0);

    if (!(nml_mat_eq(m, new_m, 0.0))) {
        printf("It's normal to see this message.\n");
    }

    // Multiply all elements of m with 2.0
    // m is modified, no new matrix is created
    nml_mat_scalarmult_r(m, 2.0);

    if (nml_mat_eq(m, new_m, 0.0)) {
        printf("It's even more normal to see this message.\n");
    }

    nml_mat_free(m);
    nml_mat_free(new_m);

    return 0;
}