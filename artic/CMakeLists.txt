#Artic

find_package(AnyDSLArtic_runtime REQUIRED)

anydsl_runtime_wrap(TEST_ART FILES test.art)

message("${AnyDSLArtic_runtime_DIR}")
message("Artic:  ${AnyDSL_runtime_INCLUDE_DIRS}")


add_executable(test_art ${TEST_ART} intrincics.cpp)
target_include_directories(test_art PUBLIC ${AnyDSL_runtime_INCLUDE_DIRS})
target_link_libraries(test_art ${AnyDSL_runtime_LIBRARIES})