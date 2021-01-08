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

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>

#include "nml.h"

#define DEFAULT_VALUE 0.0

#define CANNOT_ADD \
  "Cannot add two matrices with different dimensions.\n" \

#define CANNOT_SUBTRACT \
  "Cannot subctract two matrices with different dimensions.\n" \

#define CANNOT_MULITPLY \
  "Cannot multiply two matrices where \
  the number of columns of the first one \
  is different than the number of rows of the second one.\n" \

#define CANNOT_REMOVE_COLUMN \
  "Cannot remove matrix column %d. The value should be less than %d.\n" \

#define CANNOT_REMOVE_ROW \
  "Cannot remove matrix row %d. The value should be less than %d.\n" \

#define INVALID_ROWS \
  "Cannot create matrix with 0 number of rows. Aborting.\n" \

#define INVALID_COLS \
    "Cannot create matrix with 0 number of cols. Aborting.\n" \

#define CANNOT_TRACE \
    "Cannot calculate trace. Matrix needs to be square.\n" \

#define CANNOT_CROUT \
    "Cannot apply crout algorithm. Matrix needs to be square.\n" \

#define CANNOT_SWAP_ROWS \
     "Cannot swap rows (%d, %d) because the matrix number of rows is %d.\n" \

#define CANNOT_ROW_MULTIPLY \
      "Cannot multiply row (%d), maximum number of rows is %d.\n" \

#define CANNOT_ADD_TO_ROW \
      "Cannot add %2.2f x (row=%d) to row=%d. Total number of rows is: %d.\n" \

#define CANNOT_LU_MATRIX_SQUARE \
      "Canot LU. Matrix (%d, %d) needs to be square.\n" \

#define CANNOT_LU_MATRIX_DEGENERATE \
      "Cannot LU. Matrix is degenerate or almost degenerate.\n" \

#define CANNOT_SOLVE_LIN_SYS_INVALID_B \
      "Cannot solve system. b[%d][%d] should have size b[%d][%d].\n" \

#define CANNOT_SET_DIAG \
      "Cannot set diag with value(=%2.2f). Matrix is not square.\n" \

#define CANNOT_CONCATENATE_H \
      "Cannot concatenate. Matrices have a different number of rows. Expected %d, found: %d.\n" \

#define CANNOT_CONCATENATE_V \
      "Cannot concatenate. Matrices have a different number of cols. Expected %d, found: %d.\n" \

// *****************************************************************************
//
// Constructing and destroying a matrix struct
//
// *****************************************************************************

// Dynamically allocates a new matrix struct
nml_mat *nml_mat_new(unsigned int num_rows, unsigned int num_cols) {
  if (num_rows == 0) {
    NML_ERROR(INVALID_ROWS);
    abort();
  }
  if (num_cols == 0) {
    NML_ERROR(INVALID_COLS);
    abort();
  }
  nml_mat *m = calloc(1, sizeof(*m));
  NP_CHECK(m);
  m->num_rows = num_rows;
  m->num_cols = num_cols;
  m->is_square = (num_rows == num_cols) ? 1 : 0;
  m->data = calloc(m->num_rows, sizeof(*m->data));
  NP_CHECK(m->data);
  int i;
  for(i = 0; i < m->num_rows; ++i) {
    m->data[i] = calloc(m->num_cols, sizeof(**m->data));
    NP_CHECK(m->data[i]);
  }
  return m;
}

// Dynamically alloctes a new matrix struct
// The matrix is square (number of rows is equl with the number of cols)
nml_mat *nml_mat_sqr(unsigned int size) {
  return nml_mat_new(size, size);
}

// Dynamically allocates a a new matrix struct
// The resulting matrix is an ID matrix (I)
// I has 1.0 on the first diagonal
// I is square
nml_mat *nml_mat_id(unsigned int size) {
  nml_mat *r = nml_mat_new(size, size);
  int i;
  for(i = 0; i < r->num_rows; i++) {
    r->data[i][i] = 1.0;
  }
  return r;
}

// Dynamically allocates a new matrix struct
// Initialise the matrix by reading values from a vector
nml_mat *nml_mat_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals) {
  nml_mat *m = nml_mat_new(num_rows, num_cols);
  int i, j, v_idx;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      v_idx = i * m->num_cols + j;
      m->data[i][j] = (v_idx < n_vals) ? vals[v_idx] : 0.0;
    }
  }
  return m;
}

// Dynamically allocates a new Matrix
// Initialise the matrix by copying another one
nml_mat *nml_mat_cp(nml_mat *m) {
  nml_mat *r  = nml_mat_new(m->num_rows, m->num_cols);
  int i,j;
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[i][j];
    }
  }
  return r;
}

