#pragma once

#include "Exponentials.h"
namespace apex {
	namespace math {
		class Decibels {
			public:
				static inline constexpr float linearToDecibels(float linear) noexcept {
					return 20.0f * log10f(linear);
				}

				static inline constexpr float decibelsToLinear(float decibels) noexcept {
					return pow10f(decibels / 20.0f);
				}

				static inline constexpr double linearToDecibels(double linear) noexcept {
					return 20.0 * log10(linear);
				}

				static inline constexpr double decibelsToLinear(double decibels) noexcept {
					return pow10(decibels / 20.0);
				}
		};
	}
}
