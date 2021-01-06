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

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "smlc.h"

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


//
// Basic Matrix Methods
//
smlc_matrix *smlc_new(unsigned int num_rows, unsigned int num_cols) {
  if (num_rows == 0) {
    SMLC_ERROR(INVALID_ROWS);
    abort();
  }
  if (num_cols == 0) {
    SMLC_ERROR(INVALID_COLS);
    abort();
  }
  smlc_matrix *m = calloc(1, sizeof(*m));
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

smlc_matrix *smlc_new_identity(unsigned int size) {
  smlc_matrix *r = smlc_new(size, size);
  int i;
  for(i = 0; i < r->num_rows; i++) {
    r->data[i][i] = 1.0;
  }
  return r;
}

smlc_matrix *smlc_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals) {
  smlc_matrix *m = smlc_new(num_rows, num_cols);
  int i, j, v_idx;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      v_idx = i * m->num_cols + j;
      m->data[i][j] = (v_idx < n_vals) ? vals[v_idx] : 0.0;
    }
  }
  return m;
}

smlc_matrix *smlc_new_copy(smlc_matrix *m) {
  smlc_matrix *r  = smlc_new(m->num_rows, m->num_cols);
  int i,j;
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[i][j];
    }
  }
  return r;
}

void smlc_free(smlc_matrix *matrix) {
  int i;
  for(i = 0; i < matrix->num_rows; ++i) {
    free(matrix->data[i]);
  }
  free(matrix->data);
}

double smlc_get(smlc_matrix *matrix, unsigned int i, unsigned int j) {
  return matrix->data[i][j];
}

void smlc_set(smlc_matrix *matrix, unsigned int i, unsigned int j, double value) {
  matrix->data[i][j] = value;
}


void smlc_set_all(smlc_matrix *matrix, double value) {
  int i, j;
  for(i = 0; i < matrix->num_rows; i++) {
    for(j = 0; j < matrix->num_cols; j++) {
      matrix->data[i][j] = value;
    }
  }
}

void smlc_print(smlc_matrix *matrix) {
  smlc_printf(matrix, "%2.2f\t");
}

void smlc_printf(smlc_matrix *matrix, const char *d_fmt) {
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
int smlc_eq_dim(smlc_matrix *m1, smlc_matrix *m2) {
  return (m1->num_cols == m2->num_cols) &&
          (m1->num_rows == m2->num_rows);
}

//
// Matrix LU
//


smlc_matrix_lu *smlc_matrix_lu_new(smlc_matrix *L, smlc_matrix *U, smlc_matrix *P) {
  smlc_matrix_lu *r = malloc(sizeof(*r));
  NP_CHECK(r);
  r->L = L;
  r->U = U;
  r->P = P;
  return r;
}
void smlc_matrix_lu_free(smlc_matrix_lu* lu) {
  if (!lu) free(lu);
}

//
// Basic Row Operations
//

smlc_matrix *smlc_rem_col(smlc_matrix *m, unsigned int column) {
  if(column >= m->num_cols) {
    SMLC_FERROR(CANNOT_REMOVE_COLUMN, column, m->num_cols);
    return NULL;
  }
  smlc_matrix *r = smlc_new(m->num_rows, m->num_cols-1);
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

smlc_matrix *smlc_rem_row(smlc_matrix *m, unsigned int row) {
  if (row >= m->num_rows) {
    SMLC_FERROR(CANNOT_REMOVE_ROW, row, m->num_rows);
    return NULL;
  }
  smlc_matrix *r = smlc_new(m->num_rows-1, m->num_cols);
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

smlc_matrix *smlc_swap_rows(smlc_matrix *m, unsigned int row1, unsigned int row2) {
  smlc_matrix *r = smlc_new_copy(m);
  if (!smlc_swap_rows_ip(r, row1, row2)) {
    smlc_free(r);
    return NULL;
  }
  return r;
}

int smlc_swap_rows_ip(smlc_matrix *m, unsigned int row1, unsigned int row2) {
  if (row1 >= m->num_rows || row2 >= m->num_rows) {
    SMLC_FERROR(CANNOT_SWAP_ROWS, row1, row2, m->num_rows);
    return 0;
  }
  double *tmp = m->data[row2];
  m->data[row2] = m->data[row1];
  m->data[row1] = tmp;
  return 1;
}

smlc_matrix *smlc_multiply_row(smlc_matrix *m, unsigned int row, double num) {
  smlc_matrix *r = smlc_new_copy(m);
  if (smlc_multiply_row_ip(r, row, num)) {
    smlc_free(r);
    return NULL;
  }
  return r;
}

int smlc_multiply_row_ip(smlc_matrix *m, unsigned int row, double num) {
  if (row>=m->num_rows) {
    SMLC_FERROR(CANNOT_ROW_MULTIPLY, row, m->num_rows);
    return 0;
  }
  int i;
  for(i=0; i < m->num_cols; i++) {
    m->data[row][i] *= num;
  }
  return 0;
}

smlc_matrix *smlc_add_to_row(smlc_matrix *m, unsigned int where, unsigned int row, double multiplier) {
  smlc_matrix *r = smlc_new_copy(m);
  if (!smlc_add_to_row_ip(m, where, row, multiplier)) {
    smlc_free(r);
    return NULL;
  }
  return r;
}

int smlc_add_to_row_ip(smlc_matrix *m, unsigned int where, unsigned int row, double multiplier) {
  if (where >= m->num_rows || row >= m->num_rows) {
    SMLC_FERROR(CANNOT_ADD_TO_ROW, multiplier, row, where, m->num_rows);
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

smlc_matrix *smlc_plus(smlc_matrix *m1, smlc_matrix *m2) {
  if (!smlc_eq_dim(m1, m2)) {
    SMLC_ERROR(CANNOT_ADD);
    return NULL;
  }
  int i, j;
  smlc_matrix *r = smlc_new(m1->num_rows, m1->num_cols);
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m2->num_rows; j++) {
      r->data[i][j] = m1->data[i][j] + m2->data[i][j];
    }
  }
  return r;
}

smlc_matrix *smlc_minus(smlc_matrix *m1, smlc_matrix *m2) {
  if (!smlc_eq_dim(m1, m2)) {
    SMLC_ERROR(CANNOT_SUBTRACT);
    return NULL;
  }
  int i, j;
  smlc_matrix *r = smlc_new(m1->num_rows, m1->num_cols);
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m2->num_rows; j++) {
      r->data[i][j] = m1->data[i][j] - m2->data[i][j];
    }
  }
  return r;
}

smlc_matrix *smlc_smultiply(smlc_matrix *m, double num) {
  int i, j;
  smlc_matrix *r = smlc_new(m->num_rows, m->num_cols);
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      r->data[i][j] = m->data[i][j] * num;
    }
  }
  return r;
}

