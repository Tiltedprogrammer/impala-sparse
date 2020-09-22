#include <stdio.h>

#include "cuda_sparse_wrappers.h"

#include <cuda_runtime.h> // cudaMalloc, cudaMemcpy, etc.
#include <cusparse.h>   
#include "cuda_helpers.h"

//implement scan?
extern "C"{
    void prefixSum(unsigned int * csrOffsetsStart,unsigned int csrOffsetsEnd) {
        thrust::inclusive_scan(thrust::device, csrOffsetsStart, csrOffsetsStart + csrOffsetsEnd, csrOffsetsStart); // in-place scan
    }
}

#define CHECK_CUDA(func)                                                       \
{                                                                              \
    cudaError_t status = (func);                                               \
    if (status != cudaSuccess) {                                               \
        printf("CUDA API failed at line %d with error: %s (%d)\n",             \
               __LINE__, cudaGetErrorString(status), status);                  \
        return EXIT_FAILURE;                                                   \
    }                                                                          \
}

__device__ int min_(int l, int r){
    if (r == 0) return l;
    else if(l == 0) return r;
    else{
        if(l < r) return l;
        else return r;
    }
}


__device__ int getGlobalIdx_2D_1D(){
    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;
    return threadId;
    }

//think about minimum
//SHUFFLE UP OR make all threads run through for
__device__ int WarpMin (unsigned int mask,int value, int laneId ){
        for (int offset = 16; offset > 0; offset/=2)
        value = min_(value,__shfl_up_sync(mask, value, offset));
     return value ;
}

__device__ int WarpMax (unsigned int mask,int value){
    
    for (int offset = 8; offset > 0; offset *=2)
    
    value = max(value,__shfl_down_sync(mask, value, offset));
     return value ;
}

__device__ float WarpSum (unsigned int mask,float value, int laneId){
    for (int offset = 16; offset > 0; offset/=2) 
        value += __shfl_down_sync(mask, value, offset);
     return value ;
}


//maybe indexing from 1

__device__ int nextLaneId(int laneId, unsigned int mask){
    int laneIdNum = 1 << (laneId); // laneId == 1 => mask = 000..10
    int laneMask = laneIdNum * 2 - 1;
    int nextLaneId = __ffs(mask & (~laneMask)) - 1;
    return nextLaneId;
}

//C.row[warpId] = ... =>

//make for any to keep threads running, tmp will be the same across threads

// 1 0 2 3      1 0 2 3
// 0 4 0 0   *  0 4 0 0
// 5 0 6 7      5 0 6 7
// 0 8 0 9      0 8 0 9

__global__ void preproccessRows(struct CSR a, struct CSR b, struct CSR c){
    
    #define FULL_MASK 0xffffffff

    int threadId = getGlobalIdx_2D_1D();
    
    int warpId = threadId / 32; //is rowId
    int laneId = threadIdx.x % 32;

    if(warpId < a.M) {

        int aStart = a.row_index[warpId];
        int aEnd = a.row_index[warpId + 1];

        // unsigned int mask = 0;
        unsigned int mask = __ballot_sync(FULL_MASK, (aStart + laneId < aEnd));
        int tmp = 0;
        // int frontCol = 0;
        
        if(aStart + laneId < aEnd){
            int bRowId = a.cols[aStart + laneId];
            
            //elems are adjacent with the mask
            
            for(int rowPos = b.row_index[bRowId]; __any_sync(mask,rowPos < b.row_index[bRowId+1]);tmp++){

                // mask = __ballot_sync(FULL_MASK, (aStart + laneId < aEnd) && (rowPos < b.row_index[bRowId+1]));
                
                int frontCol = rowPos < b.row_index[bRowId+1] ? (b.cols[rowPos] + 1) : 0;

                // printf("WarpId=%i LaneId=%i= frontCol=%i rowPos=%i\n",warpId,laneId,frontCol,rowPos);
                // if (rowPos < b.row_index[bRowId+1]){
                    // frontCol = b.cols[rowPos] + 1;
                // } else {
                    // frontCol = 0;
                // }

                // printf("WarpId=%i, LanedId=%i,Mask=%i, number of 1's=%i\n",warpId,laneId,mask,__popc(mask));
                // int frontCol = b.cols[rowPos] + 1; //+1 to find minimum correctly
                // float frontVal = b.values[rowPos];
                
                //find minimum frontCol ???????
                int frontColMin = WarpMin(mask,frontCol,laneId);
                frontColMin = __shfl_sync(mask,frontColMin,__popc(mask)-1);

                // printf("WarpId=%i, LaneId=%i, frontCol=%i, frontColMin=%i\n",warpId,laneId,frontCol,frontColMin);
                
                if(frontCol == frontColMin) {
                    rowPos++;
                }

            }
            
        }
        if(laneId == 0){
            if (mask == 0){ //empty row
                c.row_index[warpId+1] = 0;    
            } else {
                c.row_index[warpId+1] = tmp;    
            }
            
        }
    }
    
}

