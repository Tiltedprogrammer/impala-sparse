#futhark build script

set(futhark_lib "sparse.c")

cuda_add_executable(sparse_futhark main.c ${futhark_lib} read_matrix.c simple_timer.c)
target_link_libraries(sparse_futhark CUDA::cudart CUDA::nvrtc ${GRAPHBLASS_LINK})
target_include_directories(sparse_futhark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
add_definitions(sparse_futhark -DBENCHMARKING)
