#include <gtest/gtest.h>

#include <string>

#include "../include/FileReader.hpp"
#include "../include/SquareRoot.hpp"

TEST(SquareRootTest, Sqrt2With6000000DigitsAtK23) {
  constexpr unsigned long target = 2UL;
  ;
  constexpr unsigned long k = 22UL;
  constexpr std::size_t precision_digits = 6'000'0002;
  constexpr std::size_t truncate_to = 6'000'000;
  const std::string data_path = "../data/sqrt_2.txt";

  const SquareRoot subject = SquareRoot::of(target);
  const Fraction result = subject.k_th_value(k);
  const std::string actual =
      result.get_value(precision_digits).substr(0, truncate_to);

  const FileReader reader(data_path);
  const std::string expected = reader.read_all().substr(0, truncate_to);

  EXPECT_EQ(expected, actual);
}
