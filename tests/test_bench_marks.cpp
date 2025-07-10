#include <gtest/gtest.h>
#include <chrono>
#include "../include/SquareRoot.hpp"
#include "../include/Fraction.hpp"


/*
* [Benchmark of SquareRoot::k_th_value]
	 - nat			= 2,
	 - k			= 22,
	 - Precision	= 6000002 decimal digits,
	 - Duration		= 7.79397 seconds
 */

TEST(SquareRootBenchmark, TimeExecutionOfKthValueComputation) {
	constexpr unsigned long target = 2UL;
	constexpr unsigned long k = 22UL;
	constexpr std::size_t precision_digits = 6'000'002;

	const SquareRoot subject = SquareRoot::of(target);

	const auto start = std::chrono::high_resolution_clock::now();

	const Fraction result = subject.k_th_value(k);

	const auto end = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "[Benchmark of SquareRoot::k_th_value] \n "
			  << "\t - nat\t\t\t= " << subject.get_nat()
	          << ",\n\t - k\t\t\t= " << k
			  << ",\n\t - Precision\t= " << precision_digits << " decimal digits"
			  << ",\n\t - Duration\t\t= " << elapsed_seconds.count() << " seconds"
	<< std::endl;

	SUCCEED();
}
