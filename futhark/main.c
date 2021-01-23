#include "sparse.h"
#include <stdio.h>
#include "GraphBLAS.h"

#include <time.h>

#ifdef BENCHMARKING
#pragma message("BENCHMARKING MODE")
#define number_of_iterations 15
#endif 
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
}\

#define OK(method)                                                      \
{                                                                       \
    info = method ;                                                     \
    if (!(info == GrB_SUCCESS || info == GrB_NO_VALUE))                 \
    {                                                                   \
        printf ("GraphBLAS error:\n%s\n", GrB_error ( )) ;              \
        CHECK (false, info) ;                                           \
    }                                                                   \
}

GB_PUBLIC
GrB_Info read_matrix        // read a double-precision matrix
(
    GrB_Matrix *A,          // handle of matrix to create
    FILE *f,                // file to read the tuples from
    bool make_symmetric,    // if true, return A as symmetric
    bool no_self_edges,     // if true, then remove self edges from A
    bool one_based,         // if true, input matrix is 1-based
    bool boolean,           // if true, input is GrB_BOOL, otherwise GrB_FP64
    bool printstuff         // if true, print status to stdout
) ;

typedef struct{
  int64_t* offsets;
  double* values;
  int64_t* columns;
  int64_t nnz;
  int64_t cols;
  int64_t rows;
} CSR;

void delete_CSR(CSR* csrA) {
  free(csrA->offsets);
  free(csrA->values);
  free(csrA->columns);
  free(csrA);
}

CSR* blas_to_csr(GrB_Matrix* A) {

  GrB_Info info;

  GrB_Index anrows, ancols, anvals;
  OK (GrB_Matrix_nrows (&anrows, *A));
  OK (GrB_Matrix_ncols (&ancols, *A));
  OK (GrB_Matrix_nvals (&anvals, *A));

  CSR* csrA = malloc(sizeof(CSR));
  csrA->nnz = anvals;
  csrA->cols = ancols;
  csrA->rows = anrows;
  csrA->columns = malloc ((anvals) * sizeof (int64_t));
  GrB_Index* I = (GrB_Index *) malloc ((anvals) * sizeof (GrB_Index)) ;
  csrA->values = (double*) malloc ((anvals) * sizeof (double)) ;
  csrA->offsets = malloc(((anrows + 1) * sizeof(int64_t)));
  csrA->offsets[0] = 0;
  
  GrB_Matrix_extractTuples_FP64(I,csrA->columns,csrA->values,&anvals,*A);

  //to csr
  int row_length = 0;
    
  for (int r = 0, r_id = 0; r < anvals || r_id < anrows;) {
        if (r < anvals && I[r] == r_id) {
            row_length ++;
            r++;
        } else {
            r_id++;
            csrA->offsets[r_id] = csrA->offsets[r_id - 1] + row_length;
            row_length = 0;    
        }
  }

  free(I);

  return csrA;

}

void print_CSR(CSR* csrA) {
  printf("values: ");
  for(int i = 0; i < csrA->nnz; i++) {
    printf("%lf ", csrA->values[i]);
  }
  printf("\n");

  printf("columns: ");
  for(int i = 0; i < csrA->nnz; i++) {
    printf("%li ", csrA->columns[i]);
  }
  printf("\n");

  printf("offsets: ");
  for(int i = 0; i < csrA->rows + 1; i++) {
    printf("%li ", csrA->offsets[i]);
  }
  printf("\n");
}


