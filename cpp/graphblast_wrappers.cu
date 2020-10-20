// GraphBlast

#define GRB_USE_CUDA
#define private public
#include <boost/program_options.hpp>

#include "graphblas/graphblas.hpp"
#include "test/test.hpp"
#include "cuda_sparse_wrappers.h"


void spGEMMGraphblast(int a_num_rows, int a_num_cols, int a_num_nnz, int* a_rows,int* a_cols, float* a_values,
    int b_num_rows, int b_num_cols, int b_num_nnz, int* b_rows,int* b_cols, float* b_values,
    std::vector<int>& c_rows,std::vector<int>& c_cols, std::vector<float>& c_values) {

    std::vector<graphblas::Index> a_row_indices, b_row_indices;
    std::vector<graphblas::Index> a_col_indices, b_col_indices;
    std::vector<float> a_values_v, b_values_v;
    

    graphblas::Matrix<float> a(a_num_rows,a_num_cols);
    graphblas::Matrix<float> b(b_num_rows, b_num_cols);

    for (int i = 0; i < a_num_rows; i++) {
        for (int j = a_rows[i]; j < a_rows[i+1]; j++) {
            a_row_indices.push_back(i);
            a_col_indices.push_back(a_cols[j]);
            a_values_v.push_back(a_values[j]);
        }
    }

    for (int i = 0; i < b_num_rows; i++) {
        for (int j = b_rows[i]; j < b_rows[i+1]; j++) {
            b_row_indices.push_back(i);
            b_col_indices.push_back(b_cols[j]);
            b_values_v.push_back(b_values[j]);
        }
    }

    a.build(&a_row_indices, &a_col_indices, &a_values_v, a_num_nnz, GrB_NULL);
    b.build(&b_row_indices, &b_col_indices, &b_values_v, b_num_nnz, GrB_NULL);

    graphblas::Matrix<float> c(a_num_rows, b_num_cols);
    graphblas::Descriptor desc;
    desc.descriptor_.debug_ = false;
    
    graphblas::mxm<float,float,float,float>(
      &c,
      GrB_NULL,
      GrB_NULL,
      graphblas::MinimumPlusSemiring<float>(),
      &a,
      &b,
      &desc
    );

    graphblas::Index c_nnz;
    c.nvals(&c_nnz);
 

    c.extractTuples(&c_rows,&c_cols,&c_values,&c_nnz);


}

void spGEMMGraphblast(int a_num_rows, int a_num_cols, int a_num_nnz, int* a_rows,int* a_cols, bool* a_values,
    int b_num_rows, int b_num_cols, int b_num_nnz, int* b_rows,int* b_cols, bool* b_values,
    std::vector<int>& c_rows,std::vector<int>& c_cols, std::vector<bool>& c_values) {

    std::vector<graphblas::Index> a_row_indices, b_row_indices;
    std::vector<graphblas::Index> a_col_indices, b_col_indices;
    std::vector<bool> a_values_v, b_values_v;
    

    graphblas::Matrix<bool> a(a_num_rows,a_num_cols);
    graphblas::Matrix<bool> b(b_num_rows, b_num_cols);

    for (int i = 0; i < a_num_rows; i++) {
        for (int j = a_rows[i]; j < a_rows[i+1]; j++) {
            a_row_indices.push_back(i);
            a_col_indices.push_back(a_cols[j]);
            a_values_v.push_back(a_values[j]);
        }
    }

    for (int i = 0; i < b_num_rows; i++) {
        for (int j = b_rows[i]; j < b_rows[i+1]; j++) {
            b_row_indices.push_back(i);
            b_col_indices.push_back(b_cols[j]);
            b_values_v.push_back(b_values[j]);
        }
    }

    a.build(&a_row_indices, &a_col_indices, &a_values_v, a_num_nnz, GrB_NULL);
    b.build(&b_row_indices, &b_col_indices, &b_values_v, b_num_nnz, GrB_NULL);

    graphblas::Matrix<bool> c(a_num_rows, b_num_cols);
    graphblas::Descriptor desc;
    desc.descriptor_.debug_ = false;
    desc.set(GrB_MASK, GrB_SCMP);

    graphblas::Matrix<bool> mask(a_num_rows,b_num_cols);
    
    std::vector<bool> mask_vals;
    std::vector<int> mask_cols;
    std::vector<int> mask_rows;
    
    for (int i = 0; i < a_num_rows; i++) {
        for (int j = 0; j < b_num_cols; j++) {
            mask_cols.push_back(j);
            mask_rows.push_back(i);
            mask_vals.push_back(0);
        }
        
    }

    mask.build(&mask_rows, &mask_cols, &mask_vals, a_num_rows*b_num_cols, GrB_NULL);
    mask.print();
    
    graphblas::mxm<bool,bool,bool,bool>(
      &c,
      &mask,
      GrB_NULL,
      graphblas::LogicalOrAndSemiring<bool>(),
      &a,
      &b,
      &desc
    );

    graphblas::Index c_nnz;
    c.nvals(&c_nnz);
 

    c.extractTuples(&c_rows,&c_cols,&c_values,&c_nnz);


}

#undef private