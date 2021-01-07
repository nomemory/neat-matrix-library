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

#include "nna.h"

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

//
// Basic Matrix Methods
//

// Dynamically allocates a new
nna_matrix *nna_new(unsigned int num_rows, unsigned int num_cols) {
  if (num_rows == 0) {
    NNA_ERROR(INVALID_ROWS);
    abort();
  }
  if (num_cols == 0) {
    NNA_ERROR(INVALID_COLS);
    abort();
  }
  nna_matrix *m = calloc(1, sizeof(*m));
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

nna_matrix *nna_new_identity(unsigned int size) {
  nna_matrix *r = nna_new(size, size);
  int i;
  for(i = 0; i < r->num_rows; i++) {
    r->data[i][i] = 1.0;
  }
  return r;
}

nna_matrix *nna_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals) {
  nna_matrix *m = nna_new(num_rows, num_cols);
  int i, j, v_idx;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      v_idx = i * m->num_cols + j;
      m->data[i][j] = (v_idx < n_vals) ? vals[v_idx] : 0.0;
    }
  }
  return m;
}

nna_matrix *nna_new_copy(nna_matrix *m) {
  nna_matrix *r  = nna_new(m->num_rows, m->num_cols);
  int i,j;
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[i][j];
    }
  }
  return r;
}

void nna_free(nna_matrix *matrix) {
  int i;
  for(i = 0; i < matrix->num_rows; ++i) {
    free(matrix->data[i]);
  }
  free(matrix->data);
}

double nna_get(nna_matrix *matrix, unsigned int i, unsigned int j) {
  return matrix->data[i][j];
}

void nna_set(nna_matrix *matrix, unsigned int i, unsigned int j, double value) {
  matrix->data[i][j] = value;
}


void nna_set_all(nna_matrix *matrix, double value) {
  int i, j;
  for(i = 0; i < matrix->num_rows; i++) {
    for(j = 0; j < matrix->num_cols; j++) {
      matrix->data[i][j] = value;
    }
  }
}

int nna_set_diag(nna_matrix *m, double value) {
  if (!m->is_square) {
    NNA_FERROR(CANNOT_SET_DIAG, value);
    return 0;
  }
  int i;
  for(i = 0; i < m->num_rows; i++) {
    m->data[i][i] = value;
  }
  return 1;
}

void nna_print(nna_matrix *matrix) {
  nna_printf(matrix, "%2.2f\t");
}

