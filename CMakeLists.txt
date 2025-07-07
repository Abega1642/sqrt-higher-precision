cmake_minimum_required(VERSION 3.30)
project(sqrt_higher_precision)

set(CMAKE_CXX_STANDARD 20)

# === Main Executable ===
add_executable(sqrt_higher_precision src/main.cpp)

# Use pkg-config to find GMP and MPFR
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(MPFR REQUIRED IMPORTED_TARGET mpfr)

target_link_libraries(sqrt_higher_precision PRIVATE
        PkgConfig::GMP
        PkgConfig::MPFR
)

# === Unit Tests ===
enable_testing()
find_package(GTest CONFIG REQUIRED)

add_executable(tests
        tests/test_main.cpp
        tests/test_fraction.cpp
        src/Fraction.cpp
)

target_link_libraries(tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        PkgConfig::GMP
        PkgConfig::MPFR
        gmpxx
        gmp
        pthread
)


add_test(NAME all_tests COMMAND tests)
