cmake_minimum_required(VERSION 3.16)
project(SPARSE LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -G")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(Boost COMPONENTS program_options REQUIRED)

set( BLAST_INCLUDES   "${CMAKE_CURRENT_SOURCE_DIR}/graphblast/ext/moderngpu/include" "${CMAKE_CURRENT_SOURCE_DIR}/graphblast/ext/cub/cub")
set( mgpu_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/graphblast/ext/moderngpu/src/mgpucontext.cu ${CMAKE_CURRENT_SOURCE_DIR}/graphblast/ext/moderngpu/src/mgpuutil.cpp)


set( CUDA_CUSPARSE_LIBRARY "$ENV{CUDA_HOME}/lib64/libcusparse.so" )
FILE( GLOB_RECURSE PROJ_HEADERS graphblast/graphblas/*.hpp)

include_directories( ${BLAST_INCLUDES} ${Boost_INCLUDE_DIRS} )
# cuda_add_library( graphblast ${PROJ_LIBRARIES} ${PROJ_HEADERS} )

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

if(SUITESPARSE) 
  add_definitions(-DSUITE_SPARSE)
  set(GRAPHBLASS_EXECUTABLES "")
  set(GRAPHBLASS_INCLUDE "" )
  set(GRAPHBLASS_LINK "graphblas")

else()
  set( PROJ_INCLUDES  "${CMAKE_SOURCE_DIR}/graphblast" "${CMAKE_SOURCE_DIR}/graphblast/ext/moderngpu/include" "${CMAKE_SOURCE_DIR}/graphblast/ext/cub/cub")
  set( mgpu_SRC_FILES "${CMAKE_SOURCE_DIR}/graphblast/ext/moderngpu/src/mgpucontext.cu" "${CMAKE_SOURCE_DIR}/graphblast/ext/moderngpu/src/mgpuutil.cpp")
  set( PROJ_HEADERS   "" )
  set( PROJ_LIBRARIES "" )
  FILE( GLOB_RECURSE PROJ_HEADERS graphblas/*.hpp)

  # backup
  set(CUDA_FLAGS "${CUDA_NVCC_FLAGS}")
  message("cuda flags ${CUDA_NVCC_FLAGS}")
  cuda_add_library( graphblast ${PROJ_LIBRARIES} ${PROJ_HEADERS} ${mgpu_SRC_FILES})
  target_link_libraries(graphblast ${CUDA_cusparse_LIBRARY} ${Boost_LIBRARIES})
  target_include_directories(graphblast PUBLIC ${PROJ_INCLUDES})
  # set(CUDA_NVCC_FLAGS "${CUDA_FLAGS}")
  message("cuda flags ${CUDA_NVCC_FLAGS}")
  target_compile_options(graphblast PRIVATE $<$<COMPILE_LANGUAGE:CUDA>: -arch=sm_35 >) 



  set(GRAPHBLASS_EXECUTABLES "${CMAKE_SOURCE_DIR}/cpp/graphblast_wrappers.cu;")
  set(GRAPHBLASS_INCLUDE "${CMAKE_SOURCE_DIR}/graphblast")
  set(GRAPHBLASS_LINK "graphblast")

endif()

message("========= ${GRAPHBLASS_EXECUTABLES} ============")

add_subdirectory(cpp)
add_subdirectory(artic)



enable_testing()
message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")
add_subdirectory(test)


####