CSR* eWiseAdd_futhark(struct futhark_context* fcntx, CSR* csrA, CSR* csrB) {

  struct futhark_f64_1d* csrA_values_fut = futhark_new_f64_1d(fcntx,csrA->values,csrA->nnz);
  struct futhark_i64_1d* csrA_columns_fut = futhark_new_i64_1d(fcntx,csrA->columns,csrA->nnz);
  struct futhark_i64_1d* csrA_offsets_fut = futhark_new_i64_1d(fcntx,csrA->offsets,csrA->rows + 1);

  struct futhark_f64_1d* csrB_values_fut = futhark_new_f64_1d(fcntx,csrB->values,csrB->nnz);
  struct futhark_i64_1d* csrB_columns_fut = futhark_new_i64_1d(fcntx,csrB->columns,csrB->nnz);
  struct futhark_i64_1d* csrB_offsets_fut = futhark_new_i64_1d(fcntx,csrB->offsets,csrB->rows + 1);

  struct futhark_f64_1d* csrC_values_fut;
  struct futhark_i64_1d* csrC_columns_fut;
  struct futhark_i64_1d* csrC_offsets_fut;

  #ifdef BENCHMARKING
  double run_times[number_of_iterations] = {0};
  
  for (int i = 0; i < number_of_iterations; i++) {
    clock_t tic = clock();
  #endif

    futhark_entry_eWiseAddEntry(fcntx,
			      &csrC_values_fut,
			      &csrC_columns_fut,
			      &csrC_offsets_fut,
			      csrA_values_fut,
			      csrA_columns_fut,
			      csrA_offsets_fut,
			      csrB_values_fut,
			      csrB_columns_fut,
			      csrB_offsets_fut);

  #ifdef BENCHMARKING
    clock_t toc = clock();
    
    if (i != number_of_iterations - 1) {
      futhark_free_f64_1d(fcntx,csrC_values_fut);
      futhark_free_i64_1d(fcntx,csrC_columns_fut);
      futhark_free_i64_1d(fcntx,csrC_offsets_fut);
    }
    run_times[i] = (double)(toc - tic) / CLOCKS_PER_SEC;
  }
  double avg = 0.0;
  for(int i = 0; i < number_of_iterations;i++) {
    avg += run_times[i];
  }
  avg = avg / number_of_iterations;
  double std_dev = 0.0;
  for(int i = 0; i < number_of_iterations; i++) {
    std_dev += pow(run_times[i] - avg,2);
  }
  std_dev = sqrt(std_dev);
  printf("futhark avg runtime (seconds): %f\n", avg);
  printf("futhark std_dev (seconds): %f\n", std_dev);
  #endif

  

  // int64_t csrC_nnz = (*futhark_shape_f64_1d(fcntx,csrC_values_fut));
  // int64_t csrC_rows = (*futhark_shape_i64_1d(fcntx,csrC_offsets_fut));


  CSR* csrC = malloc(sizeof(CSR));
  csrC->nnz = (*futhark_shape_f64_1d(fcntx,csrC_values_fut));
  csrC->rows = (*futhark_shape_i64_1d(fcntx,csrC_offsets_fut)) - 1;
  csrC->cols = csrA->cols;
  csrC->values = malloc(sizeof(double) * csrC->nnz);
  csrC->columns = malloc(sizeof(int64_t) * csrC->nnz);
  csrC->offsets = malloc(sizeof(int64_t) * csrC->rows + 1);

  futhark_values_f64_1d(fcntx,csrC_values_fut,csrC->values);
  futhark_values_i64_1d(fcntx,csrC_columns_fut,csrC->columns);
  futhark_values_i64_1d(fcntx,csrC_offsets_fut,csrC->offsets);

  futhark_free_f64_1d(fcntx,csrA_values_fut);
  futhark_free_i64_1d(fcntx,csrA_columns_fut);
  futhark_free_i64_1d(fcntx,csrA_offsets_fut);

  futhark_free_f64_1d(fcntx,csrB_values_fut);
  futhark_free_i64_1d(fcntx,csrB_columns_fut);
  futhark_free_i64_1d(fcntx,csrB_offsets_fut);

  futhark_free_f64_1d(fcntx,csrC_values_fut);
  futhark_free_i64_1d(fcntx,csrC_columns_fut);
  futhark_free_i64_1d(fcntx,csrC_offsets_fut);


  return csrC;
}

