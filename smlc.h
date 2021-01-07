/**
Copyright 20201 Andrei N. Ciobanu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef SMLC_UTIL
#define SMLC_UTIL

#include "smlc_util.h"

typedef struct smlc_matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} smlc_matrix;

typedef struct smlc_matrix_lu_s {
  smlc_matrix *L;
  smlc_matrix *U;
  smlc_matrix *P;
} smlc_matrix_lu;

//
// Basic Matrix Methods
//
smlc_matrix *smlc_new(unsigned int num_rows, unsigned int num_cols);
smlc_matrix *smlc_new_identity(unsigned int size);
smlc_matrix *smlc_new_copy(smlc_matrix *m);
smlc_matrix *smlc_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
void smlc_free(smlc_matrix *matrix);
double smlc_get(smlc_matrix *matrix, unsigned int i, unsigned int j);
void smlc_set(smlc_matrix *matrix, unsigned int i, unsigned int j, double value);
void smlc_set_all(smlc_matrix *matrix, double value);
void smlc_set_diag(smlc_matrix *matrix, double value);
void smlc_print(smlc_matrix *matrix);
void smlc_printf(smlc_matrix *matrix, const char *d_fmt);
int smlc_eq_dim(smlc_matrix *m1, smlc_matrix *m2);

// LU object

smlc_matrix_lu *smlc_matrix_lu_new(smlc_matrix *L, smlc_matrix *U,smlc_matrix *P);
void smlc_matrix_lu_free(smlc_matrix_lu* lu);

//
// Basic Row and Operations
//
smlc_matrix *smlc_rem_col(smlc_matrix *m, unsigned int column);
smlc_matrix *smlc_rem_row(smlc_matrix *m, unsigned int row);
smlc_matrix *smlc_swap_rows(smlc_matrix *m, unsigned int row1, unsigned int row2);
int smlc_swap_rows_r(smlc_matrix *m, unsigned int row1, unsigned int row2);
smlc_matrix *smlc_multiply_row(smlc_matrix *m, unsigned int row, double num);
int smlc_multiply_row_ip(smlc_matrix *m, unsigned int row, double num);
smlc_matrix *smlc_add_to_row(smlc_matrix *m, unsigned int where, unsigned int row, double multiplier);
int smlc_add_to_row_ip(smlc_matrix *m, unsigned int where, unsigned int row, double multiplier);

//
// Matrix Operations
//
smlc_matrix *smlc_plus(smlc_matrix *m1, smlc_matrix *m2);
smlc_matrix *smlc_minus(smlc_matrix *m1, smlc_matrix *m2);
smlc_matrix *smlc_smultiply(smlc_matrix *m, double num);
smlc_matrix *smlc_multiply(smlc_matrix *m1, smlc_matrix *m2);
smlc_matrix *smlc_transpose(smlc_matrix *m);
double smlc_trace(smlc_matrix* m);

//
// LU Decomposition
//
// Determines the row index for which the value on column k is the absolute max.
// Function is used for pivoting rows for PA=LU decomposition.
int smlc_absmax_row(smlc_matrix *m1, unsigned int k);
smlc_matrix_lu *smlc_lup(smlc_matrix *m);

#endif
