#include "LevelDetector1176.h"

namespace apex {
	namespace dsp {
		/// @brief Constructs a `LevelDetector1176` with the following values:
		/// * attack: 0.39mS,
		/// * release: 525mS,
		/// * sampleRate: 44100Hz
		LevelDetector1176<float>::LevelDetector1176() noexcept
			: LevelDetector<float>()
			{

			}

		/// @brief Constructs a `LevelDetector1176` with the given shared state
		///
		/// @param state - The shared state
		LevelDetector1176<float>::LevelDetector1176(DynamicsState* state) noexcept
			: LevelDetector<float>(state, DetectorType::NonCorrected)
			{

			}

		/// @brief Constructs a `LevelDetector1176` with the following values:
		/// * attack: 0.39mS,
		/// * release: 525mS,
		/// * sampleRate: 44100Hz
		LevelDetector1176<double>::LevelDetector1176() noexcept
			: LevelDetector<double>()
			{

			}

		/// @brief Constructs a `LevelDetector1176` with the given shared state
		///
		/// @param state - The shared state
		LevelDetector1176<double>::LevelDetector1176(DynamicsState* state) noexcept
			: LevelDetector<double>(state, DetectorType::NonCorrected)
			{

			}
	}
}
