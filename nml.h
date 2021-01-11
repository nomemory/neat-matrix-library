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

// *****************************************************************************
//
// Constructing and destroying a matrix struct
//
// *****************************************************************************
nml_mat *nml_mat_new(unsigned int num_rows, unsigned int num_cols);
nml_mat *nml_mat_new_rnd(unsigned int num_rows, unsigned int num_cols, double min, double max);
nml_mat *nml_mat_sqr(unsigned int size);
nml_mat *nml_mat_sqr_rnd(unsigned int size, double min, double max);
nml_mat *nml_mat_id(unsigned int size);
nml_mat *nml_mat_cp(nml_mat *m);
nml_mat *nml_mat_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);
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
nml_mat *nml_mat_getcol(nml_mat *m, unsigned int col);
nml_mat *nml_mat_getrow(nml_mat *m, unsigned int row);
void nml_mat_set(nml_mat *matrix, unsigned int i, unsigned int j, double value);
void nml_mat_setall(nml_mat *matrix, double value);
int nml_mat_setdiag(nml_mat *matrix, double value);
nml_mat *nml_mat_multrow(nml_mat *m, unsigned int row, double num);
int nml_mat_multrow_r(nml_mat *m, unsigned int row, double num);
nml_mat *nml_mat_rowplusrow(nml_mat *m, unsigned int where, unsigned int row, double multiplier);
int nml_mat_rowplusrow_r(nml_mat *m, unsigned int where, unsigned int row, double multiplier);
nml_mat *nml_mat_scalarmult(nml_mat *m, double num);
int nml_mat_scalarmult_r(nml_mat *m, double num);

// *****************************************************************************
//
// Modifying the matrix structure
//
// *****************************************************************************
nml_mat *nml_mat_remcol(nml_mat *m, unsigned int column);
nml_mat *nml_mat_remrow(nml_mat *m, unsigned int row);
nml_mat *nml_mat_swaprows(nml_mat *m, unsigned int row1, unsigned int row2);
int nml_mat_swaprows_r(nml_mat *m, unsigned int row1, unsigned int row2);
nml_mat *nml_mat_swapcols(nml_mat *m, unsigned int col1, unsigned int col2);
int nml_mat_swapcols_r(nml_mat *m, unsigned int col1, unsigned int col2);
nml_mat *nml_mat_concath(unsigned int mnun, nml_mat **matrices);
nml_mat *nml_mat_concatv(unsigned int mnum, nml_mat **matrices);

// *****************************************************************************
//
// Matrix Operations
//
// *****************************************************************************
nml_mat *nml_mat_add(nml_mat *m1, nml_mat *m2);
int nml_mat_add_r(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_sub(nml_mat *m1, nml_mat *m2);
int nml_mat_sub_r(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_mult(nml_mat *m1, nml_mat *m2);
nml_mat *nml_mat_trs(nml_mat *m);
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
double nml_mat_determinant(nml_mat_lup* lup);
nml_mat *nml_mat_getlu(nml_mat_lup* lup);
nml_mat *nml_mat_inverse(nml_mat_lup *m);

// *****************************************************************************
//
// Solving linear systems of equations
//
// *****************************************************************************

nml_mat *nml_ls_solvefwd(nml_mat *low_triang, nml_mat *b);
nml_mat *nml_ls_solvebck(nml_mat *upper_triang, nml_mat *b);
nml_mat *nml_ls_solve(nml_mat_lup *lup, nml_mat* b);

#endif
