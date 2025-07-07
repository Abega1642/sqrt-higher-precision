#pragma once
#include "Fraction.hpp"

class Sequence {
	unsigned long n = 1;

	static unsigned validator(const unsigned long number) {
		if (number <= 0)
			throw std::invalid_argument("n must be greater than 0");
		return number;
	}
	public:
		virtual ~Sequence() = default;

		explicit Sequence(unsigned long number);
		[[nodiscard]] virtual Fraction k_th_value(unsigned long k) = 0;
		[[nodiscard]] unsigned long get_n() const;

};

