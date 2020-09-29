cmake_minimum_required(VERSION 3.16)
project(SPARSE LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -G")



#CUDA

find_package(CUDA REQUIRED)

find_library(GBLAS NAMES libgraphblas.a)

if(GBLAS-NOTFOUND)
  message("graphblas not found")
else()
  message("graphblas found at ${GBLAS}")
endif(GBLAS-NOTFOUND)


# find_package(AnyDSLArtic_runtime REQUIRED)
# message("Artic:  ${AnyDSL_runtime_INCLUDE_DIRS}")


#AnyDSL

find_package(AnyDSL_runtime REQUIRED)
include_directories(${AnyDSL_runtime_INCLUDE_DIRS})

message("Impala:  ${AnyDSL_runtime_LIBRARIES}")



set(ANYDSL_RUNTIME_LIBRARIES ${AnyDSL_runtime_LIBRARIES})

set(IMPALA_FLAGS --log-level info)


#TESTS

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


#Matrix reader

add_subdirectory(matrix_reader)

#Main sparse application
anydsl_runtime_wrap(SPARSE_PROGRAM
    IMPALA_FLAGS ${IMPALA_FLAGS}
    FILES cpp/sparse.impala)



set(SPARSE_HEADER_FILE sparse)

add_subdirectory(cpp)
add_subdirectory(artic)



enable_testing()
message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")
add_subdirectory(test)


####