#ifndef SMLC_UTIL
#define SMLC_UTIL

#include "smlc_util.h"

typedef struct smlc_matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} smlc_matrix;

//
// Basic Matrix Methods
//
smlc_matrix *smlc_new(unsigned int num_rows, unsigned int num_cols);
smlc_matrix *smlc_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
void smlc_free(smlc_matrix *matrix);
double smlc_get(smlc_matrix *matrix, unsigned int i, unsigned int j);
void smlc_set(smlc_matrix *matrix, unsigned int i, unsigned int j, double value);
void smlc_set_all(smlc_matrix *matrix, double value);
void smlc_set_diag(smlc_matrix *matrix, double value);
void smlc_print(smlc_matrix *matrix, const char *d_fmt);
int smlc_eq_dim(smlc_matrix *m1, smlc_matrix *m2);

//
// Operations
//
smlc_matrix *smlc_plus(smlc_matrix *m1, smlc_matrix *m2);
smlc_matrix *smlc_minus(smlc_matrix *m1, smlc_matrix *m2);
smlc_matrix *smlc_smultiply(smlc_matrix *m, double num);
smlc_matrix *smlc_multiply(smlc_matrix *m1, smlc_matrix *m2);
smlc_matrix *smlc_transpose(smlc_matrix *m);

#endif
