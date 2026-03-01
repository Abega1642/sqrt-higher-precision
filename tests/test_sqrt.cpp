#include <gtest/gtest.h>

#include <string>

#include "../include/FileReader.hpp"
#include "../include/SquareRoot.hpp"

#ifndef PROJECT_SOURCE_DIR
#error \
    "PROJECT_SOURCE_DIR is not defined. Add target_compile_definitions(tests PRIVATE PROJECT_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\") to CMakeLists.txt"
#endif

namespace {

std::string data_path(const std::string& filename) {
  return std::string(PROJECT_SOURCE_DIR) + "/data/" + filename;
}

}  // namespace

TEST(SquareRootTest, Sqrt2With_6_000_000_DigitsAtK22) {
  constexpr unsigned long target = 2UL;
  constexpr unsigned long k = 22UL;
  constexpr std::size_t precision_digits = 6'000'002;
  constexpr std::size_t truncate_to = 6'000'000;

  const SquareRoot subject = SquareRoot::of(target);
  const Fraction result = subject.k_th_value(k);
  const std::string actual =
      result.get_value(precision_digits).substr(0, truncate_to);

  const FileReader reader(data_path("sqrt_2.txt"));
  const std::string expected = reader.read_all().substr(0, truncate_to);

  EXPECT_EQ(expected, actual);
}

TEST(SquareRootTest, Sqrt3With_1_000_000_DigitsAtK19) {
  constexpr unsigned long target = 3UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;
  constexpr std::size_t truncate_to = 1'000'000;

  const SquareRoot subject = SquareRoot::of(target);
  const Fraction result = subject.k_th_value(k);
  const std::string actual =
      result.get_value(precision_digits).substr(0, truncate_to);

  const FileReader reader(data_path("sqrt_3.txt"));
  const std::string expected = reader.read_all().substr(0, truncate_to);

  EXPECT_EQ(expected, actual);
}

TEST(SquareRootTest, Sqrt7With_1_000_000_DigitsAtK19) {
  constexpr unsigned long target = 7UL;
  constexpr unsigned long k = 19UL;
  constexpr std::size_t precision_digits = 1'000'002;
  constexpr std::size_t truncate_to = 1'000'000;

  const SquareRoot subject = SquareRoot::of(target);
  const Fraction result = subject.k_th_value(k);
  const std::string actual =
      result.get_value(precision_digits).substr(0, truncate_to);

  const FileReader reader(data_path("sqrt_7.txt"));
  const std::string expected = reader.read_all().substr(0, truncate_to);

  EXPECT_EQ(expected, actual);
}

TEST(SquareRootTest, Sqrt5With_1_000_000_DigitsAtK20) {
  constexpr unsigned long target = 5UL;
  constexpr unsigned long k = 20UL;
  constexpr std::size_t precision_digits = 1'000'002;
  constexpr std::size_t truncate_to = 1'000'000;

  const SquareRoot subject = SquareRoot::of(target);
  const Fraction result = subject.k_th_value(k);
  const std::string actual =
      result.get_value(precision_digits).substr(0, truncate_to);

  const FileReader reader(data_path("sqrt_5.txt"));
  const std::string expected = reader.read_all().substr(0, truncate_to);

  EXPECT_EQ(expected, actual);
}

TEST(SquareRootTest, Sqrt2With_10_000_000_DigitsAtK24) {
  constexpr unsigned long target = 2UL;
  constexpr unsigned long k = 24UL;
  constexpr std::size_t precision_digits = 10'000'002;
  constexpr std::size_t truncate_to = 10'000'000;

  const SquareRoot subject = SquareRoot::of(target);
  const Fraction result = subject.k_th_value(k);
  const std::string actual =
      result.get_value(precision_digits).substr(0, truncate_to);

  const FileReader reader(data_path("data_sqrt2_10mil.txt"));
  const std::string expected = reader.read_all().substr(0, truncate_to);

  EXPECT_EQ(expected, actual);
}