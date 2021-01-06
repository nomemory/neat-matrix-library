/**

Copyright 2021

Andrei N. Ciobanu
http://www.andreinc.net/
https://github.com/nomemory/

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

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
int smlc_swap_rows_ip(smlc_matrix *m, unsigned int row1, unsigned int row2);
smlc_matrix *smlc_multiply_row(smlc_matrix *m, unsigned int row, double num);
int smlc_multiply_row_ip(smlc_matrix *m, unsigned int row, double num);
smlc_matrix *smlc_add_to_row(smlc_matrix *m, unsigned int where, unsigned int row, double multiplier);
int smlc_add_to_row_ip(smlc_matrix *m, unsigned int where, unsigned int row, double multiplier);

//
// Operations
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
