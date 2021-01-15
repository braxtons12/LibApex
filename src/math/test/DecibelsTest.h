#pragma once

#ifndef __MSC_VER
	#include <cmath>
#endif

#include "../../test/TestConstants.h"
#include "../Decibels.h"
#include "gtest/gtest.h"

namespace apex::math::test {
	using apex::test::DOUBLE_ACCEPTED_ERROR;

	TEST(DecibelsTest, LinearToDecibelsFloatStaticCase0dB) {
		float linear = 1.0F;
		float decibels = 0.0F;
		ASSERT_FLOAT_EQ(decibels, Decibels::linearToDecibels(linear));
	}

	TEST(DecibelsTest, LinearToDecibelsFloatStaticCaseNeg3dB) {
		float linear = std::pow(10.0F, -3.0F / 20.0F);
		float decibels = -3.0F;
		ASSERT_FLOAT_EQ(decibels, Decibels::linearToDecibels(linear));
	}

	TEST(DecibelsTest, DecibelsToLinearFloatStaticCase0dB) {
		float decibels = 0.0F;
		float linear = 1.0F;
		ASSERT_FLOAT_EQ(linear, Decibels::decibelsToLinear(decibels));
	}

	TEST(DecibelsTest, DecibelsToLinearFloatStaticCaseNeg3dB) {
		float decibels = -3.0F;
		float linear = std::pow(10.0F, -3.0F / 20.0F);
		ASSERT_FLOAT_EQ(linear, Decibels::decibelsToLinear(decibels));
	}

	TEST(DecibelsTest, LinearToDecibelsDoubleStaticCase0dB) {
		double linear = 1.0;
		double decibels = 0.0;
		ASSERT_NEAR(decibels, Decibels::linearToDecibels(linear), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(DecibelsTest, LinearToDecibelsDoubleStaticCaseNeg3dB) {
		double linear = std::pow(10.0, -3.0 / 20.0);
		double decibels = -3.0;
		ASSERT_NEAR(decibels, Decibels::linearToDecibels(linear), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(DecibelsTest, DecibelsToLinearDoubleStaticCase0dB) {
		double decibels = 0.0;
		double linear = 1.0;
		ASSERT_NEAR(linear, Decibels::decibelsToLinear(decibels), DOUBLE_ACCEPTED_ERROR);
	}

	TEST(DecibelsTest, DecibelsToLinearDoubleStaticCaseNeg3dB) {
		double decibels = -3.0;
		double linear = std::pow(10.0, -3.0 / 20.0);
		ASSERT_NEAR(linear, Decibels::decibelsToLinear(decibels), DOUBLE_ACCEPTED_ERROR);
	}
} // namespace apex::math::test
