#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include "../include/Fraction.hpp"
#include "../include/SquareRoot.hpp"

void show_result(const std::string& title, const unsigned long nat,
                 const unsigned long range,
                 const unsigned long precision_digits, const double time) {
  std::cout << "[Benchmark of SquareRoot::k_th_value]  - " << title
            << "\n\t - nat\t\t\t= " << nat << ",\n\t - k\t\t\t= " << range
            << ",\n\t - Precision\t= " << precision_digits << " decimal digits"
            << ",\n\t - Duration\t\t= " << time << " seconds" << std::endl;
}

/**
[Benchmark of SquareRoot::k_th_value] - Fraction Form
         - nat			= 2,
         - k			= 22,
         - Precision	= 6000002 decimal digits,
         - Duration		= 1.01156 seconds
 */

TEST(SquareRootBenchmark, TimeExecutionOfKthValueComputation) {
  constexpr unsigned long target = 2UL;
  constexpr unsigned long k = 22UL;
  constexpr std::size_t precision_digits = 6'000'002;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();

  const Fraction result = subject.k_th_value(k);

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed_seconds = end - start;

  show_result("Fraction form", subject.get_nat(), k, precision_digits,
              elapsed_seconds.count());

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value] - Decimal Form
         - nat			= 2,
         - k			= 22,
         - Precision	= 6000002 decimal digits,
         - Duration		= 3.06032 seconds
 */

TEST(SquareRootBenchmark,
     TimeExecutionOfKthValueComputationWithSixMillionPreision) {
  constexpr unsigned long target = 2UL;
  constexpr unsigned long k = 22UL;
  constexpr std::size_t precision_digits = 6'000'002;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();

  const std::string result = subject.k_th_value(k).get_value(precision_digits);

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed_seconds = end - start;

  show_result("Decimal Form", subject.get_nat(), k, precision_digits,
              elapsed_seconds.count());

  SUCCEED();
}
/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 3,
         - k			= 19,
         - Precision	= 1000002 decimal digits,
         - Duration		= 0.414324 seconds
 */
TEST(SquareRootBenchmark,
     TimeExecutionOf19thValueComputationWithSixMillionPreisionOf3) {
  constexpr unsigned long target = 3UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();

  const std::string result = subject.k_th_value(k).get_value(precision_digits);

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed_seconds = end - start;

  show_result("Decimal Form", subject.get_nat(), k, precision_digits,
              elapsed_seconds.count());

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 7,
         - k			= 19,
         - Precision	= 1000002 decimal digits,
         - Duration		= 0.434439 seconds
*/
TEST(SquareRootBenchmark,
     TimeExecutionOf19thValueComputationWithSixMillionPreisionOf7) {
  constexpr unsigned long target = 7UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();

  const std::string result = subject.k_th_value(k).get_value(precision_digits);

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed_seconds = end - start;

  show_result("Decimal Form", subject.get_nat(), k, precision_digits,
              elapsed_seconds.count());

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 5,
         - k			= 19,
         - Precision	= 1000002 decimal digits,
         - Duration		= 0.388175 seconds
 */
TEST(SquareRootBenchmark,
     TimeExecutionOf19thValueComputationWithSixMillionPreisionOf5) {
  constexpr unsigned long target = 5UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();

  const std::string result = subject.k_th_value(k).get_value(precision_digits);

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed_seconds = end - start;

  show_result("Decimal Form", subject.get_nat(), k, precision_digits,
              elapsed_seconds.count());

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 2,
         - k			= 24,
         - Precision	= 10000002 decimal digits,
         - Duration		= 8.75233 seconds
 */
TEST(SquareRootBenchmark,
     TimeExecutionOf24thValueComputationWithTenMillionPreisionOfSqrtOf2) {
  constexpr unsigned long target = 2UL;
  constexpr unsigned long k = 24UL;
  constexpr std::size_t precision_digits = 10'000'002;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();

  const std::string result = subject.k_th_value(k).get_value(precision_digits);

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed_seconds = end - start;

  show_result("Decimal Form", subject.get_nat(), k, precision_digits,
              elapsed_seconds.count());

  SUCCEED();
}
