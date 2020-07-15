#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Base Gain Computer behaviors for use in Dynamic Range Processors's Sidechain
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class GainComputer {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputer)
			};

		/// @brief Base Gain Computer behaviors for use in Dynamic Range Processors's Sidechain
		template<>
			class GainComputer<float> {
				public:

					/// @brief Constructs a `GainComputer` with the following defaults:
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					GainComputer() noexcept = default;

					/// @brief Constructs a `GainComputer` with the given values
					///
					/// @param ratio - The ratio
					/// @param threshold - The threshold, in Decibels
					/// @param kneeWidth - The knee width, in Decibels
					GainComputer(float ratio, float threshold, float kneeWidth) noexcept;

					/// @brief Move constructs a `GainComputer` from the given one
					///
					/// @param computer - The `GainComputer` to move
					GainComputer(GainComputer<float>&& computer) noexcept = default;
					virtual ~GainComputer() noexcept = default;

					/// @brief Sets the ratio to the given value
					///
					/// @param ratio - The new ratio
					void setRatio(float ratio) noexcept;

					/// @brief Returns the current ratio
					///
					/// @return - The current ratio
					float getRatio() const noexcept;

					/// @brief Sets the threshold to the given value
					///
					/// @param threshold - The new threshold, in Decibels
					void setThreshold(float threshold) noexcept;

					/// @brief Returns the current threshold
					///
					/// @return - The current threshold, in Decibels
					float getThreshold() const noexcept;

					/// @brief Sets the knee width to the given value
					///
					/// @param kneeWidth - The new knee width, in Decibels
					void setKneeWidth(float kneeWidth) noexcept;

					/// @brief Returns the current knee width
					///
					/// @return - The current knee width, in Decibels
					float getKneeWidth() const noexcept;

					/// @brief Determines the processed output level from the given input
					///
					/// @param input - The input to process
					///
					/// @return - The resulting output
					virtual float process(float input) noexcept = 0;

					GainComputer<float>& operator=(GainComputer<float>&& computer) noexcept = default;

				protected:
					float mRatio = 1.1f;
					float mThreshold = -12.0f;
					float mKneeWidth = 6.0f;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputer)
			};

		/// @brief Base Gain Computer behaviors for use in Dynamic Range Processors's Sidechain
		template<>
			class GainComputer<double> {
				public:

					/// @brief Constructs a `GainComputer` with the following defaults:
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					GainComputer() noexcept = default;

					/// @brief Constructs a `GainComputer` with the given values
					///
					/// @param ratio - The ratio
					/// @param threshold - The threshold, in Decibels
					/// @param kneeWidth - The knee width, in Decibels
					GainComputer(double ratio, double threshold, double kneeWidth) noexcept;

					/// @brief Move constructs a `GainComputer` from the given one
					///
					/// @param computer - The `GainComputer` to move
					GainComputer(GainComputer<double>&& computer) noexcept = default;
					virtual ~GainComputer() noexcept = default;

					/// @brief Sets the ratio to the given value
					///
					/// @param ratio - The new ratio
					void setRatio(double ratio) noexcept;

					/// @brief Returns the current ratio
					///
					/// @return - The current ratio
					double getRatio() const noexcept;

					/// @brief Sets the threshold to the given value
					///
					/// @param threshold - The new threshold, in Decibels
					void setThreshold(double threshold) noexcept;

					/// @brief Returns the current threshold
					///
					/// @return - The current threshold, in Decibels
					double getThreshold() const noexcept;

					/// @brief Sets the knee width to the given value
					///
					/// @param kneeWidth - The new knee width, in Decibels
					void setKneeWidth(double kneeWidth) noexcept;

					/// @brief Returns the current knee width
					///
					/// @return - The current knee width, in Decibels
					double getKneeWidth() const noexcept;

					/// @brief Determines the processed output level from the given input
					///
					/// @param input - The input to process
					///
					/// @return - The resulting output
					virtual double process(double input) noexcept = 0;

					GainComputer<double>& operator=(GainComputer<double>&& computer) noexcept = default;

				protected:
					double mRatio = 1.1;
					double mThreshold = -12.0;
					double mKneeWidth = 6.0;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputer)
			};
	}
}
