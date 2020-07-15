#pragma once

#include <type_traits>
#include <utility>

#include "GainComputer.h"
#include "../../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {
		///@brief Gain Computer for compressors
		///
		///@tparam T - The floating point type to back operations
		template<typename T>
			class GainComputerCompressor : public GainComputer<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
			};

		///@brief Gain Computer for compressors
		template<>
			class GainComputerCompressor<float> : public GainComputer<float> {
				public:
					///@brief Constructs a `GainComputerCompressor` with the following defaults:
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					GainComputerCompressor() noexcept = default;

					///@brief Construct a `GainComputerCompressor` with the given parameters
					///
					///@param ratio - The ratio
					///@param threshold - The threshold, in Decibels
					///@param kneeWidth - The knee width, in Decibels
					GainComputerCompressor(float ratio, float threshold, float kneeWidth) noexcept;

					///@brief Move constructs a `GainComputerCompressor` from the given one
					///
					///@param computer - The `GainComputerCompressor` to move
					GainComputerCompressor(GainComputerCompressor<float>&& computer) noexcept = default;
					~GainComputerCompressor() noexcept override = default;

					///@brief Calculates the target compressed output value 
					///
					///@param input - The input to calculate compression for
					///
					///@return - The target output
					float process(float input) noexcept override;

					GainComputerCompressor<float>& operator=(GainComputerCompressor<float>&& comptuer) noexcept = default;
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
			};

		///@brief Gain Computer for compressors
		template<>
			class GainComputerCompressor<double> : public GainComputer<double> {
				public:
					///@brief Constructs a `GainComputerCompressor` with the following defaults:
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					GainComputerCompressor() noexcept = default;

					///@brief Construct a `GainComputerCompressor` with the given parameters
					///
					///@param ratio - The ratio
					///@param threshold - The threshold, in Decibels
					///@param kneeWidth - The knee width, in Decibels
					GainComputerCompressor(double ratio, double threshold, double kneeWidth) noexcept;

					///@brief Move constructs a `GainComputerCompressor` from the given one
					///
					///@param computer - The `GainComputerCompressor` to move
					GainComputerCompressor(GainComputerCompressor<double>&& computer) noexcept = default;
					~GainComputerCompressor() noexcept override = default;

					///@brief Calculates the target compressed output value 
					///
					///@param input - The input to calculate compression for
					///
					///@return - The target output
					double process(double input) noexcept override;

					GainComputerCompressor<double>& operator=(GainComputerCompressor<double>&& comptuer) noexcept = default;
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
			};
	}

}
