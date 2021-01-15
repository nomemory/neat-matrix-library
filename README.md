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

## Creating a Matrix

### Creating a Matrix manually

### Creating a Matrix from existing data

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

### Creating a square Matrix