void nna_printf(nna_matrix *matrix, const char *d_fmt) {
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

// Checks if two matrices have the same dimesions
int nna_eq_dim(nna_matrix *m1, nna_matrix *m2) {
  return (m1->num_cols == m2->num_cols) &&
          (m1->num_rows == m2->num_rows);
}

//
// Matrix LU
//


nna_matrices_lu *nna_matrices_lu_new(nna_matrix *L, nna_matrix *U, nna_matrix *P, unsigned int num_permutations) {
  nna_matrices_lu *r = malloc(sizeof(*r));
  NP_CHECK(r);
  r->L = L;
  r->U = U;
  r->P = P;
  r->num_permutations = num_permutations;
  return r;
}
void nna_matrices_lu_free(nna_matrices_lu* lu) {
  if (!lu) free(lu);
}

//
// Basic Row Operations
//

nna_matrix *nna_rem_col(nna_matrix *m, unsigned int column) {
  if(column >= m->num_cols) {
    NNA_FERROR(CANNOT_REMOVE_COLUMN, column, m->num_cols);
    return NULL;
  }
  nna_matrix *r = nna_new(m->num_rows, m->num_cols-1);
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

nna_matrix *nna_rem_row(nna_matrix *m, unsigned int row) {
  if (row >= m->num_rows) {
    NNA_FERROR(CANNOT_REMOVE_ROW, row, m->num_rows);
    return NULL;
  }
  nna_matrix *r = nna_new(m->num_rows-1, m->num_cols);
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

nna_matrix *nna_swap_rows(nna_matrix *m, unsigned int row1, unsigned int row2) {
  nna_matrix *r = nna_new_copy(m);
  if (!nna_swap_rows_r(r, row1, row2)) {
    nna_free(r);
    return NULL;
  }
  return r;
}

int nna_swap_rows_r(nna_matrix *m, unsigned int row1, unsigned int row2) {
  if (row1 >= m->num_rows || row2 >= m->num_rows) {
    NNA_FERROR(CANNOT_SWAP_ROWS, row1, row2, m->num_rows);
    return 0;
  }
  double *tmp = m->data[row2];
  m->data[row2] = m->data[row1];
  m->data[row1] = tmp;
  return 1;
}

nna_matrix *nna_multiply_row(nna_matrix *m, unsigned int row, double num) {
  nna_matrix *r = nna_new_copy(m);
  if (nna_multiply_row_r(r, row, num)) {
    nna_free(r);
    return NULL;
  }
  return r;
}

int nna_multiply_row_r(nna_matrix *m, unsigned int row, double num) {
  if (row>=m->num_rows) {
    NNA_FERROR(CANNOT_ROW_MULTIPLY, row, m->num_rows);
    return 0;
  }
  int i;
  for(i=0; i < m->num_cols; i++) {
    m->data[row][i] *= num;
  }
  return 0;
}

nna_matrix *nna_add_to_row(nna_matrix *m, unsigned int where, unsigned int row, double multiplier) {
  nna_matrix *r = nna_new_copy(m);
  if (!nna_add_to_row_r(m, where, row, multiplier)) {
    nna_free(r);
    return NULL;
  }
  return r;
}

int nna_add_to_row_r(nna_matrix *m, unsigned int where, unsigned int row, double multiplier) {
  if (where >= m->num_rows || row >= m->num_rows) {
    NNA_FERROR(CANNOT_ADD_TO_ROW, multiplier, row, where, m->num_rows);
    return 0;
  }
  int i = 0;
  for(i = 0; i < m->num_cols; i++) {
    m->data[where][i] += multiplier * m->data[row][i];
  }
  return 1;
}

//
// Matrix Operations
//

nna_matrix *nna_plus(nna_matrix *m1, nna_matrix *m2) {
  if (!nna_eq_dim(m1, m2)) {
    NNA_ERROR(CANNOT_ADD);
    return NULL;
  }
  int i, j;
  nna_matrix *r = nna_new(m1->num_rows, m1->num_cols);
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m2->num_rows; j++) {
      r->data[i][j] = m1->data[i][j] + m2->data[i][j];
    }
  }
  return r;
}

nna_matrix *nna_minus(nna_matrix *m1, nna_matrix *m2) {
  if (!nna_eq_dim(m1, m2)) {
    NNA_ERROR(CANNOT_SUBTRACT);
    return NULL;
  }
  int i, j;
  nna_matrix *r = nna_new(m1->num_rows, m1->num_cols);
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m2->num_rows; j++) {
      r->data[i][j] = m1->data[i][j] - m2->data[i][j];
    }
  }
  return r;
}

nna_matrix *nna_smultiply(nna_matrix *m, double num) {
  int i, j;
  nna_matrix *r = nna_new(m->num_rows, m->num_cols);
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      r->data[i][j] = m->data[i][j] * num;
    }
  }
  return r;
}

nna_matrix *nna_multiply(nna_matrix *m1, nna_matrix *m2) {
  if (!(m1->num_cols == m2->num_rows)) {
    NNA_ERROR(CANNOT_MULITPLY);
    return NULL;
  }
  int i, j, k;
  nna_matrix *r = nna_new(m1->num_rows, m2->num_cols);
  nna_set_all(r, 0.0);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      for(k = 0; k < m1->num_cols; k++) {
        r->data[i][j] += m1->data[i][k] * m2->data[k][j];
      }
    }
  }
  return r;
}

nna_matrix *nna_transpose(nna_matrix *m) {
  int i, j;
  nna_matrix *r = nna_new(m->num_cols, m->num_rows);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[j][i];
    }
  }
  return r;
}

double nna_trace(nna_matrix* m) {
  if (!m->is_square) {
    NNA_ERROR(CANNOT_TRACE);
  }
  int i;
  double trace = 0.0;
  for(i = 0; i < m->num_rows; i++) {
    trace += m->data[i][i];
  }
  return trace;
}