CSR* eWiseAddMult_futhark(struct futhark_context* fcntx, CSR* csrA, CSR* csrB) {

  struct futhark_f64_1d* csrA_values_fut = futhark_new_f64_1d(fcntx,csrA->values,csrA->nnz);
  struct futhark_i64_1d* csrA_columns_fut = futhark_new_i64_1d(fcntx,csrA->columns,csrA->nnz);
  struct futhark_i64_1d* csrA_offsets_fut = futhark_new_i64_1d(fcntx,csrA->offsets,csrA->rows + 1);

  struct futhark_f64_1d* csrB_values_fut = futhark_new_f64_1d(fcntx,csrB->values,csrB->nnz);
  struct futhark_i64_1d* csrB_columns_fut = futhark_new_i64_1d(fcntx,csrB->columns,csrB->nnz);
  struct futhark_i64_1d* csrB_offsets_fut = futhark_new_i64_1d(fcntx,csrB->offsets,csrB->rows + 1);

  struct futhark_f64_1d* csrC_values_fut;
  struct futhark_i64_1d* csrC_columns_fut;
  struct futhark_i64_1d* csrC_offsets_fut;

  #ifdef BENCHMARKING
  double run_times[number_of_iterations] = {0};
  
  for (int i = 0; i < number_of_iterations; i++) {
    clock_t tic = clock();
  #endif

    futhark_entry_eWiseAddMultEntry(fcntx,
			      &csrC_values_fut,
			      &csrC_columns_fut,
			      &csrC_offsets_fut,
			      csrA_values_fut,
			      csrA_columns_fut,
			      csrA_offsets_fut,
			      csrB_values_fut,
			      csrB_columns_fut,
			      csrB_offsets_fut);

  #ifdef BENCHMARKING
    clock_t toc = clock();
    
    if (i != number_of_iterations - 1) {
      futhark_free_f64_1d(fcntx,csrC_values_fut);
      futhark_free_i64_1d(fcntx,csrC_columns_fut);
      futhark_free_i64_1d(fcntx,csrC_offsets_fut);
    }
    run_times[i] = (double)(toc - tic) / CLOCKS_PER_SEC;
  }
  double avg = 0.0;
  for(int i = 0; i < number_of_iterations;i++) {
    avg += run_times[i];
  }
  avg = avg / number_of_iterations;
  double std_dev = 0.0;
  for(int i = 0; i < number_of_iterations; i++) {
    std_dev += pow(run_times[i] - avg,2);
  }
  std_dev = sqrt(std_dev);
  printf("futhark avg runtime (seconds): %f\n", avg);
  printf("futhark std_dev (seconds): %f\n", std_dev);
  #endif

  

  // int64_t csrC_nnz = (*futhark_shape_f64_1d(fcntx,csrC_values_fut));
  // int64_t csrC_rows = (*futhark_shape_i64_1d(fcntx,csrC_offsets_fut));


  CSR* csrC = malloc(sizeof(CSR));
  csrC->nnz = (*futhark_shape_f64_1d(fcntx,csrC_values_fut));
  csrC->rows = (*futhark_shape_i64_1d(fcntx,csrC_offsets_fut)) - 1;
  csrC->cols = csrA->cols;
  csrC->values = malloc(sizeof(double) * csrC->nnz);
  csrC->columns = malloc(sizeof(int64_t) * csrC->nnz);
  csrC->offsets = malloc(sizeof(int64_t) * csrC->rows + 1);

  futhark_values_f64_1d(fcntx,csrC_values_fut,csrC->values);
  futhark_values_i64_1d(fcntx,csrC_columns_fut,csrC->columns);
  futhark_values_i64_1d(fcntx,csrC_offsets_fut,csrC->offsets);

  futhark_free_f64_1d(fcntx,csrA_values_fut);
  futhark_free_i64_1d(fcntx,csrA_columns_fut);
  futhark_free_i64_1d(fcntx,csrA_offsets_fut);

  futhark_free_f64_1d(fcntx,csrB_values_fut);
  futhark_free_i64_1d(fcntx,csrB_columns_fut);
  futhark_free_i64_1d(fcntx,csrB_offsets_fut);

  futhark_free_f64_1d(fcntx,csrC_values_fut);
  futhark_free_i64_1d(fcntx,csrC_columns_fut);
  futhark_free_i64_1d(fcntx,csrC_offsets_fut);


  return csrC;
}

