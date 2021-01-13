#!/usr/bin/env python

from random import randrange
from sympy import Matrix, pprint, randMatrix, N
from sympy.printing.str import StrPrinter
from sympy.abc import x 

filename = "test_nml_mat_rref.data"

num_tests = 400
min_M_cols = 1
min_M_rows = 1
max_M_cols = 15
max_M_rows = 15
min_val = 0
max_val = 15
percent_min = 50
percent_max = 100

fs = open(filename, "w")

fs.write("{}\n\n".format(num_tests))

for i in range(num_tests):
    M_rows = randrange(min_M_rows, max_M_rows)
    M_cols = randrange(min_M_cols, max_M_cols)
    M = randMatrix(M_rows, M_cols, min=0, max=10, percent=randrange(percent_min, percent_max))
    MRREF = M.rref()[0].applyfunc(N)
    print("Creating test cases {}", i)
    fs.write("{} {}\n".format(M_rows, M_cols))
    fs.write(M.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n")
    fs.write("{} {}\n".format(M_rows, M_cols))
    fs.write(MRREF.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n")


fs.close()