smlc_matrix *smlc_multiply(smlc_matrix *m1, smlc_matrix *m2) {
  if (!(m1->num_cols == m2->num_rows)) {
    SMLC_ERROR(CANNOT_MULITPLY);
    return NULL;
  }
  int i, j, k;
  smlc_matrix *r = smlc_new(m1->num_rows, m2->num_cols);
  smlc_set_all(r, 0.0);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      for(k = 0; k < m1->num_cols; k++) {
        r->data[i][j] += m1->data[i][k] * m2->data[k][j];
      }
    }
  }
  return r;
}

smlc_matrix *smlc_transpose(smlc_matrix *m) {
  int i, j;
  smlc_matrix *r = smlc_new(m->num_cols, m->num_rows);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[j][i];
    }
  }
  return r;
}

double smlc_trace(smlc_matrix* m) {
  if (!m->is_square) {
    SMLC_ERROR(CANNOT_TRACE);
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
int smlc_absmax_row(smlc_matrix *m, unsigned int k) {
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

smlc_matrix_lu *smlc_lup(smlc_matrix *m) {
  if (!m->is_square) {
    SMLC_FERROR(CANNOT_LU_MATRIX_SQUARE, m->num_rows, m-> num_cols);
    return NULL;
  }
  smlc_matrix *L = smlc_new_identity(m->num_rows);
  smlc_matrix *U = smlc_new_copy(m);
  smlc_matrix *P = smlc_new_identity(m->num_rows);

  int j,i, pivot;
  double mult;

  for(j = 0; j < U->num_cols; j++) {
    // Retrieves the row with the biggest element for column (j)
    pivot = smlc_absmax_row(U, j);
    if (pivot!=j) {
      // Pivots LU and P accordingly to the rule
      smlc_swap_rows_ip(U, j, pivot);
      smlc_swap_rows_ip(P, j, pivot);
    }
    for(i = j+1; i < U->num_rows; i++) {
      mult = U->data[i][j] / U->data[j][j];
      smlc_add_to_row_ip(U, i, j, -mult);
      L->data[i][j] = mult;
    }
  }

  return smlc_matrix_lu_new(L, U, P);
}

int main(int argc, char *argv[]) {
  double mv[9] = {
    2.0, 1.0, 5.0,
    4.0, 4.0, -4.0,
    1.0, 3.0, 1.0
  };

  smlc_matrix *m = smlc_new_from(3,3,9,mv);
  smlc_print(m);
  smlc_matrix_lu *lup = smlc_lup(m);
  smlc_print(lup->U);
  smlc_print(lup->L);
  smlc_print(lup->P);

  return 0;
}
