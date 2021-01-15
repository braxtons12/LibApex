#pragma once

#include "../Frequency.h"
#include "gtest/gtest.h"
#include "juce_core/juce_core.h"

namespace apex::math::test {

	TEST(HertzTest, hertzToRadiansFloatStaticCase1) {
		float input = 15000.0F;
		ASSERT_EQ(Hertz::hertzToRadians(input), input / juce::MathConstants<float>::twoPi);
	}

	TEST(HertzTest, hertzToRadiansFloatStaticCase2) {
		float input = 1000.0F;
		ASSERT_EQ(Hertz::hertzToRadians(input), input / juce::MathConstants<float>::twoPi);
	}

	TEST(HertzTest, hertzToRadiansDoubleStaticCase1) {
		double input = 15000.0;
		ASSERT_EQ(Hertz::hertzToRadians(input), input / juce::MathConstants<double>::twoPi);
	}

	TEST(HertzTest, hertzToRadiansDoubleStaticCase2) {
		double input = 1000.0;
		ASSERT_EQ(Hertz::hertzToRadians(input), input / juce::MathConstants<double>::twoPi);
	}

	TEST(RadiansTest, radiansToHertzFloatStaticCase1) {
		float input = 15000.0F;
		ASSERT_EQ(Radians::radiansToHertz(input), input * juce::MathConstants<float>::twoPi);
	}

	TEST(RadiansTest, radiansToHertzFloatStaticCase2) {
		float input = 1000.0F;
		ASSERT_EQ(Radians::radiansToHertz(input), input * juce::MathConstants<float>::twoPi);
	}

	TEST(RadiansTest, radiansToHertzDoubleStaticCase1) {
		double input = 15000.0;
		ASSERT_EQ(Radians::radiansToHertz(input), input * juce::MathConstants<double>::twoPi);
	}

	TEST(RadiansTest, radiansToHertzDoubleStaticCase2) {
		double input = 1000.0;
		ASSERT_EQ(Radians::radiansToHertz(input), input * juce::MathConstants<double>::twoPi);
	}
} // namespace apex::math::test
