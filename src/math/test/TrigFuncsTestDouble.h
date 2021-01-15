#pragma once

#ifndef __MSC_VER
	#include <cmath>
#endif

#include "../../test/TestConstants.h"
#include "../TrigFuncs.h"
#include "gtest/gtest.h"

namespace apex::math::test {
	using apex::test::DOUBLE_ACCEPTED_ERROR;

	TEST(TrigFuncsTestDouble, cosCase1) {
		double input = Constants<double>::pi;
		ASSERT_NEAR(Trig<double>::cos(input), std::cos(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, cosCase2) {
		double input = Constants<double>::piOver2;
		ASSERT_NEAR(Trig<double>::cos(input), std::cos(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, cosCase3) {
		double input = Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::cos(input), std::cos(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, cosCase4) {
		double input = -Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::cos(input), std::cos(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, sinCase1) {
		double input = Constants<double>::pi;
		ASSERT_NEAR(Trig<double>::sin(input), std::sin(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, sinCase2) {
		double input = Constants<double>::piOver2;
		ASSERT_NEAR(Trig<double>::sin(input), std::sin(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, sinCase3) {
		double input = Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::sin(input), std::sin(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, sinCase4) {
		double input = -Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::sin(input), std::sin(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanCase1) {
		double input = Constants<double>::pi;
		ASSERT_NEAR(Trig<double>::tan(input), std::tan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanCase2) {
		double input = Constants<double>::piOver12;
		ASSERT_NEAR(Trig<double>::tan(input), std::tan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanCase3) {
		double input = Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::tan(input), std::tan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanCase4) {
		double input = -Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::tan(input), std::tan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, atanCase1) {
		double input = Constants<double>::pi;
		ASSERT_NEAR(Trig<double>::atan(input), std::atan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, atanCase2) {
		double input = Constants<double>::piOver12;
		ASSERT_NEAR(Trig<double>::atan(input), std::atan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, atanCase3) {
		double input = Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::atan(input), std::atan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, atanCase4) {
		double input = -Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::atan(input), std::atan(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanhCase1) {
		double input = Constants<double>::pi;
		ASSERT_NEAR(Trig<double>::tanh(input), std::tanh(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanhCase2) {
		double input = Constants<double>::piOver12;
		ASSERT_NEAR(Trig<double>::tanh(input), std::tanh(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanhCase3) {
		double input = Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::tanh(input), std::tanh(input), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(TrigFuncsTestDouble, tanhCase4) {
		double input = -Constants<double>::piOver4;
		ASSERT_NEAR(Trig<double>::tanh(input), std::tanh(input), DOUBLE_ACCEPTED_ERROR);
	}
} // namespace apex::math::test
