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


}
//