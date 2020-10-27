#include "gtest/gtest.h"

#include "csr_wrapper.h"

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>


namespace fs = std::filesystem;



struct TestMatrix{
    public: 
        std::vector<std::string> matrix_paths;
        explicit TestMatrix(std::string path_to_directory);
};

TestMatrix::TestMatrix(std::string path_to_directory){
    matrix_paths = std::vector<std::string>();
    for (const auto & entry : fs::directory_iterator(path_to_directory))
        matrix_paths.push_back(entry.path());
}

TEST(TestForTest, TestOne){
    ASSERT_EQ(1,1);
}

TEST(TestCudaMultiplicationSimple, Test1) {
    // TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data");
    TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/simple_tests");
    
    for (const auto& path : tm.matrix_paths) {
        if(!fs::is_directory(path)){
            std::cout << "\033[0;32m" << "[          ] " << "\u001b[35m" 
                << "Launching test for "<< path << std::endl;
            std::ifstream is(path);
            auto csr = CSRWrapper<float>(is);
                 // std::cerr << "Launching test for " << path << std::endl;
            // auto csr_cuda = csr.multiply_cuda(csr);
            auto csr_cuda = csr.multiply_cusparse(csr);
            auto csr_ss = csr.multiply_cuda(csr);

            ASSERT_TRUE(csr_cuda == csr_ss);
            // ASSERT_TRUE(csr_cuda.get_cols() == csr_cusparse.get_cols());

            
            
        } else {
            ASSERT_TRUE(1==1);
        }
    }
}

// TEST(TestCudaMultiplicationComplex, Test1) {
//     // TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data");
//     TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/more_complex_tests");
    
//     for (const auto& path : tm.matrix_paths) {
//         if(!fs::is_directory(path)){
//             std::cout << "\033[0;32m" << "[          ] " << "\u001b[35m" 
//                 << "Launching test for "<< path << std::endl;
//             std::ifstream is(path);
//             auto csr = CSRWrapper<float>(is);
//                  // std::cerr << "Launching test for " << path << std::endl;
//             // auto csr_cuda = csr.multiply_cuda(csr);
//             auto csr_cuda = csr.multiply_cuda(csr);
//             auto csr_ss = csr.multiply_graphblas(csr);

//             ASSERT_TRUE(csr_cuda == csr_ss);
//             // ASSERT_TRUE(csr_cuda.get_cols() == csr_cusparse.get_cols());

            
            
//         } else {
//             ASSERT_TRUE(1==1);
//         }
//     }
// }

// TEST(TestImpala,TestImpalaMultiplicationSimple) {
//     // TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data");
//     TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/simple_tests");
    
//     for (const auto& path : tm.matrix_paths) {
//         if(!fs::is_directory(path)){
//             std::cout << "\033[0;32m" << "[          ] " << "\u001b[35m" 
//                 << "Launching test for "<< path << std::endl;
//             std::ifstream is(path);
//             auto csr = CSRWrapper<float>(is);
//                  // std::cerr << "Launching test for " << path << std::endl;
//             // auto csr_cuda = csr.multiply_cuda(csr);
//             auto csr_impala = csr.multiply_impala(csr);
//             auto csr_ss = csr.multiply_suite_sparse(csr);

//             ASSERT_TRUE(csr_impala == csr_ss);
//             // ASSERT_TRUE(csr_cuda.get_cols() == csr_cusparse.get_cols());

            
            
//         } else {
//             ASSERT_TRUE(1==1);
//         }
//     }

// }


// TEST(TestImpala,TestImpalaMultiplicationComplex) {
//     // TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data");
//     TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/more_complex_tests");
    
//     for (const auto& path : tm.matrix_paths) {
//         if(!fs::is_directory(path)){
//             std::cout << "\033[0;32m" << "[          ] " << "\u001b[35m" 
//                 << "Launching test for "<< path << std::endl;
//             std::ifstream is(path);
//             auto csr = CSRWrapper<float>(is);
//                  // std::cerr << "Launching test for " << path << std::endl;
//             // auto csr_cuda = csr.multiply_cuda(csr);
//             auto csr_impala = csr.multiply_impala(csr);
//             auto csr_mkl = csr.multiply_suite_sparse(csr);

//             ASSERT_TRUE(csr_impala == csr_mkl);
//             // ASSERT_TRUE(csr_cuda.get_cols() == csr_cusparse.get_cols());

            
            
//         } else {
//             ASSERT_TRUE(1==1);
//         }
//     }

// }

// TEST(TestCudaMultiplication, Benchmark) {
//     TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data/benchmarks");
    
//     for (const auto& path : tm.matrix_paths) {
//         if(!fs::is_directory(path)){
//             std::cout << "\033[0;32m" << "[          ] " << "\u001b[35m" 
//                 << "Launching test for "<< path << std::endl;
//             std::ifstream is(path);
//             auto csr = CSRWrapper<float>(is);
            
//             std::cout << "\033[0;32m" << "[          ] " << "\u001b[35m" 
//                 << "Reading finished "<< path << std::endl;

//             std::cout << "\u001b[31m" << "[          ] " 
//                 << "Rows "<< csr.M << " nnz "  << csr.nnz << "\u001b[35m"  <<  std::endl;


//             // std::cerr << "Launching test for " << path << std::endl;
//             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//             for (int i = 0; i < 10; i++) {
//                 auto csr_cuda = csr.multiply_cuda(csr);
//             }
//             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

//             std::cout << "Avg cuda time for = " << path << "=" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/10 << "[µs]" << std::endl;

//             begin = std::chrono::steady_clock::now();
//             for (int i = 0; i < 10; i++) {
//                 auto csr_cuda = csr.multiply_cusparse(csr);
//             }
//             end = std::chrono::steady_clock::now();

//             std::cout << "Avg cuda time for = " << path << "=" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/10 << "[µs]" << std::endl;
 
            
//             // ASSERT_TRUE(csr_mtl == csr_cuda);
//             // ASSERT_TRUE(1==1);
            
            
//         } else {
//             ASSERT_TRUE(1==1);
//         }
//     }
// }

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}