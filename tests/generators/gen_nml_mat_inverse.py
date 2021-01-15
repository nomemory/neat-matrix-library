#!/usr/bin/env python

from random import randrange
from sympy import Matrix, pprint, randMatrix, N, eye
from sympy.printing.str import StrPrinter
from sympy.abc import x 

filename = "test_nml_mat_inverse.data"

num_tests = 400
min_M_cols = 1
min_M_rows = 1
max_M_cols = 15
max_M_rows = 15
min_val = 0
max_val = 100

fs = open(filename, "w")

fs.write("{}".format(num_tests))

for i in range(num_tests):
    M_dim = randrange(min_M_cols, max_M_cols)
    M = randMatrix(M_dim, M_dim, min=min_val, max=max_val, percent=100)
    M_inv = M.inv().applyfunc(N)
    print("Creating test case: ", i)
    fs.write("\n{} {}\n".format(M_dim, M_dim))
    fs.write(M.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n{} {}\n".format(M_dim, M_dim))
    fs.write(M_inv.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))  

fs.close()