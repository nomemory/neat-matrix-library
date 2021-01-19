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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef nml_UTIL
#define nml_UTIL

#include "nml_util.h"

#define NML_MIN_COEF 0.000000000000001

// *****************************************************************************
//
// Library structures
//
// *****************************************************************************
typedef struct nml_mat_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} nml_mat;

typedef struct nml_mat_lup_s {
  nml_mat *L;
  nml_mat *U;
  nml_mat *P;
  unsigned int num_permutations;
} nml_mat_lup;

typedef struct nml_mat_qr_s {
  nml_mat *Q;
  nml_mat *R;
} nml_mat_qr;

// *****************************************************************************
//
// Constructing and destroying a matrix struct
//
// *****************************************************************************
nml_mat *nml_mat_new(unsigned int num_rows, unsigned int num_cols);
nml_mat *nml_mat_rnd(unsigned int num_rows, unsigned int num_cols, double min, double max);
nml_mat *nml_mat_sqr(unsigned int size);
nml_mat *nml_mat_sqr_rnd(unsigned int size, double min, double max);
nml_mat *nml_mat_eye(unsigned int size);
nml_mat *nml_mat_cp(nml_mat *m);
nml_mat *nml_mat_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
nml_mat *nml_mat_fromfile(const char *file);
nml_mat *nml_mat_fromfilef(FILE *f);
void nml_mat_free(nml_mat *matrix);

// *****************************************************************************
//
// Matrix Equality
//
// *****************************************************************************
int nml_mat_eqdim(nml_mat *m1, nml_mat *m2);
int nml_mat_eq(nml_mat *m1, nml_mat *m2, double tolerance);

// *****************************************************************************
//
// Matrix printing
//
// *****************************************************************************
void nml_mat_print(nml_mat *matrix);
void nml_mat_printf(nml_mat *matrix, const char *d_fmt);

// *****************************************************************************
//
// Accessing and modifying matrix elements
//
// *****************************************************************************
double nml_mat_get(nml_mat *matrix, unsigned int i, unsigned int j);
void nml_mat_set(nml_mat *matrix, unsigned int i, unsigned int j, double value);
nml_mat *nml_mat_col_get(nml_mat *m, unsigned int col);
nml_mat *nml_mat_col_mult(nml_mat *m, unsigned int col, double num);
int nml_mat_col_mult_r(nml_mat *m, unsigned int col, double num);
nml_mat *nml_mat_row_get(nml_mat *m, unsigned int row);
nml_mat *nml_mat_row_mult(nml_mat *m, unsigned int row, double num);
int nml_mat_row_mult_r(nml_mat *m, unsigned int row, double num);
void nml_mat_all_set(nml_mat *matrix, double value);
int nml_mat_diag_set(nml_mat *matrix, double value);
nml_mat *nml_mat_row_addrow(nml_mat *m, unsigned int where, unsigned int row, double multiplier);
int nml_mat_row_addrow_r(nml_mat *m, unsigned int where, unsigned int row, double multiplier);
nml_mat *nml_mat_smult(nml_mat *m, double num);
int nml_mat_smult_r(nml_mat *m, double num);

// *****************************************************************************
//
// Modifying the matrix structure
//
// *****************************************************************************
nml_mat *nml_mat_col_rem(nml_mat *m, unsigned int column);
nml_mat *nml_mat_row_rem(nml_mat *m, unsigned int row);
nml_mat *nml_mat_row_swap(nml_mat *m, unsigned int row1, unsigned int row2);
int nml_mat_row_swap_r(nml_mat *m, unsigned int row1, unsigned int row2);
nml_mat *nml_mat_col_swap(nml_mat *m, unsigned int col1, unsigned int col2);
int nml_mat_col_swap_r(nml_mat *m, unsigned int col1, unsigned int col2);
nml_mat *nml_mat_cath(unsigned int mnun, nml_mat **matrices);
nml_mat *nml_mat_catv(unsigned int mnum, nml_mat **matrices);

// *****************************************************************************
//
// Matrix Operations
//
// *****************************************************************************
nml_mat *nml_mat_add(nml_mat *m1, nml_mat *m2);
int nml_mat_add_r(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_sub(nml_mat *m1, nml_mat *m2);
int nml_mat_sub_r(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_dot(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_transp(nml_mat *m);
double nml_mat_trace(nml_mat* m);

// *****************************************************************************
//
// Row Echelon
//
// *****************************************************************************
nml_mat *nml_mat_ref(nml_mat *m);
nml_mat *nml_mat_rref(nml_mat *m);

// *****************************************************************************
//
// LUP Decomposition
//
// *****************************************************************************

nml_mat_lup *nml_mat_lup_new(nml_mat *L, nml_mat *U, nml_mat *P, unsigned int num_permutations);
nml_mat_lup *nml_mat_lup_solve(nml_mat *m);
void nml_mat_lup_free(nml_mat_lup* lu);
void nml_mat_lup_print(nml_mat_lup *lu);
void nml_mat_lup_printf(nml_mat_lup *lu, const char *fmt);
double nml_mat_det(nml_mat_lup* lup);
nml_mat *nml_mat_lu_get(nml_mat_lup* lup);
nml_mat *nml_mat_inv(nml_mat_lup *m);

// *****************************************************************************
//
// Solving linear systems of equations
//
// *****************************************************************************

nml_mat *nml_ls_solvefwd(nml_mat *low_triang, nml_mat *b);
nml_mat *nml_ls_solvebck(nml_mat *upper_triang, nml_mat *b);
nml_mat *nml_ls_solve(nml_mat_lup *lup, nml_mat* b);

// *****************************************************************************
//
// QR Decomposition
//
// *****************************************************************************

double nml_vect_dot(nml_mat *m1, unsigned int m1col, nml_mat *m2, unsigned m2col);
nml_mat *nml_mat_l2norm(nml_mat *m);
double nml_mat_col_l2norm(nml_mat *m1, unsigned int j);
nml_mat *nml_mat_normalize(nml_mat *m);
int nml_mat_normalize_r(nml_mat *m);
nml_mat_qr *nml_mat_qr_new();
void nml_mat_qr_free(nml_mat_qr *qr);
nml_mat_qr * nml_mat_qr_solve(nml_mat *m);

#endif

#ifdef __cplusplus
}
#endif