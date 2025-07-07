#include "../include/Approximator.hpp"
#include <cmath>

Approximator::Approximator(const unsigned long number) : Sequence(number) {
	root_value = static_cast<long>(std::ceil(std::sqrt(static_cast<double>(get_n()))));
	root_value_squared = root_value * root_value;
	square_deviation = root_value_squared - get_n();
}

Fraction Approximator::k_th_value(const unsigned long k) {
	if (root_value_squared == this->get_n())
		return Fraction(0);

	if (k == 0)
		return Fraction(static_cast<long>(root_value));

	if (k == 1)
		return {root_value << 1, square_deviation};

	Fraction sqK2(root_value << 1, square_deviation);
	Fraction sqK1((root_value * (root_value_squared + this->get_n())) << 2,
				  square_deviation * square_deviation);

	Fraction k_th = sqK1;

	for (long i = 3; i <= k; ++i) {
		Fraction ratio = sqK1 / sqK2;
		Fraction term = ratio.pow(2) + Fraction(-2);
		k_th = sqK1 * term;

		sqK2 = std::move(sqK1);
		sqK1 = std::move(k_th);
	}

	return k_th;
}
