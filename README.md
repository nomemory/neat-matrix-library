**nml** is simple [matrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)) and [linear algebra](https://en.wikipedia.org/wiki/Linear_algebra) library written in pure C.

The library was written with focus on code readability in mind, and less on performance considerations.

It currently supports:
* Basic Matrix Operations;
* LUP Decomposition;
* Inverse and Determinant computation
* Solving linear systems;

It's currently work in progress, more algorithms are being added.

# Compile

```
make all
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
