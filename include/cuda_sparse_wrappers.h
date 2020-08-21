#ifndef CUDA_SPARSE_WRAPPERS_H
#define CUDA_SPARSE_WRAPPERS_H

#include "sparse.h"

int spGEMMCuda(struct CSR* a, struct CSR* b, struct CSR* c);

int spGEMMCusparse(struct CSR* a, struct CSR* b, struct CSR* c);


#endif