**nml** is simple [matrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)) and [linear algebra](https://en.wikipedia.org/wiki/Linear_algebra) library written in pure C.

It currently supports:
* Basic Matrix Operations;
* [LUP Decomposition](https://en.wikipedia.org/wiki/LU_decomposition);
* Inverse and Determinant computation
* Solving [linear systems of equations](https://en.wikipedia.org/wiki/System_of_linear_equations);

The library is still under heavy development.
Documentation is under heavy editing.

# Compile / Run Examples

The build file for the library it's called `nml.sh`. It's actually a `bash` script (not a `makefile`!).

## Building the library

```bash
./nml.sh build
```

This will compile the library, create a `dist` folder where you will find `*.a` static library file and the header files.

`gcc` and `ar` should be available in `$PATH`.

## Building the examples

Examples can be found in the [`./examples` folder](https://github.com/nomemory/neat-matrix-library/tree/main/examples).

To build the code:

```bash
./nml.sh examples
```

1. This will create an `examples/lib` folder where the `libnml.a` and the header files will be copied;
2. The `examples/*.c` will be compiled with the latest version of `libnml`;
3. For each `examples/*.c` an executable (`*.ex`) will be created.

## Cleaning

```bash
./nml.sh clean
```

This will clean everything (`*.o`,`*.ex`,`*.a`) and will leave the library folder in a clean state.

# Suported methods



```c 
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
// Matrix Construction / Destruction
//
// *****************************************************************************

// Dynamically allocates memory for a new matrix struct
// All values are initialised with zero (0.0)
nml_mat *nml_mat_new(unsigned int num_rows, unsigned int num_cols);

// Dynamically allocates memory a new matrix struct
// All values are initialised with random values in the [min, max) interval
// call srand(time(NULL)); once before using this
nml_mat *nml_mat_new_rnd(unsigned int num_rows, unsigned int num_cols, double min, double max);

// Dynamically allocates memory for a new square matrix (num_rows = num_cols)
// All values are initialised with zero (0.0)
nml_mat *nml_mat_sqr(unsigned int size);

// Dynamically allocates memory for a new squqare matrix (num_rows = num_cols)
// All values are initialised with random values in the [min, max] interval
// call srand(time(NULL)); once before using this
nml_mat *nml_mat_sqr_rnd(unsigned int size, double min, double max);

// Dynamically allocates memory for an Identity (I) matrix 
nml_mat *nml_mat_id(unsigned int size);

// Copies all the content from an existing matrix to a new matrix
nml_mat *nml_mat_cp(nml_mat *m);

// Creates a matrix from an array of values
nml_mat *nml_mat_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);

// Frees the matrix struct object
void nml_mat_free(nml_mat *matrix);

// *****************************************************************************
//
// Matrix Equality
//
// *****************************************************************************

// 1 if m2 and m1 have the same dimensions
// 0 otherwise
int nml_mat_eqdim(nml_mat *m1, nml_mat *m2);

// 1 if (m2 and m1 have the same dimensions) and (the same values)
// 0 oterwise
int nml_mat_eq(nml_mat *m1, nml_mat *m2, double tolerance);

// *****************************************************************************
//
// Matrix pretty printing
//
// *****************************************************************************

// Prints a matrix to stdout
void nml_mat_print(nml_mat *matrix);

// Prints a matrix to stdout using a custom format for the element
// E.g.: d_fmt = "%e\t"
void nml_mat_printf(nml_mat *matrix, const char *d_fmt);

// *****************************************************************************
//
// Accessing and modifying matrix elements
//
// *****************************************************************************
double nml_mat_get(nml_mat *matrix, unsigned int i, unsigned int j);

// Retrieves the column matrix of a given matrix 
nml_mat *nml_mat_getcol(nml_mat *m, unsigned int col);

// Retrieves the row matrix of a given matrix 
nml_mat *nml_mat_getrow(nml_mat *m, unsigned int row);

void nml_mat_set(nml_mat *matrix, unsigned int i, unsigned int j, double value);

// Sets all values of the matrix to a given value
void nml_mat_setall(nml_mat *matrix, double value);

// Sets all the values from the first diagonal to a given value
int nml_mat_setdiag(nml_mat *matrix, double value);

// Multiplies all elements from a row with scalar
// Returns a new matrix 
nml_mat *nml_mat_multrow(nml_mat *m, unsigned int row, double num);

// Multuplies all elemenets from a row with scalar
// Changes are done on the m matrix through reference
int nml_mat_multrow_r(nml_mat *m, unsigned int row, double num);

// Adds all elements from a row to another row so that:
// m->data[where][*] += multiplier * m->data[row][*]
// Returns a new matrix
nml_mat *nml_mat_rowplusrow(nml_mat *m, unsigned int where, unsigned int row, double multiplier);

// Adds all elements from a row to another row so that:
// m->data[where][*] += multiplier * m->data[row][*]
// Changes are done on the m matrix through reference
int nml_mat_rowplusrow_r(nml_mat *m, unsigned int where, unsigned int row, double multiplier);

// Multiplies all elements from a matrix with scalar
// Returns a new matrix
nml_mat *nml_mat_scalarmult(nml_mat *m, double num);

// Multiplies all elements from a matrix with scalar
// Changes are done on the m matrix through reference
int nml_mat_scalarmult_r(nml_mat *m, double num);

// *****************************************************************************
//
// Modifying the matrix structure
//
// *****************************************************************************

// Removes a column from the matrix
// Returns a new matrix
nml_mat *nml_mat_remcol(nml_mat *m, unsigned int column);

// Removes a row from the matrix
// Returns a new matrix
nml_mat *nml_mat_remrow(nml_mat *m, unsigned int row);

// Swaps two rows inside the matrix
// Returns a new matrix
nml_mat *nml_mat_swaprows(nml_mat *m, unsigned int row1, unsigned int row2);

// Swaps two rows inside the matrix
// Changes are done on the m matrix through reference
int nml_mat_swaprows_r(nml_mat *m, unsigned int row1, unsigned int row2);

// Swaps two columns inside the matrix
// Returns a new matrix
nml_mat *nml_mat_swapcols(nml_mat *m, unsigned int col1, unsigned int col2);

// Swaps two rows inside the matrix
// Changes are done on the m matrix through reference
int nml_mat_swapcols_r(nml_mat *m, unsigned int col1, unsigned int col2);

// Horizontally concatenates matrices
// Returns a new matrix
nml_mat *nml_mat_concath(unsigned int mnun, nml_mat **matrices);
nml_mat *nml_mat_concath_va(unsigned int mnum, ...);

// Horizontally concatenates matrices
// Returns a new matrix
nml_mat *nml_mat_concatv(unsigned int mnum, nml_mat **matrices);
nml_mat *nml_mat_concatv_va(unsigned int mnum, ...);

// *****************************************************************************
//
// Matrix Operations
//
// *****************************************************************************

// A + B = C 
// Returns C
nml_mat *nml_mat_add(nml_mat *m1, nml_mat *m2);

// A + B
// Changes are kept in A (no new matrix is allocated)
int nml_mat_add_r(nml_mat *m1, nml_mat *m2);

// A - B = C
// Returns C
nml_mat *nml_mat_sub(nml_mat *m1, nml_mat *m2);

// A - B
// Changes are kept in A
int nml_mat_sub_r(nml_mat *m1, nml_mat *m2);

// A * B
nml_mat *nml_mat_mult(nml_mat *m1, nml_mat *m2);

// Transpose A
nml_mat *nml_mat_trs(nml_mat *m);

// Trace A
double nml_mat_trace(nml_mat* m);

// *****************************************************************************
//
// Row Echelon
//
// *****************************************************************************

// Returns the Row Echelon Form of the Matrix A (using Guassian Elimination)
nml_mat *nml_mat_ref(nml_mat *m);

// Returns the Row Echelon form of Matrix A (using Gauss-Jordan)
nml_mat *nml_mat_rref(nml_mat *m);

// *****************************************************************************
//
// LUP Decomposition
//
// *****************************************************************************

nml_mat_lup *nml_mat_lup_new(nml_mat *L, nml_mat *U, nml_mat *P, unsigned int num_permutations);

// If possible decompose a Matrix in LU
// P * A = L * U
// Where P is row permutation of I (identity matrix)
// L - lower triangular matrix (with 1s on the first diagonal)
// U - upper triangular matrix
nml_mat_lup *nml_mat_lup_solve(nml_mat *m);

// Deallocates memory
void nml_mat_lup_free(nml_mat_lup* lu);

// Computes determinant(A) using the LU decomposition of the matrix
double nml_mat_determinant(nml_mat_lup* lup);

// Computes the LU matrix from L and U
nml_mat *nml_mat_getlu(nml_mat_lup* lup);

// Calculates the inverse matrix of A
nml_mat *nml_mat_inverse(nml_mat_lup *m);

// *****************************************************************************
//
// Solving linear systems of equations
//
// *****************************************************************************

// Solves a linear system of equations as long as the matrix is lower triangular 
// uses forward substitution
nml_mat *nml_ls_solvefwd(nml_mat *low_triang, nml_mat *b);

// Solves a linear system of equations as long as the matrix is upper triangular
// uses backward subsistution
nml_mat *nml_ls_solvebck(nml_mat *upper_triang, nml_mat *b);

// Solves a lineary systems of equations using LU(P) decomposition of the matrix m
nml_mat *nml_ls_solve(nml_mat_lup *lup, nml_mat* b);

#endif
```


# Short Example:

```c
#include <stdlib>
#include <stdio.h>
#include "nml.h"

int main(int argc, char *argv[]) {
  double A_v[25] = {
    1.0, 2.0, 5.0, 0.0, 5.0,
    3.0, 4.0, 9.0, 7.0, 8.0,
    9.0, 8.0, 2.0, 8.0, 0.0,
    1.0, 2.5, 3.7, 3.0, 2.5,
    4.0, 8.0, 3.0, 1.0, 0.0
  };

  double b_v[5] = {
    8.00,
    3.00,
    7.00,
    9.00,
    8.00
  };

  // The initial matrix
  nml_mat *A = nml_mat_from(5, 5, 25, A_v);
  // The b = vector of solutions
  nml_mat *b = nml_mat_from(5, 1, 5, b_v);
  // LUP Decomposition (P*A = L*U)
  nml_mat_lup *A_LUP = nml_mat_lup_solve(A);
  // Computes the determinant of A
  double d = nml_mat_det(A_LUP);
  // Solves A*x=b using LUP
  nml_mat *x = nml_solve_ls(A_LUP, b);


  // Print everything
  printf("\nA[%dx%d]:\n", A->num_rows, A->num_cols);
  nml_mat_print(A);
  printf("\nb[%dx%d]:\n", b->num_rows, b->num_cols);
  nml_mat_print(b);
  printf("\nL[%dx%d]:\n", A_LUP->L->num_rows, A_LUP->L->num_cols);
  nml_mat_print(A_LUP->L);
  printf("\nU[%dx%d] =\n", A_LUP->U->num_rows, A_LUP->U->num_cols);
  nml_mat_print(A_LUP->U);
  printf("\nP[%dx%d]=\n", A_LUP->P->num_rows, A_LUP->P->num_cols);
  nml_mat_print(A_LUP->P);
  printf("\nd=%2.2f\n", d);
  printf("\nx[%d][%d] =\n", x->num_rows, x->num_cols);
  nml_mat_print(x);

  nml_mat_lup_free(A_LUP);
  nml_mat_free(A);
  nml_mat_free(b);
  nml_mat_free(x);

  return 0;
}
```

Output:

```
A[5x5]:

1.00	2.00	5.00	0.00	5.00
3.00	4.00	9.00	7.00	8.00
9.00	8.00	2.00	8.00	0.00
1.00	2.50	3.70	3.00	2.50
4.00	8.00	3.00	1.00	0.00


b[5x1]:

8.00
3.00
7.00
9.00
8.00


L[5x5]:

1.00	0.00	0.00	0.00	0.00
0.44	1.00	0.00	0.00	0.00
0.33	0.30	1.00	0.00	0.00
0.11	0.36	0.35	1.00	0.00
0.11	0.25	0.55	-2.47	1.00


U[5x5] =

9.00	8.00	2.00	8.00	0.00
0.00	4.44	2.11	-2.56	0.00
0.00	0.00	7.70	5.10	8.00
0.00	0.00	0.00	1.24	-0.32
0.00	0.00	0.00	0.00	-0.20


P[5x5]=

0.00	0.00	1.00	0.00	0.00
0.00	0.00	0.00	0.00	1.00
0.00	1.00	0.00	0.00	0.00
0.00	0.00	0.00	1.00	0.00
1.00	0.00	0.00	0.00	0.00


d=77.00

x[5][1] =

60.25
-76.65
134.71
-23.94
-114.50
```
