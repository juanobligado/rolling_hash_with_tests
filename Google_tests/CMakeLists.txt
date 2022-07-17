project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Google_Tests_run RollingHashTest.cpp RollingChecksumTest.cpp Sha1Test.cpp)

target_link_libraries(Google_Tests_run RollingHash_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_inputs)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ModifiedInput.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_inputs)