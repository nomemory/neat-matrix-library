#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"
#include "nml_test.h"

int main(int argc, char *argv[]) {
     FILE *t_file = fopen(argv[1], "r");

    if (NULL == t_file) {
        fprintf(stderr, "Cannot open input file. %s", argv[1]);
        return -1;
    }

    int k, num_cases;
    fscanf(t_file, "%d", &num_cases);
    NML_TEST_HEADER(num_cases, argv[1], argv[0]);

    for(k = 0; k < num_cases; k++) {
        nml_mat *input = nml_mat_fromfilef(t_file);
        nml_mat_lup *input_lup = nml_mat_lup_solve(input);

        // PA = LU

        nml_mat *pdotinput = nml_mat_dot(input_lup->P, input);
        nml_mat *ldotu = nml_mat_dot(input_lup->L, input_lup->U);

        if (nml_mat_eq(pdotinput, ldotu, 0.001)) {
            printf(GREEN "%d " RESET, k);    
        } else {
            printf(RED "%d" RESET, k);
            printf("P * A=\n");
            nml_mat_print(pdotinput);
            printf("L * U=\n");
            nml_mat_print(ldotu);
            return -1;
        }

        nml_mat_free(pdotinput);
        nml_mat_free(input);
        nml_mat_free(ldotu);
        nml_mat_lup_free(input_lup);
    }

    fclose(t_file);

    printf("\n");
    return 0;
}