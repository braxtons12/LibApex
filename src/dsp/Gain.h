#pragma once

#include <type_traits>
#include <utility>

#include "../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Class used for managing, storing, and applying a specifc gain.
		///
		/// @tparam T - The floating point type to operate with, either `float` or `double`
		template<typename T>
			class Gain {
				public:
					static_assert(std::is_floating_point<T>::value, "Gain must be instantiated with a floating point template parameter (float or double)");
			};

		/// @brief Class used for managing, storing, and applying a specifc gain; operating with `float`s
		template<>
			class Gain<float> {
				public:

					/// @brief Constructs a default `Gain` with an initial linear value of 1.0
					Gain();

					/// @brief Constructs a `Gain` with the given gain value.
					///
					/// @param gain - The gain value to use
					/// @param gainIsDecibels - Whether the gain value is in Decibels
					Gain(float gain, bool gainIsDecibels = false);
					~Gain();

					/// @brief Sets the gain of this `Gain` to be the given linear value
					///
					/// @param gain - The linear gain value to set this `Gain` to
					void setGainLinear(float gain);

					/// @brief Returns the currently set linear gain value
					///
					/// @return The linear gain value
					float getGainLinear() const;

					/// @brief Sets the gain of this `Gain` to be the given Decibel value
					///
					/// @param gainDecibels - The Decibel gain value to set this `Gain` to
					void setGainDecibels(float gainDecibels);

					/// @brief Returns the currently set gain value, in Decibels
					///
					/// @return The gain value, in Decibels
					float getGainDecibels() const;

					/// @brief Applies this `Gain` to the input
					///
					/// @param input - The input to apply the gain to
					///
					/// @return The resulting value after applying the gain
					float process(float input) const;

					/// @brief Applies this `Gain` to the input, in place
					///
					/// @param input - The input to apply the gain to
					void process(float& input) const;

					/// @brief Applies this `Gain` to the pair of input values
					///
					/// @param inputL - The left input to apply the gain to
					/// @param inputR - The right input to apply the gain to
					///
					/// @return - The resulting pair of values after applying the gain
					std::tuple<float, float> process(float inputL, float inputR) const;

					/// @brief Applies this `Gain` to the pair of input values, in place
					///
					/// @param inputL - The left input to apply the gain to
					/// @param inputR - The right input to apply the gain to
					void process(float& inputL, float& inputR) const;

					/// @brief Applies this `Gain` to the block of input values, in place
					///
					/// @param input - The block of input values to apply the gain to
					/// @param numChannels - The number of channels in the input block
					/// @param numSamples - The number of samples in the input block
					void processBlock(float** input, size_t numChannels, size_t numSamples) const;

				private:
					///The linear gain value
					float mGainLinear = 1.0f;
					///The gain value, in Decibels
					float mGainDecibels = 0.0f;
			};

		/// @brief Class used for managing, storing, and applying a specifc gain; operating with `double`s
		template<>
			class Gain<double> {
				public:

					/// @brief Constructs a default `Gain` with an initial linear value of 1.0
					Gain();

					/// @brief Constructs a `Gain` with the given gain value.
					///
					/// @param gain - The gain value to use
					/// @param gainIsDecibels - Whether the gain value is in Decibels
					Gain(double gain, bool gainIsDecibels = false);
					~Gain();

					/// @brief Sets the gain of this `Gain` to be the given linear value
					///
					/// @param gain - The linear gain value to set this `Gain` to
					void setGainLinear(double gain);

					/// @brief Returns the currently set linear gain value
					///
					/// @return The linear gain value
					double getGainLinear() const;

					/// @brief Sets the gain of this `Gain` to be the given Decibel value
					///
					/// @param gainDecibels - The Decibel gain value to set this `Gain` to
					void setGainDecibels(double gainDecibels);

					/// @brief Returns the currently set gain value, in Decibels
					///
					/// @return The gain value, in Decibels
					double getGainDecibels() const;

					/// @brief Applies this `Gain` to the input
					///
					/// @param input - The input to apply the gain to
					///
					/// @return The resulting value after applying the gain
					double process(double input) const;

					/// @brief Applies this `Gain` to the input, in place
					///
					/// @param input - The input to apply the gain to
					void process(double& input) const;

					/// @brief Applies this `Gain` to the pair of input values
					///
					/// @param inputL - The left input to apply the gain to
					/// @param inputR - The right input to apply the gain to
					///
					/// @return - The resulting pair of values after applying the gain
					std::tuple<double, double> process(double inputL, double inputR) const;

					/// @brief Applies this `Gain` to the pair of input values, in place
					///
					/// @param inputL - The left input to apply the gain to
					/// @param inputR - The right input to apply the gain to
					void process(double& inputL, double& inputR) const;

					/// @brief Applies this `Gain` to the block of input values, in place
					///
					/// @param input - The block of input values to apply the gain to
					/// @param numChannels - The number of channels in the input block
					/// @param numSamples - The number of samples in the input block
					void processBlock(double** input, size_t numChannels, size_t numSamples) const;

				private:
					///The linear gain value
					double mGainLinear = 1.0;
					///The gain value, in Decibels
					double mGainDecibels = 0.0;
			};
	}
}