// Frees a matrix structure
void nml_mat_free(nml_mat *matrix) {
  int i;
  for(i = 0; i < matrix->num_rows; ++i) {
    free(matrix->data[i]);
  }
  free(matrix->data);
}

// *****************************************************************************
//
// Matrix Equality
//
// *****************************************************************************

// Checks if two matrices have the same dimesions
int nml_mat_eqdim(nml_mat *m1, nml_mat *m2) {
  return (m1->num_cols == m2->num_cols) &&
          (m1->num_rows == m2->num_rows);
}

// Checks if two matrices have the same dimensions, and the elements
// are all equal to each other with a given tolerance;
// For exact equality use tolerance = 0.0
int nml_mat_eq(nml_mat *m1, nml_mat *m2, double tolerance) {
  if (!nml_mat_eqdim(m1, m2)) {
    return 0;
  }
  int i, j;
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; i < m1->num_cols; j++) {
      if (m1->data[i][j] - m2->data[i][j] > tolerance) {
        return 0;
      }
    }
  }
  return 1;
}

// *****************************************************************************
//
// Matrix printing
//
// *****************************************************************************

// Prints the matrix on the stdout
void nml_mat_print(nml_mat *matrix) {
  nml_mat_printf(matrix, "%2.2f\t");
}

// Prints the matrix on the stdout (with a custom formatting for elements)
void nml_mat_printf(nml_mat *matrix, const char *d_fmt) {
  int i, j;
  fprintf(stdout, "\n");
  for(i = 0; i < matrix->num_rows; ++i) {
    for(j = 0; j < matrix->num_cols; ++j) {
      fprintf(stdout, d_fmt, matrix->data[i][j]);
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
}

// *****************************************************************************
//
// Accessing and modifying matrix elements
//
// *****************************************************************************

double nml_mat_get(nml_mat *matrix, unsigned int i, unsigned int j) {
  return matrix->data[i][j];
}

void nml_mat_set(nml_mat *matrix, unsigned int i, unsigned int j, double value) {
  matrix->data[i][j] = value;
}

// Sets all elements of a matrix to a given value
void nml_mat_setall(nml_mat *matrix, double value) {
  int i, j;
  for(i = 0; i < matrix->num_rows; i++) {
    for(j = 0; j < matrix->num_cols; j++) {
      matrix->data[i][j] = value;
    }
  }
}

// Sets all elements of the matrix to given value
int nml_mat_setdiag(nml_mat *m, double value) {
  if (!m->is_square) {
    NML_FERROR(CANNOT_SET_DIAG, value);
    return 0;
  }
  int i;
  for(i = 0; i < m->num_rows; i++) {
    m->data[i][i] = value;
  }
  return 1;
}

nml_mat *nml_mat_multr(nml_mat *m, unsigned int row, double num) {
  nml_mat *r = nml_mat_cp(m);
  if (nml_mat_multr_r(r, row, num)) {
    nml_mat_free(r);
    return NULL;
  }
  return r;
}

int nml_mat_multr_r(nml_mat *m, unsigned int row, double num) {
  if (row>=m->num_rows) {
    NML_FERROR(CANNOT_ROW_MULTIPLY, row, m->num_rows);
    return 0;
  }
  int i;
  for(i=0; i < m->num_cols; i++) {
    m->data[row][i] *= num;
  }
  return 0;
}

nml_mat *nml_mat_add2r(nml_mat *m, unsigned int where, unsigned int row, double multiplier) {
  nml_mat *r = nml_mat_cp(m);
  if (!nml_mat_add2r_r(m, where, row, multiplier)) {
    nml_mat_free(r);
    return NULL;
  }
  return r;
}

int nml_mat_add2r_r(nml_mat *m, unsigned int where, unsigned int row, double multiplier) {
  if (where >= m->num_rows || row >= m->num_rows) {
    NML_FERROR(CANNOT_ADD_TO_ROW, multiplier, row, where, m->num_rows);
    return 0;
  }
  int i = 0;
  for(i = 0; i < m->num_cols; i++) {
    m->data[where][i] += multiplier * m->data[row][i];
  }
  return 1;
}

nml_mat *nml_mat_smult(nml_mat *m, double num) {
  nml_mat *r = nml_mat_cp(m);
  nml_mat_smult_r(r, num);
  return r;
}

int nml_mat_smult_r(nml_mat *m, double num) {
  int i, j;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      m->data[i][j] *= num;
    }
  }
  return 1;
}

