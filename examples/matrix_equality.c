#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

    srand(time(NULL));
    nml_mat *m1 = nml_mat_rnd(2, 3, 1.0, 10.0);
    nml_mat *m2 = nml_mat_rnd(2, 3, 1.0, 10.0);

    if (nml_mat_eq(m1, m2, 0.001)) {
        printf("Wow, what were the oddss..\n");
    } else {
        printf("It's ok, nobody is that lucky!\n");
    }
    if (nml_mat_eqdim(m1, m2)) {
        printf("At least we know they both have the same number of rows and columns.\n");
    }

    nml_mat_free(m1);
    nml_mat_free(m2);
    return 0;
}