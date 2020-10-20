# #TESTS
cuda_add_executable(test1 test.cpp ${SPARSE_PROGRAM} ${CMAKE_SOURCE_DIR}/cpp/sparse_cuda.cu ${GRAPHBLASS_EXECUTABLES})
# add_dependencies(test1 gtest sparse_header)
target_link_directories(test1 PUBLIC $ENV{MKLROOT}/lib/intel64)
target_include_directories(test1 PUBLIC ${AnyDSL_runtime_INCLUDE_DIRS} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${gtest_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include matrix_reader/include $ENV{MKLROOT}/include)
target_include_directories(test1 PUBLIC ${GRAPHBLASS_INCLUDE})

target_link_libraries(test1 MatrixMarketReader ${ANYDSL_RUNTIME_LIBRARIES} ${CUDA_cusparse_LIBRARY} -Wl,--start-group /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_intel_lp64.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_core.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_sequential.a -Wl,--end-group pthread iomp5 m dl)
# target_link_libraries(test1 graphblas)
target_link_libraries(test1 ${GRAPHBLASS_LINK})
target_compile_options(test1 PUBLIC -Wall -m64)

target_link_libraries(test1 gtest)
target_link_libraries(test1 gtest_main)

add_test(NAME test1
         COMMAND test1)