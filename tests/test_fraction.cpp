#include <gmpxx.h>
#include <gtest/gtest.h>
#include <mpfr.h>

#include <chrono>
#include <sstream>
#include <string>

#include "../include/Fraction.hpp"

namespace {

template <typename Fn>
double measure_ms(Fn&& fn) {
  const auto start = std::chrono::high_resolution_clock::now();
  fn();
  const auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

mpz_class make_large(std::size_t digits, unsigned long offset = 1UL) {
  std::string s(digits, '9');
  mpz_class v(s, 10);
  v -= offset;
  return v;
}

}  // namespace

// ---------------------------------------------------------------------------
// Correctness
// ---------------------------------------------------------------------------

class FractionCorrectnessTest : public ::testing::Test {
 protected:
  const Fraction f_2_3{"4", "6"};
  const Fraction f_neg2_3{"-10", "15"};
  const Fraction f_zero{"0", "5"};
  const Fraction f_7{"7", "1"};
  const Fraction f_3_2{"3", "2"};
  const Fraction f_5_7{"5", "7"};
  const Fraction f_1_3{"1", "3"};
  const Fraction f_1_6{"1", "6"};
};

TEST_F(FractionCorrectnessTest, ConstructorReducesPositive) {
  EXPECT_EQ(f_2_3.get_num(), mpz_class(2));
  EXPECT_EQ(f_2_3.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, ConstructorReducesNegative) {
  EXPECT_EQ(f_neg2_3.get_num(), mpz_class(-2));
  EXPECT_EQ(f_neg2_3.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, ConstructorNormalisesZero) {
  EXPECT_EQ(f_zero.get_num(), mpz_class(0));
  EXPECT_EQ(f_zero.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, StaticConstants) {
  EXPECT_EQ(Fraction::ZERO.get_num(), mpz_class(0));
  EXPECT_EQ(Fraction::ONE.get_num(), mpz_class(1));
  EXPECT_EQ(Fraction::ONE.get_den(), mpz_class(1));
  EXPECT_EQ(Fraction::TWO.get_num(), mpz_class(2));
  EXPECT_EQ(Fraction::TWO.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, AdditionToZero) {
  Fraction r = f_2_3 + f_neg2_3;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(0));
  EXPECT_EQ(r.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, AdditionKnownResult) {
  Fraction r = f_1_3 + f_1_6;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(1));
  EXPECT_EQ(r.get_den(), mpz_class(2));
}

TEST_F(FractionCorrectnessTest, AdditionCommutativity) {
  Fraction ab = f_2_3 + f_1_3;
  ab.simplify();
  Fraction ba = f_1_3 + f_2_3;
  ba.simplify();
  EXPECT_EQ(ab.get_num(), ba.get_num());
  EXPECT_EQ(ab.get_den(), ba.get_den());
}

TEST_F(FractionCorrectnessTest, AdditionAssociativity) {
  Fraction a{"1", "6"}, b{"1", "4"}, c{"1", "3"};
  Fraction lhs = (a + b) + c;
  lhs.simplify();
  Fraction rhs = a + (b + c);
  rhs.simplify();
  EXPECT_EQ(lhs.get_num(), rhs.get_num());
  EXPECT_EQ(lhs.get_den(), rhs.get_den());
}

TEST_F(FractionCorrectnessTest, SubtractionKnownResult) {
  Fraction r = f_2_3 - f_neg2_3;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(4));
  EXPECT_EQ(r.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, SubtractionSelf) {
  Fraction r = f_2_3 - f_2_3;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(0));
}

TEST_F(FractionCorrectnessTest, MultiplicationKnownResult) {
  Fraction r = f_2_3 * f_neg2_3;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(-4));
  EXPECT_EQ(r.get_den(), mpz_class(9));
}

TEST_F(FractionCorrectnessTest, MultiplicationByOne) {
  Fraction r = f_2_3 * Fraction::ONE;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(2));
  EXPECT_EQ(r.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, MultiplicationByZero) {
  Fraction r = f_2_3 * f_zero;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(0));
}

TEST_F(FractionCorrectnessTest, MultiplicationAssociativity) {
  Fraction a{"2", "5"}, b{"3", "7"}, c{"4", "11"};
  Fraction lhs = (a * b) * c;
  lhs.simplify();
  Fraction rhs = a * (b * c);
  rhs.simplify();
  EXPECT_EQ(lhs.get_num(), rhs.get_num());
  EXPECT_EQ(lhs.get_den(), rhs.get_den());
}

TEST_F(FractionCorrectnessTest, Distributivity) {
  Fraction a{"2", "3"}, b{"1", "4"}, c{"1", "5"};
  Fraction lhs = a * (b + c);
  lhs.simplify();
  Fraction rhs = a * b + a * c;
  rhs.simplify();
  EXPECT_EQ(lhs.get_num(), rhs.get_num());
  EXPECT_EQ(lhs.get_den(), rhs.get_den());
}

TEST_F(FractionCorrectnessTest, DivisionKnownResult) {
  Fraction r = f_2_3 / f_neg2_3;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(-1));
  EXPECT_EQ(r.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, DivisionBySelf) {
  Fraction r = f_2_3 / f_2_3;
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(1));
  EXPECT_EQ(r.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, DivisionByZeroThrows) {
  EXPECT_THROW(f_2_3 / f_zero, std::invalid_argument);
}

TEST_F(FractionCorrectnessTest, CompoundAddAssign) {
  Fraction acc{"1", "4"};
  acc += Fraction{"1", "4"};
  acc.simplify();
  EXPECT_EQ(acc.get_num(), mpz_class(1));
  EXPECT_EQ(acc.get_den(), mpz_class(2));
}

TEST_F(FractionCorrectnessTest, CompoundAddAssignChain) {
  Fraction acc = Fraction::ZERO;
  for (int i = 1; i <= 6; ++i) {
    acc += Fraction(1UL, static_cast<unsigned long>(i * (i + 1)));
  }
  acc.simplify();
  EXPECT_EQ(acc.get_num(), mpz_class(6));
  EXPECT_EQ(acc.get_den(), mpz_class(7));
}

TEST_F(FractionCorrectnessTest, NegatePositive) {
  Fraction r = f_2_3.negate();
  EXPECT_EQ(r.get_num(), mpz_class(-2));
  EXPECT_EQ(r.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, NegateNegative) {
  Fraction r = f_neg2_3.negate();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(2));
  EXPECT_EQ(r.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, NegReciprocal) {
  Fraction r = f_2_3.neg_reciprocal();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(-3));
  EXPECT_EQ(r.get_den(), mpz_class(2));
}

TEST_F(FractionCorrectnessTest, InverseCorrect) {
  Fraction r = f_2_3.inverse();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(3));
  EXPECT_EQ(r.get_den(), mpz_class(2));
}

