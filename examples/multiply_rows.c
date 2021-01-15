#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *a = nml_mat_new(4,5);
    nml_mat_setall(a, 1.0);
    int i = 0;
    for(; i < a->num_rows; ++i) {
        // Changes are doing on matrix a
        // row[i] is multiplied with (double) i
        nml_mat_multrow_r(a, i, (double)i);
    }
    nml_mat_print(a);

    // Create a new matrix b by multiplying row[1] 
    // in matrix a with 5.0.
    // Matrix a remains unchanged
    nml_mat *b = nml_mat_multrow(a, 1, 5.0);
    nml_mat_print(b);
    nml_mat_free(a);
    nml_mat_free(b);
    return 0;
}