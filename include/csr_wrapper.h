#ifndef CSR_WRAPPER_H
#define CSR_WRAPPER_H

#include <vector>
#include <cassert>
#include <cmath>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <iostream>


//impala compatabiity
#include <stdlib.h>

#include <chrono>


//malloc
#include <cstdlib>
#include <new>      
#include <fstream>
    // std::bad_alloc

#include <cstring>


#include "matrix_market_reader.h"

#include "sparse.h"

// Intel MKL (Sparse algebra library)
#include "mkl.h"

#include "cuda_sparse_wrappers.h"

#include "mkl_spblas.h"



#ifdef SUITE_SPARSE

#pragma message("SUITE_SPARSE BACKEND")

extern "C" {
    #include "GraphBLAS.h"
}

//------------------------------------------------------------------------------
// OK: call a GraphBLAS method and check the result
//------------------------------------------------------------------------------

// OK(method) is a macro that calls a GraphBLAS method and checks the status;
// if a failure occurs, it handles the error via the CHECK macro above, and
// returns the error status to the caller.

#define CHECK(expr,info)                                                \
{                                                                       \
    if (! (expr))                                                       \
    {                                                                   \
        /* free the result and all workspace, and return NULL */        \
        printf ("Failure: line %d file %s\n", __LINE__, __FILE__) ;     \
    }                                                                   \
}

#define OK(method)                                                      \
{                                                                       \
    info = method ;                                                     \
    if (!(info == GrB_SUCCESS || info == GrB_NO_VALUE))                 \
    {                                                                   \
        printf ("GraphBLAS error:\n%s\n", GrB_error ( )) ;              \
        CHECK (false, info) ;                                           \
    }                                                                   \
}





