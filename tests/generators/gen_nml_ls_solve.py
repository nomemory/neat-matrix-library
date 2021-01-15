from random import randrange
from sympy import Matrix, pprint, randMatrix, N, eye
from sympy.matrices.dense import zeros
from sympy.printing.str import StrPrinter
from sympy.abc import x 

filename = "test_nml_ls_solve.data"

num_tests = 400
min_M_cols = 1
min_M_rows = 1
max_M_cols = 15
max_M_rows = 15
min_val = 1
max_val = 100

fs = open(filename, "w")

fs.write("{}\n".format(num_tests))

for i in range(num_tests):
    print("Creating test case: ", i)
    M_dim = randrange(min_M_cols, max_M_cols)
    M = randMatrix(M_dim, M_dim, min=min_val, max=max_val, percent=100)            
    B = randMatrix(M_dim, 1, min=min_val, max=max_val, percent=100)            
    x = M.LUsolve(B).applyfunc(N)
    fs.write("{} {}\n".format(M_dim, M_dim))            
    fs.write(M.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n")
    fs.write("{} {}\n".format(M_dim, 1))            
    fs.write(B.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))           
    fs.write("\n")
    fs.write("{} {}\n".format(M_dim, 1))
    fs.write(x.table(StrPrinter(), rowstart="", rowend="", colsep="\t"))
    fs.write("\n")

fs.close()