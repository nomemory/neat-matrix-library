#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

// nml_mat *nml_mat_fromfile(const char *file) {
//   FILE *m_file = fopen(file, "r");
//   if (NULL == m_file) {
//     NML_FERROR(CANNOT_OPEN_FILE, file);
//     return NULL;
//   }
//   int i, j;
//   unsigned int num_rows = 0, num_cols = 0;
//   fscanf(m_file, "%d", &num_rows);
//   fscanf(m_file, "%d", &num_cols);
//   nml_mat *r = nml_mat_new(num_rows, num_cols);
//   for(i = 0; i < r->num_rows; i++) {
//     for(j = 0; j < num_cols; j++) {
//       fscanf(m_file, "%lf\t", &r->data[i][j]);
//     }
//   }
//   fclose(m_file);
//   return r;
// }

int main(int argc, char *argv[]) {

    FILE *t_file = fopen(argv[1], "r");
    if (NULL == t_file) {
        fprintf(stderr, "Cannot open input file. %s", argv[1]);
        return -1;
    }
    
    int i, j, k, num_cases;
    unsigned int num_rows, num_cols;
    nml_mat *input;
    nml_mat *expected;
    nml_mat *computed;

    fscanf(t_file, "%d", &num_cases);

    printf("\nFound %d test cases in file: '%s'\n", num_cases, argv[1]);

    for(k = 0; k < num_cases; k++) {
        printf("\nTest case %d.\n", k);
        fscanf(t_file, "%d", &num_rows);
        fscanf(t_file, "%d", &num_cols);
        
        input = nml_mat_new(num_rows, num_cols);
        expected = nml_mat_new(num_rows, num_cols);
       
        for(i = 0; i < input->num_rows; i++) {
            for(j = 0; j < input->num_cols; j++) {
                fscanf(t_file, "%lf\t", &input->data[i][j]);
            }
        }

        for(i = 0; i < input->num_rows; i++) {
            for(j = 0; j < input->num_cols; j++) {
                fscanf(t_file, "%lf\t", &expected->data[i][j]);
            }
        }


        computed = nml_mat_rref(input);

        if (nml_mat_eq(computed, expected, 0.001)) {
            printf("\nTest passed\n");
        } else {
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

    return -1;
}