// *****************************************************************************
//
// Modifying the matrix structure
//
// *****************************************************************************
nml_mat *nml_mat_rcol(nml_mat *m, unsigned int column) {
  if(column >= m->num_cols) {
    NML_FERROR(CANNOT_REMOVE_COLUMN, column, m->num_cols);
    return NULL;
  }
  nml_mat *r = nml_mat_new(m->num_rows, m->num_cols-1);
  int i, j, k;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0, k=0; j < m->num_cols; j++) {
      if (column!=j) {
        r->data[i][k++] = m->data[i][j];
      }
    }
  }
  return r;
}

nml_mat *nml_mat_rrow(nml_mat *m, unsigned int row) {
  if (row >= m->num_rows) {
    NML_FERROR(CANNOT_REMOVE_ROW, row, m->num_rows);
    return NULL;
  }
  nml_mat *r = nml_mat_new(m->num_rows-1, m->num_cols);
  int i, j, k;
  for(i = 0, k = 0; i < m->num_rows; i++) {
    if (row!=i) {
      for(j = 0; j < m->num_cols; j++) {
        r->data[k][j] = m->data[i][j];
      }
      k++;
    }
  }
  return r;
}

nml_mat *nml_mat_swaprs(nml_mat *m, unsigned int row1, unsigned int row2) {
  nml_mat *r = nml_mat_cp(m);
  if (!nml_mat_swaprs_r(r, row1, row2)) {
    nml_mat_free(r);
    return NULL;
  }
  return r;
}

int nml_mat_swaprs_r(nml_mat *m, unsigned int row1, unsigned int row2) {
  if (row1 >= m->num_rows || row2 >= m->num_rows) {
    NML_FERROR(CANNOT_SWAP_ROWS, row1, row2, m->num_rows);
    return 0;
  }
  double *tmp = m->data[row2];
  m->data[row2] = m->data[row1];
  m->data[row1] = tmp;
  return 1;
}

// Concatenates a variable number of matrices into one
// The concentation is done horizontally this means the matrices need to have
// the same number of rows, while the number of columns is allowed to
// be variable
nml_mat *nml_mat_cath(unsigned int mnum, ...) {
  if (0==mnum) {
    return NULL;
  }
  va_list argp;
  va_start(argp, mnum);
  nml_mat * fm = va_arg(argp, nml_mat*);
  if (1==mnum) {
    // We just return the one matrix supplied as the first param
    // no need for additional logic
    va_end(argp);
    return nml_mat_cp(fm);
  }
  // We compute to see if the matrices have the same number of rows
  // We fillup the array containing the matrices from the varags
  // We calculate the total number of columns to know how to allocate memory
  // for the resulting matrix]
  int i,j,k,jk,offset;
  unsigned int lrow, ncols;
  nml_mat **marr;
  marr = malloc(sizeof(*marr) * mnum);
  marr[0] = fm;
  lrow = fm->num_rows;
  ncols = fm->num_cols;
  for(k = 1; k < mnum; k++) {
    marr[k] = va_arg(argp, nml_mat*);
    if (lrow!=marr[k]->num_rows) {
      NML_FERROR(CANNOT_CONCATENATE_H, lrow, marr[k]->num_rows);
      free(marr);
      return NULL;
    }
    ncols+=marr[k]->num_cols;
  }
  va_end(argp);

  // At this point we know how the resulting matrix looks like,
  // we allocate memory for it accordingly
  nml_mat *r = nml_mat_new(lrow, ncols);
  nml_mat_print(r);
  for(i = 0; i < r->num_rows; i++) {
    k = 0;
    offset = 0;
    for(j = 0; j < r->num_cols; j++) {
      jk = j - offset;
      // If the column index of marr[k] overflows
      // We jump to the next matrix in the array
      if (jk >= marr[k]->num_cols) {
        offset = marr[k]->num_cols;
        k++;
      }
      r->data[i][j] = marr[k]->data[i][jk];
    }
  }
  free(marr);
  return r;
}

// Concatenates a variable number of matrices into one.
// The concentation is done vertically this means the matrices need to have
// the same number of columns, while the number of rows is allowed to
// be variable
nml_mat *nml_concat_v(unsigned int mnum, ...) {
  return NULL;
}

// *****************************************************************************
//
// Matrix Operations
//
// *****************************************************************************
//
// Matrix Operations
//

nml_mat *nml_mat_add(nml_mat *m1, nml_mat *m2) {
  nml_mat *r = nml_mat_cp(m1);
  if (!nml_mat_add_r(r, m2)) {
    nml_mat_free(r);
    return NULL;
  }
  return r;
}

