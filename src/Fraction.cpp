#include "../include/Fraction.hpp"

#include <mpfr.h>

#include <sstream>
#include <utility>
#include <vector>

const Fraction Fraction::ZERO(0UL);
const Fraction Fraction::ONE(1UL);
const Fraction Fraction::TWO(2UL);

Fraction::Fraction(const unsigned long numerator) : num(numerator), den(1) {}

Fraction::Fraction(const unsigned long numerator,
                   const unsigned long denominator)
    : num(numerator), den(denominator) {
  simplify();
}

Fraction::Fraction(const std::string& numerator, const std::string& denominator)
    : num(numerator), den(denominator) {
  simplify();
}

Fraction::Fraction(mpz_class numerator, mpz_class denominator)
    : num(std::move(numerator)), den(std::move(denominator)) {
  simplify();
}

void Fraction::normalize_sign() {
  if (den < 0) {
    num = -num;
    den = -den;
  }
}

void Fraction::simplify() {
  mpz_class gcd;
  mpz_gcd(gcd.get_mpz_t(), num.get_mpz_t(), den.get_mpz_t());
  if (gcd != 0) {
    num /= gcd;
    den /= gcd;
  }
  normalize_sign();
}

Fraction Fraction::negate() const { return {-num, den}; }

Fraction Fraction::neg_reciprocal() const { return {-den, num}; }

std::string Fraction::get_value(const std::size_t decimal_digits) const {
  const auto bits = static_cast<mpfr_prec_t>(decimal_digits * 3.32193 + 100);

  mpfr_t mp_num, mp_den, result;
  mpfr_init2(mp_num, bits);
  mpfr_init2(mp_den, bits);
  mpfr_init2(result, bits);

  mpfr_set_z(mp_num, num.get_mpz_t(), MPFR_RNDN);
  mpfr_set_z(mp_den, den.get_mpz_t(), MPFR_RNDN);
  mpfr_div(result, mp_num, mp_den, MPFR_RNDN);

  std::ostringstream fmt;
  fmt << "%." << decimal_digits << "Rf";

  std::vector<char> buffer(decimal_digits + 20);
  mpfr_snprintf(buffer.data(), buffer.size(), fmt.str().c_str(), result);

  mpfr_clears(mp_num, mp_den, result, static_cast<mpfr_ptr>(nullptr));
  return buffer.data();
}

const mpz_class& Fraction::get_num() const { return num; }
const mpz_class& Fraction::get_den() const { return den; }

Fraction Fraction::inverse() const {
  if (num == 0) throw std::invalid_argument("Cannot invert a zero fraction");
  return {den, num};
}

Fraction Fraction::operator+(const Fraction& other) const {
  return {num * other.den + other.num * den, den * other.den};
}

Fraction& Fraction::operator+=(const Fraction& other) {
  num = num * other.den + other.num * den;
  den = den * other.den;
  simplify();
  return *this;
}

Fraction Fraction::square() const { return {num * num, den * den}; }

Fraction Fraction::pow(const long n) const {
  if (n == 0) return Fraction(1UL);

  mpz_class new_num, new_den;
  if (n > 0) {
    mpz_pow_ui(new_num.get_mpz_t(), num.get_mpz_t(), n);
    mpz_pow_ui(new_den.get_mpz_t(), den.get_mpz_t(), n);
  } else {
    mpz_pow_ui(new_num.get_mpz_t(), den.get_mpz_t(), -n);
    mpz_pow_ui(new_den.get_mpz_t(), num.get_mpz_t(), -n);
  }

  return {new_num, new_den};
}

Fraction Fraction::operator-(const Fraction& other) const {
  return {num * other.den - other.num * den, den * other.den};
}

Fraction Fraction::operator*(const Fraction& other) const {
  return {num * other.num, den * other.den};
}

Fraction Fraction::operator/(const Fraction& other) const {
  return {num * other.den, den * other.num};
}

std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
  const mpz_class numerator = frac.num;
  const mpz_class denominator = frac.den;
  os << numerator << "/" << denominator;
  return os;
}