__global__ void spGEMMDevice(struct CSR a, struct CSR b, struct CSR c) {
    #define FULL_MASK 0xffffffff
    
    int threadId = getGlobalIdx_2D_1D();
    int warpId = threadId / 32; //is rowId
    int laneId = threadIdx.x % 32;

    if(warpId < a.M) {

        int aStart = a.row_index[warpId];
        int aEnd = a.row_index[warpId + 1];

        unsigned int mask = 0;
        float tmp = 0.0;
        
        if(aStart + laneId < aEnd){
            
            int bRowId = a.cols[aStart + laneId];
            float rowWeight = a.values[aStart + laneId];

            mask = __ballot_sync(FULL_MASK, (aStart + laneId < aEnd));


            int currentCol = 0;
            
            for(int rowPos = b.row_index[bRowId]; __any_sync(mask,rowPos < b.row_index[bRowId+1]); currentCol++){

                // mask = __ballot_sync(FULL_MASK, (aStart + laneId < aEnd) && (rowPos < b.row_index[bRowId+1]));
                
                
                //int frontCol = b.cols[rowPos] + 1; //+1 to find minimum correctly
                //float frontVal = b.values[rowPos];
                int frontCol = rowPos < b.row_index[bRowId+1] ? (b.cols[rowPos] + 1) : 0;
                float frontVal = rowPos < b.row_index[bRowId+1] ? (b.values[rowPos]) : 0.0f;
                //find minimum frontCol
                int frontColMin = WarpMin(mask,frontCol,laneId);
                frontColMin = __shfl_sync(mask,frontColMin,__popc(mask)-1);

                // printf("WarpId=%i, LaneId=%i, frontCol=%i, frontColMin=%i\n",warpId,laneId,frontCol,frontColMin);

                if(frontCol == frontColMin) {
                    rowPos++;
                    tmp = frontVal * rowWeight;
                    // printf("LaneID=%i, a=%f,b=%f,tmp=%f\n",laneId,rowWeight,frontVal,tmp);
                } else {
                    tmp = 0.0;
                }

                float sum = WarpSum(mask,tmp,laneId);
                sum = __shfl_sync(mask,sum,0); //broadcast
                
                if(laneId == 0){
                    // printf("LaneId=%i\n",laneId);
                    // printf("WARP=%i WRITING COL=%i for %i pos, current COL=%i\n",warpId,frontColMin-1,c.row_index[warpId]+currentCol,currentCol);
                    c.values[c.row_index[warpId]+currentCol] = sum;
                    c.cols[c.row_index[warpId]+currentCol] = frontColMin - 1;
                }

            }
            
        }
    }
}

//not more than 65000 rows

