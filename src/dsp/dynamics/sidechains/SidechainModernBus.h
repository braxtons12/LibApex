#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "../gaincomputers/GainComputer.h"
#include "../gaincomputers/GainComputerCompressor.h"
#include "../gaincomputers/GainComputerExpander.h"
#include "../gainreductions/GainReduction.h"
#include "../leveldetectors/LevelDetectorModernBus.h"
#include "Sidechain.h"

namespace apex::dsp {
	/// @brief Modern Bus-style dynamics processor sidechain
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class SidechainModernBus : Sidechain<FloatType> {
	  private:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, FloatType, FloatType>;
		using LevelDetectorModernBus = LevelDetectorModernBus<FloatType>;
		using GainReduction = GainReduction<FloatType>;
		using Sidechain = Sidechain<FloatType>;

	  public:
		/// @brief Constructs a `SidechainModernBus` with the following defaults:
		/// * attack: 10ms
		/// * release: 50ms
		/// * ratio: 1.1
		/// * threshold: -12dB
		/// * knee width: 6dB
		/// * sampleRate 44100Hz
		SidechainModernBus() noexcept {
			Sidechain::mLevelDetector = LevelDetectorModernBus(&Sidechain::mState);
		}

		/// @brief Move constructs the given `SidechainModernBus`
		///
		/// @param sidechain - The `SidechainModernBus` to move
		SidechainModernBus(SidechainModernBus&& sidechain) noexcept = default;
		~SidechainModernBus() noexcept override = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		inline auto process(FloatType input) noexcept -> Decibels override {
			return processFeedForwardAlternateReturnToThreshold(input);
		}

		auto operator=(SidechainModernBus&& sidechain) noexcept -> SidechainModernBus& = default;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainModernBus)
	};
} // namespace apex::dsp
