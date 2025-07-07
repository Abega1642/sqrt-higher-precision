#include "../include/Fraction.hpp"
#include <mpfr.h>
#include <sstream>
#include <utility>
#include <vector>

Fraction::Fraction(const std::string& numerator, const std::string& denominator)
    : num(numerator), den(denominator) {
    simplify();
}

Fraction::Fraction(mpz_class  numerator, mpz_class  denominator)
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

    const std::size_t buffer_size = decimal_digits + 20;
    std::vector<char> buffer(buffer_size);

    mpfr_snprintf(buffer.data(), buffer.size(), fmt.str().c_str(), result);

    mpfr_clears(mp_num, mp_den, result, static_cast<mpfr_ptr>(nullptr));
    return buffer.data();
}


const mpz_class& Fraction::get_num() const { return num; }
const mpz_class& Fraction::get_den() const { return den; }

Fraction Fraction::operator+(const Fraction& other) const {
    const mpz_class n = num * other.den + other.num * den;
    const mpz_class d = den * other.den;
    return {n, d};
}

Fraction Fraction::operator-(const Fraction& other) const {
    const mpz_class n = num * other.den - other.num * den;
    const mpz_class d = den * other.den;
    return {n, d};
}

Fraction Fraction::operator*(const Fraction& other) const {
    return {num * other.num, den * other.den};
}

Fraction Fraction::operator/(const Fraction& other) const {
    return {num * other.den, den * other.num};
}

std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
    os << fraction.num << "/" << fraction.den;
    return os;
}
