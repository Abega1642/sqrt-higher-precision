#include "../include/SquareRoot.hpp"
#include "../include/Approximator.hpp"

SquareRoot::SquareRoot(const unsigned long numerator): Sequence(numerator) {}

Fraction SquareRoot::k_th_value(const unsigned long k) {
	if (this->get_n() == 1) {
		return Fraction(1);
	}

	auto sq_sub = Approximator(this->get_n());

	if (this->get_n() == sq_sub.get_root_value_squared())
		return Fraction(sq_sub.get_root_value());

	if (k == 0)
		return Fraction(sq_sub.get_root_value());

	Fraction sum(0UL);

	for (long i = 1; i <= k; ++i) {
		Fraction term = sq_sub.k_th_value(i);
		sum += term.inverse().negate();
	}

	return sum + Fraction(sq_sub.get_root_value());
}
