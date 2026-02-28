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
unsigned long SquareRoot::get_root_value() const { return root_value; }
unsigned long SquareRoot::get_root_value_squared() const {
  return root_value_squared;
}
unsigned long SquareRoot::get_square_deviation() const {
  return square_deviation;
}

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
  mpz_class rn2, rd2, bracket_num;
  mpz_class g1, g2;
  mpz_class next_num, next_den;
  mpz_class bn_reduced, cd_reduced;

  for (unsigned long i = 2; i <= k; ++i) {
    const Fraction ratio = curr / prev;
    const mpz_class& rn = ratio.get_num();
    const mpz_class& rd = ratio.get_den();

    mpz_mul(rn2.get_mpz_t(), rn.get_mpz_t(), rn.get_mpz_t());
    mpz_mul(rd2.get_mpz_t(), rd.get_mpz_t(), rd.get_mpz_t());
    mpz_mul_2exp(bracket_num.get_mpz_t(), rd2.get_mpz_t(), 1);
    mpz_sub(bracket_num.get_mpz_t(), rn2.get_mpz_t(), bracket_num.get_mpz_t());

    const mpz_class& cn = curr.get_num();
    const mpz_class& cd = curr.get_den();

    mpz_gcd(g1.get_mpz_t(), cn.get_mpz_t(), rd2.get_mpz_t());
    mpz_gcd(g2.get_mpz_t(), bracket_num.get_mpz_t(), cd.get_mpz_t());

    mpz_divexact(next_num.get_mpz_t(), cn.get_mpz_t(), g1.get_mpz_t());
    mpz_divexact(next_den.get_mpz_t(), rd2.get_mpz_t(), g1.get_mpz_t());
    mpz_divexact(bn_reduced.get_mpz_t(), bracket_num.get_mpz_t(),
                 g2.get_mpz_t());
    mpz_divexact(cd_reduced.get_mpz_t(), cd.get_mpz_t(), g2.get_mpz_t());

    mpz_mul(next_num.get_mpz_t(), next_num.get_mpz_t(), bn_reduced.get_mpz_t());
    mpz_mul(next_den.get_mpz_t(), next_den.get_mpz_t(), cd_reduced.get_mpz_t());

    Fraction next(next_num, next_den);

    result += next.neg_reciprocal();

    prev = std::move(curr);
    curr = std::move(next);
  }

  return result;
}