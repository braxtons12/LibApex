#include "Decibels.h"

#include "Exponentials.h"

namespace apex {
	namespace math {
		inline float Decibels::linearToDecibels(float linear) {
			return 20.0f * log10(linear);
		}

		inline float Decibels::decibelsToLinear(float decibels) {
			return pow10f(decibels / 20.0f);
		}

		inline double Decibels::linearToDecibels(double linear) {
			return 20.0 * log10(linear);
		}

		inline double Decibels::decibelsToLinear(double decibels) {
			return pow10(decibels / 20.0);
		}
	}
}
