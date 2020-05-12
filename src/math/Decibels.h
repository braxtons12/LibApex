#pragma once

namespace apex {
	namespace math {
		class Decibels {
			public:
				static float linearToDecibels(float linear);
				static float decibelsToLinear(float decibels);

				static double linearToDecibels(double linear);
				static double decibelsToLinear(double decibels);
		};
	}
}
