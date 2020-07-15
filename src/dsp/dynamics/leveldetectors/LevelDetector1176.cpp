#include "LevelDetector1176.h"

namespace apex {
	namespace dsp {
		/// @brief Constructs a `LevelDetector1176` with the following values:
		/// * attack: 0.39mS,
		/// * release: 525mS,
		/// * sampleRate: 44100Hz
		LevelDetector1176<float>::LevelDetector1176() noexcept
			: LevelDetector<float>(INITIAL_ATTACK_MS, INITIAL_RELEASE_MS,
					INITIAL_SAMPLERATE, DetectorType::NonCorrected)
			{

			}

		/// @brief Constructs a `LevelDetector1176` with the given parameters
		///
		/// @param attackMs - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		LevelDetector1176<float>::LevelDetector1176(float attackMS, float releaseMS,
				size_t sampleRate) noexcept
			: LevelDetector<float>(attackMS, releaseMS, sampleRate, DetectorType::NonCorrected)
			{

			}

		/// @brief Constructs a `LevelDetector1176` with the following values:
		/// * attack: 0.39mS,
		/// * release: 525mS,
		/// * sampleRate: 44100Hz
		LevelDetector1176<double>::LevelDetector1176() noexcept
			: LevelDetector<double>(INITIAL_ATTACK_MS, INITIAL_RELEASE_MS,
					INITIAL_SAMPLERATE, DetectorType::NonCorrected)
			{

			}

		/// @brief Constructs a `LevelDetector1176` with the given parameters
		///
		/// @param attackMs - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		LevelDetector1176<double>::LevelDetector1176(double attackMS, double releaseMS,
				size_t sampleRate) noexcept
			: LevelDetector<double>(attackMS, releaseMS, sampleRate, DetectorType::NonCorrected)
			{

			}
	}
}
