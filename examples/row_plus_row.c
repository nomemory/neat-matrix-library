#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

    nml_mat *m = nml_mat_rnd(5, 4, 1.0, 2.0);
    nml_mat_print(m);

    // Add row[1] elements to row[2] elements

    nml_mat_row_addrow_r(m, 2, 1, 1.0);

    // Add row[1] to row[0] with a multiplier of 2.0

    nml_mat_row_addrow_r(m, 0, 1, 2.0);

    nml_mat_print(m);
    nml_mat_free(m);

    return 0;
}