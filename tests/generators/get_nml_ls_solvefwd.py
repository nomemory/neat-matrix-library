from random import randrange
from sympy import Matrix, pprint, randMatrix, N, eye
from sympy.matrices.dense import zeros
from sympy.printing.str import StrPrinter
from sympy.abc import x 

filename = "test_nml_ls_solvefwd.data"

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
    M = zeros(M_dim)
    for i in range(M_dim):
        for j in range(i+1):
            M[i, j] = randrange(min_val, max_val)
    B = randMatrix(M_dim, 1, min=min_val, max=max_val, percent=100)            
    x = M.lower_triangular_solve(B).applyfunc(N)
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