#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include "../include/Fraction.hpp"
#include "../include/SquareRoot.hpp"

struct BenchmarkResult {
  std::string title;
  std::uint64_t nat;
  std::uint64_t range;
  std::uint64_t precision_digits;
  std::chrono::duration<double> elapsed;
};

[[nodiscard]] std::string format_benchmark_result(
    const BenchmarkResult& result) {
  return "[Benchmark of SquareRoot::k_th_value] - " + result.title +
         "\n\t - nat            = " + std::to_string(result.nat) +
         "\n\t - k              = " + std::to_string(result.range) +
         "\n\t - Precision      = " + std::to_string(result.precision_digits) +
         " decimal digits" +
         "\n\t - Duration       = " + std::to_string(result.elapsed.count()) +
         " seconds";
}

void show_result(std::ostream& out, const BenchmarkResult& result) {
  out << format_benchmark_result(result) << '\n';
}

/**
[Benchmark of SquareRoot::k_th_value] - Fraction Form
         - nat			= 2,
         - k			= 22,
         - Precision	= 6000002 decimal digits,
         - Duration		= 0.972028 seconds
 */

TEST(SquareRootBenchmark, TimeExecutionOfKthValueComputation) {
  constexpr std::uint64_t target = 2ULL;
  constexpr std::uint64_t k = 22ULL;
  constexpr std::uint64_t precision_digits = 6'000'002ULL;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();
  const Fraction result = subject.k_th_value(k);
  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = end - start;

  show_result(std::cout, BenchmarkResult{.title = "Fraction Form",
                                         .nat = subject.get_nat(),
                                         .range = k,
                                         .precision_digits = precision_digits,
                                         .elapsed = elapsed});

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value] - Decimal Form
         - nat			= 2,
         - k			= 22,
         - Precision	= 6000002 decimal digits,
         - Duration		= 3.03634 seconds
 */

TEST(SquareRootBenchmark,
     TimeExecutionOfKthValueComputationWithSixMillionPrecision) {
  constexpr std::uint64_t target = 2ULL;
  constexpr std::uint64_t k = 22ULL;
  constexpr std::uint64_t precision_digits = 6'000'002ULL;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();
  const std::string result = subject.k_th_value(k).get_value(precision_digits);
  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = end - start;

  show_result(std::cout, BenchmarkResult{.title = "Decimal Form",
                                         .nat = subject.get_nat(),
                                         .range = k,
                                         .precision_digits = precision_digits,
                                         .elapsed = elapsed});

  SUCCEED();
}
/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 3,
         - k			= 19,
         - Precision	= 1000002 decimal digits,
         - Duration		= 0.360016 seconds
 */

TEST(SquareRootBenchmark,
     TimeExecutionOf19thValueComputationWithOneMillionPrecisionOf3) {
  constexpr std::uint64_t target = 3ULL;
  constexpr std::uint64_t k = 19ULL;
  constexpr std::uint64_t precision_digits = 1'000'002ULL;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();
  const std::string result = subject.k_th_value(k).get_value(precision_digits);
  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = end - start;

  show_result(std::cout, BenchmarkResult{.title = "Decimal Form",
                                         .nat = subject.get_nat(),
                                         .range = k,
                                         .precision_digits = precision_digits,
                                         .elapsed = elapsed});

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 7,
         - k			= 19,
         - Precision	= 1000002 decimal digits,
         - Duration		= 0.370041 seconds
*/

TEST(SquareRootBenchmark,
     TimeExecutionOf19thValueComputationWithOneMillionPrecisionOf7) {
  constexpr std::uint64_t target = 7ULL;
  constexpr std::uint64_t k = 19ULL;
  constexpr std::uint64_t precision_digits = 1'000'002ULL;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();
  const std::string result = subject.k_th_value(k).get_value(precision_digits);
  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = end - start;

  show_result(std::cout, BenchmarkResult{.title = "Decimal Form",
                                         .nat = subject.get_nat(),
                                         .range = k,
                                         .precision_digits = precision_digits,
                                         .elapsed = elapsed});

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 5,
         - k			= 19,
         - Precision	= 1000002 decimal digits,
         - Duration		= 0.323097 seconds
 */

TEST(SquareRootBenchmark,
     TimeExecutionOf19thValueComputationWithOneMillionPrecisionOf5) {
  constexpr std::uint64_t target = 5ULL;
  constexpr std::uint64_t k = 19ULL;
  constexpr std::uint64_t precision_digits = 1'000'002ULL;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();
  const std::string result = subject.k_th_value(k).get_value(precision_digits);
  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = end - start;

  show_result(std::cout, BenchmarkResult{.title = "Decimal Form",
                                         .nat = subject.get_nat(),
                                         .range = k,
                                         .precision_digits = precision_digits,
                                         .elapsed = elapsed});

  SUCCEED();
}

/**
[Benchmark of SquareRoot::k_th_value]  - Decimal Form
         - nat			= 2,
         - k			= 24,
         - Precision	= 10000002 decimal digits,
         - Duration		= 8.60041 seconds
 */

TEST(SquareRootBenchmark,
     TimeExecutionOf24thValueComputationWithTenMillionPrecisionOfSqrtOf2) {
  constexpr std::uint64_t target = 2ULL;
  constexpr std::uint64_t k = 24ULL;
  constexpr std::uint64_t precision_digits = 10'000'002ULL;

  const SquareRoot subject = SquareRoot::of(target);

  const auto start = std::chrono::high_resolution_clock::now();
  const std::string result = subject.k_th_value(k).get_value(precision_digits);
  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = end - start;

  show_result(std::cout, BenchmarkResult{.title = "Decimal Form",
                                         .nat = subject.get_nat(),
                                         .range = k,
                                         .precision_digits = precision_digits,
                                         .elapsed = elapsed});

  SUCCEED();
}