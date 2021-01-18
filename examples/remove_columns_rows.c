#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

    nml_mat *m = nml_mat_sqr_rnd(4, 1.0, 2.0);
    nml_mat_print(m);

    // Remove column[1] from m
    // m remains the same
    // less_columns is another matrix 
    nml_mat *less_columns = nml_mat_col_rem(m, 1);
    nml_mat_print(less_columns);

    // Remove row[0] from less_columns
    // less_columns remains the same
    // less_rows is another matrix
    nml_mat *less_rows = nml_mat_row_rem(less_columns, 0);
    nml_mat_print(less_rows);

    nml_mat_free(m);
    nml_mat_free(less_columns);
    nml_mat_free(less_rows);

    return 0;
}