CSR* eWiseMult_futhark(struct futhark_context* fcntx, CSR* csrA, CSR* csrB) {

  struct futhark_f64_1d* csrA_values_fut = futhark_new_f64_1d(fcntx,csrA->values,csrA->nnz);
  struct futhark_i64_1d* csrA_columns_fut = futhark_new_i64_1d(fcntx,csrA->columns,csrA->nnz);
  struct futhark_i64_1d* csrA_offsets_fut = futhark_new_i64_1d(fcntx,csrA->offsets,csrA->rows + 1);

  struct futhark_f64_1d* csrB_values_fut = futhark_new_f64_1d(fcntx,csrB->values,csrB->nnz);
  struct futhark_i64_1d* csrB_columns_fut = futhark_new_i64_1d(fcntx,csrB->columns,csrB->nnz);
  struct futhark_i64_1d* csrB_offsets_fut = futhark_new_i64_1d(fcntx,csrB->offsets,csrB->rows + 1);

  struct futhark_f64_1d* csrC_values_fut;
  struct futhark_i64_1d* csrC_columns_fut;
  struct futhark_i64_1d* csrC_offsets_fut;

  #ifdef BENCHMARKING
  double run_times[number_of_iterations] = {0};
  
  for (int i = 0; i < number_of_iterations; i++) {
    clock_t tic = clock();
  #endif

    futhark_entry_eWiseMultEntry(fcntx,
			      &csrC_values_fut,
			      &csrC_columns_fut,
			      &csrC_offsets_fut,
			      csrA_values_fut,
			      csrA_columns_fut,
			      csrA_offsets_fut,
			      csrB_values_fut,
			      csrB_columns_fut,
			      csrB_offsets_fut);

  #ifdef BENCHMARKING
    clock_t toc = clock();
    
    if (i != number_of_iterations - 1) {
      futhark_free_f64_1d(fcntx,csrC_values_fut);
      futhark_free_i64_1d(fcntx,csrC_columns_fut);
      futhark_free_i64_1d(fcntx,csrC_offsets_fut);
    }
    run_times[i] = (double)(toc - tic) / CLOCKS_PER_SEC;
  }
  double avg = 0.0;
  for(int i = 0; i < number_of_iterations;i++) {
    avg += run_times[i];
  }
  avg = avg / number_of_iterations;
  double std_dev = 0.0;
  for(int i = 0; i < number_of_iterations; i++) {
    std_dev += pow(run_times[i] - avg,2);
  }
  std_dev = sqrt(std_dev);
  printf("futhark avg runtime (seconds): %f\n", avg);
  printf("futhark std_dev (seconds): %f\n", std_dev);
  #endif

  

  // int64_t csrC_nnz = (*futhark_shape_f64_1d(fcntx,csrC_values_fut));
  // int64_t csrC_rows = (*futhark_shape_i64_1d(fcntx,csrC_offsets_fut));


  CSR* csrC = malloc(sizeof(CSR));
  csrC->nnz = (*futhark_shape_f64_1d(fcntx,csrC_values_fut));
  csrC->rows = (*futhark_shape_i64_1d(fcntx,csrC_offsets_fut)) - 1;
  csrC->cols = csrA->cols;
  csrC->values = malloc(sizeof(double) * csrC->nnz);
  csrC->columns = malloc(sizeof(int64_t) * csrC->nnz);
  csrC->offsets = malloc(sizeof(int64_t) * csrC->rows + 1);

  futhark_values_f64_1d(fcntx,csrC_values_fut,csrC->values);
  futhark_values_i64_1d(fcntx,csrC_columns_fut,csrC->columns);
  futhark_values_i64_1d(fcntx,csrC_offsets_fut,csrC->offsets);

  futhark_free_f64_1d(fcntx,csrA_values_fut);
  futhark_free_i64_1d(fcntx,csrA_columns_fut);
  futhark_free_i64_1d(fcntx,csrA_offsets_fut);

  futhark_free_f64_1d(fcntx,csrB_values_fut);
  futhark_free_i64_1d(fcntx,csrB_columns_fut);
  futhark_free_i64_1d(fcntx,csrB_offsets_fut);

  futhark_free_f64_1d(fcntx,csrC_values_fut);
  futhark_free_i64_1d(fcntx,csrC_columns_fut);
  futhark_free_i64_1d(fcntx,csrC_offsets_fut);


  return csrC;
}



