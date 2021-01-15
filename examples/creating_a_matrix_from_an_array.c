#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) { 
    double array[6] = { 
        1.0, 0.2, 3.0, 4.0, 5.0, 3.1 
    };
    nml_mat* my;
    
    // 3 rows, 2 columns
    // read exactly 6 numbers from array[6]
    my = nml_mat_from(3, 2, 6, array);
    nml_mat_print(my);
    nml_mat_free(my);

    // 4 rows, 2 columns
    // read exactly 3 numbers from array[6]
    my = nml_mat_from(4, 2, 3, array);
    nml_mat_print(my);
    nml_mat_free(my);

    return 0;
}