int spGEMMCuda(struct CSR* a, struct CSR* b, struct CSR* c){
    
    struct CSR aDevice = {N: a-> N, M: a->M, nnz : a->nnz};
    struct CSR bDevice = {N: b-> N, M: b->M, nnz : b->nnz};
    struct CSR cDevice = {N: b-> N, M : a->M};

    c-> N = b-> N;
    c-> M = a-> M;

    unsigned int   *dA_csrOffsets, *dA_columns, *dB_csrOffsets, *dB_columns, *dC_csrOffsets, *dC_columns;
    float *dA_values, *dB_values, *dC_values;
    
    CHECK_CUDA( cudaMalloc((void**) &dA_csrOffsets,
                            (aDevice.M + 1) * sizeof(int)) )
    CHECK_CUDA( cudaMalloc((void**) &dA_columns, aDevice.nnz * sizeof(int))   )
    CHECK_CUDA( cudaMalloc((void**) &dA_values,  aDevice.nnz * sizeof(float)) )
    // allocate B
    CHECK_CUDA( cudaMalloc((void**) &dB_csrOffsets,
                            (bDevice.M + 1) * sizeof(int)) )
    CHECK_CUDA( cudaMalloc((void**) &dB_columns, bDevice.nnz * sizeof(int))   )
    CHECK_CUDA( cudaMalloc((void**) &dB_values,  bDevice.nnz * sizeof(float)) )
    // allocate C offsets
    CHECK_CUDA( cudaMalloc((void**) &dC_csrOffsets,
                            (cDevice.M + 1) * sizeof(int)) )

    //copy 
    CHECK_CUDA( cudaMemcpy(dA_csrOffsets, a->row_index,
                                (a->M + 1) * sizeof(int),
                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dA_columns, a->cols, a->nnz * sizeof(int),
                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dA_values, a->values,
                                                a->nnz * sizeof(float), cudaMemcpyHostToDevice) )
                        // copy B
    CHECK_CUDA( cudaMemcpy(dB_csrOffsets, b->row_index,
                                                (b->M + 1) * sizeof(int),
                                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dB_columns, b->cols, b->nnz * sizeof(int),
                                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dB_values, b->values,
                                                b->nnz * sizeof(float), cudaMemcpyHostToDevice) )

    CHECK_CUDA( cudaMemset(dC_csrOffsets,0,(cDevice.M + 1) * sizeof(int)));

    // int block = a->M;

    dim3 grid(1,1,1);

    //
    int k = (a->M / 65535) + 1;
    if (k > 1) {
        grid.x = 65535;
        if(k > 65535){
            grid.y = 65535;
        } else {
            grid.y = k;
        }
        
    } else {
        grid.x = a->M;
    }

    aDevice.cols = dA_columns;
    aDevice.row_index = dA_csrOffsets;
    aDevice.values = dA_values;

    bDevice.cols = dB_columns;
    bDevice.row_index = dB_csrOffsets;
    bDevice.values = dB_values;

    cDevice.row_index = dC_csrOffsets;

    preproccessRows<<<grid,32>>>(aDevice,bDevice,cDevice);
    cudaDeviceSynchronize();
    
    // thrust::inclusive_scan(thrust::Tracy Ellisdevice, dC_csrOffsets, dC_csrOffsets + cDevice.M + 1, dC_csrOffsets); // in-place scan
    prefixSum(dC_csrOffsets, cDevice.M + 1);
    // unsigned int crows = 0;
    CHECK_CUDA( cudaMemcpy(&(c->nnz), dC_csrOffsets+c->M,
        (1) * sizeof(int), cudaMemcpyDeviceToHost) )

    //if zero return empty matrix;

    CHECK_CUDA( cudaMalloc((void**) &dC_columns, c->nnz * sizeof(int))   )
    CHECK_CUDA( cudaMalloc((void**) &dC_values,  c->nnz * sizeof(float)) )

    cDevice.values = dC_values;
    cDevice.cols = (unsigned int*)dC_columns;
    
    
    spGEMMDevice<<<grid,32>>>(aDevice,bDevice,cDevice);

    cudaDeviceSynchronize();

    //allocate C;
    unsigned int* hC_csrOffsets = new unsigned int[c->M + 1];
    unsigned int* hC_columns = new unsigned int[c->nnz];
    float* hC_values = new float[c->nnz];

    c->row_index = hC_csrOffsets;
    c->cols = hC_columns;
    c->values = hC_values;

    CHECK_CUDA( cudaMemcpy(c->row_index, dC_csrOffsets, (c->M + 1)* sizeof(int),
                                               cudaMemcpyDeviceToHost) )

    CHECK_CUDA( cudaMemcpy(c->cols, dC_columns, c->nnz * sizeof(int),
                                               cudaMemcpyDeviceToHost) )
    CHECK_CUDA( cudaMemcpy(c->values, dC_values,
                                               c->nnz * sizeof(float), cudaMemcpyDeviceToHost) )

    CHECK_CUDA( cudaFree(dA_csrOffsets) )
    CHECK_CUDA( cudaFree(dA_columns) )
    CHECK_CUDA( cudaFree(dA_values) )
    CHECK_CUDA( cudaFree(dB_csrOffsets) )
    CHECK_CUDA( cudaFree(dB_columns) )
    CHECK_CUDA( cudaFree(dB_values) )
    CHECK_CUDA( cudaFree(dC_csrOffsets) )
    CHECK_CUDA( cudaFree(dC_columns) )
    CHECK_CUDA( cudaFree(dC_values) )

    return EXIT_SUCCESS;

}



