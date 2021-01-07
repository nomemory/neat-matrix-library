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

#ifndef nna_UTIL
#define nna_UTIL

#include "nna_util.h"

typedef struct nna_matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} nna_matrix;

typedef struct nna_matrices_lu_s {
  nna_matrix *L;
  nna_matrix *U;
  nna_matrix *P;
  unsigned int num_permutations;
} nna_matrices_lu;

//
// Basic Matrix Methods
//
nna_matrix *nna_new(unsigned int num_rows, unsigned int num_cols);
nna_matrix *nna_new_identity(unsigned int size);
nna_matrix *nna_new_copy(nna_matrix *m);
nna_matrix *nna_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
void nna_free(nna_matrix *matrix);
double nna_get(nna_matrix *matrix, unsigned int i, unsigned int j);
void nna_set(nna_matrix *matrix, unsigned int i, unsigned int j, double value);
void nna_set_all(nna_matrix *matrix, double value);
int nna_set_diag(nna_matrix *matrix, double value);
void nna_print(nna_matrix *matrix);
void nna_printf(nna_matrix *matrix, const char *d_fmt);
int nna_eq_dim(nna_matrix *m1, nna_matrix *m2);

// LU object

nna_matrices_lu *nna_matrices_lu_new(nna_matrix *L, nna_matrix *U, nna_matrix *P, unsigned int num_permutations);
void nna_matrices_lu_free(nna_matrices_lu* lu);

//
// Basic Row and Operations
//
nna_matrix *nna_rem_col(nna_matrix *m, unsigned int column);
nna_matrix *nna_rem_row(nna_matrix *m, unsigned int row);
nna_matrix *nna_swap_rows(nna_matrix *m, unsigned int row1, unsigned int row2);
int nna_swap_rows_r(nna_matrix *m, unsigned int row1, unsigned int row2);
nna_matrix *nna_multiply_row(nna_matrix *m, unsigned int row, double num);
int nna_multiply_row_r(nna_matrix *m, unsigned int row, double num);
nna_matrix *nna_add_to_row(nna_matrix *m, unsigned int where, unsigned int row, double multiplier);
int nna_add_to_row_r(nna_matrix *m, unsigned int where, unsigned int row, double multiplier);

//
// Matrix Operations
//
nna_matrix *nna_plus(nna_matrix *m1, nna_matrix *m2);
nna_matrix *nna_minus(nna_matrix *m1, nna_matrix *m2);
nna_matrix *nna_smultiply(nna_matrix *m, double num);
nna_matrix *nna_multiply(nna_matrix *m1, nna_matrix *m2);
nna_matrix *nna_transpose(nna_matrix *m);
double nna_trace(nna_matrix* m);

//
// LU Decomposition / Determinant / Inverse
//
// Determines the row index for which the value on column k is the absolute max.
// Function is used for pivoting rows for PA=LU decomposition.
int nna_absmax_row(nna_matrix *m1, unsigned int k);
nna_matrices_lu *nna_lup(nna_matrix *m);
double nna_det(nna_matrices_lu* lup);
nna_matrix *nna_inverse(nna_matrices_lu *m);


//
// Solving Linear Systems
//
nna_matrix *nna_solve_ls_fwdsub(nna_matrix *low_triang, nna_matrix *b);
nna_matrix *nna_solve_ls_bcksub(nna_matrix *upper_triang, nna_matrix *b);
nna_matrix *nna_solve_ls(nna_matrices_lu *lu, nna_matrix* b);



#endif