TEST_F(FractionCorrectnessTest, InverseRoundTrip) {
  Fraction r = f_3_2.inverse().inverse();
  r.simplify();
  EXPECT_EQ(r.get_num(), f_3_2.get_num());
  EXPECT_EQ(r.get_den(), f_3_2.get_den());
}

TEST_F(FractionCorrectnessTest, InverseOfZeroThrows) {
  EXPECT_THROW(f_zero.inverse(), std::invalid_argument);
}

TEST_F(FractionCorrectnessTest, InverseNegativeSign) {
  Fraction neg{"-3", "4"};
  Fraction r = neg.inverse();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(-4));
  EXPECT_EQ(r.get_den(), mpz_class(3));
}

TEST_F(FractionCorrectnessTest, SquareCorrect) {
  Fraction r = f_3_2.square();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(9));
  EXPECT_EQ(r.get_den(), mpz_class(4));
}

TEST_F(FractionCorrectnessTest, SquareMatchesPow2) {
  Fraction sq = f_3_2.square();
  sq.simplify();
  Fraction pw = f_3_2.pow(2);
  pw.simplify();
  EXPECT_EQ(sq.get_num(), pw.get_num());
  EXPECT_EQ(sq.get_den(), pw.get_den());
}

TEST_F(FractionCorrectnessTest, SquareOfNegative) {
  Fraction r = f_neg2_3.square();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(4));
  EXPECT_EQ(r.get_den(), mpz_class(9));
}

TEST_F(FractionCorrectnessTest, PowPositive) {
  Fraction r = f_3_2.pow(10);
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class("59049"));
  EXPECT_EQ(r.get_den(), mpz_class("1024"));
}

TEST_F(FractionCorrectnessTest, PowNegative) {
  Fraction r = f_5_7.pow(-3);
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class("343"));
  EXPECT_EQ(r.get_den(), mpz_class("125"));
}

TEST_F(FractionCorrectnessTest, PowZero) {
  Fraction r = f_2_3.pow(0);
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(1));
  EXPECT_EQ(r.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, PowZeroNumerator) {
  Fraction subject{"0", "12345678901234567890"};
  Fraction r = subject.pow(100000);
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(0));
  EXPECT_EQ(r.get_den(), mpz_class(1));
}

