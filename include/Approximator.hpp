#pragma once
#include "Sequence.hpp"

class Approximator : public Sequence {
	unsigned long root_value;
	unsigned long root_value_squared;
	unsigned long square_deviation;

	public:
		explicit Approximator(unsigned long number);
		Fraction k_th_value(unsigned long k) override;
};

