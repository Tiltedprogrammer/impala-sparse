#define GRB_USE_CUDA
#define private public
#include <iostream>
#include <algorithm>
#include <string>

#include <cstdio>
#include <cstdlib>

#include <boost/program_options.hpp>
#include "graphblas/graphblas.hpp"
#include "graphblas/algorithm/sssp.hpp"
#include "graphblas/algorithm/common.hpp"
#include "test/test.hpp"

int main( int argc, char** argv )
{
  bool DEBUG = true;

  std::vector<graphblas::Index> a_row_indices, b_row_indices;
  std::vector<graphblas::Index> a_col_indices, b_col_indices;
  std::vector<float> a_values, b_values;
  graphblas::Index a_num_rows, a_num_cols, a_num_edges;
  graphblas::Index b_num_rows, b_num_cols, b_num_edges;
  char* dat_name;
  graphblas::Descriptor desc;

  // Load A
  std::cout << "loading A" << std::endl;
  readMtx("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/sssp/roadNet-CA.mtx", &a_row_indices, &a_col_indices,
      &a_values, &a_num_rows, &a_num_cols, &a_num_edges, 0, false);
  graphblas::Matrix<float> a(a_num_rows, a_num_cols);

  std::cout << a_num_edges << " " << a_num_rows << std::endl;
//   std::cout << std::endl;
  a.build(&a_row_indices, &a_col_indices, &a_values, a_num_edges, GrB_NULL);
  // a.print();
  // Vector v
  graphblas::Vector<float> v(a_num_rows);
  
  graphblas::algorithm::sssp(&v, &a, 0, &desc);
  std::vector<float> h_sssp_gpu;
  CHECK(v.extractTuples(&h_sssp_gpu, &a_num_rows));
  graphblas::Index v_nnz;
  v.nvals(&v_nnz);
  std::cout << v_nnz << std::endl;
  // v.print();
  // for(auto e : h_sssp_gpu) {
  //   std::cout << e << " ";
  // }
// //   if(DEBUG) a.print();


}