TEST_F(FractionCorrectnessTest, StreamOutput) {
  std::ostringstream oss;
  oss << f_2_3;
  EXPECT_EQ(oss.str(), "2/3");
}

TEST_F(FractionCorrectnessTest, GetValue50Digits) {
  const std::string result = Fraction{"1", "7"}.get_value(50);
  EXPECT_EQ(result.substr(0, 12), "0.1428571428");
  EXPECT_EQ(result.size(), 52u);
}

TEST_F(FractionCorrectnessTest, GetValue1Over3AllThrees) {
  const std::string result = Fraction{"1", "3"}.get_value(200);
  ASSERT_EQ(result.substr(0, 2), "0.");
  for (std::size_t i = 2; i < result.size(); ++i) {
    ASSERT_EQ(result[i], '3') << "Unexpected digit at position " << i;
  }
}

// Critical: RawTag path must preserve sign correctness end-to-end
TEST_F(FractionCorrectnessTest, NegRecipThenAddIsCorrect) {
  // neg_reciprocal(2/3) = -3/2; then (1/1) + (-3/2) = -1/2
  Fraction r = Fraction::ONE + f_2_3.neg_reciprocal();
  r.simplify();
  EXPECT_EQ(r.get_num(), mpz_class(-1));
  EXPECT_EQ(r.get_den(), mpz_class(2));
}

TEST_F(FractionCorrectnessTest, ChainedOpsIdentity) {
  // (a * b) / b == a  for any non-zero b
  Fraction a{"7", "11"}, b{"13", "17"};
  Fraction r = (a * b) / b;
  r.simplify();
  EXPECT_EQ(r.get_num(), a.get_num());
  EXPECT_EQ(r.get_den(), a.get_den());
}

// ---------------------------------------------------------------------------
// Performance
// ---------------------------------------------------------------------------

class FractionPerformanceTest : public ::testing::Test {};

TEST_F(FractionPerformanceTest, Addition1000DigitOperands100Reps) {
  const Fraction a(make_large(1000, 3UL), make_large(1000, 7UL));
  const Fraction b(make_large(1000, 11UL), make_large(1000, 13UL));

  const double ms = measure_ms([&]() {
    for (int i = 0; i < 100; ++i) {
      Fraction r = a + b;
      (void)r;
    }
  });

  std::cout << "[PERF] operator+ 1000-digit x100: " << ms << " ms\n";
  EXPECT_LT(ms, 3000.0);
}

TEST_F(FractionPerformanceTest, Multiplication1000DigitOperands100Reps) {
  const Fraction a(make_large(1000, 3UL), make_large(1000, 7UL));
  const Fraction b(make_large(1000, 11UL), make_large(1000, 13UL));

  const double ms = measure_ms([&]() {
    for (int i = 0; i < 100; ++i) {
      Fraction r = a * b;
      (void)r;
    }
  });

  std::cout << "[PERF] operator* 1000-digit x100: " << ms << " ms\n";
  EXPECT_LT(ms, 3000.0);
}

TEST_F(FractionPerformanceTest, Division1000DigitOperands100Reps) {
  const Fraction a(make_large(1000, 3UL), make_large(1000, 7UL));
  const Fraction b(make_large(1000, 11UL), make_large(1000, 13UL));

  const double ms = measure_ms([&]() {
    for (int i = 0; i < 100; ++i) {
      Fraction r = a / b;
      (void)r;
    }
  });

  std::cout << "[PERF] operator/ 1000-digit x100: " << ms << " ms\n";
  EXPECT_LT(ms, 3000.0);
}

// Telescoping series: sum of 1/k(k+1) for k=1..N = N/(N+1)
// Uses operator+= on growing-digit accumulator — validates Knuth path
// correctness AND speed simultaneously.
TEST_F(FractionPerformanceTest, TelescopingSum10000Terms) {
  constexpr int N = 10000;

  Fraction acc = Fraction::ZERO;
  const double ms = measure_ms([&]() {
    for (int k = 1; k <= N; ++k) {
      acc += Fraction(1UL, static_cast<unsigned long>(k) *
                               static_cast<unsigned long>(k + 1));
    }
  });

  acc.simplify();
  EXPECT_EQ(acc.get_num(), mpz_class(N));
  EXPECT_EQ(acc.get_den(), mpz_class(N + 1));

  std::cout << "[PERF] Telescoping += x10000: " << ms << " ms\n";
  EXPECT_LT(ms, 3000.0);
}

