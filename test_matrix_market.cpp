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

    // OK(GrB_init(GrB_BLOCKING));
    
    std::ifstream isA ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixA_cusparse");
    std::ifstream isB ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixB_cusparse");
    
    // std::ifstream oscii_dcop_46("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/$as-caida_G_003.mtx-out");
    // std::ifstream oscii_dcop_46("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/$as-caida_G_077.mtx-out");

   
    //init graphBlass:
        
    auto csrA = CSRWrapper<float>(isA);
    auto csrB = CSRWrapper<float>(isB);

    auto csrC_ss = csrA.multiply_suite_sparse(csrB);

    auto csrC_impala = csrA.multiply_cuda(csrB);

    // std::cout << csrC_impala.nnz << std::endl;
    // std::cout << csrC_impala.M << std::endl;

    // std::cout << "C csr offsets: ";
    // for (int i = 0; i < csrC_impala.M + 1; i++) {
    //     std::cout << csrC_impala.get_row_index()[i] << " ";
    // }
    // std::cout << std::endl;

    // auto csrC_impala = csrA.multiply_impala(csrA);

    std::cout << csrC_ss.nnz << std::endl;
    std::cout << csrC_ss.M << std::endl;
    std::cout << csrC_ss.N << std::endl;


    std::cout << csrC_impala.nnz << std::endl;
    std::cout << csrC_impala.M << std::endl;
    std::cout << csrC_impala.N << std::endl;
    // std::cout << csrC_impala.nnz << std::endl;

    for(auto i : csrC_impala.get_row_index()) {
        std::cout << i << " ";
    }
    std::cout << std::endl;


    for(auto i : csrC_ss.get_row_index()) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    assert(csrC_ss  == csrC_impala);
    // OK(GrB_finalize());
}


//TODO: handle empty matricies, write impala code up to the end, so the only thing
// is used from c++ is matrix reader and thrust, todo spmm decompose impala
// benchmark 
// think about design e.g.
/* struct Semiring {
    zero : fn() -> f32, 
    one : fn() -> f32,
    plus : fn(f32,f32) -> f32,
    multiply : fn(f32,f32) -> f32
}*/

// Maybe change intel mkl for suite sparse graphblas
// abstract over implementation of multiplication and matrix representation
// PRESENTATION 

//rewrite impala add graphblas

//benchmark