//
// LU Decomposition
//
int nna_absmax_row(nna_matrix *m, unsigned int k) {
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

nna_matrices_lu *nna_lup(nna_matrix *m) {
  if (!m->is_square) {
    NNA_FERROR(CANNOT_LU_MATRIX_SQUARE, m->num_rows, m-> num_cols);
    return NULL;
  }
  nna_matrix *L = nna_new(m->num_rows, m->num_rows);
  nna_matrix *U = nna_new_copy(m);
  nna_matrix *P = nna_new_identity(m->num_rows);

  int j,i, pivot;
  unsigned int num_permutations = 0;
  double mult;

  for(j = 0; j < U->num_cols; j++) {
    // Retrieves the row with the biggest element for column (j)
    pivot = nna_absmax_row(U, j);
    if (U->data[pivot][j] < DBL_EPSILON) {
      NNA_ERROR(CANNOT_LU_MATRIX_DEGENERATE);
      return NULL;
    }
    if (pivot!=j) {
      // Pivots LU and P accordingly to the rule
      nna_swap_rows_r(U, j, pivot);
      nna_swap_rows_r(L, j, pivot);
      nna_swap_rows_r(P, j, pivot);
      // Keep the number of permutations to easily calculate the
      // determinant sign afterwards
      num_permutations++;
    }
    for(i = j+1; i < U->num_rows; i++) {
      mult = U->data[i][j] / U->data[j][j];
      // Building the U upper rows
      nna_add_to_row_r(U, i, j, -mult);
      // Store the multiplier in L
      L->data[i][j] = mult;
    }
  }
  nna_set_diag(L, 1.0);

  return nna_matrices_lu_new(L, U, P, num_permutations);
}

// After the LU(P) factorisation the determinant can be easily calculated
// by multiplying the main diagonal of matrix U with the sign.
// the sign is -1 if the number of permutations is odd
// the sign is +1 if the number of permutations is even
double nna_det(nna_matrices_lu* lup) {
  int k;
  int sign = (lup->num_permutations%2==0) ? 1 : -1;
  nna_matrix *U = lup->U;
  double product = 1.0;
  for(k = 0; k < U->num_rows; k++) {
    product *= U->data[k][k];
  }
  return product * sign;
}

// After the LU(P) factorisation the determinant can be easily calculated
nna_matrix *nna_inverse(nna_matrices_lu *m) {
    return NULL;
}

//
// Solving Linear Systems
//

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
nna_matrix *nna_solve_ls_fwdsub(nna_matrix *L, nna_matrix *b) {
  nna_matrix* x = nna_new(L->num_cols, 1);
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
nna_matrix *nna_solve_ls_bcksub(nna_matrix *U, nna_matrix *b) {
  nna_matrix *x = nna_new(U->num_cols, 1);
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
nna_matrix *nna_solve_ls(nna_matrices_lu *lu, nna_matrix* b) {
  if (lu->U->num_rows != b->num_rows || b->num_cols != 1) {
    NNA_FERROR(CANNOT_SOLVE_LIN_SYS_INVALID_B,
      b->num_rows,
      b->num_cols,
      lu->U->num_rows,
      lu->U->num_cols);
      return NULL;
  }
  nna_matrix *Pb = nna_multiply(lu->P, b);

  // We solve L*y = P*b using forward substition
  nna_matrix *y = nna_solve_ls_fwdsub(lu->L, Pb);

  // We solve U*x=y
  nna_matrix *x = nna_solve_ls_bcksub(lu->U, y);

  nna_free(y);
  nna_free(Pb);
  return x;
}

int main(int argc, char *argv[]) {
  double MV[25] = {
    1.0, 2.0, 5.0, 0.0, 5.0,
    3.0, 4.0, 9.0, 7.0, 8.0,
    9.0, 8.0, 2.0, 8.0, 0.0,
    1.0, 2.5, 3.7, 3.0, 2.5,
    4.0, 8.0, 3.0, 1.0, 0.0
  };

  double B3[5] = {
    8.00,
    3.00,
    7.00,
    9.00,
    8.00
  };

  nna_matrix *M = nna_new_from(5, 5, 25, MV);
  printf("\nM =\n");
  nna_print(M);

  nna_matrix *b3 = nna_new_from(5, 1, 5, B3);
  printf("\nb3 =\n");
  nna_print(b3);

  nna_matrices_lu *M_LUP = nna_lup(M);
  printf("\nL=\n");
  nna_print(M_LUP->L);
  printf("\nU =\n");
  nna_print(M_LUP->U);
  printf("\nP =\n");
  nna_print(M_LUP->P);

  nna_matrix *x3 = nna_solve_ls(M_LUP, b3);
  printf("\nx3 =\n");
  nna_print(x3);

  return 0;
}
