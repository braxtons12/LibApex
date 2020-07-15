 #pragma once

#include <type_traits>
#include <utility>

#include "GainComputer.h"
#include "../../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {
		///@brief Gain Computer for expanders
		///
		///@tparam T - The floating point type to back operations
		template<typename T>
			class GainComputerExpander : GainComputer<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
			};

		///@brief Gain Computer for expanders
		template<>
			class GainComputerExpander<float> : GainComputer<float> {
				public:
					///@brief Constructs a `GainComputerExpander` with the following defaults:
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					GainComputerExpander() noexcept = default;

					///@brief Construct a `GainComputerExpander` with the given parameters
					///
					///@param ratio - The ratio
					///@param threshold - The threshold, in Decibels
					///@param kneeWidth - The knee width, in Decibels
					GainComputerExpander(float ratio, float threshold, float kneeWidth) noexcept;

					///@brief Move constructs a `GainComputerExpander` from the given one
					///
					///@param computer - The `GainComputerExpander` to move
					GainComputerExpander(GainComputerExpander<float>&& computer) noexcept = default;
					~GainComputerExpander() noexcept override = default;

					///@brief Calculates the target expanded output value 
					///
					///@param input - The input to calculate expansion for
					///
					///@return - The target output
					float process(float input) noexcept override;

					GainComputerExpander<float>& operator=(GainComputerExpander<float>&& comptuer) noexcept = default;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
			};

		///@brief Gain Computer for expanders
		template<>
			class GainComputerExpander<double> : GainComputer<double> {
				public:
					///@brief Constructs a `GainComputerExpander` with the following defaults:
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					GainComputerExpander() noexcept = default;

					///@brief Construct a `GainComputerExpander` with the given parameters
					///
					///@param ratio - The ratio
					///@param threshold - The threshold, in Decibels
					///@param kneeWidth - The knee width, in Decibels
					GainComputerExpander(double ratio, double threshold, double kneeWidth) noexcept;

					///@brief Move constructs a `GainComputerExpander` from the given one
					///
					///@param computer - The `GainComputerExpander` to move
					GainComputerExpander(GainComputerExpander<double>&& computer) noexcept = default;
					~GainComputerExpander() noexcept override = default;

					///@brief Calculates the target expanded output value 
					///
					///@param input - The input to calculate expansion for
					///
					///@return - The target output
					double process(double input) noexcept override;

					GainComputerExpander<double>& operator=(GainComputerExpander<double>&& comptuer) noexcept = default;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
			};
	}

}
