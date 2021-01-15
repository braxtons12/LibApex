#pragma once

#include "../../test/TestConstants.h"
#include "../WaveShaper.h"
#include "gtest/gtest.h"

namespace apex::dsp::test {
	using apex::test::DOUBLE_ACCEPTED_ERROR;
	using apex::test::FLOAT_ACCEPTED_ERROR;

	TEST(WaveShaperTestFloat, softSaturationCase1) {
		float input = 0.7F;
		float expected = 0.749849964F;
		ASSERT_FLOAT_EQ(waveshapers::softSaturation(input), expected);
	}

	TEST(WaveShaperTestFloat, softSaturationCase2) {
		float input = 0.7F;
		float expected = 0.8388198498F;
		ASSERT_FLOAT_EQ(waveshapers::softSaturation(input, 2.0F, 0.8F), expected);
	}

	TEST(WaveShaperTestFloat, softClipCase1) {
		float input = 0.7F;
		float expected = 0.8235294118F;
		ASSERT_FLOAT_EQ(waveshapers::softClip(input), expected);
	}

	TEST(WaveShaperTestFloat, softClipCase2) {
		float input = 0.7F;
		float expected = 0.875F;
		ASSERT_FLOAT_EQ(waveshapers::softClip(input, 2.0F), expected);
	}

	TEST(WaveShaperTestFloat, hardClipCase1) {
		float input = 0.7F;
		float expected = 0.8F;
		ASSERT_NEAR(waveshapers::hardClip(input, 0.8F, 2.0F), expected, FLOAT_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestFloat, hardClipCase2) {
		float input = -0.7F;
		float expected = 0.55F;
		ASSERT_NEAR(waveshapers::hardClip(input, -0.55F, 2.0F), expected, FLOAT_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestDouble, softSaturationCase1) {
		double input = 0.7;
		double expected = 0.749849964;
		ASSERT_NEAR(waveshapers::softSaturation<double>(input), expected, DOUBLE_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestDouble, softSaturationCase2) {
		double input = 0.7;
		double expected = 0.8388198498;
		ASSERT_NEAR(waveshapers::softSaturation<double>(input, 2.0, 0.8),
					expected,
					DOUBLE_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestDouble, softClipCase1) {
		double input = 0.7;
		double expected = 0.8235294118;
		ASSERT_NEAR(waveshapers::softClip<double>(input), expected, DOUBLE_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestDouble, softClipCase2) {
		double input = 0.7;
		double expected = 0.875;
		ASSERT_NEAR(waveshapers::softClip<double>(input, 2.0), expected, DOUBLE_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestDouble, hardClipCase1) {
		double input = 0.7;
		double expected = 0.8;
		ASSERT_NEAR(waveshapers::hardClip<double>(input, 0.8, 2.0),
					expected,
					DOUBLE_ACCEPTED_ERROR);
	}

	TEST(WaveShaperTestDouble, hardClipCase2) {
		double input = -0.7;
		double expected = 0.55;
		ASSERT_NEAR(waveshapers::hardClip<double>(input, -0.55, 2.0),
					expected,
					DOUBLE_ACCEPTED_ERROR);
	}
} // namespace apex::dsp::test
