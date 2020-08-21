#ifndef CSR_WRAPPER_H
#define CSR_WRAPPER_H

#include <vector>
#include <cassert>
#include <cmath>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <iostream>


// CUSPARSE STUFF
// #include <cuda_runtime.h> // cudaMalloc, cudaMemcpy, etc.
// #include <cusparse.h>         // cusparseSpGEMM


#include "cuda_helpers.h"



#include "matrix_market_reader.h"

#include "sparse.h"

// Intel MKL (Sparse algebra library)
#include "mkl.h"

#include "cuda_sparse_wrappers.h"
// mkl_sparse_s_add(...)
//  mkl_sparse_spmm
#include "mkl_spblas.h"


#define CALL_AND_CHECK_STATUS(function, error_message) do { \
          switch (function)                                 \
          {                                                 \
          case SPARSE_STATUS_SUCCESS:       \
              /* code */                                    \
              break;\
          case SPARSE_STATUS_NOT_INITIALIZED:\
            std::cout << "The routine encountered an empty handle or matrix array." << std::endl;\
            break;\
          case SPARSE_STATUS_ALLOC_FAILED:\
            std::cout << "Internal memory allocation failed." << std::endl;\
            break;\
          case SPARSE_STATUS_INVALID_VALUE:\
            std::cout << "The input parameters contain an invalid value." << std::endl;\
          default:\
            std::cout << "Other error" << std::endl;\
              break;\
          }             \
} while(0)
//from MM to CSR struct from Impala

