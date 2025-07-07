#include "../include/Sequence.hpp"

Sequence::Sequence(const unsigned long number)
	: n(validator(number)) {}

unsigned long Sequence::get_n() const {
	return n;
}
