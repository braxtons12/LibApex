#pragma once

#ifndef __MSC_VER
	#include <cmath>
#endif

#include "../../test/TestConstants.h"
#include "../TrigFuncs.h"
#include "gtest/gtest.h"

namespace apex::math::test {
	using apex::test::FLOAT_ACCEPTED_ERROR;

	TEST(TrigFuncsTestFloat, cosfCase1) {
		float input = Constants<>::pi;
		ASSERT_NEAR(Trig<>::cos(input), std::cos(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, cosfCase2) {
		float input = Constants<>::piOver2;
		ASSERT_NEAR(Trig<>::cos(input), std::cos(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, cosfCase3) {
		float input = Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::cos(input), std::cos(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, cosfCase4) {
		float input = -Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::cos(input), std::cos(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, sinfCase1) {
		float input = Constants<>::pi;
		ASSERT_NEAR(Trig<>::sin(input), std::sin(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, sinfCase2) {
		float input = Constants<>::piOver2;
		ASSERT_NEAR(Trig<>::sin(input), std::sin(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, sinfCase3) {
		float input = Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::sin(input), std::sin(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, sinfCase4) {
		float input = -Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::sin(input), std::sin(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanfCase1) {
		float input = Constants<>::pi;
		ASSERT_NEAR(Trig<>::tan(input), std::tan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanfCase2) {
		float input = Constants<>::piOver12;
		ASSERT_NEAR(Trig<>::tan(input), std::tan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanfCase3) {
		float input = Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::tan(input), std::tan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanfCase4) {
		float input = -Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::tan(input), std::tan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, atanfCase1) {
		float input = Constants<>::pi;
		ASSERT_NEAR(Trig<>::atan(input), std::atan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, atanfCase2) {
		float input = Constants<>::piOver12;
		ASSERT_NEAR(Trig<>::atan(input), std::atan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, atanfCase3) {
		float input = Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::atan(input), std::atan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, atanfCase4) {
		float input = -Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::atan(input), std::atan(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanhfCase1) {
		float input = Constants<>::pi;
		ASSERT_NEAR(Trig<>::tanh(input), std::tanh(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanhfCase2) {
		float input = Constants<>::piOver12;
		ASSERT_NEAR(Trig<>::tanh(input), std::tanh(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanhfCase3) {
		float input = Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::tanh(input), std::tanh(input), FLOAT_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestFloat, tanhfCase4) {
		float input = -Constants<>::piOver4;
		ASSERT_NEAR(Trig<>::tanh(input), std::tanh(input), FLOAT_ACCEPTED_ERROR);
	}
} // namespace apex::math::test
