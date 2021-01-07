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


nna_matrix_lu *nna_matrix_lu_new(nna_matrix *L, nna_matrix *U, nna_matrix *P, unsigned int num_permutations) {
  nna_matrix_lu *r = malloc(sizeof(*r));
  NP_CHECK(r);
  r->L = L;
  r->U = U;
  r->P = P;
  r->num_permutations = num_permutations;
  return r;
}
void nna_matrix_lu_free(nna_matrix_lu* lu) {
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

nna_matrix_lu *nna_lup(nna_matrix *m) {
  if (!m->is_square) {
    NNA_FERROR(CANNOT_LU_MATRIX_SQUARE, m->num_rows, m-> num_cols);
    return NULL;
  }
  nna_matrix *L = nna_new_identity(m->num_rows);
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

  return nna_matrix_lu_new(L, U, P, num_permutations);
}

// After the LUP factorisation the determinant can be easily calculated
// by multiplying the main diagonal of matrix U with the sign.
// the sign is -1 if the number of permutations is odd
// the sign is +1 if the number of permutations is even
double nna_det(nna_matrix_lu* lup) {
  int k;
  int sign = (lup->num_permutations%2==0) ? 1 : -1;
  nna_matrix *U = lup->U;
  double product = 1.0;
  for(k = 0; k < U->num_rows; k++) {
    product *= U->data[k][k];
  }
  return product * sign;
}

int main(int argc, char *argv[]) {
  double mv[16] = {
    2.0, 7.0, 6.0, 1.0,
    9.0, 5.0, 1.0, 2.0,
    4.0, 3.0, 8.0, 3.0,
    0.0, 3.0, 0.0, 1.0
  };

  nna_matrix *m = nna_new_from(4,4,16,mv);
  nna_print(m);
  nna_matrix_lu *lup = nna_lup(m);
  nna_print(lup->U);
  nna_print(lup->L);
  nna_print(lup->P);
  printf("D=%2.2f\n",nna_det(lup));

  return 0;
}
