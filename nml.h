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

typedef struct nml_mat_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} nml_mat;

typedef struct nml_mat_cols_s {
  unsigned int num_cols;
  nml_mat *cols;
} nml_mat_cols;

typedef struct nml_mat_rows_s {
  unsigned int num_rows;
  nml_mat *rows;
} num_mat_rows;

typedef struct nml_mat_lup_s {
  nml_mat *L;
  nml_mat *U;
  nml_mat *P;
  unsigned int num_permutations;
} nml_mat_lup;

//
// Basic Matrix Methods
//
nml_mat *nml_mat_new(unsigned int num_rows, unsigned int num_cols);
nml_mat *nml_mat_sqr(unsigned int size);
nml_mat *nml_mat_id(unsigned int size);
nml_mat *nml_mat_cp(nml_mat *m);
nml_mat *nml_mat_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
void nml_mat_free(nml_mat *matrix);
double nml_mat_get(nml_mat *matrix, unsigned int i, unsigned int j);
void nml_mat_set(nml_mat *matrix, unsigned int i, unsigned int j, double value);
void nml_mat_setall(nml_mat *matrix, double value);
int nml_mat_setdiag(nml_mat *matrix, double value);
void nml_mat_print(nml_mat *matrix);
void nml_mat_printf(nml_mat *matrix, const char *d_fmt);
int nml_mat_eqdim(nml_mat *m1, nml_mat *m2);
int nml_mat_eq(nml_mat *m1, nml_mat *m2, double tolerance);
int nml_mat_absmaxr(nml_mat *m1, unsigned int k);

//
// Rows, Columns and Concatenation
//
nml_mat *nml_mat_rcol(nml_mat *m, unsigned int column);
nml_mat *nml_mat_rrow(nml_mat *m, unsigned int row);
nml_mat *nml_mat_swaprs(nml_mat *m, unsigned int row1, unsigned int row2);
int nml_mat_swaprs_r(nml_mat *m, unsigned int row1, unsigned int row2);
//TODO
nml_mat *nml_mat_swapcs(nml_mat *m, unsigned int col1, unsigned int col2);
//TODO
int nml_mat_swapcs_r(nml_mat *m, unsigned int col1, unsigned int col2);
nml_mat *nml_mat_multr(nml_mat *m, unsigned int row, double num);
int nml_mat_multr_r(nml_mat *m, unsigned int row, double num);
nml_mat *nml_mat_add2r(nml_mat *m, unsigned int where, unsigned int row, double multiplier);
int nml_mat_add2r_r(nml_mat *m, unsigned int where, unsigned int row, double multiplier);
nml_mat *nml_mat_cath(unsigned int mnun, ...);
nml_mat *nml_mat_catv(unsigned int mnum, ...);

//
// Matrix Operations
//
nml_mat *nml_mat_add(nml_mat *m1, nml_mat *m2);
int nml_mat_add_r(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_sub(nml_mat *m1, nml_mat *m2);
int nml_mat_sub_r(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_smult(nml_mat *m, double num);
int nml_mat_smult_r(nml_mat *m, double num);
nml_mat *nml_mat_mult(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_trans(nml_mat *m);
double nml_mat_trace(nml_mat* m);

// Column and Row Vectors
nml_mat *nml_cols(nml_mat *m1);
nml_mat *nml_rows(nml_mat *m2);

//
// LU Decomposition / Determinant / Inverse
//
// Determines the row index for which the value on column k is the absolute max.
// Function is used for pivoting rows for PA=LU decomposition.
nml_mat_lup *nml_mat_lup_new(nml_mat *L, nml_mat *U, nml_mat *P, unsigned int num_permutations);
nml_mat_lup *nml_mat_lup_solve(nml_mat *m);
void nml_mat_lup_free(nml_mat_lup* lu);

double nml_mat_det(nml_mat_lup* lup);
nml_mat *nml_mat_inv(nml_mat_lup *m);


//
// Solving Linear Systems
//
nml_mat *nml_ls_solvefwd(nml_mat *low_triang, nml_mat *b);
nml_mat *nml_ls_solvebck(nml_mat *upper_triang, nml_mat *b);
nml_mat *nml_ls_solve(nml_mat_lup *lu, nml_mat* b);

#endif
