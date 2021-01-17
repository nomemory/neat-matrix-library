#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *m1 = nml_mat_sqr_rnd(4, 0.0, 10.0);
    nml_mat_lup *m1_lup = nml_mat_lup_solve(m1);
    
    printf("m1=\n");
    nml_mat_print(m1);
    printf("determinant=%lf\n", nml_mat_det(m1_lup));

    nml_mat_free(m1);
    nml_mat_lup_free(m1_lup);

    return 0;
}