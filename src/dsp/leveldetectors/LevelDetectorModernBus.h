#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex {
	namespace dsp {
		/// @brief Modern-style Bus compressor Level Detector
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class LevelDetectorModernBus : LevelDetector<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
			};

		/// @brief Modern-style Bus compressor Level Detector
		template<>
			class LevelDetectorModernBus<float> : LevelDetector<float> {
				public:
					/// @brief Constructs a `LevelDetectorModernBus` with the following defaults:
					/// * attack: 30mS
					/// * release: 75mS
					/// * sampleRate: 44100Hz
					/// * auto release: disabled
					LevelDetectorModernBus() noexcept;

					/// @brief Constructs a `LevelDetectorModernBus` with the given parameters
					///
					/// @param attackMS - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					/// @param autoRelease - whether auto release is enabled
					LevelDetectorModernBus(float attackMS, float releaseMS,
							size_t sampleRate, bool autoRelease = false) noexcept;

					/// @brief Move constructs a `LevelDetectorModernBus` from the given one
					///
					/// @param detector - The `LevelDetectorModernBus` to move
					LevelDetectorModernBus(
							LevelDetectorModernBus<float>&& detector) noexcept = default;
					~LevelDetectorModernBus() noexcept override = default;

					/// @brief Sets whether auto release is enabled
					///
					/// @param autoRelease - Whether auto release should be enabled
					void setAutoRelease(bool autoRelease) noexcept;

					/// @brief Returns whether auto release is enabled
					///
					/// @return - If auto release is enabled
					bool getAutoRelease() const noexcept;

					/// @brief Sets the attack time to the given value
					///
					/// @param attackMS - The new attack time, in milliseconds
					void setAttackTime(float attackMS) noexcept override;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in milliseconds
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
					bool mAutoRelease = false;
					float mAutoReleaseAttack2Coeff = 0.9999477652f;
					float mAutoRelease1Coeff = 0.999469962f;
					float mAutoRelease2Coeff = 0.9999886622f;
					float mY1N1 = 0.0f;
					float mY2N1 = 0.0f;
					static const constexpr float AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47f;
					static const constexpr float AUTO_RELEASE1_S = 0.04277f;
					static const constexpr float AUTO_RELEASE2_S = 2.0f;
					static const constexpr float INITIAL_ATTACK_MS = 30.0f;
					static const constexpr float INITIAL_RELEASE_MS = 75.0f;
					static const constexpr size_t INITIAL_SAMPLERATE = 44100;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
			};

		/// @brief Modern-style Bus compressor Level Detector
		template<>
			class LevelDetectorModernBus<double> : LevelDetector<double> {
				public:
					/// @brief Constructs a `LevelDetectorModernBus` with the following defaults:
					/// * attack: 30mS
					/// * release: 75mS
					/// * sampleRate: 44100Hz
					/// * auto release: disabled
					LevelDetectorModernBus() noexcept;

					/// @brief Constructs a `LevelDetectorModernBus` with the given parameters
					///
					/// @param attackMS - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					/// @param autoRelease - whether auto release is enabled
					LevelDetectorModernBus(double attackMS, double releaseMS,
							size_t sampleRate, bool autoRelease = false) noexcept;

					/// @brief Move constructs a `LevelDetectorModernBus` from the given one
					///
					/// @param detector - The `LevelDetectorModernBus` to move
					LevelDetectorModernBus(
							LevelDetectorModernBus<double>&& detector) noexcept = default;
					~LevelDetectorModernBus() noexcept override = default;

					/// @brief Sets whether auto release is enabled
					///
					/// @param autoRelease - Whether auto release should be enabled
					void setAutoRelease(bool autoRelease) noexcept;

					/// @brief Returns whether auto release is enabled
					///
					/// @return - If auto release is enabled
					bool getAutoRelease() const noexcept;

					/// @brief Sets the attack time to the given value
					///
					/// @param attackMS - The new attack time, in milliseconds
					void setAttackTime(double attackMS) noexcept override;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in milliseconds
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
					bool mAutoRelease = false;
					double mAutoReleaseAttack2Coeff = 0.9999477652;
					double mAutoRelease1Coeff = 0.999469962;
					double mAutoRelease2Coeff = 0.9999886622;
					double mY1N1 = 0.0;
					double mY2N1 = 0.0;
					static const constexpr double AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47;
					static const constexpr double AUTO_RELEASE1_S = 0.04277;
					static const constexpr double AUTO_RELEASE2_S = 2.0;
					static const constexpr double INITIAL_ATTACK_MS = 30.0f;
					static const constexpr double INITIAL_RELEASE_MS = 75.0f;
					static const constexpr size_t INITIAL_SAMPLERATE = 44100;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
			};
	}
}
