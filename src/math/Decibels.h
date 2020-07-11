#pragma once

namespace apex {
	namespace math {
		class Decibels {
			public:
				static float linearToDecibels(float linear) noexcept;
				static float decibelsToLinear(float decibels) noexcept;

				static double linearToDecibels(double linear) noexcept;
				static double decibelsToLinear(double decibels) noexcept;
		};
	}
}