int spGEMMCusparse(struct CSR* a, struct CSR* b, struct CSR* c){
    
    struct CSR aDevice = {N: a-> N, M: a->M, nnz : a->nnz};
    struct CSR bDevice = {N: b-> N, M: b->M, nnz : b->nnz};
    struct CSR cDevice = {N: b-> N, M : a->M};

    c-> N = b-> N;
    c-> M = a-> M;

    unsigned int   *dA_csrOffsets, *dA_columns, *dB_csrOffsets, *dB_columns, *dC_csrOffsets, *dC_columns;
    float *dA_values, *dB_values, *dC_values;

    float               alpha       = 1.0f;
    float               beta        = 0.0f;
    cusparseOperation_t opA         = CUSPARSE_OPERATION_NON_TRANSPOSE; //only this is supported
    cusparseOperation_t opB         = CUSPARSE_OPERATION_NON_TRANSPOSE; //
    cudaDataType        computeType = CUDA_R_32F; //32-bit IEEE-754 real float
    
    CHECK_CUDA( cudaMalloc((void**) &dA_csrOffsets,
                            (aDevice.M + 1) * sizeof(int)) )
    CHECK_CUDA( cudaMalloc((void**) &dA_columns, aDevice.nnz * sizeof(int))   )
    CHECK_CUDA( cudaMalloc((void**) &dA_values,  aDevice.nnz * sizeof(float)) )
    // allocate B
    CHECK_CUDA( cudaMalloc((void**) &dB_csrOffsets,
                            (bDevice.M + 1) * sizeof(int)) )
    CHECK_CUDA( cudaMalloc((void**) &dB_columns, bDevice.nnz * sizeof(int))   )
    CHECK_CUDA( cudaMalloc((void**) &dB_values,  bDevice.nnz * sizeof(float)) )
    // allocate C offsets
    CHECK_CUDA( cudaMalloc((void**) &dC_csrOffsets,
                            (cDevice.M + 1) * sizeof(int)) )

    //copy 
    CHECK_CUDA( cudaMemcpy(dA_csrOffsets, a->row_index,
                                (a->M + 1) * sizeof(int),
                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dA_columns, a->cols, a->nnz * sizeof(int),
                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dA_values, a->values,
                                                a->nnz * sizeof(float), cudaMemcpyHostToDevice) )
                        // copy B
    CHECK_CUDA( cudaMemcpy(dB_csrOffsets, b->row_index,
                                                (b->M + 1) * sizeof(int),
                                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dB_columns, b->cols, b->nnz * sizeof(int),
                                                cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dB_values, b->values,
                                                b->nnz * sizeof(float), cudaMemcpyHostToDevice) )

    CHECK_CUDA( cudaMemset(dC_csrOffsets,0,(cDevice.M + 1) * sizeof(int)));

    aDevice.cols = dA_columns;
    aDevice.row_index = dA_csrOffsets;
    aDevice.values = dA_values;

    bDevice.cols = dB_columns;
    bDevice.row_index = dB_csrOffsets;
    bDevice.values = dB_values;

    cDevice.row_index = dC_csrOffsets;

    //--------------------------------------------------------------------------
    // CUSPARSE APIs
    cusparseHandle_t     handle = NULL;
    cusparseSpMatDescr_t matA, matB, matC;
    void*  dBuffer1    = NULL, *dBuffer2   = NULL;
    size_t bufferSize1 = 0,    bufferSize2 = 0;
    CHECK_CUSPARSE( cusparseCreate(&handle) )
    // Create sparse matrix A in CSR format
    CHECK_CUSPARSE( cusparseCreateCsr(&matA, aDevice.M, aDevice.N, aDevice.nnz,
                                      dA_csrOffsets, dA_columns, dA_values,
                                      CUSPARSE_INDEX_32I, CUSPARSE_INDEX_32I,
                                      CUSPARSE_INDEX_BASE_ZERO, CUDA_R_32F) )
    CHECK_CUSPARSE( cusparseCreateCsr(&matB, bDevice.M, bDevice.N, bDevice.nnz,
                                      dB_csrOffsets, dB_columns, dB_values,
                                      CUSPARSE_INDEX_32I, CUSPARSE_INDEX_32I,
                                      CUSPARSE_INDEX_BASE_ZERO, CUDA_R_32F) )
    CHECK_CUSPARSE( cusparseCreateCsr(&matC, aDevice.M, bDevice.N, 0,
                                      NULL, NULL, NULL,
                                      CUSPARSE_INDEX_32I, CUSPARSE_INDEX_32I,
                                      CUSPARSE_INDEX_BASE_ZERO, CUDA_R_32F) )
    //--------------------------------------------------------------------------

    // SpGEMM Computation
    cusparseSpGEMMDescr_t spgemmDesc;
    CHECK_CUSPARSE( cusparseSpGEMM_createDescr(&spgemmDesc) )

    // ask bufferSize1 bytes for external memory
    CHECK_CUSPARSE(
        cusparseSpGEMM_workEstimation(handle, opA, opB,
                                      &alpha, matA, matB, &beta, matC,
                                      computeType, CUSPARSE_SPGEMM_DEFAULT,
                                      spgemmDesc, &bufferSize1, NULL) )
    CHECK_CUDA( cudaMalloc((void**) &dBuffer1, bufferSize1) )
    // inspect the matrices A and B to understand the memory requiremnent for
    // the next step
    CHECK_CUSPARSE(
        cusparseSpGEMM_workEstimation(handle, opA, opB,
                                      &alpha, matA, matB, &beta, matC,
                                      computeType, CUSPARSE_SPGEMM_DEFAULT,
                                      spgemmDesc, &bufferSize1, dBuffer1) )

    // ask bufferSize2 bytes for external memory
    CHECK_CUSPARSE(
        cusparseSpGEMM_compute(handle, opA, opB,
                               &alpha, matA, matB, &beta, matC,
                               computeType, CUSPARSE_SPGEMM_DEFAULT,
                               spgemmDesc, &bufferSize2, NULL) )
    CHECK_CUDA( cudaMalloc((void**) &dBuffer2, bufferSize2) )

    // compute the intermediate product of A * B
    CHECK_CUSPARSE( cusparseSpGEMM_compute(handle, opA, opB,
                                           &alpha, matA, matB, &beta, matC,
                                           computeType, CUSPARSE_SPGEMM_DEFAULT,
                                           spgemmDesc, &bufferSize2, dBuffer2) )
    // get matrix C non-zero entries C_num_nnz1
    int64_t C_num_rows1, C_num_cols1, C_num_nnz1;
    CHECK_CUSPARSE( cusparseSpMatGetSize(matC, &C_num_rows1, &C_num_cols1,
                                         &C_num_nnz1) )
    // allocate matrix C
    CHECK_CUDA( cudaMalloc((void**) &dC_columns, C_num_nnz1 * sizeof(int))   )
    CHECK_CUDA( cudaMalloc((void**) &dC_values,  C_num_nnz1 * sizeof(float)) )
    // update matC with the new pointers
    CHECK_CUSPARSE(
        cusparseCsrSetPointers(matC, dC_csrOffsets, dC_columns, dC_values) )

    // copy the final products to the matrix C
    CHECK_CUSPARSE(
        cusparseSpGEMM_copy(handle, opA, opB,
                            &alpha, matA, matB, &beta, matC,
                            computeType, CUSPARSE_SPGEMM_DEFAULT, spgemmDesc) )

    // destroy matrix/vector descriptors

    CHECK_CUSPARSE( cusparseSpGEMM_destroyDescr(spgemmDesc) )
    CHECK_CUSPARSE( cusparseDestroySpMat(matA) )
    CHECK_CUSPARSE( cusparseDestroySpMat(matB) )
    CHECK_CUSPARSE( cusparseDestroySpMat(matC) )
    CHECK_CUSPARSE( cusparseDestroy(handle) )
    

    
    cDevice.values = dC_values;
    cDevice.cols = (unsigned int*)dC_columns;

    c->nnz = C_num_nnz1;
    
    
    
    //allocate C;
    unsigned int* hC_csrOffsets = new unsigned int[c->M + 1];
    unsigned int* hC_columns = new unsigned int[c->nnz];
    float* hC_values = new float[c->nnz];

    c->row_index = hC_csrOffsets;
    c->cols = hC_columns;
    c->values = hC_values;

    CHECK_CUDA( cudaMemcpy(c->row_index, dC_csrOffsets, (c->M + 1)* sizeof(int),
                                               cudaMemcpyDeviceToHost) )

    CHECK_CUDA( cudaMemcpy(c->cols, dC_columns, c->nnz * sizeof(int),
                                               cudaMemcpyDeviceToHost) )
    CHECK_CUDA( cudaMemcpy(c->values, dC_values,
                                               c->nnz * sizeof(float), cudaMemcpyDeviceToHost) )

    
    CHECK_CUDA( cudaFree(dBuffer1) )
    CHECK_CUDA( cudaFree(dBuffer2) )                                            
    CHECK_CUDA( cudaFree(dA_csrOffsets) )
    CHECK_CUDA( cudaFree(dA_columns) )
    CHECK_CUDA( cudaFree(dA_values) )
    CHECK_CUDA( cudaFree(dB_csrOffsets) )
    CHECK_CUDA( cudaFree(dB_columns) )
    CHECK_CUDA( cudaFree(dB_values) )
    CHECK_CUDA( cudaFree(dC_csrOffsets) )
    CHECK_CUDA( cudaFree(dC_columns) )
    CHECK_CUDA( cudaFree(dC_values) )

    return EXIT_SUCCESS;

}
//find minimum
//find sum

