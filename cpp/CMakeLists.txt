
# cuda_add_executable(SparseMainApp main.cpp ${SPARSE_PROGRAM})
# target_include_directories(SparseMainApp PUBLIC ${GTEST_INCLUDE_DIR} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} include)
# target_link_libraries(SparseMainApp ${ANYDSL_RUNTIME_LIBRARIES} ${GTEST_LIB} ${CUDA_cusparse_LIBRARY} MatrixMarketReader)
# add_dependencies(SparseMainApp sparse_header)

#### Main sparse application end


##matrix market test
cuda_add_executable(MatrixMarket test_matrix_market.cpp ${SPARSE_PROGRAM} sparse_cuda.cu)

target_link_directories(MatrixMarket PUBLIC $ENV{MKLROOT}/lib/intel64)
target_include_directories(MatrixMarket PUBLIC ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${CMAKE_SOURCE_DIR}/include $ENV{MKLROOT}/include})

target_link_libraries(MatrixMarket MatrixMarketReader ${ANYDSL_RUNTIME_LIBRARIES} ${CUDA_cusparse_LIBRARY} -Wl,--start-group /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_intel_lp64.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_core.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_sequential.a -Wl,--end-group pthread iomp5 m dl)
target_link_libraries(MatrixMarket graphblas)
target_compile_options(MatrixMarket PUBLIC -Wall -m64)


