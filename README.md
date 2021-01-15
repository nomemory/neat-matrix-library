**nml** is simple [matrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)) and [linear algebra](https://en.wikipedia.org/wiki/Linear_algebra) library written in pure C.

It currently supports:
* Basic Matrix Operations (row swaps, colum swaps, multiplication, addition, etc.)
* [LU Decomposition](https://en.wikipedia.org/wiki/LU_decomposition);
* Inverse(A);
* Determinant(A) 
* Solving [linear systems of equations](https://en.wikipedia.org/wiki/System_of_linear_equations) using LU decomposition;
* [Row Echelon Form](https://en.wikipedia.org/wiki/Gaussian_elimination);
* Reduced Row Echelon Form [Gauss-Jordan](https://brilliant.org/wiki/gaussian-elimination/);

The library is still under heavy development. The plan for the near future:

* Implement [QR decomposition](https://en.wikipedia.org/wiki/QR_decomposition)
* Tests 

Documentation is under heavy editing.

# Compile / Run Examples

The build file for the library it's called `nml.sh`. It's actually a `bash` script (not a `makefile`!).

## Building the library

```bash
./nml.sh clean build
```

This will compile the library, create a `dist` folder where you will find `*.a` static library file and the header files.

`gcc` and `ar` should be available in `$PATH`.

## Building the examples

Examples can be found in the [`./examples` folder](https://github.com/nomemory/neat-matrix-library/tree/main/examples).

To build the code examples:

```bash
./nml.sh clean examples
```

1. This will create an `examples/lib` folder where the `libnml.a` and the header files will be copied;
2. The `examples/*.c` will be compiled with the latest version of `libnml`;
3. For each `examples/*.c` an executable (`*.ex`) will be created.

## Running the tests 

To run the tests 

```bash
./nml.sh clean test
```

1. This will create a `test/lib` folder where the `libnml.a` and the header files will be copied;
2. Each test `tests/*.c` will be compiled with the latest version of `libnml`;
3. For each test `tests/*/c` an executable (`*.ex`) will be created.

The test data was generated using [sympy](https://docs.sympy.org/). 
In the `tests/generators/*.py` you can find the python3 scripts used to generate the data.

## Cleaning

```bash
./nml.sh clean
```

This will clean everything (`*.o`,`*.ex`,`*.a`) and will leave the library folder in a clean state.

# How to use the library

A few examples can be found in the `./examples/*.c` folder.

## Creating a Matrix

### Creating a Matrix manually

### Creating a Matrix from existing data

### Creating a Matrix from an external file

### Creating a square Matrix