// int main(int argc, char** argv){
    
//     const unsigned int A_num_rows = 4;
//     const unsigned int A_num_cols = 4;
//     const unsigned int A_num_nnz  = 9;
//     const unsigned int B_num_rows = 4;
//     const unsigned int B_num_cols = 4;
//     const unsigned int B_num_nnz  = 8;
//     unsigned int   hA_csrOffsets[] = { 0, 3, 4, 7, 9 };
//     unsigned int   hA_columns[]    = { 0, 2, 3, 1, 0, 2, 3, 1, 3 };
//     float hA_values[]     = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
//                               6.0f, 7.0f, 8.0f, 9.0f };
//     // int   hA_csrOffsets[] = { 0, 3, 4, 7, 9 };
//     // int   hA_csrOffsets[] = { 0, 3, 3, 7, 8 };
//     // int   hA_columns[]    = { 0, 2, 3, 2, 0, 2, 3, 1, 3 };
//     // float hA_values[]     = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
//                             //   6.0f, 7.0f, 8.0f, 9.0f };
//     unsigned int   hB_csrOffsets[] = { 0, 2, 4, 7, 8 };
//     unsigned int   hB_columns[]    = { 0, 3, 1, 3, 0, 1, 2, 1 };
//     float hB_values[]     = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
//                               6.0f, 7.0f, 8.0f };
//     unsigned int   hC_csrOffsetsTest[] = { 0, 4, 6, 10, 12 };
//     // int hC_csrOffsetsTest[5] = {0};
//     unsigned int   hC_columns[]    = { 0, 1, 2, 3, 1, 3, 0, 1, 2, 3, 1, 3 };
//     float hC_values[]     = { 11.0f, 36.0f, 14.0f, 2.0f,  12.0f,
//                               16.0f, 35.0f, 92.0f, 42.0f, 10.0f,
//                               96.0f, 32.0f };