//transpose
template<typename T>
class CSRWrapper{
    public:
        unsigned int N, M, nnz; //maybe int64?
        CSRWrapper(std::ifstream &is);
        CSRWrapper(unsigned int m, unsigned int n, unsigned int nnz, std::unique_ptr<T[]> values,std::unique_ptr<unsigned int[]> cols,std::unique_ptr<unsigned int[]> rows);
        CSRWrapper(const CSRWrapper<T>& another);
        ~CSRWrapper() = default;
        struct CSR csr_to_struct() const;
        CSRWrapper<T> multiply (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_impala (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_cusparse (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_cuda (const CSRWrapper<T>& another) const;
        CSRWrapper<T> subtract (const CSRWrapper<T>& another) const;
        T get_matrix_elem(int row, int col);
        // T const * get_values();
        // unsigned int const * get_cols();
        std::vector<unsigned int> const get_row_index() const;
        std::vector<unsigned int> const get_cols() const;
        std::vector<T> const get_values() const;
        unsigned int const max_row_length() const;
        //output
        template<typename U>
        friend std::ostream & operator << (std::ostream &out, const CSRWrapper<U> &c);
        template<typename U>
        friend bool operator==(const CSRWrapper<U>& l, const CSRWrapper<U>& r);
    private:
        std::unique_ptr<T[]> values_;
        std::unique_ptr<unsigned int[]> cols_;
        std::unique_ptr<unsigned int[]> row_index_;

};


template<typename T>
CSRWrapper<T>::CSRWrapper(std::ifstream &is) {
    
    matrix_market::reader reader (is);

    if (reader) {
        auto &matrix = reader.matrix();
        auto &meta = matrix.meta;
        N = meta.cols_count;
        M = meta.rows_count;
        nnz = meta.non_zero_count;

        if(nnz != 0){

            std::vector<T> values = std::vector<T>();
            auto cols = std::vector<unsigned int>();
            auto row_index = std::vector<unsigned int>();

            auto col_ids = matrix.get_col_ids ();
            auto row_ids = matrix.get_row_ids ();
            auto data    = matrix.get_dbl_data ();

            //sort values by rows
            row_index.push_back(0);
            //for each row
            for(int r = 0, r_id = 0; r < M; r++){
                int row_size = 0;
                auto tmp_cols = std::vector<unsigned int>();
                auto tmp_values = std::vector<std::pair<unsigned int, T>>();
                for(int i = 0; i < nnz; i++) {
                    //could be unordered with respect to columns
                    if(row_ids[i] == r){
                        row_size++;
                        // values.push_back(data[i]);
                        tmp_values.emplace_back(i,data[i]);
                        tmp_cols.push_back(col_ids[i]);
                    }
                }
                std::sort(tmp_cols.begin(),tmp_cols.end());
                std::sort(tmp_values.begin(),tmp_values.end(),
                    [col_ids] (std::pair<unsigned int, T> const& lhs, std::pair<unsigned int, T> const& rhs) -> bool {
                        return col_ids[lhs.first] < col_ids[rhs.first];
                    });
                
                cols.insert(cols.end(),tmp_cols.begin(),tmp_cols.end());
                for(auto &e : tmp_values) {
                    values.push_back(e.second);
                }
                r_id += row_size;
                row_index.push_back(r_id);
            }
            values_ = std::unique_ptr<T[]>(new T[values.size()]);
            cols_ = std::unique_ptr<unsigned int[]>(new unsigned int[cols.size()]);
            row_index_ = std::unique_ptr<unsigned int[]>(new unsigned int[row_index.size()]);

            std::copy(values.begin(),values.end(),values_.get());
            std::copy(cols.begin(),cols.end(),cols_.get());
            std::copy(row_index.begin(),row_index.end(),row_index_.get());
        } else {
            values_ = std::unique_ptr<T[]>(nullptr);
            cols_ = std::unique_ptr<unsigned int[]>(nullptr);
            row_index_ = std::unique_ptr<unsigned int[]>(nullptr);
        }

    
    } else {
        N = 0;
        M = 0;
        nnz = 0;
        values_ = nullptr;
        cols_ = nullptr;
        row_index_ = nullptr;

    }
    
}

template<typename T>
CSRWrapper<T>::CSRWrapper(unsigned int m,
   unsigned int n,
   unsigned int nnz_,
   std::unique_ptr<T[]> values,
   std::unique_ptr<unsigned int[]> cols,
   std::unique_ptr<unsigned int[]> rows)
   : M(m),
    N(n),
    nnz(nnz_),
    values_(std::move(values)),
    cols_(std::move(cols)),
    row_index_(std::move(rows))
    {}

template<typename T>
CSRWrapper<T>::CSRWrapper(const CSRWrapper<T>& another){

    M = another.M;
    N = another.N;
    nnz = another.nnz;
    
    values_.reset(new T[nnz]);
    cols_.reset(new unsigned int[nnz]);
    row_index_.reset(new unsigned int[M+1]);

    std::copy(another.values_.get(),another.values_.get()+nnz,values_.get());
    std::copy(another.cols_.get(),another.cols_.get()+nnz,cols_.get());
    std::copy(another.row_index_.get(),another.row_index_.get()+M+1,row_index_.get());
}

template<>
struct CSR CSRWrapper<float>::csr_to_struct() const{
    struct CSR csr = CSR{N : N, M : M, nnz : nnz, values : values_.get(), cols : cols_.get(), row_index : row_index_.get()};
    return csr;
}

template<typename T>
const std::vector<unsigned int> CSRWrapper<T>::get_row_index() const{
    auto csrOffsets = std::vector<unsigned int>();
    for (int i = 0; i < M + 1; i++){
        csrOffsets.push_back(row_index_[i]);
    }
    return csrOffsets;
}

template<typename T>
const std::vector<unsigned int> CSRWrapper<T>::get_cols() const{
    auto cols = std::vector<unsigned int>();
    for (int i = 0; i < nnz; i++){
        cols.push_back(cols_[i]);
    }
    return cols;
}

template<typename T>
const std::vector<T> CSRWrapper<T>::get_values() const{
    auto vals = std::vector<T>();
    for (int i = 0; i < nnz; i++){
        vals.push_back(values_[i]);
    }
    return vals;
}

template<>
CSRWrapper<float> CSRWrapper<float>::multiply (const CSRWrapper<float>& another) const{
    
    if (N != another.M) throw new std::invalid_argument("Numer of colons of the left oparand should match the number of rows of the right one");
    
    sparse_matrix_t mkl_this;
    sparse_matrix_t mkl_another;

    // check tmp arrays

    auto values_tmp = new float[nnz];
    auto cols_tmp = new int[nnz];
    auto rows_tmp = new int[M + 1];

    std::copy(values_.get(),values_.get()+nnz,values_tmp);
    std::copy((int*)cols_.get(),(int*)cols_.get()+nnz,cols_tmp);
    std::copy((int*)row_index_.get(),(int*)row_index_.get()+M+1,rows_tmp);

    // CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , M, N,
    //  (int*)(row_index_.get()), (int*)(row_index_.get()) + 1, (int*)cols_.get(), values_.get()),"Error\n");

    CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , M, N,
     rows_tmp, rows_tmp + 1, cols_tmp, values_tmp),"Error\n");
    // assert(mkl_this_status == SPARSE_STATUS_SUCCESS);
    
    CALL_AND_CHECK_STATUS(
    mkl_sparse_s_create_csr (&mkl_another, SPARSE_INDEX_BASE_ZERO, another.M, another.N,
     (int*)another.row_index_.get(), (int*)another.row_index_.get() + 1, (int*)another.cols_.get(), another.values_.get()),"Error");
    // assert(mkl_another_status == SPARSE_STATUS_SUCCESS);
    // assert(1==2);

    sparse_matrix_t mkl_result;
    
    CALL_AND_CHECK_STATUS(mkl_sparse_spmm (SPARSE_OPERATION_NON_TRANSPOSE, mkl_this,mkl_another,&mkl_result),"Error");
    
    sparse_index_base_t indexing;
    int rows;
    int cols;
    int *rows_start;
    int *rows_end;
    int *cols_idx;
    float *values;
    sparse_status_t export_result = mkl_sparse_s_export_csr (mkl_result,&indexing, &rows, &cols, &rows_start,&rows_end, &cols_idx, &values);
    unsigned int nnz = (unsigned int)rows_end[rows-1];

    
    auto values_out = std::unique_ptr<float[]>(new float[nnz]);
    std::copy(values,values+nnz,values_out.get());
    
    auto cols_out = std::unique_ptr<unsigned int[]>(new unsigned int[nnz]);
    std::copy((unsigned int*)cols_idx,(unsigned int*)cols_idx + nnz,cols_out.get());

    //what if 0 matrix?
    auto rows_out = std::unique_ptr<unsigned int[]>(new unsigned int[rows+1]);
    rows_out.get()[0] = 0;
    std::copy((unsigned int*)rows_end, (unsigned int*) rows_end + rows,rows_out.get() + 1 );
    
    mkl_sparse_destroy(mkl_result);
    mkl_sparse_destroy(mkl_this);
    mkl_sparse_destroy(mkl_another);

    delete[] values_tmp;
    delete[] rows_tmp;
    delete[] cols_tmp;


    return CSRWrapper(rows,cols,nnz,std::move(values_out),std::move(cols_out),std::move(rows_out));
    //multiply subtract unique_ptr

}

template<>
CSRWrapper<float> CSRWrapper<float>::multiply_cuda(const CSRWrapper<float>& another) const{
    struct CSR csrA = csr_to_struct();
    struct CSR csrB = another.csr_to_struct();
    struct CSR csrC = {};

    spGEMMCuda(&csrA, &csrB,&csrC);

    //copy to unique ptr;
    auto hC_values = std::unique_ptr<float[]>(new float[csrC.nnz]);

    auto hC_cols = std::unique_ptr<unsigned int[]>(new unsigned int[csrC.nnz]);

    auto hC_csrOffsets = std::unique_ptr<unsigned int[]>(new unsigned int[csrC.M + 1]);

    std::copy(csrC.values, csrC.values + csrC.nnz, hC_values.get());
    std::copy(csrC.cols, csrC.cols + csrC.nnz, hC_cols.get());
    std::copy(csrC.row_index, csrC.row_index + csrC.M + 1, hC_csrOffsets.get());

    delete[] csrC.values;
    delete[] csrC.row_index;
    delete[] csrC.cols;

    return CSRWrapper(csrC.M,csrC.N,csrC.nnz,std::move(hC_values),std::move(hC_cols),std::move(hC_csrOffsets));



}


template<>
CSRWrapper<float> CSRWrapper<float>::multiply_cusparse(const CSRWrapper<float>& another) const{
    struct CSR csrA = csr_to_struct();
    struct CSR csrB = another.csr_to_struct();
    struct CSR csrC = {};

    spGEMMCusparse(&csrA, &csrB,&csrC);

    //copy to unique ptr;
    auto hC_values = std::unique_ptr<float[]>(new float[csrC.nnz]);

    auto hC_cols = std::unique_ptr<unsigned int[]>(new unsigned int[csrC.nnz]);

    auto hC_csrOffsets = std::unique_ptr<unsigned int[]>(new unsigned int[csrC.M + 1]);

    std::copy(csrC.values, csrC.values + csrC.nnz, hC_values.get());
    std::copy(csrC.cols, csrC.cols + csrC.nnz, hC_cols.get());
    std::copy(csrC.row_index, csrC.row_index + csrC.M + 1, hC_csrOffsets.get());

    delete[] csrC.values;
    delete[] csrC.row_index;
    delete[] csrC.cols;

    return CSRWrapper(csrC.M,csrC.N,csrC.nnz,std::move(hC_values),std::move(hC_cols),std::move(hC_csrOffsets));



}

template<>
CSRWrapper<float> CSRWrapper<float>::subtract(const CSRWrapper<float>& another) const{

    if (N != another.N || M != another.M) throw new std::invalid_argument("Numer of colons of the left oparand should match the number of rows of the right one");
    sparse_matrix_t mkl_this;
    sparse_matrix_t mkl_another;

    // check tmp arrays

    auto values_tmp = new float[nnz];
    auto cols_tmp = new int[nnz];
    auto rows_tmp = new int[M + 1];

    std::copy(values_.get(),values_.get()+nnz,values_tmp);
    std::copy((int*)cols_.get(),(int*)cols_.get()+nnz,cols_tmp);
    std::copy((int*)row_index_.get(),(int*)row_index_.get()+M+1,rows_tmp);

    // CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , M, N,
    //  (int*)(row_index_.get()), (int*)(row_index_.get()) + 1, (int*)cols_.get(), values_.get()),"Error\n");

    CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , M, N,
     rows_tmp, rows_tmp + 1, cols_tmp, values_tmp),"Error\n");
    // assert(mkl_this_status == SPARSE_STATUS_SUCCESS);
    
    CALL_AND_CHECK_STATUS(
    mkl_sparse_s_create_csr (&mkl_another, SPARSE_INDEX_BASE_ZERO, another.M, another.N,
     (int*)another.row_index_.get(), (int*)another.row_index_.get() + 1, (int*)another.cols_.get(), another.values_.get()),"Error");
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

    
    auto values_out = std::unique_ptr<float[]>(new float[nnz]);
    std::copy(values,values+nnz,values_out.get());
    
    auto cols_out = std::unique_ptr<unsigned int[]>(new unsigned int[nnz]);
    std::copy((unsigned int*)cols_idx,(unsigned int*)cols_idx + nnz,cols_out.get());

    //what if 0 matrix?
    auto rows_out = std::unique_ptr<unsigned int[]>(new unsigned int[rows+1]);
    rows_out.get()[0] = 0;
    std::copy((unsigned int*)rows_end, (unsigned int*) rows_end + rows,rows_out.get() );
    
    mkl_sparse_destroy(mkl_result);
    mkl_sparse_destroy(mkl_this);
    mkl_sparse_destroy(mkl_another);

    delete[] values_tmp;
    delete[] rows_tmp;
    delete[] cols_tmp;


    return CSRWrapper(rows,cols,nnz,std::move(values_out),std::move(cols_out),std::move(rows_out));
}

