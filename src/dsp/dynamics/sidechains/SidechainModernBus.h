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
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class SidechainModernBus : Sidechain<T> {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainModernBus)
	};

	/// @brief Modern Bus-style dynamics processor sidechain
	template<>
	class SidechainModernBus<float> : Sidechain<float> {
	  private:
		using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

	  public:
		/// @brief Constructs a `SidechainModernBus` with the following defaults:
		/// * attack: 10ms
		/// * release: 50ms
		/// * ratio: 1.1
		/// * threshold: -12dB
		/// * knee width: 6dB
		/// * sampleRate 44100Hz
		SidechainModernBus() noexcept = default;

		/// @brief Move constructs the given `SidechainModernBus`
		///
		/// @param sidechain - The `SidechainModernBus` to move
		SidechainModernBus(SidechainModernBus<float>&& sidechain) noexcept = default;
		~SidechainModernBus() noexcept override = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		auto process(float input) noexcept -> float override;

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		void setDynamicsType(DynamicsType type) noexcept override;

		auto operator=(SidechainModernBus<float>&& sidechain) noexcept
			-> SidechainModernBus<float>& = default;

	  private:
		LevelDetectorModernBus<float> mLevelDetector = LevelDetectorModernBus<float>(&mState);
		GainReduction<float, float, float> mGainReduction
			= GainReduction<float, float, float>(&mState);
		GainComputerExpander<float, float, float> mExpanderComputer
			= GainComputerExpander<float, float, float>(&mState);
		GainComputerCompressor<float, float, float> mCompressorComputer
			= GainComputerCompressor<float, float, float>(&mState);
		GainComputer<float, float, float>* mGainComputer = &mCompressorComputer;

		auto processFeedForwardAlternateReturnToThreshold(float input) noexcept -> float override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainModernBus)
	};

	/// @brief Modern Bus-style dynamics processor sidechain
	template<>
	class SidechainModernBus<double> : Sidechain<double> {
	  private:
		using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

	  public:
		/// @brief Constructs a `SidechainModernBus` with the following defaults:
		/// * attack: 10ms
		/// * release: 50ms
		/// * ratio: 1.1
		/// * threshold: -12dB
		/// * knee width: 6dB
		/// * sampleRate 44100Hz
		SidechainModernBus() noexcept = default;

		/// @brief Move constructs the given `SidechainModernBus`
		///
		/// @param sidechain - The `SidechainModernBus` to move
		SidechainModernBus(SidechainModernBus<double>&& sidechain) noexcept = default;
		~SidechainModernBus() noexcept override = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		auto process(double input) noexcept -> double override;

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		auto setDynamicsType(DynamicsType type) noexcept -> void override;

		auto operator=(SidechainModernBus<double>&& sidechain) noexcept
			-> SidechainModernBus<double>& = default;

	  private:
		LevelDetectorModernBus<double> mLevelDetector = LevelDetectorModernBus<double>(&mState);
		GainReduction<double, double, double> mGainReduction
			= GainReduction<double, double, double>(&mState);
		GainComputerExpander<double, double, double> mExpanderComputer
			= GainComputerExpander<double, double, double>(&mState);
		GainComputerCompressor<double, double, double> mCompressorComputer
			= GainComputerCompressor<double, double, double>(&mState);
		GainComputer<double, double, double>* mGainComputer = &mCompressorComputer;

		auto processFeedForwardAlternateReturnToThreshold(double input) noexcept -> double override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainModernBus)
	};
} // namespace apex::dsp
