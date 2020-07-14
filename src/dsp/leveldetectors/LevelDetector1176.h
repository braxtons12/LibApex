#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex {
	namespace dsp {

		/// @brief 1176-style Level Detector for Dynamic Range Processors
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class LevelDetector1176 : LevelDetector<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
			};

		/// @brief 1176-style Level Detector for Dynamic Range Processors
		template<>
			class LevelDetector1176<float> : LevelDetector<float> {
				public:
					/// @brief Constructs a `LevelDetector1176` with the following values:
					/// * attack: 0.39mS,
					/// * release: 525mS,
					/// * sampleRate: 44100Hz
					LevelDetector1176() noexcept;

					/// @brief Constructs a `LevelDetector1176` with the given parameters
					///
					/// @param attackMs - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					LevelDetector1176(float attackMs, float releaseMS, size_t sampleRate) noexcept;

					/// @brief Move constructs a `LevelDetector1176` from the given one
					///
					/// @param detector - The `LevelDetector1176` to move
					LevelDetector1176(LevelDetector1176<float>&& detector) noexcept = default;
					~LevelDetector1176() noexcept override = default;

					LevelDetector1176<float>& operator=(LevelDetector1176<float>&& detector) noexcept = default;

				private:
					static const constexpr float INITIAL_ATTACK_MS = 0.39f;
					static const constexpr float INITIAL_RELEASE_MS = 525.0f;
					static const constexpr size_t INITIAL_SAMPLERATE = 44100;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
			};

		/// @brief 1176-style Level Detector for Dynamic Range Processors
		template<>
			class LevelDetector1176<double> : LevelDetector<double> {
				public:
					/// @brief Constructs a `LevelDetector1176` with the following values:
					/// * attack: 0.39mS,
					/// * release: 525mS,
					/// * sampleRate: 44100Hz
					LevelDetector1176() noexcept;

					/// @brief Constructs a `LevelDetector1176` with the given parameters
					///
					/// @param attackMs - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					LevelDetector1176(double attackMs, double releaseMS, size_t sampleRate) noexcept;

					/// @brief Move constructs a `LevelDetector1176` from the given one
					///
					/// @param detector - The `LevelDetector1176` to move
					LevelDetector1176(LevelDetector1176<double>&& detector) noexcept = default;
					~LevelDetector1176() noexcept override = default;

					LevelDetector1176<double>& operator=(LevelDetector1176<double>&& detector) noexcept = default;

				private:
					static const constexpr double INITIAL_ATTACK_MS = 0.39;
					static const constexpr double INITIAL_RELEASE_MS = 525.0;
					static const constexpr size_t INITIAL_SAMPLERATE = 44100;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
			};
	}
}