// (2/3)^5000 then verify: num=2^5000, den=3^5000
// Validates that RawTag on operator* does not corrupt the result
// and that not calling simplify() on each step is safe here.
TEST_F(FractionPerformanceTest, MultiplicationChain5000Steps) {
  constexpr int N = 5000;
  const Fraction base{"2", "3"};

  Fraction acc = Fraction::ONE;
  const double ms = measure_ms([&]() {
    for (int i = 0; i < N; ++i) {
      acc = acc * base;
    }
  });

  mpz_class expected_num, expected_den;
  mpz_pow_ui(expected_num.get_mpz_t(), mpz_class(2).get_mpz_t(), N);
  mpz_pow_ui(expected_den.get_mpz_t(), mpz_class(3).get_mpz_t(), N);
  EXPECT_EQ(acc.get_num(), expected_num);
  EXPECT_EQ(acc.get_den(), expected_den);

  std::cout << "[PERF] operator* chain x5000: " << ms << " ms\n";
  EXPECT_LT(ms, 5000.0);
}

// Measures RawTag benefit directly: deferred simplify must be
// strictly faster than calling simplify() after every multiply.
TEST_F(FractionPerformanceTest, DeferredSimplifyFasterThanEager) {
  constexpr int N = 200;
  const Fraction base(make_large(200, 3UL), make_large(200, 7UL));

  const double deferred_ms = measure_ms([&]() {
    Fraction acc = Fraction::ONE;
    for (int i = 0; i < N; ++i) acc = acc * base;
    acc.simplify();
  });

  const double eager_ms = measure_ms([&]() {
    Fraction acc = Fraction::ONE;
    for (int i = 0; i < N; ++i) {
      acc = acc * base;
      acc.simplify();
    }
  });

  std::cout << "[PERF] deferred=" << deferred_ms << " ms  eager=" << eager_ms
            << " ms  ratio=" << (eager_ms / deferred_ms) << "x\n";

  EXPECT_LT(deferred_ms, eager_ms);
}

// Measures Knuth benefit directly: operator+ must be faster than
// naive a*d + b*c / b*d on operands with a large shared denominator factor.
TEST_F(FractionPerformanceTest, KnuthAdditionFasterThanNaive) {
  const mpz_class shared = make_large(300, 1UL);
  const Fraction a(make_large(200, 11UL), shared * make_large(50, 3UL));
  const Fraction b(make_large(200, 13UL), shared * make_large(50, 7UL));

  constexpr int reps = 20;

  const double knuth_ms = measure_ms([&]() {
    for (int i = 0; i < reps; ++i) {
      Fraction r = a + b;
      (void)r;
    }
  });

  const double naive_ms = measure_ms([&]() {
    for (int i = 0; i < reps; ++i) {
      mpz_class n = a.get_num() * b.get_den() + b.get_num() * a.get_den();
      mpz_class d = a.get_den() * b.get_den();
      mpz_class g;
      mpz_gcd(g.get_mpz_t(), n.get_mpz_t(), d.get_mpz_t());
      n /= g;
      d /= g;
      (void)n;
      (void)d;
    }
  });

  std::cout << "[PERF] knuth=" << knuth_ms << " ms  naive=" << naive_ms
            << " ms  ratio=" << (naive_ms / knuth_ms) << "x\n";

  EXPECT_LT(knuth_ms, naive_ms);
}

TEST_F(FractionPerformanceTest, GetValue10kDigits) {
  const Fraction f{"1", "7"};

  const double ms = measure_ms([&]() {
    const std::string r = f.get_value(10000);
    ASSERT_EQ(r.substr(0, 12), "0.1428571428");
    ASSERT_EQ(r.size(), 10002u);
  });

  std::cout << "[PERF] get_value 10k digits: " << ms << " ms\n";
  EXPECT_LT(ms, 5000.0);
}

TEST_F(FractionPerformanceTest, GetValue100kDigitsAllThrees) {
  const Fraction f{"1", "3"};

  const double ms = measure_ms([&]() {
    const std::string r = f.get_value(100000);
    ASSERT_EQ(r.size(), 100002u);
    for (std::size_t i = 2; i < r.size(); ++i) {
      ASSERT_EQ(r[i], '3') << "Bad digit at " << i;
    }
  });

  std::cout << "[PERF] get_value 100k digits: " << ms << " ms\n";
  EXPECT_LT(ms, 30000.0);
}