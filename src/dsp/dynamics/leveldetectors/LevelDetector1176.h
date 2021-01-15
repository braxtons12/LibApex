#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "LevelDetector.h"

namespace apex::dsp {
	/// @brief 1176-style Level Detector for Dynamic Range Processors
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType, std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class LevelDetector1176 : public LevelDetector<FloatType> {
	  private:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, FloatType, FloatType>;
		using LevelDetector = LevelDetector<FloatType>;

	  public:
		/// @brief Constructs a `LevelDetector1176` with the following values:
		/// * attack: 0.39mS,
		/// * release: 525mS,
		/// * sampleRate: 44100Hz
		LevelDetector1176() noexcept {
#ifdef TESTING_LEVELDETECTOR_1176
			Logger::LogMessage("Creating Level Detector 1176");
#endif
			setAttackTime(DEFAULT_ATTACK_SECONDS);
			setReleaseTime(DEFAULT_RELEASE_SECONDS);
			LevelDetector::setDetectorType(DetectorType::NonCorrected);
		}

		/// @brief Constructs a `LevelDetector1176` with the given shared state
		///
		/// @param state - The shared state
		explicit LevelDetector1176(DynamicsState* state) noexcept
			: LevelDetector(state, DetectorType::NonCorrected) {
#ifdef TESTING_LEVELDETECTOR_1176
			Logger::LogMessage("Creating Level Detector 1176");
#endif
			setAttackTime(DEFAULT_ATTACK_SECONDS);
			setReleaseTime(DEFAULT_RELEASE_SECONDS);
		}

		/// @brief Move constructs a `LevelDetector1176` from the given one
		///
		/// @param detector - The `LevelDetector1176` to move
		LevelDetector1176(LevelDetector1176&& detector) noexcept = default;
		~LevelDetector1176() noexcept override = default;

		auto operator=(LevelDetector1176&& detector) noexcept -> LevelDetector1176& = default;

	  private:
		static const constexpr FloatType DEFAULT_ATTACK_SECONDS = narrow_cast<FloatType>(0.00039);
		static const constexpr FloatType DEFAULT_RELEASE_SECONDS = narrow_cast<FloatType>(0.525);
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
	};
} // namespace apex::dsp
