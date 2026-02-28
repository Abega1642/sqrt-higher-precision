#include "../include/Fraction.hpp"

#include <mpfr.h>

#include <sstream>
#include <utility>

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
  if (num == 0) {
    den = 1;
    return;
  }
  mpz_class g;
  mpz_gcd(g.get_mpz_t(), num.get_mpz_t(), den.get_mpz_t());
  if (g > 1) {
    mpz_divexact(num.get_mpz_t(), num.get_mpz_t(), g.get_mpz_t());
    mpz_divexact(den.get_mpz_t(), den.get_mpz_t(), g.get_mpz_t());
  }
  normalize_sign();
}

Fraction Fraction::negate() const {
  mpz_class n;
  mpz_neg(n.get_mpz_t(), num.get_mpz_t());
  return Fraction(std::move(n), den, RawTag{});
}

Fraction Fraction::neg_reciprocal() const {
  mpz_class n;
  mpz_neg(n.get_mpz_t(), den.get_mpz_t());
  return Fraction(std::move(n), num, RawTag{});
}

Fraction Fraction::inverse() const {
  if (num == 0) throw std::invalid_argument("Cannot invert a zero fraction");
  if (num < 0) {
    mpz_class n, d;
    mpz_neg(n.get_mpz_t(), den.get_mpz_t());
    mpz_neg(d.get_mpz_t(), num.get_mpz_t());
    return Fraction(std::move(n), std::move(d), RawTag{});
  }
  return Fraction(den, num, RawTag{});
}

Fraction Fraction::square() const {
  mpz_class n, d;
  mpz_mul(n.get_mpz_t(), num.get_mpz_t(), num.get_mpz_t());
  mpz_mul(d.get_mpz_t(), den.get_mpz_t(), den.get_mpz_t());
  return Fraction(std::move(n), std::move(d), RawTag{});
}

Fraction Fraction::pow(const long n) const {
  if (n == 0) return Fraction(1UL);
  mpz_class new_num, new_den;
  if (n > 0) {
    mpz_pow_ui(new_num.get_mpz_t(), num.get_mpz_t(),
               static_cast<unsigned long>(n));
    mpz_pow_ui(new_den.get_mpz_t(), den.get_mpz_t(),
               static_cast<unsigned long>(n));
  } else {
    mpz_pow_ui(new_num.get_mpz_t(), den.get_mpz_t(),
               static_cast<unsigned long>(-n));
    mpz_pow_ui(new_den.get_mpz_t(), num.get_mpz_t(),
               static_cast<unsigned long>(-n));
  }
  return Fraction(std::move(new_num), std::move(new_den), RawTag{});
}

Fraction Fraction::operator+(const Fraction& other) const {
  mpz_class g;
  mpz_gcd(g.get_mpz_t(), den.get_mpz_t(), other.den.get_mpz_t());

  mpz_class den_a, den_b;
  mpz_divexact(den_a.get_mpz_t(), den.get_mpz_t(), g.get_mpz_t());
  mpz_divexact(den_b.get_mpz_t(), other.den.get_mpz_t(), g.get_mpz_t());

  mpz_class new_num = num * den_b + other.num * den_a;
  mpz_class new_den = den * den_b;

  mpz_class g2;
  mpz_gcd(g2.get_mpz_t(), new_num.get_mpz_t(), g.get_mpz_t());
  if (g2 > 1) {
    mpz_divexact(new_num.get_mpz_t(), new_num.get_mpz_t(), g2.get_mpz_t());
    mpz_divexact(new_den.get_mpz_t(), new_den.get_mpz_t(), g2.get_mpz_t());
  }

  return Fraction(std::move(new_num), std::move(new_den), RawTag{});
}

Fraction Fraction::operator-(const Fraction& other) const {
  mpz_class g;
  mpz_gcd(g.get_mpz_t(), den.get_mpz_t(), other.den.get_mpz_t());

  mpz_class den_a, den_b;
  mpz_divexact(den_a.get_mpz_t(), den.get_mpz_t(), g.get_mpz_t());
  mpz_divexact(den_b.get_mpz_t(), other.den.get_mpz_t(), g.get_mpz_t());

  mpz_class new_num = num * den_b - other.num * den_a;
  mpz_class new_den = den * den_b;

  mpz_class g2;
  mpz_gcd(g2.get_mpz_t(), new_num.get_mpz_t(), g.get_mpz_t());
  if (g2 > 1) {
    mpz_divexact(new_num.get_mpz_t(), new_num.get_mpz_t(), g2.get_mpz_t());
    mpz_divexact(new_den.get_mpz_t(), new_den.get_mpz_t(), g2.get_mpz_t());
  }

  return Fraction(std::move(new_num), std::move(new_den), RawTag{});
}

