#pragma once
#include "Sequence.hpp"

class SquareRoot final : public Sequence {
	public:
		explicit SquareRoot(unsigned long numerator);
		[[nodiscard]] Fraction k_th_value(unsigned long k) override;
};
