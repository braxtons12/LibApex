#pragma once

#ifndef __MSC_VER
	#include <cmath>
#endif

#include <limits>

#include "../../test/TestConstants.h"
#include "../General.h"
#include "gtest/gtest.h"

namespace apex::math::test {
	using apex::test::DOUBLE_ACCEPTED_ERROR;

	TEST(GeneralTestDouble, sqrtCase1) {
		double input = 1.0;
		ASSERT_NEAR(General<double>::sqrt(input), std::sqrt(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, sqrtCase2) {
		double input = 2.0;
		ASSERT_NEAR(General<double>::sqrt(input), std::sqrt(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, sqrtCase3) {
		double input = 3.12345;
		ASSERT_NEAR(General<double>::sqrt(input), std::sqrt(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, sqrtCase4) {
		double input = 6.12345;
		ASSERT_NEAR(General<double>::sqrt(input), std::sqrt(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fabsCase1) {
		double input = 1.0;
		ASSERT_NEAR(General<double>::abs(input), std::fabs(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fabsCase2) {
		double input = 2.0;
		ASSERT_NEAR(General<double>::abs(input), std::fabs(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fabsCase3) {
		double input = 3.12345;
		ASSERT_NEAR(General<double>::abs(input), std::fabs(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fabsCase4) {
		double input = -3.12345;
		ASSERT_NEAR(General<double>::abs(input), std::fabs(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, truncCase1) {
		double input = 1.0;
		ASSERT_EQ(General<double>::trunc(input), std::trunc(input));
	}

	TEST(GeneralTestDouble, truncCase2) {
		double input = 2.0;
		ASSERT_EQ(General<double>::trunc(input), std::trunc(input));
	}

	TEST(GeneralTestDouble, truncCase3) {
		double input = 3.12345;
		ASSERT_EQ(General<double>::trunc(input), std::trunc(input));
	}

	TEST(GeneralTestDouble, truncCase4) {
		double input = -3.12345;
		ASSERT_EQ(General<double>::trunc(input), std::trunc(input));
	}

	TEST(GeneralTestDouble, fmodCase1) {
		double input = 1.0;
		double mod = 0.3;
		ASSERT_NEAR(General<double>::fmod(input, mod),
					std::fmod(input, mod),
					DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fmodCase2) {
		double input = 2.0;
		double mod = 0.3;
		ASSERT_NEAR(General<double>::fmod(input, mod),
					std::fmod(input, mod),
					DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fmodCase3) {
		double input = 3.12345;
		double mod = 0.3;
		ASSERT_NEAR(General<double>::fmod(input, mod),
					std::fmod(input, mod),
					DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, fmodCase4) {
		double input = -3.12345;
		double mod = 0.3;
		ASSERT_NEAR(General<double>::fmod(input, mod),
					std::fmod(input, mod),
					DOUBLE_ACCEPTED_ERROR);
	}

	TEST(GeneralTestDouble, maxCase1) {
		double less = 1.0;
		double more = 3.12345;
		ASSERT_EQ(General<double>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestDouble, maxCase2) {
		double less = -3.0;
		double more = 12.12345;
		ASSERT_EQ(General<double>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestDouble, maxCase3) {
		double less = 7.0;
		double more = -123.12345;
		ASSERT_EQ(General<double>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestDouble, maxCase4) {
		double less = 10.0;
		double more = 123.12345;
		ASSERT_EQ(General<double>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestDouble, roundCase1) {
		double input = 1.5;
		ASSERT_EQ(General<double>::round(input), 2);
	}

	TEST(GeneralTestDouble, roundCase2) {
		double input = -1.5;
		ASSERT_EQ(General<double>::round(input), -1);
	}

	TEST(GeneralTestDouble, roundCase3) {
		double input = 4.2;
		ASSERT_EQ(General<double>::round(input), 4);
	}

	TEST(GeneralTestDouble, roundUCase1) {
		double input = 1.5;
		ASSERT_EQ(General<double>::roundU(input), 2);
	}

	TEST(GeneralTestDouble, roundUCase2) {
		double input = 3.2;
		ASSERT_EQ(General<double>::roundU(input), 3);
	}

	TEST(GeneralTestDouble, roundUCase3) {
		double input = -1.5;
		ASSERT_EQ(General<double>::roundU(input), std::numeric_limits<size_t>::max());
	}
} // namespace apex::math::test
