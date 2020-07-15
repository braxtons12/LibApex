#include "GainStageVCA.h"

#include "../WaveShaper.h"

namespace apex {
	namespace dsp {
		float GainStageVCA<float>::process(float input) noexcept {
			return waveshapers::softSaturation(input, SATURATION_AMOUNT, SATURATION_SLOPE);
		}

		double GainStageVCA<double>::process(double input) noexcept {
			return waveshapers::softSaturation(input, SATURATION_AMOUNT, SATURATION_SLOPE);
		}
	}
}
