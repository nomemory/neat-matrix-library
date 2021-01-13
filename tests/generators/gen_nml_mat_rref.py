#!/usr/bin/env python

from random import randrange
from sympy import Matrix, pprint, randMatrix, N
from sympy.printing.str import StrPrinter
from sympy.abc import x 

filename = "test_nml_mat_rref.data"

num_tests = 5000
min_M_cols = 2
min_M_rows = 2
max_M_cols = 20
max_M_rows = 20
min_val = 0
max_val = 10

fs = open(filename, "w");

fs.write("{}\n\n".format(num_tests))

for i in range(num_tests):
    M_rows = randrange(min_M_rows, max_M_rows)
    M_cols = randrange(min_M_cols, max_M_cols)
    M = randMatrix(M_rows, M_cols, min=0, max=10, percent=randrange(50, 100));
    MRREF = M.rref()[0].applyfunc(N)
    print("Creating test cases {}", i)
    fs.write("{} {}\n".format(M_rows, M_cols));
    fs.write(M.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n")
    fs.write(MRREF.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n")


fs.close()