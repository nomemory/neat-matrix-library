# CHANGELOG

## [Unreleased]

### Changed

- [*In progress*] Implementing continuous storage for elements in the matrix.

## [1.0.0] - 2022.05.15

### Added

- Compile linking option `-lm` to the bash script. Also the option `--std=gnu99` to make the program ANSI/ISO conformant, plus allowing GNU extensions, like the constant `M_PI`.
- Qualifier `inline` (**C99**) to some 'fast' functions, like `nml_mat_get` and `nml_mat_set`.
- In `nml_mat_free()`: adding sentences to set pointers to `NULL`, after freeing.
- Adding documentation in a 'JavaDoc' style.
- Creating documentation in `LaTEX` format.
- Adding examples to the documentation (the purpose here is to generate a pedagogical material to teach the fundaments of matrices, using `mnl` as background).

### TODO

- Makefile