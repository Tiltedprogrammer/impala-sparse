cmake_minimum_required(VERSION 3.16)
project(SPARSE LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -G")



#CUDA

find_package(CUDA REQUIRED)

#AnyDSL

find_package(AnyDSL_runtime REQUIRED)
include_directories(${AnyDSL_runtime_INCLUDE_DIRS})

set(ANYDSL_RUNTIME_LIBRARIES ${AnyDSL_runtime_LIBRARIES})

set(IMPALA_FLAGS --log-level info)


#TESTS

include(ExternalProject)
ExternalProject_Add(gtest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
  # Comment above line, and uncomment line below to use subversion.
  # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/ 
  # Uncomment line below to freeze a revision (here the one for 1.7.0)
  # SVN_REVISION -r700

  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir)

################

#Matrix reader

add_subdirectory(matrix_reader)


set(SPARSE_HEADER_FILE sparse)
add_custom_command(OUTPUT ${SPARSE_HEADER_FILE} COMMAND  impala sparse.impala --emit-c-interface -o include/${SPARSE_HEADER_FILE} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMENT "Generating sparse.h")
set_source_files_properties(${SPARSE_HEADER_FILE} PROPERTIES GENERATED TRUE)


add_custom_target(
  sparse_header
  DEPENDS ${SPARSE_HEADER_FILE}
)

# # target zoo is always built
# add_custom_target(zoo ALL
#     COMMAND echo "This is ALL target 'zoo', and it depends on ${TEST_FILE}"
#     # If the file exists, then commands related to that file won't be executed
#     # DONOT let other target depends on the same OUTPUT as current target,
#     #   or it may be bad when doing parallel make
#     DEPENDS ${TEST_FILE}

#     # to make quotes printable,for example
#     VERBATIM
# )


#Main sparse application
anydsl_runtime_wrap(SPARSE_PROGRAM
    IMPALA_FLAGS ${IMPALA_FLAGS}
    FILES sparse.impala)


cuda_add_executable(SparseMainApp main.cpp ${SPARSE_PROGRAM})
target_include_directories(SparseMainApp PUBLIC ${GTEST_INCLUDE_DIR} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} include)
target_link_libraries(SparseMainApp ${ANYDSL_RUNTIME_LIBRARIES} ${GTEST_LIB} ${CUDA_cusparse_LIBRARY} MatrixMarketReader)
add_dependencies(SparseMainApp sparse_header)

#### Main sparse application end


##matrix market test
cuda_add_executable(MatrixMarket test_matrix_market.cpp ${SPARSE_PROGRAM} sparse_cuda.cu)

target_link_directories(MatrixMarket PUBLIC $ENV{MKLROOT}/lib/intel64)
target_include_directories(MatrixMarket PUBLIC ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ./include $ENV{MKLROOT}/include})

target_link_libraries(MatrixMarket MatrixMarketReader ${CUDA_cusparse_LIBRARY} -Wl,--start-group /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_intel_lp64.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_core.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_sequential.a -Wl,--end-group pthread iomp5 m dl)
target_compile_options(MatrixMarket PUBLIC -Wall -m64)

add_dependencies(MatrixMarket sparse_header)





##matrix market test



##end test

#main.impala
anydsl_runtime_wrap(MAIN_IMPALA
    IMPALA_FLAGS ${IMPALA_FLAGS}
    FILES main.impala)
add_executable(test_impala ${MAIN_IMPALA} c_helpers.cpp)
###


#TESTS
cuda_add_executable(test1 test.cpp ${SPARSE_PROGRAM} sparse_cuda.cu)
add_dependencies(test1 gtest sparse_header)
target_link_directories(test1 PUBLIC $ENV{MKLROOT}/lib/intel64)
target_include_directories(test1 PUBLIC ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${source_dir}/googletest/include include matrix_reader/include $ENV{MKLROOT}/include})
target_link_libraries(test1 MatrixMarketReader ${CUDA_cusparse_LIBRARY} -Wl,--start-group /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_intel_lp64.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_core.a /opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64/libmkl_sequential.a -Wl,--end-group pthread iomp5 m dl)
target_compile_options(test1 PUBLIC -Wall -m64)

target_link_libraries(test1 ${binary_dir}/lib/libgtest.a)
target_link_libraries(test1 ${binary_dir}/lib/libgtest_main.a)


if(ENABLE_TESTING)
  # enable_testing()
  message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")
  # add_subdirectory(test)
endif()

enable_testing()

add_test(NAME test1
         COMMAND test1)

####