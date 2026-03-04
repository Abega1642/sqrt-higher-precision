#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/Fraction.hpp"
#include "../include/SquareRoot.hpp"

namespace {

struct Stats {
  double min_s;
  double mean_s;
  double max_s;
  double stddev_s;
  double cv_pct;
  int reps;
};

struct BenchmarkContext {
  std::string label;
  std::uint64_t nat;
  std::uint64_t k;
  std::uint64_t precision_digits;
};

template <typename Fn>
Stats measure(Fn&& fn, const int reps) {
  fn();

  std::vector<double> times;
  times.reserve(reps);

  for (int i = 0; i < reps; ++i) {
    const auto start = std::chrono::high_resolution_clock::now();
    fn();
    const auto end = std::chrono::high_resolution_clock::now();
    times.push_back(std::chrono::duration<double>(end - start).count());
  }

  std::ranges::sort(times);

  const double mean = std::accumulate(times.begin(), times.end(), 0.0) /
                      static_cast<double>(reps);

  double sq_sum = 0.0;
  for (const double t : times) {
    sq_sum += (t - mean) * (t - mean);
  }
  const double stddev = std::sqrt(sq_sum / static_cast<double>(reps));

  return {times.front(),
          mean,
          times.back(),
          stddev,
          (mean > 0.0 ? stddev / mean * 100.0 : 0.0),
          reps};
}

[[nodiscard]] std::string format_stats(const BenchmarkContext& ctx,
                                       const Stats& s) {
  constexpr int w = 12;

  std::ostringstream oss;
  oss << "\n[Benchmark] " << ctx.label << "\n"
      << "  nat             = " << ctx.nat << "\n"
      << "  k               = " << ctx.k << "\n"
      << "  precision       = " << ctx.precision_digits << " digits"
      << "\n"
      << "  reps            = " << s.reps << " (+ 1 warmup discarded)"
      << "\n"
      << std::fixed << std::setprecision(6)
      << "  min             = " << std::setw(w) << s.min_s
      << " s  <-- REFERENCE VALUE"
      << "\n"
      << "  mean            = " << std::setw(w) << s.mean_s << " s\n"
      << "  max             = " << std::setw(w) << s.max_s << " s\n"
      << "  stddev          = " << std::setw(w) << s.stddev_s << " s\n"
      << std::setprecision(2) << "  cv              = " << s.cv_pct << " %\n";

  if (s.cv_pct > 5.0) {
    oss << "  [WARNING] cv > 5% -- environment is not stable.\n"
        << "            Run via: sudo ./.shell/bench-marks.sh <binary>\n";
  }

  return oss.str();
}

void print_stats(std::ostream& out, const BenchmarkContext& ctx,
                 const Stats& s) {
  out << format_stats(ctx, s);
}

}  // namespace

TEST(SquareRootBenchmark, Sqrt2_K22_FractionFormOnly) {
  constexpr std::uint64_t nat = 2ULL;
  constexpr std::uint64_t k = 22ULL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const Fraction result = subject.k_th_value(k);
        (void)result;
      },
      7);

  print_stats(std::cout,
              BenchmarkContext{.label = "sqrt(2) k=22 fraction form",
                               .nat = nat,
                               .k = k,
                               .precision_digits = 0ULL},
              stats);

  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt2_K22_6MillionDigits) {
  constexpr std::uint64_t nat = 2ULL;
  constexpr std::uint64_t k = 22ULL;
  constexpr std::uint64_t precision_digits = 6'000'002ULL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats(std::cout,
              BenchmarkContext{.label = "sqrt(2) k=22 6M digits",
                               .nat = nat,
                               .k = k,
                               .precision_digits = precision_digits},
              stats);

  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt3_K19_1MillionDigits) {
  constexpr std::uint64_t nat = 3ULL;
  constexpr std::uint64_t k = 19ULL;
  constexpr std::uint64_t precision_digits = 1'000'002ULL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats(std::cout,
              BenchmarkContext{.label = "sqrt(3) k=19 1M digits",
                               .nat = nat,
                               .k = k,
                               .precision_digits = precision_digits},
              stats);

  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt5_K19_1MillionDigits) {
  constexpr std::uint64_t nat = 5ULL;
  constexpr std::uint64_t k = 19ULL;
  constexpr std::uint64_t precision_digits = 1'000'002ULL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats(std::cout,
              BenchmarkContext{.label = "sqrt(5) k=19 1M digits",
                               .nat = nat,
                               .k = k,
                               .precision_digits = precision_digits},
              stats);

  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt7_K19_1MillionDigits) {
  constexpr std::uint64_t nat = 7ULL;
  constexpr std::uint64_t k = 19ULL;
  constexpr std::uint64_t precision_digits = 1'000'002ULL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats(std::cout,
              BenchmarkContext{.label = "sqrt(7) k=19 1M digits",
                               .nat = nat,
                               .k = k,
                               .precision_digits = precision_digits},
              stats);

  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt2_K24_10MillionDigits) {
  constexpr std::uint64_t nat = 2ULL;
  constexpr std::uint64_t k = 24ULL;
  constexpr std::uint64_t precision_digits = 10'000'002ULL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      3);

  print_stats(std::cout,
              BenchmarkContext{.label = "sqrt(2) k=24 10M digits",
                               .nat = nat,
                               .k = k,
                               .precision_digits = precision_digits},
              stats);

  SUCCEED();
}