class GraphBLASHandler {
    public:
        static GraphBLASHandler& getInstance()
        {
            static GraphBLASHandler instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        GraphBLASHandler(GraphBLASHandler const&) = delete;
        void operator=(GraphBLASHandler const&)  = delete;
        ~GraphBLASHandler() {
            GrB_Info info;
            OK(GrB_finalize());
        }

    private:
        GraphBLASHandler() {
            GrB_Info info;
            OK(GrB_init(GrB_BLOCKING));
        }
};


#else 
#pragma message("GRAPHBLAST BACKEND")

#endif

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



//------------------------------------------------------------------------------
// CHECK: expr must be true; if not, return an error condition
//------------------------------------------------------------------------------

// the #include'ing file must define the FREE_ALL macro



//transpose
template<typename T>
class CSRWrapper{
    public:
        unsigned int N, M, nnz; //maybe int64?
        CSRWrapper(std::ifstream &is);
        CSRWrapper(unsigned int m, unsigned int n, unsigned int nnz, std::unique_ptr<T[]> values,std::unique_ptr<unsigned int[]> cols,std::unique_ptr<unsigned int[]> rows);
        CSRWrapper(const CSRWrapper<T>& another);
        ~CSRWrapper() = default;
        CSRWrapper<bool> csr_to_bool() const;
        struct CSR csr_to_struct() const;
        struct CSR csr_to_struct_deep() const;
        std::vector<std::unique_ptr<CSRWrapper<T>>> spmmDecompose() const;
        CSRWrapper<T> multiply (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_impala (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_cusparse (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_cuda (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_graphblas (const CSRWrapper<T>& another) const;
        // CSRWrapper<T> multiply_graphblast (const CSRWrapper<T>& another) const;

        CSRWrapper<T> subtract (const CSRWrapper<T>& another) const;
        T get_matrix_elem(int row, int col);
        // T const * get_values();
        // unsigned int const * get_cols();
        std::vector<unsigned int> const get_row_index() const;
        std::vector<unsigned int> const get_cols() const;
        std::vector<T> const get_values() const;
        unsigned int const max_row_length() const;
        //output
        void write(std::ofstream& os) const;
     
        template<typename U>
        friend std::ostream & operator << (std::ostream &out, const CSRWrapper<U> &c);
        template<typename U>
        friend bool operator==(const CSRWrapper<U>& l, const CSRWrapper<U>& r);
        CSRWrapper<T>& operator = (const CSRWrapper<T>& csr);
    private:
    #ifdef SUITE_SPARSE
        GraphBLASHandler& gb_handler = GraphBLASHandler::getInstance(); //invoked on construction
    #endif
        CSRWrapper<T> multiply_cuda_simple (const CSRWrapper<T>& another) const;
        CSRWrapper<T> multiply_impala_simple (const CSRWrapper<T>& another) const;
        std::unique_ptr<T[]> values_;
        std::unique_ptr<unsigned int[]> cols_;
        std::unique_ptr<unsigned int[]> row_index_;

};


template<typename T>
CSRWrapper<T>::CSRWrapper(std::ifstream &is) {
    
    if(!is) {
        throw new std::runtime_error("Error opening the supplied file"); 
    }
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
            auto data    = (matrix.get_dbl_data ());

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

//reduction of the resulting vector should give the initial matrix
template<>
std::vector<std::unique_ptr<CSRWrapper<float>>> CSRWrapper<float>::spmmDecompose() const{
    
    auto vCsr = std::vector<std::unique_ptr<CSRWrapper<float>>>();

    auto G_values = std::unique_ptr<float[]> (new float[nnz]);
    auto G_cols = std::unique_ptr<unsigned int []>(new unsigned int[nnz]);

    std::copy(values_.get(),values_.get()+nnz, G_values.get());
    std::copy(cols_.get(),cols_.get()+nnz,G_cols.get());

    //for each row:
    std::vector<unsigned int> G_csrOffsetsV;
    unsigned int warp_size = 32;
    G_csrOffsetsV.push_back(0);

    std::vector<unsigned int> A1_csrOffsetsV;
    A1_csrOffsetsV.push_back(0);
    std::vector<float> A1_valuesV;
    std::vector<unsigned int> A1_colsV;

    unsigned int A1_col = 0;
    

    for (int r = 0; r < M; r++) {
        int row_length = row_index_[r+1] - row_index_[r];
        int A1_row_length = 0;
        if (row_length > warp_size) {
            //push the ending index of the row
            // int number_of_new_rows = (row_length) / warp_size + 1; //assume row_length = 33, row_idx = 0, row_index_[r+1] = 33;
            for (int row_idx = row_index_[r]; row_idx < row_index_[r+1]; A1_row_length++) { // 2 iterations, row_idx = 0;
                if (row_idx + warp_size > row_index_[r+1]){ // if i == 2
                    G_csrOffsetsV.push_back(row_idx + row_length - (warp_size * A1_row_length)); // push 33
                    row_idx += warp_size;
                } else {
                    row_idx += warp_size;
                    G_csrOffsetsV.push_back(row_idx); //push 32
                }
                A1_valuesV.push_back(1.0);
                A1_colsV.push_back(A1_col);
                A1_col++;
                
            }
            
        } else {
            A1_row_length = 1;
            G_csrOffsetsV.push_back(row_index_[r+1]);
            A1_valuesV.push_back(1.0);
            A1_colsV.push_back(A1_col);
            A1_col++;
        }

        //push A1_csrOffsets:
        A1_csrOffsetsV.push_back(A1_csrOffsetsV.back() + A1_row_length);

    }

    
    //finish constructing G1
    auto G_csrOffsets = std::unique_ptr<unsigned int []>(new unsigned int[G_csrOffsetsV.size()]);
    std::copy(G_csrOffsetsV.begin(),G_csrOffsetsV.end(),G_csrOffsets.get());

    auto A1_values = std::unique_ptr<float[]> (new float[A1_valuesV.size()]);
    auto A1_cols = std::unique_ptr<unsigned int []>(new unsigned int[A1_colsV.size()]);
    auto A1_csrOffsets = std::unique_ptr<unsigned int []>(new unsigned int[A1_csrOffsetsV.size()]);

    std::copy(A1_valuesV.begin(),A1_valuesV.end(),A1_values.get());
    std::copy(A1_colsV.begin(),A1_colsV.end(),A1_cols.get());
    std::copy(A1_csrOffsetsV.begin(),A1_csrOffsetsV.end(),A1_csrOffsets.get());
    
    auto G_ptr = std::unique_ptr<CSRWrapper<float>>(new CSRWrapper<float>(G_csrOffsetsV.size() - 1,N,nnz,std::move(G_values),std::move(G_cols),std::move(G_csrOffsets)));
    auto A1_ptr = std::unique_ptr<CSRWrapper<float>>(new CSRWrapper<float>(M,A1_col,A1_valuesV.size(),std::move(A1_values),std::move(A1_cols),std::move(A1_csrOffsets)));

    vCsr.push_back(std::move(G_ptr));
    vCsr.push_back(std::move(A1_ptr));

    return vCsr;
}

template<typename T>
CSRWrapper<T>& CSRWrapper<T>::operator = (const CSRWrapper<T>& another) {
    
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

template<typename T>
CSRWrapper<bool> CSRWrapper<T>::csr_to_bool() const {
    
    auto cols = std::unique_ptr<unsigned int[]>(new unsigned int[nnz]);
    auto values = std::unique_ptr<bool[]>(new bool [nnz]);
    auto csr_offsets = std::unique_ptr<unsigned int[]>(new unsigned int [M + 1]);

    std::copy(cols_.get(),cols_.get()+nnz,cols.get());
    std::copy(row_index_.get(),row_index_.get()+M+1,csr_offsets.get());
    std::memset(values.get(), 1, sizeof(bool) * nnz);

    return CSRWrapper<bool>(M,N, nnz, std::move(values), std::move(cols),std::move(csr_offsets));


} 
template<>
struct CSR CSRWrapper<float>::csr_to_struct() const{
    struct CSR csr = CSR{N : N, M : M, nnz : nnz, values : values_.get(), cols : cols_.get(), row_index : row_index_.get()};
    return csr;
}

template<>
struct CSR CSRWrapper<float>::csr_to_struct_deep() const{
    auto values_deepcpy = (float*)malloc(sizeof(float) * nnz);
    if (!values_deepcpy) {
        throw new std::bad_alloc();
    }

    auto cols_deepcpy = (unsigned int*)malloc(sizeof(unsigned int) * nnz);
    if (!cols_deepcpy) {
        throw new std::bad_alloc();
    }

    auto csr_offsets_deepcpy = (unsigned int*)malloc(sizeof(unsigned int) * (M + 1));
    if (!csr_offsets_deepcpy) {
        throw new std::bad_alloc();
    }

    std::copy(values_.get(), values_.get()+nnz,values_deepcpy);
    std::copy(cols_.get(), cols_.get()+nnz,cols_deepcpy);
    std::copy(row_index_.get(), row_index_.get()+ M + 1,csr_offsets_deepcpy);

    struct CSR csr = CSR{N : N, M : M, nnz : nnz, values : values_deepcpy, cols : cols_deepcpy, row_index : csr_offsets_deepcpy};
    return csr;
}

// 
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

    CALL_AND_CHECK_STATUS(mkl_sparse_s_create_csr (&mkl_this, SPARSE_INDEX_BASE_ZERO , M, N,
     (int*)row_index_.get(), (int*)row_index_.get() + 1, (int*)cols_.get(), values_.get()),"Error\n");
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


    return CSRWrapper(rows,cols,nnz,std::move(values_out),std::move(cols_out),std::move(rows_out));
    //multiply subtract unique_ptr

}

template<>
CSRWrapper<float> CSRWrapper<float>::multiply_cuda_simple(const CSRWrapper<float>& another) const{
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
CSRWrapper<float> CSRWrapper<float>::multiply_cuda(const CSRWrapper<float>& another) const{
    
    
    int warp_size = 32;

    if(this->max_row_length() <= warp_size) return this->multiply_cuda_simple(another);
    else {
        auto csrV = this->spmmDecompose();
        auto csrC_tmp = csrV[0]->multiply_cuda_simple(another);
        return csrV[1]->multiply_cuda(csrC_tmp);
        
    }
}

template<>
CSRWrapper<float> CSRWrapper<float>::multiply_impala(const CSRWrapper<float>& another) const{
    
    auto a = this->csr_to_struct();
    auto b = another.csr_to_struct();

    auto csrC = spGEMMimpala(&a,&b);

    auto hC_values = std::unique_ptr<float[]>(new float[csrC.nnz]);

    auto hC_cols = std::unique_ptr<unsigned int[]>(new unsigned int[csrC.nnz]);

    auto hC_csrOffsets = std::unique_ptr<unsigned int[]>(new unsigned int[csrC.M + 1]);

    std::copy(csrC.values, csrC.values + csrC.nnz, hC_values.get());
    std::copy(csrC.cols, csrC.cols + csrC.nnz, hC_cols.get());
    std::copy(csrC.row_index, csrC.row_index + csrC.M + 1, hC_csrOffsets.get());

    //free
    free(csrC.values);
    free(csrC.row_index);
    free(csrC.cols);

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



#ifdef SUITE_SPARSE
template<>
CSRWrapper<float> CSRWrapper<float>::multiply_graphblas(const CSRWrapper<float>& another) const{
    
    GrB_Info info ;

    //this graphblass

    GrB_Matrix thisA, anotherA, C;

    OK(GrB_Matrix_new(&thisA,GrB_FP32,this->M,this->N));
    OK(GrB_Matrix_new(&anotherA,GrB_FP32,another.M,another.N));
    OK(GrB_Matrix_new(&C,GrB_FP32,this->M,another.N));
    //get tuples for row, cols, values
    //copy
    GrB_Index* thisAcols = new uint64_t[this->nnz];
    for (uint64_t i = 0; i < this->nnz; i++)
    {
        thisAcols[i] = this->cols_.get()[i];
    }
    
    const float* thisAvals = this->values_.get();
    // csr_to_tuples
    GrB_Index* anotherAcols = new uint64_t[another.nnz];
    
    for (uint64_t i = 0; i < another.nnz; i++)
    {
        anotherAcols[i] = another.cols_.get()[i];
    }
    
    const float* anotherAvals = another.values_.get();

    //row tuples
    uint64_t* thisArows = new uint64_t[this->nnz];
    uint64_t* anotherArows = new uint64_t[another.nnz];

    for(uint64_t i = 0, k = 0; i < this->M; i++) {

        for(unsigned int j = this->row_index_.get()[i]; j < this->row_index_.get()[i+1]; j++,k++) {
            thisArows[k] = i;
        }
    }

    for(uint64_t i = 0, k = 0; i < another.M; i++) {

        for(unsigned int j = another.row_index_.get()[i]; j < another.row_index_.get()[i+1]; j++,k++) {
            anotherArows[k] = i;
        }
    }

    GrB_BinaryOp xop = GrB_PLUS_FP32;

    OK(GrB_Matrix_build_FP32(thisA,(const uint64_t*) thisArows,(const uint64_t*)thisAcols,thisAvals,(uint64_t)this->nnz,xop));
    OK(GrB_Matrix_build_FP32(anotherA,(const uint64_t*) anotherArows,(const uint64_t*)anotherAcols,anotherAvals,(uint64_t)another.nnz,xop));

    // const GrB_Semiring semiring = GxB_PLUS_TIMES_FP32;
    const GrB_Semiring semiring = GxB_MIN_PLUS_FP32;


    OK(GrB_mxm(C,NULL,NULL,semiring,thisA,anotherA,NULL));

    GrB_Index* I;
    GrB_Index* J;
    float* X;
    GrB_Index Cnnz;
    OK( GrB_Matrix_nvals(&Cnnz,C));
    I = new GrB_Index[Cnnz];

    J = new GrB_Index[Cnnz];
    X = new float [Cnnz];
    OK(GrB_Matrix_extractTuples_FP32(I,J,X,&Cnnz,C));


    // std::cout << "C nnz " << Cnnz << std::endl;

    auto Ccols = std::unique_ptr<unsigned int[]>(new unsigned int [Cnnz]);
    auto Cvals = std::unique_ptr<float[]>(new float[Cnnz]);
    std::copy(J, J + Cnnz,Ccols.get());
    std::copy(X, X + Cnnz,Cvals.get());

    auto Crows = std::unique_ptr<unsigned int[]>(new unsigned int[this->M + 1]);

    Crows.get()[0] = 0;
    
    for (int i = 0; i < Cnnz; i++) {
        Ccols.get()[i] = J[i];
        Cvals.get()[i] = X[i];
    }
    //csr offsets
    //test empty row
    int row_length = 0;
    
    for (int r = 0, r_id = 0; r < Cnnz || r_id < this->M;) {
        if (r < Cnnz && I[r] == r_id) {
            row_length ++;
            r++;
        } else {
            r_id++;
            Crows.get()[r_id] = Crows.get()[r_id - 1] + row_length;
            row_length = 0;    
        }
    }
    //build matricies

    delete [] (thisArows);
    delete [] (thisAcols);
    delete[] (anotherArows);
    delete[] (anotherAcols);

    delete[] (I);
    delete[] (J);
    delete[] (X);

    OK(GrB_Matrix_free(&thisA));
    OK(GrB_Matrix_free(&anotherA));
    OK(GrB_Matrix_free(&C));

    // OK(GrB_finalize ( )) ;

    return CSRWrapper(this->M,another.N,Cnnz,std::move(Cvals),std::move(Ccols),std::move(Crows));

}


template<>
CSRWrapper<bool> CSRWrapper<bool>::multiply_graphblas(const CSRWrapper<bool>& another) const{
    
    GrB_Info info ;

    //this graphblass

    GrB_Matrix thisA, anotherA, C;

    OK(GrB_Matrix_new(&thisA,GrB_BOOL,this->M,this->N));
    OK(GrB_Matrix_new(&anotherA,GrB_BOOL,another.M,another.N));
    OK(GrB_Matrix_new(&C,GrB_BOOL,this->M,another.N));
    //get tuples for row, cols, values
    //copy
    GrB_Index* thisAcols = new uint64_t[this->nnz];
    for (uint64_t i = 0; i < this->nnz; i++)
    {
        thisAcols[i] = this->cols_.get()[i];
    }
    
    const bool* thisAvals = this->values_.get();
    // csr_to_tuples
    GrB_Index* anotherAcols = new uint64_t[another.nnz];
    
    for (uint64_t i = 0; i < another.nnz; i++)
    {
        anotherAcols[i] = another.cols_.get()[i];
    }
    
    const bool* anotherAvals = another.values_.get();

    //row tuples
    uint64_t* thisArows = new uint64_t[this->nnz];
    uint64_t* anotherArows = new uint64_t[another.nnz];

    for(uint64_t i = 0, k = 0; i < this->M; i++) {

        for(unsigned int j = this->row_index_.get()[i]; j < this->row_index_.get()[i+1]; j++,k++) {
            thisArows[k] = i;
        }
    }

    for(uint64_t i = 0, k = 0; i < another.M; i++) {

        for(unsigned int j = another.row_index_.get()[i]; j < another.row_index_.get()[i+1]; j++,k++) {
            anotherArows[k] = i;
        }
    }

    GrB_BinaryOp xop = GrB_PLUS_BOOL;

    OK(GrB_Matrix_build_BOOL(thisA,(const uint64_t*) thisArows,(const uint64_t*)thisAcols,thisAvals,(uint64_t)this->nnz,xop));
    OK(GrB_Matrix_build_BOOL(anotherA,(const uint64_t*) anotherArows,(const uint64_t*)anotherAcols,anotherAvals,(uint64_t)another.nnz,xop));

    // const GrB_Semiring semiring = GxB_PLUS_TIMES_FP32;
    const GrB_Semiring semiring = GxB_LOR_LAND_BOOL;


    OK(GrB_mxm(C,NULL,NULL,semiring,thisA,anotherA,NULL));

    GrB_Index* I;
    GrB_Index* J;
    bool* X;
    GrB_Index Cnnz;
    OK( GrB_Matrix_nvals(&Cnnz,C));
    I = new GrB_Index[Cnnz];

    J = new GrB_Index[Cnnz];
    X = new bool [Cnnz];
    OK(GrB_Matrix_extractTuples_BOOL(I,J,X,&Cnnz,C));


    // std::cout << "C nnz " << Cnnz << std::endl;

    auto Ccols = std::unique_ptr<unsigned int[]>(new unsigned int [Cnnz]);
    auto Cvals = std::unique_ptr<bool[]>(new bool[Cnnz]);
    std::copy(J, J + Cnnz,Ccols.get());
    std::copy(X, X + Cnnz,Cvals.get());

    auto Crows = std::unique_ptr<unsigned int[]>(new unsigned int[this->M + 1]);

    Crows.get()[0] = 0;
    
    for (int i = 0; i < Cnnz; i++) {
        Ccols.get()[i] = J[i];
        Cvals.get()[i] = X[i];
    }
    //csr offsets
    //test empty row
    int row_length = 0;
    
    for (int r = 0, r_id = 0; r < Cnnz || r_id < this->M;) {
        if (r < Cnnz && I[r] == r_id) {
            row_length ++;
            r++;
        } else {
            r_id++;
            Crows.get()[r_id] = Crows.get()[r_id - 1] + row_length;
            row_length = 0;    
        }
    }
    

    //build matricies

    delete [] (thisArows);
    delete [] (thisAcols);
    delete[] (anotherArows);
    delete[] (anotherAcols);

    delete[] (I);
    delete[] (J);
    delete[] (X);

    OK(GrB_Matrix_free(&thisA));
    OK(GrB_Matrix_free(&anotherA));
    OK(GrB_Matrix_free(&C));

    return CSRWrapper<bool>(this->M,another.N,Cnnz,std::move(Cvals),std::move(Ccols),std::move(Crows));

}

#else

template<typename T>
CSRWrapper<T> CSRWrapper<T>::multiply_graphblas(const CSRWrapper<T>& another) const{
        
    std::vector<int> c_rows;
    std::vector<int> c_cols;
    std::vector<T> c_values;

    spGEMMGraphblast(M,N,nnz,(int*)row_index_.get(),(int*)cols_.get(),values_.get(),
                    another.M,another.N,another.nnz,(int*)another.row_index_.get(),(int*)another.cols_.get(),another.values_.get(),
                        c_rows, c_cols, c_values);

    //assume sorted
    if(c_values.size() != 0){

            auto c_rows_up = std::unique_ptr<unsigned int[]>(new unsigned int[M + 1]);
            auto c_cols_up = std::unique_ptr<unsigned int[]>(new unsigned int[c_values.size()]);
            auto c_values_up = std::unique_ptr<T[]>(new T[c_values.size()]);
        
            //reduce rows
            // std::vector<unsigned int> c_rows_reduced;
            // c_rows_reduced.push_back(0);
            c_rows_up.get()[0] = 0;
            
            for(int r_id = 0, r_size = 0, val_id = 0; r_id < M; r_id++) {
                while (val_id < c_rows.size() && c_rows[val_id] == r_id) {
                    r_size++;
                    val_id++;
                }
                // c_rows_reduced.push_back(r_size);
                c_rows_up.get()[r_id + 1] = r_size;
            }

            // assert(c_rows_reduced.size() == M + 1);
            
            std::copy(c_values.begin(),c_values.end(),c_values_up.get());
            std::copy(c_cols.begin(),c_cols.end(),c_cols_up.get());
            // std::copy(c_rows_reduced.begin(),c_rows_reduced.end(),c_rows_up.get());
            return CSRWrapper<T>(M,another.N,(const unsigned int)c_values.size(),std::move(c_values_up),std::move(c_cols_up),std::move(c_rows_up));

        } else {
            auto c_rows_up = std::unique_ptr<unsigned int[]>(nullptr);
            auto c_cols_up = std::unique_ptr<unsigned int[]>(nullptr);
            auto c_values_up = std::unique_ptr<T[]>(nullptr);
            return CSRWrapper<T>(M,another.N,(const unsigned int)c_values.size(),std::move(c_values_up),std::move(c_cols_up),std::move(c_rows_up));

        }

    
}
#endif


template<>
CSRWrapper<float> CSRWrapper<float>::subtract(const CSRWrapper<float>& another) const{

    if (N != another.N || M != another.M) throw new std::invalid_argument("Number of colons of the left oparand should match the number of rows of the right one");
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
        if (l.nnz == 0 && r.nnz == 0) return true;
        
        float maxC_ij = 0.0;
        CSRWrapper<float> C = l.subtract(r);
        
        for(int i = 0; i < C.nnz; i++) {
            float var = std::abs(C.values_[i]);
            if(var > maxC_ij){
                maxC_ij = var;
            }
        }

        return maxC_ij < 0.00001;
        
        
    }

template<typename T>
void CSRWrapper<T>::write(std::ofstream& os) const{
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
    

    auto row = row_index_.get();
    auto col = cols_.get();
    auto value = values_.get();

    for (int i = 0; i < M; i++) {
        for (int j = row[i]; j < row[i+1]; j++) {
            os << "  " << i + 1 << " " << col[j] + 1 << " " << value[j] << "\n";
        }
    }

    os << std::flush;


}

#endif