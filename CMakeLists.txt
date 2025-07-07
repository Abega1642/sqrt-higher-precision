cmake_minimum_required(VERSION 3.30)
project(sqrt_higher_precision)

set(CMAKE_CXX_STANDARD 20)

add_executable(sqrt_higher_precision src/main.cpp)

target_include_directories(sqrt_higher_precision PRIVATE
        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include
)
target_link_directories(sqrt_higher_precision PRIVATE
        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/lib
)
target_link_libraries(sqrt_higher_precision PRIVATE
        mpfr
        gmp
)

# === Unit Tests ===
enable_testing()

# Use GTest via vcpkg (has a proper CMake config)
find_package(GTest CONFIG REQUIRED)

add_executable(tests tests/test_main.cpp)
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main pthread)

add_test(NAME unit_tests COMMAND tests)
