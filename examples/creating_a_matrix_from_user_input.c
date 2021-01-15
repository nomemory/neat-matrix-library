#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *from_file2 = nml_mat_fromfilef(stdin);
    nml_mat_print(from_file2);
    nml_mat_free(from_file2);
    return 0;
}