template<typename T>
T CSRWrapper<T>::get_matrix_elem(int row, int col) {
      int j1 = row_index_[row];
      int j2 = row_index_[row+1];
      for(int j = j1; j < j2; j++) {
          if(cols_[j] == col){
              return values_[j];
          }
      }
      throw new std::invalid_argument("No such element");
}

template<typename T>
unsigned int const CSRWrapper<T>::max_row_length() const {
    unsigned int max = 0;
    for (int i = 0; i < M; i++){
        int new_max = row_index_[i+1] - row_index_[i];
        if(new_max > max) max = new_max;
    }

    return max;
}


template<typename T>
std::ostream & operator << (std::ostream &out, const CSRWrapper<T> &c)
{
    for(int i = 0; i < c.nnz; i++){
        out << std::setprecision(10) << c.values_[i] << " ";
    }
    out << std::endl;

    for(int i = 0; i < c.nnz; i++) {
        out << c.cols_[i] << " ";
    }
    out << std::endl;
    for(int i = 0; i < c.M + 1; i++) {
        out << c.row_index_[i] << " ";
    }
    out << std::endl;

    return out;
}

template<typename T>
bool operator==(const CSRWrapper<T>& l, const CSRWrapper<T>& r)
    {
        
        float maxC_ij = 0.0;
        CSRWrapper<float> C = l.subtract(r);
        
        for(int i = 0; i < C.nnz; i++) {
            float var = std::abs(C.values_[i]);
            if(var > maxC_ij){
                maxC_ij = var;
            }
        }
        // if (l.nnz == r.nnz)
        // {
        //     for (size_t i = 0; i < l.nnz; i++)
        //     {
        //         float var = std::abs(l.values_[i] - r.values_[i]);
        //         if(var > maxC_ij){
        //             maxC_ij = var;
        //         }
        //     }
        //     return maxC_ij < 0.001;
            
        // } else
        // {
        //     return false;
        // }
        return maxC_ij < 0.00001;
        
        
    }


// template<typename T>
// CSRWrapper<T>::~CSRWrapper() {
//     delete[] (values_);
//     delete[] (cols_);
//     delete[] (row_index_);
// }




#endif