#pragma once

#ifndef __MSC_VER
	#include <cmath>
#endif

#include <limits>

#include "../../test/TestConstants.h"
#include "../General.h"
#include "gtest/gtest.h"

namespace apex::math::test {
	using apex::test::FLOAT_ACCEPTED_ERROR;

	TEST(GeneralTestFloat, sqrtfCase1) {
		float input = 1.0F;
		ASSERT_NEAR(General<>::sqrt(input), std::sqrt(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, sqrtfCase2) {
		float input = 2.0F;
		ASSERT_NEAR(General<>::sqrt(input), std::sqrt(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, sqrtfCase3) {
		float input = 3.12345F;
		ASSERT_NEAR(General<>::sqrt(input), std::sqrt(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, sqrtfCase4) {
		float input = -3.12345F;
		ASSERT_NEAR(General<>::sqrt(input), std::sqrt(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fabsfCase1) {
		float input = 1.0F;
		ASSERT_NEAR(General<>::abs(input), std::fabs(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fabsfCase2) {
		float input = 2.0F;
		ASSERT_NEAR(General<>::abs(input), std::fabs(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fabsfCase3) {
		float input = 3.12345F;
		ASSERT_NEAR(General<>::abs(input), std::fabs(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fabsfCase4) {
		float input = -3.12345F;
		ASSERT_NEAR(General<>::abs(input), std::fabs(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, truncfCase1) {
		float input = 1.0F;
		ASSERT_EQ(General<>::trunc(input), std::truncf(input));
	}

	TEST(GeneralTestFloat, truncfCase2) {
		float input = 2.0F;
		ASSERT_EQ(General<>::trunc(input), std::truncf(input));
	}

	TEST(GeneralTestFloat, truncfCase3) {
		float input = 3.12345F;
		ASSERT_EQ(General<>::trunc(input), std::truncf(input));
	}

	TEST(GeneralTestFloat, truncfCase4) {
		float input = -3.12345F;
		ASSERT_EQ(General<>::trunc(input), std::truncf(input));
	}

	TEST(GeneralTestFloat, fmodfCase1) {
		float input = 1.0F;
		float mod = 0.3F;
		ASSERT_NEAR(General<>::fmod(input, mod), std::fmod(input, mod), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fmodfCase2) {
		float input = 2.0F;
		float mod = 0.3F;
		ASSERT_NEAR(General<>::fmod(input, mod), std::fmod(input, mod), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fmodfCase3) {
		float input = 3.12345F;
		float mod = 0.3F;
		ASSERT_NEAR(General<>::fmod(input, mod), std::fmod(input, mod), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, fmodfCase4) {
		float input = -3.12345F;
		float mod = 0.3F;
		ASSERT_NEAR(General<>::fmod(input, mod), std::fmod(input, mod), FLOAT_ACCEPTED_ERROR);
	}

	TEST(GeneralTestFloat, maxCase1) {
		float less = 1.0F;
		float more = 3.12345F;
		ASSERT_EQ(General<>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestFloat, maxCase2) {
		float less = -3.0F;
		float more = 12.12345F;
		ASSERT_EQ(General<>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestFloat, maxCase3) {
		float less = 7.0F;
		float more = -123.12345F;
		ASSERT_EQ(General<>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestFloat, maxCase4) {
		float less = 10.0F;
		float more = 123.12345F;
		ASSERT_EQ(General<>::max(less, more), std::max(less, more));
	}

	TEST(GeneralTestFloat, roundCase1) {
		float input = 1.5F;
		ASSERT_EQ(General<>::round(input), 2);
	}

	TEST(GeneralTestFloat, roundCase2) {
		float input = -1.5F;
		ASSERT_EQ(General<>::round(input), -1);
	}

	TEST(GeneralTestFloat, roundCase3) {
		float input = 4.2F;
		ASSERT_EQ(General<>::round(input), 4);
	}

	TEST(GeneralTestFloat, roundUCase1) {
		float input = 1.5F;
		ASSERT_EQ(General<>::roundU(input), 2);
	}

	TEST(GeneralTestFloat, roundUCase2) {
		float input = 3.2F;
		ASSERT_EQ(General<>::roundU(input), 3);
	}

	TEST(GeneralTestFloat, roundUCase3) {
		float input = -1.5F;
		ASSERT_EQ(General<>::roundU(input), std::numeric_limits<size_t>::max());
	}
} // namespace apex::math::test