//     struct CSR csrA = {N : A_num_cols, M : A_num_rows, nnz : A_num_nnz, values : hA_values, cols : hA_columns, row_index : hA_csrOffsets };
//     struct CSR csrB = {N : B_num_cols, M : B_num_rows, nnz : B_num_nnz, values : hB_values, cols : hB_columns, row_index : hB_csrOffsets };
//     struct CSR csrC = {};

//     spGEMMCuda(&csrA,&csrB,&csrC);


//     for(int i = 0; i < csrC.nnz; i++) {
//         printf("%f ",csrC.values[i]);
//     }
//     printf("\n");

//     delete[] csrC.values;
//     delete[] csrC.cols;
//     delete[] csrC.row_index;

    
//     // int   *dA_csrOffsets, *dA_columns, *dB_csrOffsets, *dB_columns, *dC_csrOffsets, *dC_columns;
//     // float *dA_values, *dB_values, *dC_values;
//     // // allocate A
//     // CHECK_CUDA( cudaMalloc((void**) &dA_csrOffsets,
//     //                         (A_num_rows + 1) * sizeof(int)) )
//     // CHECK_CUDA( cudaMalloc((void**) &dA_columns, A_num_nnz * sizeof(int))   )
//     // CHECK_CUDA( cudaMalloc((void**) &dA_values,  A_num_nnz * sizeof(float)) )
//     // // allocate B
//     // CHECK_CUDA( cudaMalloc((void**) &dB_csrOffsets,
//     //                         (B_num_rows + 1) * sizeof(int)) )
//     // CHECK_CUDA( cudaMalloc((void**) &dB_columns, B_num_nnz * sizeof(int))   )
//     // CHECK_CUDA( cudaMalloc((void**) &dB_values,  B_num_nnz * sizeof(float)) )
//     // // allocate C offsets
//     // CHECK_CUDA( cudaMalloc((void**) &dC_csrOffsets,
//     //                         (A_num_rows + 1) * sizeof(int)) )
                    
