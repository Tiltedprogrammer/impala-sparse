#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>

#include "csr_wrapper.h"

#include <filesystem>

#include <chrono>

#include <GraphBLAS.h>
// TODO UNIQUEPTR

namespace fs = std::filesystem;


int main(int argc, char ** argv) {

    GrB_Info info;
    
    std::ifstream isA ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/apsp/input");
    std::ifstream isB ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/apsp/output");
   
    //init graphBlass:
        
    auto csrA = CSRWrapper<float>(isA);
    auto csrB = CSRWrapper<float>(isB);
    int m = csrA.N;


    for(int i = 1; i < m - 1; i *= 2) {
        csrA = csrA.multiply_suite_sparse(csrA);
    }

    assert(csrA == csrB);
    std::cout << csrA << csrA.M << " " << csrA.nnz << std::endl;
    std::cout << csrB << csrB.M << " " << csrA.nnz << std::endl;
    
}

// think about design e.g.
/* struct Semiring {
    zero : fn() -> f32, 
    one : fn() -> f32,
    plus : fn(f32,f32) -> f32,
    multiply : fn(f32,f32) -> f32
}*/

// Maybe change intel mkl for suite sparse graphblas
// abstract over implementation of multiplication and matrix representation
