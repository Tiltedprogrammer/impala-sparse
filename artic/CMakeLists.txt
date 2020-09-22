#Artic

find_package(AnyDSLArtic_runtime REQUIRED)

anydsl_runtime_wrap(TEST_ART FILES test.art)
anydsl_runtime_wrap(SPARSE_ART FILES sparse.art)


message("${AnyDSLArtic_runtime_DIR}")
message("Artic:  ${AnyDSL_runtime_INCLUDE_DIRS}")


add_executable(test_art ${TEST_ART} intrincics.cpp)
target_include_directories(test_art PUBLIC ${AnyDSL_runtime_INCLUDE_DIRS})
target_link_libraries(test_art ${AnyDSL_runtime_LIBRARIES})

cuda_add_executable(sparse_art ${SPARSE_ART} intrincics.cpp ../sparse_cuda.cu)
target_include_directories(sparse_art PUBLIC ${AnyDSL_runtime_INCLUDE_DIRS} ../include)
target_link_libraries(sparse_art ${AnyDSL_runtime_LIBRARIES} ${CUDA_cusparse_LIBRARY})