#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>


#include <chrono>

 extern "C" {
#include "GraphBLAS.h"
 }
 
// TODO UNIQUEPTR



int main(int argc, char ** argv) {
    
    
    //init graphBlass:

    GrB_init(GrB_BLOCKING);
        
    std::cout << "Hello world" << std::endl;            
}


//TODO: handle empty matricies, write impala code up to the end, so the only thing
// is used from c++ is matrix reader and thrust, todo spmm decompose impala
// benchmark 
// think about design e.g.
/* struct Semiring {
    zero : fn() -> f32, 
    one : fn() -> f32,
    plus : fn(f32,f32) -> f32,
    multiply : fn(f32,f32) -> f32
}*/

// Maybe change intel mkl for suite sparse graphblas
// abstract over implementation of multiplication and matrix representation
// PRESENTATION 

//rewrite impala add graphblas

//benchmark