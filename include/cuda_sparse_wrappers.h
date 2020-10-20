#ifndef CUDA_SPARSE_WRAPPERS_H
#define CUDA_SPARSE_WRAPPERS_H

#include "sparse.h"
#include <vector>

int spGEMMCuda(struct CSR* a, struct CSR* b, struct CSR* c);

int spGEMMCusparse(struct CSR* a, struct CSR* b, struct CSR* c);

#ifndef SUITE_SPARSE

void spGEMMGraphblast(int a_num_rows, int a_num_cols, int a_num_nnz, int* a_rows,int* a_cols, float* a_values,
    int b_num_rows, int b_num_cols, int b_num_nnz, int* b_rows,int* b_cols, float* b_values,
    std::vector<int>& c_rows,std::vector<int>& c_cols, std::vector<float>& c_values);

void spGEMMGraphblast(int a_num_rows, int a_num_cols, int a_num_nnz, int* a_rows,int* a_cols, bool* a_values,
    int b_num_rows, int b_num_cols, int b_num_nnz, int* b_rows,int* b_cols, bool* b_values,
    std::vector<int>& c_rows,std::vector<int>& c_cols, std::vector<bool>& c_values);


#endif

#endif