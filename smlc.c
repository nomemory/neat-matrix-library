#include <stdio.h>
#include <stdlib.h>

#include "smlc.h"

#define DEFAULT_VALUE 0.0

#define CANNOT_ADD \
  "Cannot add two matrices with different dimensions." \

#define CANNOT_SUBTRACT \
  "Cannot subctract two matrices with different dimensions." \

#define CANNOT_MULITPLY \
  "Cannot multiply two matrices where \
  the number of columns of the first one \
  is different than the number of rows of the second one." \

#define CANNOT_REMOVE_COLUMN \
  "Cannot remove matrix column %d. The value should be less than %d.\n" \

#define CANNOT_REMOVE_ROW \
  "Cannot remove matrix row %d. The value should be less than %d.\n" \

#define INVALID_ROWS \
  "Cannot create matrix with 0 number of rows. Aborting." \

#define INVALID_COLS \
    "Cannot create matrix with 0 number of cols. Aborting." \

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
  smlc_matrix *m = malloc(sizeof(*m));
  NP_CHECK(m);
  m->num_rows = num_rows;
  m->num_cols = num_cols;
  m->is_square = (num_rows == num_cols) ? 1 : 0;
  m->data = malloc(sizeof(*m->data) * m->num_rows);
  NP_CHECK(m->data);
  int i;
  for(i = 0; i < m->num_rows; ++i) {
    m->data[i] = malloc(sizeof(**m->data) * m->num_cols);
    NP_CHECK(m->data[i]);
  }
  return m;
}

smlc_matrix *smlc_new_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals) {
  smlc_matrix * m = smlc_new(num_rows, num_cols);
  int i, j, v_idx;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      v_idx = i * m->num_cols + j;
      m->data[i][j] = (v_idx < n_vals) ? vals[v_idx] : 0.0;
    }
  }
  return m;
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

void smlc_print(smlc_matrix *matrix, const char *d_fmt) {
  int i, j;
  for(i = 0; i < matrix->num_rows; ++i) {
    for(j = 0; j < matrix->num_cols; ++j) {
      fprintf(stdout, d_fmt, matrix->data[i][j]);
    }
    fprintf(stdout, "\n");
  }
}

// Checks if two matrices have the same dimesions
int smlc_eq_dim(smlc_matrix *m1, smlc_matrix *m2) {
  return (m1->num_cols == m2->num_cols) &&
          (m1->num_rows == m2->num_rows);
}

//
// Structural Changes
//
// smlc_matrix *smlc_add_col(smlc_matrix *m, unsigned int size, double *values);
// smlc_matrix *smlc_add_row(smlc_matrix *m, unsigned int size, double *values);

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

int main(int argc, char *argv[]) {

  // double m1_v[9] = {
  //   1.0, 2.0, 1.0,
  //   4.0, 7.0, 8.0,
  //   3.0, 1.0, 8.0
  // };
  //
  // double m2_v[9] = {
  //   1.0, 2.0, 1.0,
  //   4.0, 7.0, 8.0,
  //   3.0, 1.0, 8.0
  // };
  //
  // smlc_matrix *m1 = smlc_new_from(3,3,9,m1_v);
  // smlc_matrix *m2 = smlc_new_from(3,3,9,m2_v);
  //
  // smlc_print(m1,"%2.2f\t");
  //
  // printf("\n + \n\n");
  //
  // smlc_print(m2, "%2.2f\t");
  //
  // printf("\n=\n\n");
  //
  // smlc_print(smlc_plus(m1,m2), "%2.2f\t");
  //
  // printf("\n\n");
  // smlc_print(smlc_smultiply(m1, 5.0), "%2.2f\t");
  //
  // double m3_v[8] = {
  //   2.0, 1.0,
  //   8.2, 8.3,
  //   7.1, 7.2,
  //   2.0, 2.5
  // };
  //
  // smlc_matrix *m3 = smlc_new_from(4,2,8,m3_v);
  //
  // printf("\n\n");
  // smlc_print(m3,"%2.2f\t");
  //
  // printf("\n\n");
  // smlc_print(smlc_transpose(m3), "%2.2f\t");
  //

  double m4_v[6] = {
    2.0, 3.0, 4.0,
    1.0, 0.0, 0.0
  };

  //
  // double m5_v[6] = {
  //   0.0, 1000,
  //   1, 100,
  //   0, 10
  // };
  //
  // smlc_matrix *m4 = smlc_new_from(2,3,6,m4_v);
  // printf("\n\n");
  // smlc_print(m4, "%2.2f\t");
  //
  // smlc_matrix *m5 = smlc_new_from(3,2,6,m5_v);
  // printf("\n\n");
  // smlc_print(m5, "%2.2f\t");
  //
  // printf("\n\n");
  //
  // smlc_print(smlc_multiply(m4, m5), "%2.2f\t");

  smlc_matrix *m4 = smlc_new_from(2,3,6,m4_v);

  smlc_print(m4, "%2.2f\t");
  printf("\n\n");
  smlc_print(smlc_rem_col(m4, 0), "%2.2f\t");
  printf("\n\n");
  smlc_print(smlc_rem_col(m4, 1), "%2.2f\t");
  printf("\n\n");
  smlc_print(smlc_rem_col(m4, 2), "%2.2f\t");
  printf("\n\n");
  smlc_print(smlc_rem_row(m4, 0), "%2.2f\t");
  printf("\n\n");
  smlc_print(smlc_rem_row(m4, 1), "%2.2f\t");

  return 0;
}
