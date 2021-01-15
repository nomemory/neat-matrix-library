#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"
#include "nml_test.h"

#define TEST_TOLERANCE 0.0001

int main(int argc, char *argv[]) {

    FILE *t_file = fopen(argv[1], "r");
    if (NULL == t_file) {
        fprintf(stderr, "Cannot open input file. %s", argv[1]);
        return -1;
    }
    
    int k, num_cases;
    nml_mat *input;
    nml_mat *expected;
    nml_mat *computed;

    fscanf(t_file, "%d", &num_cases);

    NML_TEST_HEADER(num_cases, argv[1], argv[0]);

    for(k = 0; k < num_cases; k++) {

        input = nml_mat_fromfilef(t_file);
        expected = nml_mat_fromfilef(t_file);
        computed = nml_mat_rref(input);

        if (nml_mat_eq(computed, expected, TEST_TOLERANCE)) {
            printf(GREEN "%d " RESET, k);
        } else {
            printf(RED "%d" RESET, k);
            printf("\ninput =\n");
            nml_mat_print(input);
            printf("\nexpected =\n");
            nml_mat_print(expected);
            printf("\ncomputed =\n");
            nml_mat_print(computed);
            printf("\nTest failed\n");
            return -1;
        }

        nml_mat_free(input);
        nml_mat_free(expected);
        nml_mat_free(computed);
    }

    fclose(t_file);

    printf("\n");
    return 0;
}   