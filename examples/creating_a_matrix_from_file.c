#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    const char *f = "data/examples/matrix1.data";
    nml_mat *from_file = nml_mat_fromfile(f);
    nml_mat_print(from_file);
    nml_mat_free(from_file);
    return 0;
}