#include "GainStageFET.h"

#include "../WaveShaper.h"

namespace apex::dsp {
	/// @brief Processes the input through the gain stage
	///
	/// @param input - The input to process
	///
	/// @return - The processed output
	auto GainStageFET<float>::process(float input) noexcept -> float {
		return waveshapers::softSaturation(input, SATURATION_AMOUNT, SATURATION_SLOPE);
	}

	/// @brief Processes the input through the gain stage
	///
	/// @param input - The input to process
	///
	/// @return - The processed output
	auto GainStageFET<double>::process(double input) noexcept -> double {
		return waveshapers::softSaturation(input, SATURATION_AMOUNT, SATURATION_SLOPE);
	}
} //namespace apex::dsp
