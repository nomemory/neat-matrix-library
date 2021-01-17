#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *m1 = nml_mat_new_rnd(1, 5, 1.0, 10.0);
    nml_mat_print(m1);

    nml_mat *m2 = nml_mat_transp(m1);
    nml_mat_print(m2);

    nml_mat_free(m1);
    nml_mat_free(m2);

    return 0;
}