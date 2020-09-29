#Artic

find_package(AnyDSLArtic_runtime REQUIRED)

anydsl_runtime_wrap(TEST_ART FILES test.art)
anydsl_runtime_wrap(SPARSE_ART FILES sparse.art)


message("${AnyDSLArtic_runtime_DIR}")
message("Artic:  ${AnyDSL_runtime_INCLUDE_DIRS}")

cuda_add_executable(sparse_art ${SPARSE_ART} intrincics.cpp ${CMAKE_SOURCE_DIR}/cpp/sparse_cuda.cu sparse_wrapper.cpp ${SPARSE_PROGRAM})
target_link_directories(sparse_art PUBLIC $ENV{MKLROOT}/lib/intel64)
target_include_directories(sparse_art PUBLIC ${AnyDSL_runtime_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sparse_art ${AnyDSL_runtime_LIBRARIES} ${CUDA_cusparse_LIBRARY} MatrixMarketReader)
target_link_libraries(sparse_art graphblas)
target_link_libraries(sparse_art -Wl,--start-group /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_intel_lp64.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_core.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_sequential.a -Wl,--end-group pthread iomp5 m dl)

target_compile_options(sparse_art PUBLIC -Wall -m64)