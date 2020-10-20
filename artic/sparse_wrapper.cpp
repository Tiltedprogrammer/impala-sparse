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


}
//