#include <gtest/gtest.h>
#include <gmpxx.h>
#include <mpfr.h>

#include "../include/Fraction.hpp"

class FractionTest : public ::testing::Test {
	protected:
	Fraction f1{"4", "6"};   // -> 2/3
	Fraction f2{"-10", "15"}; // -> -2/3
	Fraction f3{"7", "1"};   // Whole number
	Fraction f4{"0", "5"};   // -> 0

	void SetUp() override {
		f1.simplify();
		f2.simplify();
	}
};

TEST_F(FractionTest, SimplifyReducesCorrectly) {
	EXPECT_EQ(f1.get_num(), mpz_class("2"));
	EXPECT_EQ(f1.get_den(), mpz_class("3"));

	EXPECT_EQ(f2.get_num(), mpz_class("-2"));
	EXPECT_EQ(f2.get_den(), mpz_class("3"));
}

TEST_F(FractionTest, AdditionWorks) {
	Fraction result = f1 + f2; // 2/3 + (-2/3) = 0
	result.simplify();
	EXPECT_EQ(result.get_num(), 0);
	EXPECT_EQ(result.get_den(), 1);
}

TEST_F(FractionTest, SubtractionWorks) {
	Fraction result = f1 - f2; // 2/3 - (-2/3) = 4/3
	result.simplify();
	EXPECT_EQ(result.get_num(), 4);
	EXPECT_EQ(result.get_den(), 3);
}

TEST_F(FractionTest, MultiplicationWorks) {
	Fraction result = f1 * f2; // 2/3 * -2/3 = -4/9
	result.simplify();
	EXPECT_EQ(result.get_num(), -4);
	EXPECT_EQ(result.get_den(), 9);
}

TEST_F(FractionTest, DivisionWorks) {
	Fraction result = f1 / f2; // (2/3) / (-2/3) = -1
	result.simplify();
	EXPECT_EQ(result.get_num(), -1);
	EXPECT_EQ(result.get_den(), 1);
}

TEST_F(FractionTest, DecimalConversionIsAccurate) {
	const std::string decimal = f1.get_value(50);

	mpfr_t actual, expected;
	mpfr_init2(actual, 256);
	mpfr_init2(expected, 256);

	mpfr_set_str(actual, decimal.c_str(), 10, MPFR_RNDN);
	mpfr_set_d(expected, 2.0 / 3.0, MPFR_RNDN);

	mpfr_sub(actual, actual, expected, MPFR_RNDN);
	mpfr_abs(actual, actual, MPFR_RNDN);

	EXPECT_TRUE(mpfr_cmp_d(actual, 1e-15) < 0);

	mpfr_clear(actual);
	mpfr_clear(expected);
}


TEST_F(FractionTest, ZeroFractionBehavesCorrectly) {
	Fraction zero = f4;
	zero.simplify();
	EXPECT_EQ(zero.get_num(), 0);
	EXPECT_EQ(zero.get_den(), 1);
}

TEST_F(FractionTest, StreamOutputIsCorrect) {
	std::stringstream ss;
	ss << f1;
	EXPECT_EQ(ss.str(), "2/3");
}

TEST_F(FractionTest, PowerOfLargeExponent) {
	const Fraction subject("3", "2");
	const Fraction result = subject.pow(10);  // -> 59049 / 1024

	EXPECT_EQ(result.get_num(), mpz_class("59049"));
	EXPECT_EQ(result.get_den(), mpz_class("1024"));
}

TEST_F(FractionTest, PowerWithNegativeExponent) {
	const Fraction subject("5", "7");
	const Fraction result = subject.pow(-3);  // -> 343 / 125

	EXPECT_EQ(result.get_num(), mpz_class("343"));
	EXPECT_EQ(result.get_den(), mpz_class("125"));
}

TEST_F(FractionTest, PowerOfZeroNumerator) {
	const Fraction subject("0", "999999999999999999999999999999999999999");
	const Fraction result = subject.pow(1000000);

	EXPECT_EQ(result.get_num(), 0);
	EXPECT_EQ(result.get_den(), 1);
}

