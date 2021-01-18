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
        nml_mat *m = nml_mat_fromfilef(t_file);
        nml_mat_lup *m_lup = nml_mat_lup_solve(m);
        nml_mat *m_inv_computed = nml_mat_inv(m_lup);
        nml_mat *m_inv_expected = nml_mat_fromfilef(t_file);
        nml_mat *m_dot_m_inv_computed = nml_mat_dot(m, m_inv_computed);
        nml_mat *eye = nml_mat_eye(m->num_rows);

        if (nml_mat_eq(eye, m_dot_m_inv_computed, TEST_TOLERANCE) &&
            nml_mat_eq(m_inv_computed, m_inv_expected, TEST_TOLERANCE)) {
            printf(GREEN "%d " RESET, k);    
        } else {
            printf(RED "%d" RESET, k);
            printf("m (input)=\n");
            nml_mat_print(m);
            printf("m_inv_computed =\n");
            nml_mat_print(m_inv_expected);
            printf("m_inv_expected =\n");
            nml_mat_print(m_inv_expected);
            return -1;
        }
        
        nml_mat_free(eye);
        nml_mat_free(m);
        nml_mat_free(m_inv_computed);
        nml_mat_free(m_inv_expected);
        nml_mat_free(m_dot_m_inv_computed);
        nml_mat_lup_free(m_lup);
    }

    fclose(t_file);

    printf("\n");
    return 0;
}