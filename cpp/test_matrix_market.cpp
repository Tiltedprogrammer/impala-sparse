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

int64_t TIME = 0;

namespace fs = std::filesystem;

CSRWrapper<float> APSPexample(std::string& path) {
    std::ifstream isA (path);
    
    auto csrA = CSRWrapper<float>(isA);

    int m = csrA.N;


    for(int i = 1; i < m; i *= 2) {
        // std::cout << i << " csrA nnzs: " << csrA.nnz <<  std::endl;
        csrA = csrA.multiply_graphblas(csrA);
    }
    return csrA;
}


void MULTexample(std::string& path) {
    std::ifstream isA (path);
    
    auto csrA = CSRWrapper<float>(isA);

    int m = csrA.N;

    csrA = csrA.multiply_cuda(csrA);
    std::cout << "csrA nnzs: " << csrA.nnz << std::endl;

}


CSRWrapper<bool> CONNECTIVITYexample(std::string& path) {

    std::ifstream isA (path);
    auto csrA = CSRWrapper<float>(isA);
    auto csrAbool = csrA.csr_to_bool();
    auto csrB = csrAbool.multiply_graphblas(csrAbool);

    return csrB;
}

CSRWrapper<float> SSSPexample(CSRWrapper<float>& adj, int v) {

    auto V = CSRWrapper<float>(1,adj.M);
    V.add_element(0,v,0.0);
    auto V1 = V;
    int i = 1;


    //loop should
    for (size_t i = 1; i <= 99; i++)
    {
        V1 = V;
        V = V1.multiply_graphblas(adj);
        TIME += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        // std::cout << i << std::endl;
        if(V1 == V) {
            std::cout << "broke " << i << std::endl;
            break;
        }
    }
    
    // do {
    //     V1 = V;
    //     V = V1.multiply_graphblas(adj);
    //     i++;
    //     std::cout << i << "\r" << std::flush;
    // } while (!(V1 == V) && i < adj.N-1);
    // std::cout << std::endl;
    

    return V;
}

CSRWrapper<bool> TRANSITIVECLOSUREexample(std::string& path) {

    std::ifstream isA (path);
    auto csrA = CSRWrapper<float>(isA);
    auto csrAbool = csrA.csr_to_bool();

    int m = csrA.N;


    for(int i = 1; i < m; i *= 2) {
        csrAbool = csrAbool.multiply_graphblas(csrAbool);
    }
    return csrAbool;
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

    // std::string path = "/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/overflow-tests/soc-sign-Slashdot081106.mtx";
    // std::string path = "/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/transitive-closure/soc-sign-bitcoin-alpha.mtx";
    // std::string path = "/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/simple_tests/G32.mtx";


    // APSPexample(path);
    std::ifstream isA("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/sssp/roadNet-CA-ready.mtx");
    // std::ifstream isA("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/sssp/fosdem-example.mtx");
    //    std::ifstream isA("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/pattern/pattern_example");
    // std::ofstream osA("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/sssp/roadNet-CA-ready.mtx");
    
    // auto start = std::chrono::steady_clock::now();
    auto csrA = CSRWrapper<float>(isA);
    // auto end = std::chrono::steady_clock::now();

    std::cout << "Reading finished in " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    std::cout << "Nnz: " << csrA.nnz << std::endl;
    // for (int i = 0; i < csrA.N; i++) {
    //     csrA.add_element(i,i,0.0);
    // }
    auto csrB = csrA.multiply_cusparse(csrA);
    std::cout << "B nnz: " << csrB.nnz << std::endl;

    return 0;
    
}

// think about design e.g.
/* struct Semiring {
    zero : fn() -> f32, 
    one : fn() -> f32,
    plus : fn(f32,f32) -> f32,
    multiply : fn(f32,f32) -> f32
}*/
