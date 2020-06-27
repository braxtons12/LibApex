#include "WaveShaper.h"

#include "../math/Exponentials.h"
#include "../math/General.h"

namespace apex {
	namespace dsp {
		namespace waveshapers {

			static inline double softSaturation(double input, double amount, double slope) {
				return input * (1 + amount) / (1 + amount * math::pow(math::fabs(input), slope));
			}

			static inline float softSaturation(float input, float amount, float slope) {
				return input * (1 + amount) / (1 + amount * math::powf(math::fabsf(input), slope));
			}

			static inline double softClip(double input, double amount) {
				return input * (1 + amount) / (1 + amount * math::fabs(input));
			}

			static inline float softClip(float input, float amount) {
				return input * (1 + amount) / (1 + amount * math::fabsf(input));
			}

			static inline double hardClip(double input, double clipLevel, double amount) {
				double x = (input * (1 + amount) / (1 + amount * math::fabs(input)));
				double signX = x / math::fabs(x);
				return (math::fabs(x) > clipLevel ? clipLevel * signX : x);
			}

			static inline float hardClip(float input , float clipLevel, float amount) {
				float x = (input * (1 + amount) / (1 + amount * math::fabsf(input)));
				float signX = x / math::fabsf(x);
				return (math::fabsf(x) > clipLevel ? clipLevel * signX : x);
			}
		}
	}
}
