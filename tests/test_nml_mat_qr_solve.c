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
    fscanf(t_file, "%d", &num_cases);
    NML_TEST_HEADER(num_cases, argv[1], argv[0]);

    for(k = 0; k < num_cases; k++) {
        nml_mat *input = nml_mat_fromfilef(t_file);
        nml_mat_qr *input_qr = nml_mat_qr_solve(input);

        // A = QR
        nml_mat *qrdotinput = nml_mat_dot(input_qr->Q, input_qr->R);

        if (nml_mat_eq(qrdotinput, input, TEST_TOLERANCE)) {
            printf(GREEN "%d " RESET, k);    
        } else {
            printf(RED "%d" RESET, k);
            printf("Q * R=\n");
            nml_mat_print(qrdotinput);
            printf("A=\n");
            nml_mat_print(input);
            return -1;
        }

        nml_mat_free(qrdotinput);
        nml_mat_free(input);
        nml_mat_qr_free(input_qr);
    }

    fclose(t_file);

    printf("\n");
    return 0;
}