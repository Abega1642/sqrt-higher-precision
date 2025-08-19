#include "../include/SquareRoot.hpp"

#include <cmath>

SquareRoot::SquareRoot(const unsigned long natural_number)
    : nat(natural_number),
      root_value(static_cast<unsigned long>(
          std::ceil(std::sqrt(static_cast<double>(nat))))),
      root_value_squared(root_value * root_value),
      square_deviation(root_value_squared - natural_number) {}

SquareRoot SquareRoot::of(const unsigned long numerator) {
  return SquareRoot(numerator);
}

unsigned long SquareRoot::get_nat() const { return nat; }

unsigned long SquareRoot::get_root_value_squared() const {
  return root_value_squared;
}

unsigned long SquareRoot::get_square_deviation() const {
  return square_deviation;
}

unsigned long SquareRoot::get_root_value() const { return root_value; }

Fraction SquareRoot::k_th_value(const unsigned long k) const {
  if (nat == 1) return Fraction::ONE;

  if (nat == root_value_squared) return Fraction(root_value);

  const unsigned long root_sq_plus_n = root_value_squared + nat;

  const Fraction base(root_value);
  Fraction prev(root_value << 1, square_deviation);

  if (k == 0) return base + prev.neg_reciprocal();

  const unsigned long sd_squared = square_deviation * square_deviation;

  Fraction curr((root_value * root_sq_plus_n) << 2, sd_squared);
  Fraction result = base + prev.neg_reciprocal() + curr.neg_reciprocal();

  if (k == 1) return result;

  for (unsigned long i = 2; i <= k; ++i) {
    const Fraction ratio = curr / prev;
    Fraction next = curr * (ratio.square() - Fraction::TWO);

    result += next.neg_reciprocal();

    prev = std::move(curr);
    curr = std::move(next);
  }

  return result;
}