int nml_mat_add_r(nml_mat *m1, nml_mat *m2) {
  if (!nml_mat_eqdim(m1, m2)) {
    NML_ERROR(CANNOT_ADD);
    return 0;
  }
  int i, j;
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m2->num_rows; j++) {
      m1->data[i][j] += m2->data[i][j];
    }
  }
  return 1;
}

nml_mat *nml_mat_sub(nml_mat *m1, nml_mat *m2) {
  nml_mat *r = nml_mat_cp(m2);
  if (!nml_mat_sub_r(r, m2)) {
    nml_mat_free(r);
    return NULL;
  }
  return r;
}

int nml_mat_sub_r(nml_mat *m1, nml_mat *m2) {
  if (!nml_mat_eqdim(m1, m2)) {
    NML_ERROR(CANNOT_SUBTRACT);
    return 0;
  }
  int i, j;
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m2->num_rows; j++) {
      m1->data[i][j] -= m2->data[i][j];
    }
  }
  return 1;
}

nml_mat *nml_mat_mult(nml_mat *m1, nml_mat *m2) {
  if (!(m1->num_cols == m2->num_rows)) {
    NML_ERROR(CANNOT_MULITPLY);
    return NULL;
  }
  int i, j, k;
  nml_mat *r = nml_mat_new(m1->num_rows, m2->num_cols);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      for(k = 0; k < m1->num_cols; k++) {
        r->data[i][j] += m1->data[i][k] * m2->data[k][j];
      }
    }
  }
  return r;
}

nml_mat *nml_mat_trans(nml_mat *m) {
  int i, j;
  nml_mat *r = nml_mat_new(m->num_cols, m->num_rows);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[j][i];
    }
  }
  return r;
}

double nml_trace(nml_mat* m) {
  if (!m->is_square) {
    NML_ERROR(CANNOT_TRACE);
  }
  int i;
  double trace = 0.0;
  for(i = 0; i < m->num_rows; i++) {
    trace += m->data[i][i];
  }
  return trace;
}

// *****************************************************************************
//
// LUP Decomposition
//
// *****************************************************************************

// Finds the maxid on the column (starting from k -> num_rows)
// This method is used for pivoting in LUP decomposition
int nml_mat_absmaxr(nml_mat *m, unsigned int k) {
  // Find max id on the column;
  int i;
  double max = m->data[k][k];
  int maxIdx = k;
  for(i = k+1; i < m->num_rows; i++) {
    if (m->data[i][k] > max) {
      max = fabs(m->data[i][k]);
      maxIdx = i;
    }
  }
  return maxIdx;
}

nml_mat_lup *nml_mat_lup_new(nml_mat *L, nml_mat *U, nml_mat *P, unsigned int num_permutations) {
  nml_mat_lup *r = malloc(sizeof(*r));
  NP_CHECK(r);
  r->L = L;
  r->U = U;
  r->P = P;
  r->num_permutations = num_permutations;
  return r;
}
void nml_mat_lup_free(nml_mat_lup* lu) {
  if (!lu) free(lu);
}

nml_mat_lup *nml_mat_lup_solve(nml_mat *m) {
  if (!m->is_square) {
    NML_FERROR(CANNOT_LU_MATRIX_SQUARE, m->num_rows, m-> num_cols);
    return NULL;
  }
  nml_mat *L = nml_mat_new(m->num_rows, m->num_rows);
  nml_mat *U = nml_mat_cp(m);
  nml_mat *P = nml_mat_id(m->num_rows);

  int j,i, pivot;
  unsigned int num_permutations = 0;
  double mult;

  for(j = 0; j < U->num_cols; j++) {
    // Retrieves the row with the biggest element for column (j)
    pivot = nml_mat_absmaxr(U, j);
    if (fabs(U->data[pivot][j]) < DBL_EPSILON) {
      NML_ERROR(CANNOT_LU_MATRIX_DEGENERATE);
      return NULL;
    }
    if (pivot!=j) {
      // Pivots LU and P accordingly to the rule
      nml_mat_swaprs_r(U, j, pivot);
      nml_mat_swaprs_r(L, j, pivot);
      nml_mat_swaprs_r(P, j, pivot);
      // Keep the number of permutations to easily calculate the
      // determinant sign afterwards
      num_permutations++;
    }
    for(i = j+1; i < U->num_rows; i++) {
      mult = U->data[i][j] / U->data[j][j];
      // Building the U upper rows
      nml_mat_add2r_r(U, i, j, -mult);
      // Store the multiplier in L
      L->data[i][j] = mult;
    }
  }
  nml_mat_setdiag(L, 1.0);

  return nml_mat_lup_new(L, U, P, num_permutations);
}

