cmake_minimum_required(VERSION 3.30)
project(sqrt_higher_precision)

set(CMAKE_CXX_STANDARD 20)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(MPFR REQUIRED IMPORTED_TARGET mpfr)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(sqrt_higher_precision src/main.cpp)
target_link_libraries(sqrt_higher_precision PRIVATE
        PkgConfig::GMP
        PkgConfig::MPFR
)

enable_testing()

add_executable(tests
        tests/test_main.cpp
        tests/test_fraction.cpp
        src/Fraction.cpp
)

target_link_libraries(tests PRIVATE
        gtest
        gtest_main
        PkgConfig::GMP
        PkgConfig::MPFR
        gmpxx
        gmp
        pthread
)

add_test(NAME all_tests COMMAND tests)
