#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <numeric>
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

void print_stats(const std::string& label, const unsigned long nat,
                 const unsigned long k, const std::size_t precision_digits,
                 const Stats& s) {
  constexpr int w = 12;
  std::cout << "\n[Benchmark] " << label << "\n"
            << "  nat             = " << nat << "\n"
            << "  k               = " << k << "\n"
            << "  precision       = " << precision_digits << " digits\n"
            << "  reps            = " << s.reps << " (+ 1 warmup discarded)\n"
            << std::fixed << std::setprecision(6)
            << "  min             = " << std::setw(w) << s.min_s
            << " s  <-- REFERENCE VALUE\n"
            << "  mean            = " << std::setw(w) << s.mean_s << " s\n"
            << "  max             = " << std::setw(w) << s.max_s << " s\n"
            << "  stddev          = " << std::setw(w) << s.stddev_s << " s\n"
            << std::setprecision(2) << "  cv              = " << s.cv_pct
            << " %\n";

  if (s.cv_pct > 5.0) {
    std::cout << "  [WARNING] cv > 5% -- environment is not stable.\n"
              << "            Run via: sudo ./.shell/bench-marks.sh <binary>\n";
  }
}

}  // namespace

TEST(SquareRootBenchmark, Sqrt2_K22_FractionFormOnly) {
  constexpr unsigned long nat = 2UL;
  constexpr unsigned long k = 22UL;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const Fraction result = subject.k_th_value(k);
        (void)result;
      },
      7);

  print_stats("sqrt(2) k=22 fraction form", nat, k, 0, stats);
  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt2_K22_6MillionDigits) {
  constexpr unsigned long nat = 2UL;
  constexpr unsigned long k = 22UL;
  constexpr std::size_t precision_digits = 6'000'002;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats("sqrt(2) k=22 6M digits", nat, k, precision_digits, stats);
  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt3_K19_1MillionDigits) {
  constexpr unsigned long nat = 3UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats("sqrt(3) k=19 1M digits", nat, k, precision_digits, stats);
  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt5_K19_1MillionDigits) {
  constexpr unsigned long nat = 5UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats("sqrt(5) k=19 1M digits", nat, k, precision_digits, stats);
  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt7_K19_1MillionDigits) {
  constexpr unsigned long nat = 7UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      7);

  print_stats("sqrt(7) k=19 1M digits", nat, k, precision_digits, stats);
  SUCCEED();
}

TEST(SquareRootBenchmark, Sqrt2_K24_10MillionDigits) {
  constexpr unsigned long nat = 2UL;
  constexpr unsigned long k = 24UL;
  constexpr std::size_t precision_digits = 10'000'002;

  const SquareRoot subject = SquareRoot::of(nat);

  const auto stats = measure(
      [&] {
        const std::string result =
            subject.k_th_value(k).get_value(precision_digits);
        (void)result;
      },
      3);

  print_stats("sqrt(2) k=24 10M digits", nat, k, precision_digits, stats);
  SUCCEED();
}