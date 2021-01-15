#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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
    double det_expected, det_computed, ratio;
    fscanf(t_file, "%d", &num_cases);
    NML_TEST_HEADER(num_cases, argv[1], argv[0]);

    for(k = 0; k < num_cases; k++) { 
        nml_mat *m = nml_mat_fromfilef(t_file);
        nml_mat_lup *m_lup = nml_mat_lup_solve(m);
        det_computed = nml_mat_det(m_lup);
        fscanf(t_file, "%lf", &det_expected);

        ratio = fabs(fabs(det_expected) - fabs(det_computed)) / det_expected;

        if (ratio < TEST_TOLERANCE) {
            printf(GREEN "%d " RESET, k); 
        } else {
            printf(RED "%d" RESET, k);
            printf("m=\n");
            nml_mat_print(m);
            printf("det_expected=%lf\n", det_expected);
            printf("det_computed=%lf\n", det_computed); 
            return -1;
        }
        

        nml_mat_free(m);
        nml_mat_lup_free(m_lup);
    }
    
    fclose(t_file);
    printf("\n");
    return 0;
}