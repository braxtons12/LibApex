#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex {
	namespace dsp {
		/// @brief Modern-style Bus compressor Level Detector
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class LevelDetectorModernBus : public LevelDetector<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
			};

		/// @brief Modern-style Bus compressor Level Detector
		template<>
			class LevelDetectorModernBus<float> : public LevelDetector<float> {
				private:
					typedef typename apex::dsp::DynamicsState<float, float, float>::Field Field;
					typedef typename apex::dsp::DynamicsState<float, float, float> DynamicsState;

				public:
					/// @brief Constructs a `LevelDetectorModernBus` with zeroed shared state
					LevelDetectorModernBus() noexcept;

					///@brief Constructs a `LevelDetectorModernBus` with the given shared state
					///
					///@param state - The shared state
					LevelDetectorModernBus(DynamicsState* state) noexcept;

					/// @brief Move constructs a `LevelDetectorModernBus` from the given one
					///
					/// @param detector - The `LevelDetectorModernBus` to move
					LevelDetectorModernBus(
							LevelDetectorModernBus<float>&& detector) noexcept = default;
					~LevelDetectorModernBus() noexcept override = default;

					/// @brief Sets the attack time
					///
					/// @param attackSeconds - The attack time in seconds
					void setAttackTime(float attackSeconds) noexcept override;

					/// @brief Sets the release time
					///
					/// @param releaseSeconds - The release time in seconds
					void setReleaseTime(float releaseSeconds) noexcept override;

					/// @brief Sets whether auto release is enabled
					///
					/// @param autoRelease - Whether auto release should be enabled
					void setAutoRelease(bool autoRelease) noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					float process(float input) noexcept override;

					LevelDetectorModernBus<float>& operator=(
							LevelDetectorModernBus<float>&& detector) noexcept = default;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					float mY1N1 = 0.0f;
					float mY2N1 = 0.0f;
					static const constexpr float AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47f;
					static const constexpr float AUTO_RELEASE1_S = 0.04277f;
					static const constexpr float AUTO_RELEASE2_S = 2.0f;
						
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
			};

		/// @brief Modern-style Bus compressor Level Detector
		template<>
			class LevelDetectorModernBus<double> : public LevelDetector<double> {
				private:
					typedef typename apex::dsp::DynamicsState<double, double, double>::Field Field;
					typedef typename apex::dsp::DynamicsState<double, double, double> DynamicsState;

				public:
					/// @brief Constructs a `LevelDetectorModernBus` with zeroed shared state
					LevelDetectorModernBus() noexcept;

					///@brief Constructs a `LevelDetectorModernBus` with the given shared state
					///
					///@param state - The shared state
					LevelDetectorModernBus(DynamicsState* state) noexcept;

					/// @brief Move constructs a `LevelDetectorModernBus` from the given one
					///
					/// @param detector - The `LevelDetectorModernBus` to move
					LevelDetectorModernBus(
							LevelDetectorModernBus<double>&& detector) noexcept = default;
					~LevelDetectorModernBus() noexcept override = default;

					/// @brief Sets the attack time
					///
					/// @param attackSeconds - The attack time in seconds
					void setAttackTime(double attackSeconds) noexcept override;

					/// @brief Sets the release time
					///
					/// @param releaseSeconds - The release time in seconds
					void setReleaseTime(double releaseSeconds) noexcept override;

					/// @brief Sets whether auto release is enabled
					///
					/// @param autoRelease - Whether auto release should be enabled
					void setAutoRelease(bool autoRelease) noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					double process(double input) noexcept override;

					LevelDetectorModernBus<double>& operator=(
							LevelDetectorModernBus<double>&& detector) noexcept = default;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					double mY1N1 = 0.0f;
					double mY2N1 = 0.0f;
					static const constexpr double AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47f;
					static const constexpr double AUTO_RELEASE1_S = 0.04277f;
					static const constexpr double AUTO_RELEASE2_S = 2.0f;
						
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
			};
	}
}
