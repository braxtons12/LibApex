#include "WaveShaper.h"

#include "../math/Exponentials.h"
#include "../math/General.h"

namespace apex {
	namespace dsp {
		namespace waveshapers {

			inline float softSaturation(float input, float amount, float slope) noexcept {
				return input * (1 + amount) / (1 + amount * math::powf(math::fabsf(input), slope));
			}

			inline double softSaturation(double input, double amount, double slope) noexcept {
				return input * (1 + amount) / (1 + amount * math::pow(math::fabs(input), slope));
			}

			inline float softClip(float input, float amount) noexcept {
				return input * (1 + amount) / (1 + amount * math::fabsf(input));
			}

			inline double softClip(double input, double amount) noexcept {
				return input * (1 + amount) / (1 + amount * math::fabs(input));
			}

			inline float hardClip(float input , float clipLevel, float amount) noexcept {
				float x = (input * (1 + amount) / (1 + amount * math::fabsf(input)));
				float signX = x / math::fabsf(x);
				return (math::fabsf(x) > clipLevel ? clipLevel * signX : x);
			}

			inline double hardClip(double input, double clipLevel, double amount) noexcept {
				double x = (input * (1 + amount) / (1 + amount * math::fabs(input)));
				double signX = x / math::fabs(x);
				return (math::fabs(x) > clipLevel ? clipLevel * signX : x);
			}
		}
	}
}
