#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../../WaveShaper.h"
#include "GainReduction.h"

#ifndef GAIN_REDUCTION_VCA
	#define GAIN_REDUCTION_VCA

namespace apex::dsp {
	/// @brief Class for calculating gain reduction values adjusted to roughly model VCA topology
	/// behavior
	///
	/// @tparam FloatType - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainReductionVCA : public GainReduction<FloatType, AttackKind, ReleaseKind> {
	  protected:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, AttackKind, ReleaseKind>;
		using GainReduction = GainReduction<FloatType, AttackKind, ReleaseKind>;

	  public:
		/// @brief Constructs a default `GainReductionVCA`
		/// (zeroed shared state, rise time = 0.4ms)
		GainReductionVCA() noexcept {
	#ifdef TESTING_GAIN_REDUCTION_VCA
			apex::utils::Logger::LogMessage("Creating Gain Reduction VCA");
	#endif
			GainReduction::mRiseTimeSeconds = DEFAULT_RISE_TIME;
			GainReduction::mRiseCoefficient
				= calculateRiseCoefficient(GainReduction::mState->getSampleRate());
		}

		/// @brief Constructs a `GainReductionVCA` with the given shared state and rise time
		///
		/// @param state - The shared state
		/// @param riseTimeSeconds - The rise time, in seconds
		explicit GainReductionVCA(DynamicsState* state,
								  FloatType riseTimeSeconds = DEFAULT_RISE_TIME) noexcept
			: GainReduction(state, riseTimeSeconds) {
	#ifdef TESTING_GAIN_REDUCTION_VCA
			apex::utils::Logger::LogMessage("Creating Gain Reduction VCA");
	#endif
		}

		/// @brief Move constructs the given `GainReductionVCA`
		///
		/// @param reduction - The `GainReductionVCA` to use
		GainReductionVCA(GainReductionVCA&& reduction) noexcept = default;
		~GainReductionVCA() noexcept override = default;

		/// @brief Calculates the adjusted gain reduction based on this `GainReductionVCA`'s
		/// parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return - The adjusted gain reduction
		[[nodiscard]] auto
		adjustedGainReduction(Decibels gainReduction) noexcept -> Decibels override {
	#ifdef TESTING_GAIN_REDUCTION_VCA
			apex::utils::Logger::LogMessage(
				"Gain Reduction VCA Calculating Adjusted Gain Reduction");
	#endif
			auto sign = narrow_cast<FloatType>(1.0);
			if(gainReduction < narrow_cast<FloatType>(0.0)) {
				sign = -sign;
				gainReduction *= sign;
			}
			GainReduction::mCurrentGainReduction
				= sign
				  * (GainReduction::mCurrentGainReduction * GainReduction::mRiseCoefficient
					 + (narrow_cast<FloatType>(1.0) - GainReduction::mRiseCoefficient)
						   * narrow_cast<FloatType>(gainReduction));
			return Decibels::fromLinear(
				waveshapers::softSaturation<FloatType>(sign * GainReduction::mCurrentGainReduction,
													   WAVE_SHAPER_AMOUNT,
													   WAVE_SHAPER_SLOPE));
		}

		auto operator=(GainReductionVCA&& reduction) noexcept -> GainReductionVCA& = default;

	  private:
		/// The "amount" for the `softSaturation` wave shaper
		static const constexpr FloatType WAVE_SHAPER_AMOUNT = narrow_cast<FloatType>(0.2);
		/// The "slope" for the `softSaturation` wave shaper
		static const constexpr FloatType WAVE_SHAPER_SLOPE = narrow_cast<FloatType>(0.4);
		/// The default rise time
		static const constexpr FloatType DEFAULT_RISE_TIME = narrow_cast<FloatType>(2e-9);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionVCA)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION_VCA