// // copy A
//     // CHECK_CUDA( cudaMemcpy(dA_csrOffsets, hA_csrOffsets,
//     //                     (A_num_rows + 1) * sizeof(int),
//     //                     cudaMemcpyHostToDevice) )
//     // CHECK_CUDA( cudaMemcpy(dA_columns, hA_columns, A_num_nnz * sizeof(int),
//     //                     cudaMemcpyHostToDevice) )
//     // CHECK_CUDA( cudaMemcpy(dA_values, hA_values,
//     //                                            A_num_nnz * sizeof(float), cudaMemcpyHostToDevice) )
//     //                     // copy B
//     // CHECK_CUDA( cudaMemcpy(dB_csrOffsets, hB_csrOffsets,
//     //                                            (B_num_rows + 1) * sizeof(int),
//     //                                            cudaMemcpyHostToDevice) )
//     // CHECK_CUDA( cudaMemcpy(dB_columns, hB_columns, B_num_nnz * sizeof(int),
//     //                                            cudaMemcpyHostToDevice) )
//     // CHECK_CUDA( cudaMemcpy(dB_values, hB_values,
//     //                                            B_num_nnz * sizeof(float), cudaMemcpyHostToDevice) )

//     // CHECK_CUDA( cudaMemset(dC_csrOffsets,0,(A_num_rows + 1) * sizeof(int)));
    