Fraction Fraction::operator*(const Fraction& other) const {
  mpz_class g1, g2;
  mpz_gcd(g1.get_mpz_t(), num.get_mpz_t(), other.den.get_mpz_t());
  mpz_gcd(g2.get_mpz_t(), other.num.get_mpz_t(), den.get_mpz_t());

  mpz_class a, b, c, d;
  mpz_divexact(a.get_mpz_t(), num.get_mpz_t(), g1.get_mpz_t());
  mpz_divexact(d.get_mpz_t(), other.den.get_mpz_t(), g1.get_mpz_t());
  mpz_divexact(c.get_mpz_t(), other.num.get_mpz_t(), g2.get_mpz_t());
  mpz_divexact(b.get_mpz_t(), den.get_mpz_t(), g2.get_mpz_t());

  mpz_class new_num, new_den;
  mpz_mul(new_num.get_mpz_t(), a.get_mpz_t(), c.get_mpz_t());
  mpz_mul(new_den.get_mpz_t(), b.get_mpz_t(), d.get_mpz_t());

  return Fraction(std::move(new_num), std::move(new_den), RawTag{});
}

Fraction Fraction::operator/(const Fraction& other) const {
  if (other.num == 0) throw std::invalid_argument("Division by zero fraction");
  mpz_class g1, g2;
  mpz_gcd(g1.get_mpz_t(), num.get_mpz_t(), other.num.get_mpz_t());
  mpz_gcd(g2.get_mpz_t(), other.den.get_mpz_t(), den.get_mpz_t());

  mpz_class a, b, c, d;
  mpz_divexact(a.get_mpz_t(), num.get_mpz_t(), g1.get_mpz_t());
  mpz_divexact(c.get_mpz_t(), other.num.get_mpz_t(), g1.get_mpz_t());
  mpz_divexact(d.get_mpz_t(), other.den.get_mpz_t(), g2.get_mpz_t());
  mpz_divexact(b.get_mpz_t(), den.get_mpz_t(), g2.get_mpz_t());

  mpz_class new_num, new_den;
  mpz_mul(new_num.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t());
  mpz_mul(new_den.get_mpz_t(), b.get_mpz_t(), c.get_mpz_t());

  if (new_den < 0) {
    mpz_neg(new_num.get_mpz_t(), new_num.get_mpz_t());
    mpz_neg(new_den.get_mpz_t(), new_den.get_mpz_t());
  }

  return Fraction(std::move(new_num), std::move(new_den), RawTag{});
}

Fraction& Fraction::operator+=(const Fraction& other) {
  mpz_class g;
  mpz_gcd(g.get_mpz_t(), den.get_mpz_t(), other.den.get_mpz_t());

  mpz_class den_a, den_b;
  mpz_divexact(den_a.get_mpz_t(), den.get_mpz_t(), g.get_mpz_t());
  mpz_divexact(den_b.get_mpz_t(), other.den.get_mpz_t(), g.get_mpz_t());

  num = num * den_b + other.num * den_a;
  den = den * den_b;

  mpz_class g2;
  mpz_gcd(g2.get_mpz_t(), num.get_mpz_t(), g.get_mpz_t());
  if (g2 > 1) {
    mpz_divexact(num.get_mpz_t(), num.get_mpz_t(), g2.get_mpz_t());
    mpz_divexact(den.get_mpz_t(), den.get_mpz_t(), g2.get_mpz_t());
  }

  return *this;
}

std::string Fraction::get_value(const std::size_t decimal_digits) const {
  const auto bits = static_cast<mpfr_prec_t>(
      static_cast<double>(decimal_digits) * 3.321928094887362 + 128.0);

  mpfr_t mp_num, mp_den, result;
  mpfr_init2(mp_num, bits);
  mpfr_init2(mp_den, bits);
  mpfr_init2(result, bits);

  mpfr_set_z(mp_num, num.get_mpz_t(), MPFR_RNDN);
  mpfr_set_z(mp_den, den.get_mpz_t(), MPFR_RNDN);
  mpfr_div(result, mp_num, mp_den, MPFR_RNDN);

  char* raw_buf = nullptr;
  mpfr_asprintf(&raw_buf, "%.*Rf", static_cast<int>(decimal_digits), result);

  mpfr_clears(mp_num, mp_den, result, static_cast<mpfr_ptr>(nullptr));

  std::string output(raw_buf);
  mpfr_free_str(raw_buf);
  return output;
}

const mpz_class& Fraction::get_num() const { return num; }
const mpz_class& Fraction::get_den() const { return den; }

std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
  os << frac.num << '/' << frac.den;
  return os;
}