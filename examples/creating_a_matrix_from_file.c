#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    const char *f = "examples/data/matrix1.data";
    nml_mat *from_file = nml_mat_fromfile(f);
    nml_mat_print(from_file);
    nml_mat_free(from_file);

    // Or if the file is already opened

    FILE *m_file = fopen("examples/data/matrix2.data", "r");
    nml_mat *from_file2 = nml_mat_fromfilef(m_file);
    nml_mat_print(from_file2);
    nml_mat_free(from_file2);
    fclose(m_file);

    return 0;
}