//     // struct CSR csrA = CSR{N : A_num_cols, M : A_num_rows, nnz : A_num_nnz, values : dA_values, cols : (unsigned int*)dA_columns, row_index : (unsigned int*)dA_csrOffsets};
//     // struct CSR csrB = CSR{N : B_num_cols, M : B_num_rows, nnz : B_num_nnz, values : dB_values, cols : (unsigned int*)dB_columns, row_index : (unsigned int*)dB_csrOffsets};
//     // struct CSR csrC = CSR{N : B_num_cols, M : A_num_rows, nnz : 0, values : NULL, cols : NULL, row_index : (unsigned int*)dC_csrOffsets};

//     // preproccessRows<<<4,32>>>(csrA,csrB,csrC);
//     // cudaDeviceSynchronize();
    
//     // thrust::inclusive_scan(thrust::device, dC_csrOffsets, dC_csrOffsets + A_num_rows + 1, dC_csrOffsets); // in-place scan

//     // CHECK_CUDA( cudaMemcpy(hC_csrOffsetsTest, dC_csrOffsets,
//     //     (A_num_rows + 1) * sizeof(int), cudaMemcpyDeviceToHost) )

//     // // allocate C
//     // csrC.nnz = hC_csrOffsetsTest[A_num_rows];
//     // printf("C's nnz=%i\n",csrC.nnz);
//     // CHECK_CUDA( cudaMalloc((void**) &dC_columns, csrC.nnz * sizeof(int))   )
//     // CHECK_CUDA( cudaMalloc((void**) &dC_values,  csrC.nnz * sizeof(float)) )

//     // csrC.values = dC_values;
//     // csrC.cols = (unsigned int*)dC_columns;
    
//     // for(int i = 0; i < A_num_rows + 1; i++) {
//     //     printf("%i ",hC_csrOffsetsTest[i]);
//     // }
//     // printf("\n");

//     // spGEMMDevice<<<1,128>>>(csrA,csrB,csrC);

//     // CHECK_CUDA( cudaMemcpy(hC_columns, dC_columns, csrC.nnz * sizeof(int),
//     //                                            cudaMemcpyDeviceToHost) )
//     // CHECK_CUDA( cudaMemcpy(hC_values, dC_values,
//     //                                            csrC.nnz * sizeof(float), cudaMemcpyDeviceToHost) )

//     // for(int i = 0; i < csrC.nnz; i++) {
//     //     printf("%f ",hC_values[i]);
//     // }
//     // printf("\n");

//     // CHECK_CUDA( cudaFree(dA_csrOffsets) )
//     // CHECK_CUDA( cudaFree(dA_columns) )
//     // CHECK_CUDA( cudaFree(dA_values) )
//     // CHECK_CUDA( cudaFree(dB_csrOffsets) )
//     // CHECK_CUDA( cudaFree(dB_columns) )
//     // CHECK_CUDA( cudaFree(dB_values) )
//     // CHECK_CUDA( cudaFree(dC_csrOffsets) )
//     // CHECK_CUDA( cudaFree(dC_columns) )
//     // CHECK_CUDA( cudaFree(dC_values) )

//     // copy C back

//     //todo make wrapper and write tests

// }