#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *m1 = nml_mat_sqr_rnd(4, 0.0, 10.0);
    nml_mat *m2 = nml_mat_sqr_rnd(4, 0.0, 10.0);
    printf("m1=\n");
    nml_mat_print(m1);
    
    printf("m2=\n");
    nml_mat_print(m2);


    // Add the matrices to, result is kept in m3
    // m1 and m2 remain unchanged
    nml_mat *m3 = nml_mat_add(m1, m2);
    printf("m3=\n");
    nml_mat_print(m3);

    // Add the matrices, the result is kept in m1
    // m1 is modified, m2 remains unchanged
    nml_mat_add_r(m1, m2);
    printf("m1=\n");
    nml_mat_print(m1);

    nml_mat_free(m1);
    nml_mat_free(m2);
    nml_mat_free(m3);

    return 0;
}