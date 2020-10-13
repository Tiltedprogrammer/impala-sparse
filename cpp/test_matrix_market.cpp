#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>

#include "csr_wrapper.h"

#include <filesystem>

#include <chrono>

#include <GraphBLAS.h>

namespace fs = std::filesystem;

void APSPexample(std::string& path) {
    std::ifstream isA (path);
    
    auto csrA = CSRWrapper<float>(isA);

    int m = csrA.N;


    for(int i = 1; i < m - 1; i *= 2) {
        csrA = csrA.multiply_suite_sparse(csrA);
    }
    
}

void CONNECTIVITYexample(std::string& path) {

    std::ifstream isA (path);
    auto csrA = CSRWrapper<float>(isA);
    auto csrAbool = csrA.csr_to_bool();
    auto csrB = csrAbool.multiply_suite_sparse(csrAbool);
}

double avg(std::vector<int64_t> &times){
    
    double avg = 0.0;

    for (auto i : times) {
        avg += (double)i;
    }

    return avg / times.size();
}

double dev(std::vector<int64_t> &times, double avg) {
    
    double dev = 0.0;

    for (auto i : times) {
        dev += ((double)i - avg) * ((double)i - avg);
    }

    dev /= times.size();

    return std::sqrt(dev);
}

template<typename Func>
void benchmark(Func f,int num_iter, std::string& path) {
    //dry run
    f(path);

    auto times = std::vector<int64_t>();

    for (int i = 0; i < num_iter; i++){
        auto start = std::chrono::steady_clock::now();
        f(path);
        auto end = std::chrono::steady_clock::now();
        times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    //avg
    double avg_ = avg(times);
    //dev
    double dev_ = dev(times,avg_);

    std::cout << "Timing: " << num_iter << " runs | Average: " << avg_ / 1000.0 << "ms | Deviation: " << dev_/1000.0 << "ms" << std::endl;
    
}

int main(int argc, char ** argv) {

    std::string path = "/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/apsp/mt2010.mtx";
    benchmark(CONNECTIVITYexample,4,path);
}

//select some matrix data and make python script to collect run times and buid a graph.

// think about design e.g.
/* struct Semiring {
    zero : fn() -> f32, 
    one : fn() -> f32,
    plus : fn(f32,f32) -> f32,
    multiply : fn(f32,f32) -> f32
}*/

// Maybe change intel mkl for suite sparse graphblas
// abstract over implementation of multiplication and matrix representation
