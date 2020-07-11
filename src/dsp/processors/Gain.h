#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Processor.h"

namespace apex {
	namespace dsp {

		/// @brief Class used for managing, storing, and applying a specifc gain.
		///
		/// @tparam T - The floating point type to operate with, either `float` or `double`
		template<typename T>
			class Gain : public Processor<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "Gain must be instantiated with a floating point template parameter (float or double)");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
			};

		/// @brief Class used for managing, storing, and applying a specifc gain; operating with `float`s
		template<>
			class Gain<float> : Processor<float> {
				public:

					/// @brief Constructs a default `Gain` with an initial linear value of 1.0
					Gain() noexcept;

					/// @brief Constructs a `Gain` with the given gain value.
					///
					/// @param gain - The gain value to use
					/// @param gainIsDecibels - Whether the gain value is in Decibels
					Gain(float gain, bool gainIsDecibels = false) noexcept;
					Gain(const Gain<float>&& gain) noexcept;
					~Gain() noexcept override;

					/// @brief Sets the gain of this `Gain` to be the given linear value
					///
					/// @param gain - The linear gain value to set this `Gain` to
					void setGainLinear(float gain) noexcept;

					/// @brief Returns the currently set linear gain value
					///
					/// @return The linear gain value
					float getGainLinear() const noexcept;

					/// @brief Sets the gain of this `Gain` to be the given Decibel value
					///
					/// @param gainDecibels - The Decibel gain value to set this `Gain` to
					void setGainDecibels(float gainDecibels) noexcept;

					/// @brief Returns the currently set gain value, in Decibels
					///
					/// @return The gain value, in Decibels
					float getGainDecibels() const noexcept;

					/// @brief Applies this `Gain` to the input
					///
					/// @param input - The input to apply the gain to
					///
					/// @return The resulting value after applying the gain
					float process(float input) noexcept override;

					/// @brief Applies this `Gain` to the pair of input values
					///
					/// @param inputL - The left input to apply the gain to
					/// @param inputR - The right input to apply the gain to
					///
					/// @return - The resulting pair of values after applying the gain
					std::tuple<float, float> process(float inputL, float inputR) const noexcept;

					/// @brief Applies this `Gain` to the array of input values, in place
					///
					/// @param input - The array of input values to apply gain to
					/// @param numSamples - The number of samples
					void process(float* input, size_t numSamples) noexcept override;

					void reset() noexcept override {

					}

					Gain<float> operator=(const Gain<float>&& gain) noexcept;

				private:
					///The linear gain value
					float mGainLinear = 1.0f;
					///The gain value, in Decibels
					float mGainDecibels = 0.0f;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
			};

		/// @brief Class used for managing, storing, and applying a specifc gain; operating with `double`s
		template<>
			class Gain<double> : Processor<double> {
				public:

					/// @brief Constructs a default `Gain` with an initial linear value of 1.0
					Gain() noexcept;

					/// @brief Constructs a `Gain` with the given gain value.
					///
					/// @param gain - The gain value to use
					/// @param gainIsDecibels - Whether the gain value is in Decibels
					Gain(double gain, bool gainIsDecibels = false) noexcept;
					Gain(const Gain<double>&& gain) noexcept;
					~Gain() noexcept override;

					/// @brief Sets the gain of this `Gain` to be the given linear value
					///
					/// @param gain - The linear gain value to set this `Gain` to
					void setGainLinear(double gain) noexcept;

					/// @brief Returns the currently set linear gain value
					///
					/// @return The linear gain value
					double getGainLinear() const noexcept;

					/// @brief Sets the gain of this `Gain` to be the given Decibel value
					///
					/// @param gainDecibels - The Decibel gain value to set this `Gain` to
					void setGainDecibels(double gainDecibels) noexcept;

					/// @brief Returns the currently set gain value, in Decibels
					///
					/// @return The gain value, in Decibels
					double getGainDecibels() const noexcept;

					/// @brief Applies this `Gain` to the input
					///
					/// @param input - The input to apply the gain to
					///
					/// @return The resulting value after applying the gain
					double process(double input) noexcept override;

					/// @brief Applies this `Gain` to the pair of input values
					///
					/// @param inputL - The left input to apply the gain to
					/// @param inputR - The right input to apply the gain to
					///
					/// @return - The resulting pair of values after applying the gain
					std::tuple<double, double> process(double inputL, double inputR) const noexcept;

					/// @brief Applies this `Gain` to the array of input values, in place
					///
					/// @param input - The array of input values to apply gain to
					/// @param numSamples - The number of samples
					void process(double* input, size_t numSamples) noexcept override;

					void reset() noexcept override {

					}

					Gain<double> operator=(const Gain<double>&& gain) noexcept;

				private:
					///The linear gain value
					double mGainLinear = 1.0;
					///The gain value, in Decibels
					double mGainDecibels = 0.0;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
			};
	}
}
