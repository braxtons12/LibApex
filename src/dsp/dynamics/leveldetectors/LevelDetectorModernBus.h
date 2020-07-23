#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex::dsp {
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
				using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
				using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

			public:
				/// @brief Constructs a `LevelDetectorModernBus` with zeroed shared state
				LevelDetectorModernBus() noexcept = default;

				///@brief Constructs a `LevelDetectorModernBus` with the given shared state
				///
				///@param state - The shared state
				explicit LevelDetectorModernBus(DynamicsState* state) noexcept;

				/// @brief Move constructs a `LevelDetectorModernBus` from the given one
				///
				/// @param detector - The `LevelDetectorModernBus` to move
				LevelDetectorModernBus(LevelDetectorModernBus<float>&& detector) noexcept = default;
				~LevelDetectorModernBus() noexcept override = default;

				/// @brief Sets the attack time
				///
				/// @param attackSeconds - The attack time in seconds
				auto setAttackTime(float attackSeconds) noexcept -> void override;

				/// @brief Sets the release time
				///
				/// @param releaseSeconds - The release time in seconds
				auto setReleaseTime(float releaseSeconds) noexcept -> void override;

				/// @brief Sets whether auto release is enabled
				///
				/// @param autoRelease - Whether auto release should be enabled
				auto setAutoRelease(bool autoRelease) noexcept -> void;

				/// @brief Sets the sample rate to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				auto setSampleRate(size_t sampleRate) noexcept -> void override;

				/// @brief Generates the detected level from the given input
				///
				/// @param input - The input to detect on
				///
				/// @return - The detected level
				auto process(float input) noexcept -> float override;

				auto operator=(LevelDetectorModernBus<float>&& detector)
					noexcept -> LevelDetectorModernBus<float>& = default;

			private:
				DynamicsState DEFAULT_STATE = DynamicsState();
				DynamicsState* mState = &DEFAULT_STATE;
				float mY1N1 = 0.0F;
				float mY2N1 = 0.0F;
				static const constexpr float AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47F;
				static const constexpr float AUTO_RELEASE1_S = 0.04277F;
				static const constexpr float AUTO_RELEASE2_S = 2.0F;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
		};

	/// @brief Modern-style Bus compressor Level Detector
	template<>
		class LevelDetectorModernBus<double> : public LevelDetector<double> {
			private:
				using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
				using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

			public:
				/// @brief Constructs a `LevelDetectorModernBus` with zeroed shared state
				LevelDetectorModernBus() noexcept = default;

				///@brief Constructs a `LevelDetectorModernBus` with the given shared state
				///
				///@param state - The shared state
				explicit LevelDetectorModernBus(DynamicsState* state) noexcept;

				/// @brief Move constructs a `LevelDetectorModernBus` from the given one
				///
				/// @param detector - The `LevelDetectorModernBus` to move
				LevelDetectorModernBus(LevelDetectorModernBus<double>&& detector) noexcept = default;
				~LevelDetectorModernBus() noexcept override = default;

				/// @brief Sets the attack time
				///
				/// @param attackSeconds - The attack time in seconds
				auto setAttackTime(double attackSeconds) noexcept -> void override;

				/// @brief Sets the release time
				///
				/// @param releaseSeconds - The release time in seconds
				auto setReleaseTime(double releaseSeconds) noexcept -> void override;

				/// @brief Sets whether auto release is enabled
				///
				/// @param autoRelease - Whether auto release should be enabled
				auto setAutoRelease(bool autoRelease) noexcept -> void;

				/// @brief Sets the sample rate to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				auto setSampleRate(size_t sampleRate) noexcept -> void override;

				/// @brief Generates the detected level from the given input
				///
				/// @param input - The input to detect on
				///
				/// @return - The detected level
				auto process(double input) noexcept -> double override;

				auto operator=(LevelDetectorModernBus<double>&& detector)
					noexcept -> LevelDetectorModernBus<double>& = default;

			private:
				DynamicsState DEFAULT_STATE = DynamicsState();
				DynamicsState* mState = &DEFAULT_STATE;
				double mY1N1 = 0.0;
				double mY2N1 = 0.0;
				static const constexpr double AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47;
				static const constexpr double AUTO_RELEASE1_S = 0.04277;
				static const constexpr double AUTO_RELEASE2_S = 2.0;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
		};
} //namespace apex::dsp
