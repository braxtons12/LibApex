#include "GainStageFET.h"

#include "../WaveShaper.h"

namespace apex {
	namespace dsp {
		float GainStageFET<float>::process(float input) noexcept {
			return waveshapers::softSaturation(input, SATURATION_AMOUNT, SATURATION_SLOPE);
		}

		double GainStageFET<double>::process(double input) noexcept {
			return waveshapers::softSaturation(input, SATURATION_AMOUNT, SATURATION_SLOPE);
		}
	}
}
