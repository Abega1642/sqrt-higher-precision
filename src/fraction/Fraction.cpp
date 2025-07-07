#include "Fraction.hpp"
#include <mpfr.h>
#include <sstream>

Fraction::Fraction(const std::string& numerator, const std::string& denominator)
    : num(numerator), den(denominator) {
    normalize_sign();
}

Fraction::Fraction(const mpz_class& numerator, const mpz_class& denominator)
    : num(numerator), den(denominator) {
    normalize_sign();
}

void Fraction::normalize_sign() {
    if (den < 0) {
        num = -num;
        den = -den;
    }
}

std::string Fraction::get_value(std::size_t decimal_digits) const {
    mpfr_prec_t bits = decimal_digits * 3.32 + 100;

    mpfr_t mp_num, mp_den, result;
    mpfr_init2(mp_num, bits);
    mpfr_init2(mp_den, bits);
    mpfr_init2(result, bits);

    mpfr_set_z(mp_num, num.get_mpz_t(), MPFR_RNDN);
    mpfr_set_z(mp_den, den.get_mpz_t(), MPFR_RNDN);
    mpfr_div(result, mp_num, mp_den, MPFR_RNDN);

    char* c_str = nullptr;
    mpfr_asprintf(&c_str, "%.Rf", result);
    std::string decimal_result(c_str);
    mpfr_free_str(c_str);

    mpfr_clears(mp_num, mp_den, result, (mpfr_ptr) 0);
    return decimal_result;
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

const mpz_class& Fraction::get_num() const { return num; }
const mpz_class& Fraction::get_den() const { return den; }

Fraction Fraction::operator+(const Fraction& other) const {
    mpz_class n = num * other.den + other.num * den;
    mpz_class d = den * other.den;
    return Fraction(n, d);
}

Fraction Fraction::operator-(const Fraction& other) const {
    mpz_class n = num * other.den - other.num * den;
    mpz_class d = den * other.den;
    return Fraction(n, d);
}

Fraction Fraction::operator*(const Fraction& other) const {
    return Fraction(num * other.num, den * other.den);
}

Fraction Fraction::operator/(const Fraction& other) const {
    return Fraction(num * other.den, den * other.num);
}

std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
    os << fraction.num << "/" << fraction.den;
    return os;
}