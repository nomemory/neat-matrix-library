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

#ifndef nml_UTIL
#define nml_UTIL

#include "nml_util.h"

typedef struct nml_matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} nml_matrix;

typedef struct nml_matrices_lu_s {
  nml_matrix *L;
  nml_matrix *U;
  nml_matrix *P;
  unsigned int num_permutations;
} nml_matrices_lu;

//
// Basic Matrix Methods
//
nml_matrix *nml_new(unsigned int num_rows, unsigned int num_cols);
nml_matrix *nml_new_square(unsigned int size);
nml_matrix *nml_new_identity(unsigned int size);
nml_matrix *nml_new_copy(nml_matrix *m);
nml_matrix *nml_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
void nml_free(nml_matrix *matrix);
double nml_get(nml_matrix *matrix, unsigned int i, unsigned int j);
void nml_set(nml_matrix *matrix, unsigned int i, unsigned int j, double value);
void nml_set_all(nml_matrix *matrix, double value);
int nml_set_diag(nml_matrix *matrix, double value);
void nml_print(nml_matrix *matrix);
void nml_printf(nml_matrix *matrix, const char *d_fmt);
int nml_eq_dim(nml_matrix *m1, nml_matrix *m2);

// LU object

nml_matrices_lu *nml_matrices_lu_new(nml_matrix *L, nml_matrix *U, nml_matrix *P, unsigned int num_permutations);
void nml_matrices_lu_free(nml_matrices_lu* lu);

//
// Rows, Columns and Concatenation
//
nml_matrix *nml_rem_col(nml_matrix *m, unsigned int column);
nml_matrix *nml_rem_row(nml_matrix *m, unsigned int row);
nml_matrix *nml_swap_rows(nml_matrix *m, unsigned int row1, unsigned int row2);
int nml_swap_rows_r(nml_matrix *m, unsigned int row1, unsigned int row2);
nml_matrix *nml_multiply_row(nml_matrix *m, unsigned int row, double num);
int nml_multiply_row_r(nml_matrix *m, unsigned int row, double num);
nml_matrix *nml_add_to_row(nml_matrix *m, unsigned int where, unsigned int row, double multiplier);
int nml_add_to_row_r(nml_matrix *m, unsigned int where, unsigned int row, double multiplier);
nml_matrix *nml_concat_h(unsigned int mnun, ...);
nml_matrix *nml_concat_v(unsigned int mnum, ...);

//
// Matrix Operations
//
nml_matrix *nml_plus(nml_matrix *m1, nml_matrix *m2);
//TODO
int *nml_plus_r(nml_matrix *m1, nml_matrix *m2);
nml_matrix *nml_minus(nml_matrix *m1, nml_matrix *m2);
//TODO
int *nml_minus_r(nml_matrix *m1, nml_matrix *m2);
nml_matrix *nml_smultiply(nml_matrix *m, double num);
nml_matrix *nml_multiply(nml_matrix *m1, nml_matrix *m2);
nml_matrix *nml_transpose(nml_matrix *m);
double nml_trace(nml_matrix* m);

//
// LU Decomposition / Determinant / Inverse
//
// Determines the row index for which the value on column k is the absolute max.
// Function is used for pivoting rows for PA=LU decomposition.
int nml_absmax_row(nml_matrix *m1, unsigned int k);
nml_matrices_lu *nml_lup(nml_matrix *m);
double nml_det(nml_matrices_lu* lup);
nml_matrix *nml_inverse(nml_matrices_lu *m);


//
// Solving Linear Systems
//
nml_matrix *nml_solve_ls_fwdsub(nml_matrix *low_triang, nml_matrix *b);
nml_matrix *nml_solve_ls_bcksub(nml_matrix *upper_triang, nml_matrix *b);
nml_matrix *nml_solve_ls(nml_matrices_lu *lu, nml_matrix* b);

#endif