int main(int argc, char** argv) {


  if(argc != 3) {
     printf("usage is executable filepath1 filepath2\n");
     return 0;
  }

  GrB_Info info;
  OK(GrB_init(GrB_BLOCKING));

  struct futhark_context_config* fcfg = futhark_context_config_new();
  struct futhark_context* fcntx = futhark_context_new(fcfg);

 
  char* msg;
  if(msg = futhark_context_get_error(fcntx)) {
    printf("%s",msg);
    free(msg);
    futhark_context_free(fcntx);
    futhark_context_config_free(fcfg);
    return 0;
    
  }

  FILE *Afile = NULL, *Bfile = NULL;
  Afile = fopen(argv[1],"r");
  Bfile = fopen(argv[2],"r");
  if(Afile == NULL || Bfile == NULL) {
    printf("Cannot open specified files\n");
    fclose(Afile);
    fclose(Bfile);
    return 0;
  }
  GrB_Matrix A = NULL;
  GrB_Matrix B = NULL;

  OK (read_matrix(&A, Afile, false, false, true, false, false));
  OK (read_matrix(&B, Bfile, false, false, true, false, false));
  
  fclose(Afile);
  fclose(Bfile);

  GrB_Index anrows, ancols, anvals;
  OK (GrB_Matrix_nrows (&anrows, A));
  OK (GrB_Matrix_ncols (&ancols, A));
  OK (GrB_Matrix_nvals (&anvals, A));
  GrB_Index bnvals;
  OK (GrB_Matrix_nvals(&bnvals,B));
  printf("csrA's nnz: %li \n", anvals);
  printf("csrB's nnz: %li \n", bnvals);

  GrB_Matrix C = NULL;
  GrB_Matrix D = NULL;
  GrB_Matrix_new (&C, GrB_FP64, anrows, ancols);
  GrB_Matrix_new (&D, GrB_FP64, anrows, ancols);

  #ifdef BENCHMARKING
  double run_times[number_of_iterations] = {0};
  
  for (int i = 0; i < number_of_iterations; i++) {
    if(i > 0) {
      GrB_Matrix_new (&C, GrB_FP64, anrows, ancols);
      GrB_Matrix_new (&D, GrB_FP64, anrows, ancols);
    }
    clock_t tic = clock();
  #endif

  GrB_eWiseAdd (C, NULL, NULL, GrB_PLUS_FP64, A, B, NULL) ;
  GrB_eWiseMult (D, NULL, NULL, GrB_TIMES_FP64, C, B, NULL) ;


  #ifdef BENCHMARKING
  // GrB_eWiseMult (C, NULL, NULL, GrB_TIMES_FP64, A, B, NULL) ;
    clock_t toc = clock();
    if(i!= number_of_iterations - 1) OK(GrB_Matrix_free(&C));
    if(i!= number_of_iterations - 1) OK(GrB_Matrix_free(&D));
    run_times[i] = (double)(toc - tic) / CLOCKS_PER_SEC;
  }
  double avg = 0.0;
  for(int i = 0; i < number_of_iterations;i++) {
    avg += run_times[i];
  }
  avg = avg / number_of_iterations;
  double std_dev = 0.0;
  for(int i = 0; i < number_of_iterations; i++) {
    std_dev += pow(run_times[i] - avg,2);
  }
  std_dev = sqrt(std_dev);
  printf("Suite sparse avg runtime (seconds): %f\n", avg);
  printf("Suite sparse std_dev (seconds): %f\n", std_dev);
  #endif

  // GxB_Matrix_fprint(C,"csrC", GxB_COMPLETE, stdout);
  // CSR* csrCblas = blas_to_csr(&C);
  // print_CSR(csrCblas);
  // delete_CSR(csrCblas);
  GrB_Index cnvals;
  OK(GrB_Matrix_nvals (&cnvals, D));
  printf("SuiteSparse's csrC nnz: %li\n", cnvals);
  
  CSR* csrA = blas_to_csr(&A);
  CSR* csrB = blas_to_csr(&B);
  
  CSR* csrC = eWiseAdd_futhark(fcntx,csrA,csrB);
  CSR* csrD = eWiseMult_futhark(fcntx,csrC,csrB);
  printf("futhark csrC nnz: %li\n", csrD->nnz);
  
  delete_CSR(csrA);
  delete_CSR(csrB);
  delete_CSR(csrC);
  delete_CSR(csrD);
  

  futhark_context_free(fcntx);
  futhark_context_config_free(fcfg);
  
  
  OK(GrB_Matrix_free(&A));
  OK(GrB_Matrix_free(&B));
  OK(GrB_Matrix_free(&C));
  OK(GrB_Matrix_free(&D));

  OK(GrB_finalize());

  return 0;
}
