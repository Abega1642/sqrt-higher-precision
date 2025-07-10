#pragma once
#include "Fraction.hpp"

class SquareRoot {
	unsigned long nat;
	unsigned long root_value{};
	unsigned long root_value_squared{};
	unsigned long square_deviation{};

	explicit SquareRoot(unsigned long natural_number);

	public:
		[[nodiscard]] unsigned long get_nat() const;
		[[nodiscard]] unsigned long get_root_value() const;
		[[nodiscard]] unsigned long get_root_value_squared() const;
		[[nodiscard]] unsigned long get_square_deviation() const;
		[[nodiscard]] static SquareRoot of(unsigned long numerator);
		[[nodiscard]] Fraction k_th_value(unsigned long k) const;
};
