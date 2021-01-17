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

    // Multiply matrices
    nml_mat *m3 = nml_mat_dot(m1, m2);
    printf("m3=\n");
    nml_mat_print(m3);

    nml_mat_free(m1);
    nml_mat_free(m2);
    nml_mat_free(m3);

    return 0;
}