#include "csr_wrapper.h"
#include <stdlib.h>


extern "C"{

    void read_csr_f32(const char* path, unsigned int * M, unsigned int* N, unsigned int* nnz,unsigned int** cols, unsigned int** csr_offsets, float** values) {

        auto ifstream = std::ifstream(std::string(path));

        auto csrA = CSRWrapper<float>(ifstream);
        
        *M = csrA.M;
        *N = csrA.N;
        *nnz = csrA.nnz;


        (*cols) = (unsigned int*)malloc(sizeof(unsigned int) * csrA.nnz);//new unsigned int[csrA.nnz];
        (*values) = (float*)malloc(sizeof(float) * csrA.nnz);
        (*csr_offsets) = (unsigned int*)malloc(sizeof(unsigned int) * (csrA.M + 1));


        auto csrAcols = csrA.get_cols();
        auto csrAcsr_offsets = csrA.get_row_index();
        auto csrAvalues = csrA.get_values();
        
        for (int i = 0; i < csrA.nnz; i++) {
            (*cols)[i] = csrAcols[i];
        }
        for (int i = 0; i < csrA.nnz; i++) {
            (*values)[i] = csrAvalues[i];
        }
        for (int i = 0; i < csrA.M + 1; i++) {
            (*csr_offsets)[i] = csrAcsr_offsets[i];
        }
    
    }

    void read_csr_u8(const char* path, unsigned int * M, unsigned int* N, unsigned int* nnz,unsigned int** cols, unsigned int** csr_offsets, unsigned char** values) {

        auto ifstream = std::ifstream(std::string(path));

        auto csrA = CSRWrapper<float>(ifstream);
        
        *M = csrA.M;
        *N = csrA.N;
        *nnz = csrA.nnz;


        (*cols) = (unsigned int*)malloc(sizeof(unsigned int) * csrA.nnz);//new unsigned int[csrA.nnz];
        (*values) = (unsigned char*)malloc(sizeof(unsigned char) * csrA.nnz);
        (*csr_offsets) = (unsigned int*)malloc(sizeof(unsigned int) * (csrA.M + 1));


        auto csrAcols = csrA.get_cols();
        auto csrAcsr_offsets = csrA.get_row_index();
        
        for (int i = 0; i < csrA.nnz; i++) {
            (*cols)[i] = csrAcols[i];
        }
        for (int i = 0; i < csrA.nnz; i++) {
            (*values)[i] = 1;
        }
        for (int i = 0; i < csrA.M + 1; i++) {
            (*csr_offsets)[i] = csrAcsr_offsets[i];
        }
    
    }

    void write_f32(const char* path, unsigned int M, unsigned int N, unsigned int nnz,unsigned int* cols, unsigned int* csr_offsets, float* values) {


        std::ofstream os(path);    
        os << "%%MatrixMarket matrix coordinate real general\n";
        std::string comment = "%================================================================================="
                            "\n%\n"
                            "% This ASCII file represents a sparse MxN matrix with L \n"
                            "% nonzeros in the following Matrix Market format:\n"
                            "% +----------------------------------------------+\n"
                            "% |%%MatrixMarket matrix coordinate real general | <--- header line\n"
                            "% |%                                             | <--+\n"
                            "% |% comments                                    |    |-- 0 or more comment lines\n"
                            "% |%                                             | <--+         \n"
                            "% |    M  N  L                                   | <--- rows, columns, entries\n"
                            "% |    I1  J1  A(I1, J1)                         | <--+\n"
                            "% |    I2  J2  A(I2, J2)                         |    |\n"
                            "% |    I3  J3  A(I3, J3)                         |    |-- L lines\n"
                            "% |        . . .                                 |    |\n"
                            "% |    IL JL  A(IL, JL)                          | <--+\n"
                            "% +----------------------------------------------+   \n"
                            "%\n"
                            "% Indices are 1-based, i.e. A(1,1) is the first element.\n"
                            "%\n"
                            "%\n"
                            "%=================================================================================\n";
        os << comment;
        
        os << M << " " << N << " " << nnz << "\n";
        
        for (int i = 0; i < M; i++) {
            for (int j = csr_offsets[i]; j < csr_offsets[i+1]; j++) {
                os << "  " << i + 1 << " " << cols[j] + 1 << " " << values[j] << "\n";
            }
        }

        os << std::flush;

    }

    void write_u8(const char* path, unsigned int M, unsigned int N, unsigned int nnz,unsigned int* cols, unsigned int* csr_offsets, unsigned char* values) {


        std::ofstream os(path);    
        os << "%%MatrixMarket matrix coordinate real general\n";
        std::string comment = "%================================================================================="
                            "\n%\n"
                            "% This ASCII file represents a sparse MxN matrix with L \n"
                            "% nonzeros in the following Matrix Market format:\n"
                            "% +----------------------------------------------+\n"
                            "% |%%MatrixMarket matrix coordinate real general | <--- header line\n"
                            "% |%                                             | <--+\n"
                            "% |% comments                                    |    |-- 0 or more comment lines\n"
                            "% |%                                             | <--+         \n"
                            "% |    M  N  L                                   | <--- rows, columns, entries\n"
                            "% |    I1  J1  A(I1, J1)                         | <--+\n"
                            "% |    I2  J2  A(I2, J2)                         |    |\n"
                            "% |    I3  J3  A(I3, J3)                         |    |-- L lines\n"
                            "% |        . . .                                 |    |\n"
                            "% |    IL JL  A(IL, JL)                          | <--+\n"
                            "% +----------------------------------------------+   \n"
                            "%\n"
                            "% Indices are 1-based, i.e. A(1,1) is the first element.\n"
                            "%\n"
                            "%\n"
                            "%=================================================================================\n";
        os << comment;
        
        os << M << " " << N << " " << nnz << "\n";
        
        for (int i = 0; i < M; i++) {
            for (int j = csr_offsets[i]; j < csr_offsets[i+1]; j++) {
                os << "  " << i + 1 << " " << cols[j] + 1 << " " << (int)values[j] << "\n";
            }
        }

        os << std::flush;

    }

    int is_equal(unsigned int a_M, unsigned int a_N, unsigned int a_nnz,unsigned int* a_cols, unsigned int* a_csr_offsets, float* a_values,
        unsigned int b_M, unsigned int b_N, unsigned int b_nnz,unsigned int* b_cols, unsigned int* b_csr_offsets, float* b_values){
        
        
        if (a_N != b_N || a_M != b_M) throw new std::invalid_argument("Number of colons of the left oparand should match the number of rows of the right one");
        
        sparse_matrix_t mkl_this;
        sparse_matrix_t mkl_another;

        // check tmp arrays

        // auto values_tmp = new float[a_nnz];
        // auto cols_tmp = new int[a_nnz];
        // auto rows_tmp = new int[a_M + 1];

        // std::copy(a_values,a_values + a_nnz,values_tmp);
        // std::copy((int*)a_cols,(int*)a_cols + a_nnz,cols_tmp);
        // std::copy((int*)a_csr_offsets,(int*)a_csr_offsets + a_M + 1,rows_tmp);

        // CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , M, N,
        //  (int*)(row_index_.get()), (int*)(row_index_.get()) + 1, (int*)cols_.get(), values_.get()),"Error\n");

        CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , a_M, a_N,
        (int*)a_csr_offsets, (int*)a_csr_offsets + 1, (int*)a_cols, a_values),"Error\n");
        // assert(mkl_this_status == SPARSE_STATUS_SUCCESS);
        
        CALL_AND_CHECK_STATUS(
        mkl_sparse_s_create_csr (&mkl_another, SPARSE_INDEX_BASE_ZERO, b_M, b_N,
        (int*)b_csr_offsets, (int*)b_csr_offsets + 1, (int*)b_cols, b_values),"Error");
        // assert(mkl_another_status == SPARSE_STATUS_SUCCESS);
        // assert(1==2);

        sparse_matrix_t mkl_result;
        
        CALL_AND_CHECK_STATUS(mkl_sparse_s_add (SPARSE_OPERATION_NON_TRANSPOSE, mkl_this,-1.0,mkl_another,&mkl_result),"Error");
        
        sparse_index_base_t indexing;
        int rows;
        int cols;
        int *rows_start;
        int *rows_end;
        int *cols_idx;
        float *values;
        sparse_status_t export_result = mkl_sparse_s_export_csr (mkl_result,&indexing, &rows, &cols, &rows_start,&rows_end, &cols_idx, &values);
        
        unsigned int nnz = (unsigned int)rows_end[rows-1];

        float maxC_ij = 0.0;

        for(int i = 0; i < nnz; i++) {
            float var = std::abs(values[i]);
            if(var > maxC_ij){
                maxC_ij = var;
            }
        }
      
        mkl_sparse_destroy(mkl_result);
        mkl_sparse_destroy(mkl_this);
        mkl_sparse_destroy(mkl_another);

        if(maxC_ij < 0.00001) return 1;
        else return 0;
    
    }


}
//