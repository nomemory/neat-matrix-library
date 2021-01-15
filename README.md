```
 .----------------.  .----------------.  .----------------.  .-----------------. .----------------.  .----------------. 
| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
| |   _____      | || |     _____    | || |   ______     | || | ____  _____  | || | ____    ____ | || |   _____      | |
| |  |_   _|     | || |    |_   _|   | || |  |_   _ \    | || ||_   \|_   _| | || ||_   \  /   _|| || |  |_   _|     | |
| |    | |       | || |      | |     | || |    | |_) |   | || |  |   \ | |   | || |  |   \/   |  | || |    | |       | |
| |    | |   _   | || |      | |     | || |    |  __'.   | || |  | |\ \| |   | || |  | |\  /| |  | || |    | |   _   | |
| |   _| |__/ |  | || |     _| |_    | || |   _| |__) |  | || | _| |_\   |_  | || | _| |_\/_| |_ | || |   _| |__/ |  | |
| |  |________|  | || |    |_____|   | || |  |_______/   | || ||_____|\____| | || ||_____||_____|| || |  |________|  | |
| |              | || |              | || |              | || |              | || |              | || |              | |
| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
 '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' 
```

**nml** is a simple [matrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)) and [linear algebra](https://en.wikipedia.org/wiki/Linear_algebra) library written in standard C. Code should be portable and there are no dependencies.

It currently supports:
* Basic Matrix Operations (row swaps, colum swaps, multiplication, addition, etc.)
* [LU Decomposition](https://en.wikipedia.org/wiki/LU_decomposition);
* Inverse(A);
* Determinant(A) 
* Solving [linear systems of equations](https://en.wikipedia.org/wiki/System_of_linear_equations);
* [Row Echelon Form](https://en.wikipedia.org/wiki/Gaussian_elimination);
* Reduced Row Echelon Form ([Gauss-Jordan](https://brilliant.org/wiki/gaussian-elimination/));

The library is still under development, but a few thousands test cases are already implemented.

Next feature:

* Implement [QR decomposition](https://en.wikipedia.org/wiki/QR_decomposition)

Documentation is under construction.

Table of Contents
=================

   * [Compile / Run Examples](#compile--run-examples)
      * [Building the library](#building-the-library)
      * [Building the examples](#building-the-examples)
      * [Running the tests](#running-the-tests)
      * [Cleaning](#cleaning)
   * [How to use the library](#how-to-use-the-library)
      * [Creating matrices](#creating-matrices)
         * [Creating a new Matrix](#creating-a-new-matrix)
         * [Creating a marray from an array (double[N])](#creating-a-marray-from-an-array-doublen)
         * [Creating a Matrix from an external file](#creating-a-matrix-from-an-external-file)
         * [Creating a matrix from user input](#creating-a-matrix-from-user-input)
         * [Creating randomized matrices](#creating-randomized-matrices)
      * [Check if two matrices are equal](#check-if-two-matrices-are-equal)
      * [Accesing and modifying matrix elements](#accesing-and-modifying-matrix-elements)
         * [Select rows and columns](#select-rows-and-columns)
         * [Set all elements to a value](#set-all-elements-to-a-value)
         * [Set the first diagonal to a value](#set-the-first-diagonal-to-a-value)
         * [Scalar multiply the matrix](#scalar-multiply-the-matrix)
         * [Multiply rows](#multiply-rows)
         * [Add rows](#add-rows)
      * [Modifying the matrix structure](#modifying-the-matrix-structure)
         * [Remove rows and columns](#remove-rows-and-columns)
         * [Swap rows and columns](#swap-rows-and-columns)
         * [Concatenate matrices](#concatenate-matrices)
      * [Matrices operations](#matrices-operations)
         * [Add and subtract matrices](#add-and-subtract-matrices)
         * [Multiply matrices (dot)](#multiply-matrices-dot)
      * [Transpose matrices](#transpose-matrices)
      * [Calculate trace](#calculate-trace)
      * [Row Echelon](#row-echelon)
         * [Calculate Row Echelon Form using Gaussian Elimination](#calculate-row-echelon-form-using-gaussian-elimination)
         * [Calculate Reduced Row Echelon Form using Gauss-Jordan](#calculate-reduced-row-echelon-form-using-gauss-jordan)
      * [LU(P) Decomposition](#lup-decomposition)
      * [Matrix inverse](#matrix-inverse)
      * [Matrix determinant](#matrix-determinant)
      * [Solve linear systems of equations](#solve-linear-systems-of-equations)
      
# Compile / Run Examples

The build file for the library it's called `nml.sh`. 
It's actually a `bash` script (not a `makefile`!).

## Building the library

```bash
./nml.sh clean build
```

This will compile the library, create a `dist` folder where you will find `*.a` static library file and the header files.

`gcc` and `ar` should be available in `$PATH`.

If you want to use the `clang` compiler instead of `gcc` you need to manually edit the `./nml.sh` file, changing the variable `CC` from `gcc` to `clang`. 
Nothing else should be changed.

```bash
# COMPILING RELATED
CC=clang #<----------------- here
CCFLAGS="-Wall -c"
CCFLAGS_EXAMPLES="-Wall"
```

## Building the examples

Examples can be found in the [`./examples` folder](https://github.com/nomemory/neat-matrix-library/tree/main/examples).

To build the code examples:

```bash
./nml.sh clean examples
```

1. This will create an `examples/lib` folder where the `libnml.a` and the header files will be copied;
2. The `examples/*.c` will be compiled with the latest version of `libnml`;
3. For each `examples/*.c` an executable (`*.ex`) will be created.

To run an example:

```bash
# ./nml.sh clean examples && ./examples/<example name>.ex
./nml.sh clean examples && ./examples/playground.ex
```

## Running the tests 

To run the tests 

```bash
./nml.sh clean test
```

1. This will create a `test/lib` folder where the `libnml.a` and the header files will be copied;
2. Each test `tests/*.c` will be compiled with the latest version of `libnml`;
3. For each test `tests/*/c` an executable (`*.ex`) will be created.

The test data was generated using [sympy](https://docs.sympy.org/). 
In the `tests/generators/` folder you can find the python3 (`.py`) scripts used to generate the data.

## Cleaning

```bash
./nml.sh clean
```

This will clean everything (`*.o`,`*.ex`,`*.a`) and will leave the library folder in a clean state.

# How to use the library

A few examples can be found in the [`./examples` folder](https://github.com/nomemory/neat-matrix-library/tree/main/examples) folder.

## Creating matrices

### Creating a new Matrix

The methods for a creating a new matrix are:
* `nml_mat *nml_mat_new(unsigned int num_rows, unsigned int num_cols)`
  - Creates a `num_rows * num_cols` matrix of zeroes. 
* `nml_mat *nml_mat_sqr(unsigned int size)`
  - Creates a square `size * size` matrix  of zeroes.
* `nml_mat *nml_mat_eye(unsigned int size)`
  - Creates an identity `size * size` matrix.
* `nml_mat *nml_mat_cp(nml_mat *m)`
  - Returns a new identitcal copy of matrix `m`.
  
Everytime we create a matrix, we dynamically allocate memory. 
To free the memory please use: `nml_mat_free(nml_mat *m)`.  

```c
#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {


  nml_mat* m, *mx;

  printf("\nCreating an empty matrix with 2x3\n");
  m = nml_mat_new(2,3);
  nml_mat_print(m);
  nml_mat_free(m);

  printf("\nCreating a square matrix 5x5 \n");
  m = nml_mat_sqr(5);
  nml_mat_print(m);
  nml_mat_free(m);

  printf("\nCreating an ID 7x7 Matrix and copying it into another matrix:\n");
  m = nml_mat_eye(7);
  mx = nml_mat_cp(m);
  nml_mat_print(m);
  nml_mat_print(mx);
  nml_mat_free(m);
  nml_mat_free(mx);

  return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/creating_a_matrix.ex
```

### Creating a marray from an array (`double[N]`)

An array can be used as the "data source" for the Matrix by using:
* `nml_mat *nml_mat_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals)`
  - `num_rows` and `num_cols` represent the dimensions of the matrix;
  - `n_vals` how many values to read from the `vals` source. If `n_vals` is smaller than the product `num_cols * num_rows`, `0.0` will be used as the default value;
  - `vals` the array containing double values. 

```c
#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) { 
    double array[6] = { 
        1.0, 0.2, 3.0, 4.0, 5.0, 3.1 
    };
    nml_mat* my;
    
    // 3 rows, 2 columns
    // read exactly 6 numbers from array[6]
    my = nml_mat_from(3, 2, 6, array);
    nml_mat_print(my);
    nml_mat_free(my);

    // 4 rows, 2 columns
    // read exactly 3 numbers from array[6]
    my = nml_mat_from(4, 2, 3, array);
    nml_mat_print(my);
    nml_mat_free(my);

    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/creating_a_matrix_from_an_array.ex
```

### Creating a Matrix from an external file

The two methods that can be used to create a matrix from a file on disk are:
* `nml_mat *nml_mat_fromfile(const char *file)` 
  * Create a matrix from the `file` path. If the file cannot be opened a `NULL` matrix will be returned. 
* `nml_mat *nml_mat_fromfilef(FILE *f)` 
  * Creates a matrix from am already opened stream `f`. Does not automatically close the stream (`FILE`). 

In the file, the matrix has the following format:

```
4 5
0.0     1.0     2.0     5.0     3.0
3.0     8.0     9.0     1.0     4.0
2.0     3.0     7.0     1.0     1.0
0.0     0.0     4.0     3.0     8.0
```

On the first line `4` represents the number of rows and `5` represents the number of columns of the Matrix
Then next lines contain the matrix elements: `4 * 5 = 20` numbers.

Example code:

```c
#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    const char *f = "examples/data/matrix1.data";
    nml_mat *from_file = nml_mat_fromfile(f);
    nml_mat_print(from_file);
    nml_mat_free(from_file);

    // Or if the file is already opened

    FILE *m_file = fopen("examples/data/matrix2.data", "r");
    nml_mat *from_file2 = nml_mat_fromfilef(m_file);
    nml_mat_print(from_file2);
    nml_mat_free(from_file2);
    fclose(m_file);

    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && ./examples/creating_a_matrix_from_file.ex
```

### Creating a matrix from user input

The `nml_mat *nml_mat_fromfilef(FILE *f)` can be called, with `f=stdin`.

Code example:

```c
#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *from_file2 = nml_mat_fromfilef(stdin);
    nml_mat_print(from_file2);
    nml_mat_free(from_file2);
    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/creating_a_matrix_from_user_input.ex
```

### Creating randomized matrices

Creating a randomized matrix can be done with the following two methods:
* `nml_mat *nml_mat_new_rnd(unsigned int num_rows, unsigned int num_cols, double min, double max)`
  - Creates a randomized matrix of size `num_rows * num_cols`;
  - The random values are between `min` and `max`;
* `nml_mat *nml_mat_sqr_rnd(unsigned int size, double min, double max)`
  - Creates a randomized matrix of size `size * size`;
  - The random values are between `min` and `max`;
  
```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  srand(time(NULL)); // Should be called once per program
  nml_mat *m = nml_mat_new_rnd(5, 5, -10.0, 10.0);
  nml_mat_print(m);
  nml_mat_free(m);
  return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/create_randomized_matrix.ex
```

## Check if two matrices are equal

There are two "equality" methods for matrices:

* `int nml_mat_eqdim(nml_mat *m1, nml_mat *m2)`
  - Tests if two matrices have the same dimension.
  
* `int nml_mat_eq(nml_mat *m1, nml_mat *m2, double tolerance)`
  - Test if two matrices are equal:
    - They have the same dimensions
    - The elements are equal or close of being equal.
  - If you want the elements to be "exactly" eqaul, `tolerance=0.0`



```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {

    srand(time(NULL));
    nml_mat *m1 = nml_mat_new_rnd(2, 3, 1.0, 10.0);
    nml_mat *m2 = nml_mat_new_rnd(2, 3, 1.0, 10.0);

    if (nml_mat_eq(m1, m2, 0.001)) {
        printf("Wow, what were the oddss..\n");
    } else {
        printf("It's ok, nobody is that lucky!\n");
    }
    if (nml_mat_eqdim(m1, m2)) {
        printf("At least we know they both have the same number of rows and columns.\n");
    }

    nml_mat_free(m1);
    nml_mat_free(m2);
    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && ./examples/matrix_equality.ex
```

## Accesing and modifying matrix elements

### Select rows and columns

Two methods can be used to select rows and columns from a source matrix (`nm_mat*`):

* `nml_mat *nml_mat_getcol(nml_mat *m, unsigned int col)`
* `nml_mat *nml_mat_getrow(nml_mat *m, unsigned int row)`

The following code extracts every column of a given random matrix into a temporary column matrix (`nml_mat*`):

```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
  printf("\nExtract all matrix columns from a Matrix as matrices\n");
  srand(time(NULL));
  nml_mat *m = nml_mat_new_rnd(5, 5, -10.0, 10.0);
  nml_mat *col;
  nml_mat_print(m);
  int i = 0;
  for(i = 0; i < m->num_cols; i++) {
    col = nml_mat_getcol(m, i);
    nml_mat_print(col);
    nml_mat_free(col);
  }
  nml_mat_free(m);
  return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/select_columns.ex
```

### Set all elements to a value

Use: `void nml_mat_setall(nml_mat *matrix, double value)`

```c
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    // Creates a matrix of zeros of size = 5
    nml_mat *pi_mat = nml_mat_sqr(5);

    // Sets all elements to PI
    nml_mat_setall(pi_mat, M_PI);

    nml_mat_print(pi_mat);
    nml_mat_free(pi_mat);
    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && ./examples/set_all_elements.ex
```

### Set the first diagonal to a value

Use: `int nml_mat_setdiag(nml_mat *matrix, double value)`

```c
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    // Creates a matrix of zeros of size = 5
    nml_mat *pi_mat = nml_mat_sqr(5);

    // Sets the first diagonal to PI
    nml_mat_setdiag(pi_mat, M_PI);

    nml_mat_print(pi_mat);
    nml_mat_free(pi_mat);
    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/set_diagonal_elements.ex
```

### Scalar multiply the matrix

Use:
* `nml_mat *nml_mat_scalarmult(nml_mat *m, double num)`
   - Multiplies all elements of matrix `m` with `num`. A new matrix is returned.
* `int nml_mat_scalarmult_r(nml_mat *m, double num)`
   - Multiplies all elements of matrix `m` with `num`. All changes are done on matrix `m`.

```c
#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *m = nml_mat_eye(5);

    // Multiply all elements of m with 2.0
    // and return a new matrix

    nml_mat *new_m = nml_mat_scalarmult(m, 2.0);

    if (!(nml_mat_eq(m, new_m, 0.0))) {
        printf("It's normal to see this message.\n");
    }

    // Multiply all elements of m with 2.0
    // m is modified, no new matrix is created
    nml_mat_scalarmult_r(m, 2.0);

    if (nml_mat_eq(m, new_m, 0.0)) {
        printf("It's even more normal to see this message.\n");
    }

    nml_mat_free(m);
    nml_mat_free(new_m);
    
    return 0;
}
```

To run the example:

```sh
./nml.sh clean examples && examples/scalar_multiply.ex
```

### Multiply rows

Use:

* `nml_mat *nml_mat_multrow(nml_mat *m, unsigned int row, double num)`
   - Multiplies all elements from row `row` in matrix `m` with scalar `num`. A new matrix is returned. `m` remains un-altered.
* `int nml_mat_multrow_r(nml_mat *m, unsigned int row, double num)`
   - Multiplies all elements from row `row` in matrix `m` with scalar `num`. The changes are done directly on matrix `m`.
   
Example:  

```c
#include <stdlib.h>
#include <stdio.h>

#include "lib/nml.h"

int main(int argc, char *argv[]) {
    nml_mat *a = nml_mat_new(4,5);
    nml_mat_setall(a, 1.0);
    int i = 0;
    for(; i < a->num_rows; ++i) {
        // Changes are doing on matrix a
        // row[i] is multiplied with (double) i
        nml_mat_multrow_r(a, i, (double)i);
    }
    nml_mat_print(a);

    // Create a new matrix b by multiplying row[1] 
    // in matrix a with 5.0.
    // Matrix a remains unchanged
    nml_mat *b = nml_mat_multrow(a, 1, 5.0);
    nml_mat_free(a);
    nml_mat_print(b);
    return 0;
}
````


To run the example:

```sh
./nml.sh clean examples && examples/multiply_rows.ex
```

To run the example:

```sh
./nml.sh clean examples && examples/multiply_rows.ex
```

### Add rows

## Modifying the matrix structure

### Remove rows and columns

### Swap rows and columns

### Concatenate matrices

## Matrices operations

### Add and subtract matrices

### Multiply matrices (dot)

## Transpose matrices

## Calculate trace

## Row Echelon 

### Calculate Row Echelon Form using Gaussian Elimination

### Calculate Reduced Row Echelon Form using Gauss-Jordan

## LU(P) Decomposition

## Matrix inverse

## Matrix determinant

## Solve linear systems of equations
