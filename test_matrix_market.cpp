#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>

#include "csr_wrapper.h"

#include <filesystem>

// TODO UNIQUEPTR

namespace fs = std::filesystem;


int main(int argc, char ** argv) {
    
    std::ifstream csrA_simple ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixA");
    std::ifstream csrB_simple ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixB");
    std::ifstream csrC_simple ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixC");
    // std::ifstream is4 ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrix_market_example");
    std::ifstream csrA_cusparse ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixA_cusparse");
    std::ifstream csrB_cusparse ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixB_cusparse");
    std::ifstream csrC_cusparse ("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/matrixC_cusparse");

    std::ifstream oscii_dcop_46("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/GD01_Acap.mtx");

   
    // matrix_market::reader reader (is);

    // if (reader) {
        // auto &matrix = reader.matrix ();
        // auto &meta   = matrix.meta;
        // auto col_ids = matrix.get_col_ids ();
        // auto row_ids = matrix.get_row_ids ();
        // auto data    = matrix.get_dbl_data ();

        // is.clear();
        // is.seekg(0);
        // CSRWrapper<float> csr(is);

        // for (unsigned int i = 0; i < meta.non_zero_count; i++) {
        //     const unsigned int col = col_ids[i];
        //     const unsigned int row = row_ids[i];
        //     const auto value = data[i];
        //     assert(std::abs(value - csr.get_matrix_elem(row,col)) < 0.001);
        //     std::cout << std::setprecision(10) << row << " " << col << " " << value << " " << csr.get_matrix_elem(row,col) << std::endl;
        // }

        // auto csr_struct = csr.csr_to_struct();
        // std::cout << get_nnz(&csr_struct) << std::endl;

        // std::cout << csr;

        // assert(csr == csr);
        // auto csrA = CSRWrapper<float>(csrA_cusparse);
        // auto csrB = CSRWrapper<float>(csrB_cusparse);
        // auto csrC = CSRWrapper<float>(csrC_cusparse);
        auto csrA = CSRWrapper<float>(oscii_dcop_46);
        // auto csrB = CSRWrapper<float>(is2);
        // auto csrC = CSRWrapper<float>(is3);
        std::cout << csrA.max_row_length() << std::endl;
        if(csrA.max_row_length() <= 32) {
            CSRWrapper<float> csrAA_cuda = csrA.multiply_cuda(csrA);
            CSRWrapper<float> csrAA_mtl = csrA.multiply_cusparse(csrA);
            std::cout << csrAA_cuda.nnz << std::endl;
            std::cout << csrAA_mtl.nnz << std::endl;
        
            assert(csrAA_cuda.get_row_index() == csrAA_mtl.get_row_index() && "csrOffsets are not equal");
            // for(const auto elem : csrAA_mtl.get_values()) {
            //     std::cout << elem << " ";
            // }
            // std::cout << std::endl;

            // for(const auto elem : csrAA_cuda.get_values()) {
            //     std::cout << elem << " ";
            // }
            // std::cout << std::endl;

            auto csrA_cuda_vals = csrAA_cuda.get_values();
            auto csrA_cusparse_vals = csrAA_mtl.get_values();
            // for(int i = 0; i < csrAA_mtl.nnz; i++) {
            //     if(std::abs(csrA_cusparse_vals[i] - csrA_cuda_vals[i]) > 10)
            //     std::cout << std::setprecision(15) << csrA_cusparse_vals[i]  << " " <<  csrA_cuda_vals[i] << " i= " << i << std::endl;
            // }


            // assert(csrAA_cuda.get_values() == csrAA_mtl.get_values());
            // for(const auto elem : csrA.get_cols()) {
            //     std::cout << elem << " ";
            // }
            // std::cout << std::endl;
            // std::cout << csrAA_cuda;
            // std::cout << csrAA_mtl;
            assert(csrAA_cuda.get_cols() == csrAA_mtl.get_cols() && "cols are not equal");
            assert(csrAA_cuda == csrAA_mtl && "Matricies are not eq");
            // assert(csrA == csrA);
        }

        // auto csrCcusparse = csrA.multiply_cusparse(csrB);
        // assert(csrCcusparse == csrC);

        
        

}


//TODO write equality test for CSR, write multiplication handle valgrind
//multiply sparse stuff;