// After the LU(P) factorisation the determinant can be easily calculated
// by multiplying the main diagonal of matrix U with the sign.
// the sign is -1 if the number of permutations is odd
// the sign is +1 if the number of permutations is even
double nml_mat_det(nml_mat_lup* lup) {
  int k;
  int sign = (lup->num_permutations%2==0) ? 1 : -1;
  nml_mat *U = lup->U;
  double product = 1.0;
  for(k = 0; k < U->num_rows; k++) {
    product *= U->data[k][k];
  }
  return product * sign;
}

// *****************************************************************************
//
// Solving linear systems of equations
//
// *****************************************************************************

// Forward substitution algorithm
// Solves the linear system L * x = b
//
// L is lower triangular matrix of size NxN
// B is column matrix of size Nx1
// x is the solution column matrix of size Nx1
//
// Note: In case L is not a lower triangular matrix, the algorithm will try to
// select only the lower triangular part of the matrix L and solve the system
// with it.
//
// Note: In case any of the diagonal elements (L[i][i]) are 0 the system cannot
// be solved
//
// Note: This function is usually used with an L matrix from a LU decomposition
nml_mat *nml_solve_ls_fwdsub(nml_mat *L, nml_mat *b) {
  nml_mat* x = nml_mat_new(L->num_cols, 1);
  int i,j;
  double tmp;
  for(i = 0; i < L->num_cols; i++) {
    tmp = b->data[i][0];
    for(j = 0; j < i ; j++) {
      tmp -= L->data[i][j] * x->data[j][0];
    }
    x->data[i][0] = tmp / L->data[i][i];
  }
  return x;
}


// Back substition algorithm
// Solves the linear system U *x = b
//
// U is an upper triangular matrix of size NxN
// B is a column matrix of size Nx1
// x is the solution column matrix of size Nx1
//
// Note in case U is not an upper triangular matrix, the algorithm will try to
// select only the upper triangular part of the matrix U and solve the system
// with it
//
// Note: In case any of the diagonal elements (U[i][i]) are 0 the system cannot
// be solved
nml_mat *nml_solve_ls_bcksub(nml_mat *U, nml_mat *b) {
  nml_mat *x = nml_mat_new(U->num_cols, 1);
  int i = U->num_cols, j;
  double tmp;
  while(i-->0) {
    tmp = b->data[i][0];
    for(j = i; j < U->num_cols; j++) {
      tmp -= U->data[i][j] * x->data[j][0];
    }
    x->data[i][0] = tmp / U->data[i][i];
  }
  return x;
}

// A[n][n] is a square matrix
// m contains matrices L, U, P for A[n][n] so that P*A = L*U
//
// The linear system is:
// A*x=b  =>  P*A*x = P*b  =>  L*U*x = P*b  =>
// (where b is a matrix[n][1], and x is a matrix[n][1])
//
// if y = U*x , we solve two systems:
//    L * y = P b (forward substition)
//    U * x = y (backward substition)
//
// We obtain and return x
nml_mat *nml_solve_ls(nml_mat_lup *lu, nml_mat* b) {
  if (lu->U->num_rows != b->num_rows || b->num_cols != 1) {
    NML_FERROR(CANNOT_SOLVE_LIN_SYS_INVALID_B,
      b->num_rows,
      b->num_cols,
      lu->U->num_rows,
      lu->U->num_cols);
      return NULL;
  }
  nml_mat *Pb = nml_mat_mult(lu->P, b);

  // We solve L*y = P*b using forward substition
  nml_mat *y = nml_solve_ls_fwdsub(lu->L, Pb);

  // We solve U*x=y
  nml_mat *x = nml_solve_ls_bcksub(lu->U, y);

  nml_mat_free(y);
  nml_mat_free(Pb);
  return x;
}

// A * (A^-1) = I , where A[N][N]
// We need to find (A^-1) which is the inverse
//
// We can write this as a series of N linear systems:
// A * (a^-1)k = ik
//
// where:
// (a^-1)k is the column vector k of the inverse
// ik is the column vector k of the identity
//
// but:
// PA = LU =>
// P * A * (a^-1)k = P * ik =>
// L * U * (a^-1)k = P * ik
//
// this means we will have to solve N systems of linear equations in this form:
// L * U * (a^-1)k = P * ik
// where 0 <= k < N
//
// And then the inverse is:
// (A^-1) = ( (a^-1)0 | (a^-1)2 | ... | (A^-1)n-1)
// nml_mat *nml_mat_inv(nml_mat_lup *lu) {
//   unsigned int N = lu->L->num_cols;
//   nml_mat *i = nml_mat_new_square(N);
//   for(i = 0; i < N; i++) {
//
//   }
//   return i;
// }
