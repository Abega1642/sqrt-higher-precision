#pragma once
#include <gmpxx.h>
#include <string>
#include <iostream>

class Fraction {
	mpz_class num;
	mpz_class den;

	public:
	explicit Fraction(unsigned long numerator);
	Fraction(unsigned long numerator, unsigned long denominator);
	Fraction(const std::string& numerator, const std::string& denominator);
	Fraction(mpz_class numerator, mpz_class denominator);

	[[nodiscard]] std::string get_value(std::size_t decimal_digits) const;

	[[nodiscard]] const mpz_class& get_num() const;
	[[nodiscard]] const mpz_class& get_den() const;

	void simplify();
	void normalize_sign();

	[[nodiscard]] Fraction inverse() const;
	[[nodiscard]] Fraction negate() const;
	[[nodiscard]] Fraction pow(int n) const;

	Fraction operator+(const Fraction& other) const;
	Fraction operator-(const Fraction& other) const;
	Fraction operator*(const Fraction& other) const;
	Fraction operator/(const Fraction& other) const;
	Fraction& operator+=(const Fraction& other);

	friend std::ostream& operator<<(std::ostream& os, const Fraction& frac);

};
