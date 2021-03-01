#pragma once

#include <gtest/gtest.h>

#include "../../math/General.h"
#include "../Interpolator.h"

namespace apex::utils::test {

	TEST(InterpolatorTest, linearInterpolator) {
		auto interp = Interpolator<float, InterpolationType::Linear>(1.0F, 0.0F, 1.0F, 10ULL);

		ASSERT_FLOAT_EQ(0.0F, interp.getNextValue(Some(size_t(0))));
		ASSERT_FLOAT_EQ(1.0F, interp.getNextValue(Some(size_t(10))));
	}

	TEST(InterpolatorTest, logInterpolator) {
		auto interp = Interpolator<float, InterpolationType::Logarithmic>(1.0F, 0.0F, 1.0F, 10ULL);
		auto percent99 = 1.0F * 0.99F;

		ASSERT_FLOAT_EQ(0.0F, interp.getNextValue(Some(size_t(0))));
		// we want to be within 1% of the target value
		ASSERT_TRUE(math::General<float>::abs(interp.getNextValue(Some(size_t(10))) - percent99)
					<= (1.0F - percent99));
	}

	TEST(InterpolatorTest, expInterpolator) {
		auto interp = Interpolator<float, InterpolationType::Exponential>(1.0F, 0.0F, 1.0F, 10ULL);
		auto percent99 = 1.0F * 0.99F;

		ASSERT_FLOAT_EQ(0.0F, interp.getNextValue(Some(size_t(0))));
		// we want to be within 1% of the target value
		ASSERT_TRUE(math::General<float>::abs(interp.getNextValue(Some(size_t(10))) - percent99)
					<= (1.0F - percent99));
	}
} // namespace apex::utils::test
