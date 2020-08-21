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

TEST(TestCudaMultiplication, Test1) {
    TestMatrix tm("/home/alexey.tyurin/specialization/impala-worksheet/sparse/matrix_data");
    std::cout << "\033[0;32m" << "[          ] " << "\033[0;0m" 
                << "random seed = " << std::endl;
    for (const auto& path : tm.matrix_paths) {
        if(!fs::is_directory(path)){
            std::cerr << path << std::endl;
            std::ifstream is(path);
            auto csr = CSRWrapper<float>(is);
            if(csr.max_row_length() <= 32){
                std::cerr << "Launching test for " << path << std::endl;
                auto csr_cuda = csr.multiply_cuda(csr);
                auto csr_mtl = csr.multiply_cusparse(csr);
                std::cerr << csr_cuda.nnz << std::endl;
                std::cerr << csr_mtl.nnz << std::endl;

                ASSERT_TRUE(csr_mtl == csr_cuda);
                
            } else {
                ASSERT_TRUE(1==1);
            }
        } else {
            ASSERT_TRUE(1==1);
        }
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}