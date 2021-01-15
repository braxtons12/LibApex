#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../../WaveShaper.h"
#include "GainReduction.h"

#ifndef GAIN_REDUCTION_FET
	#define GAIN_REDUCTION_FET

namespace apex::dsp {
	/// @brief Class for calculating gain reduction values adjusted to roughly model FET topology
	/// behavior
	///
	/// @tparam T - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainReductionFET : public GainReduction<FloatType, AttackKind, ReleaseKind> {
	  protected:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, AttackKind, ReleaseKind>;
		using GainReduction = GainReduction<FloatType, AttackKind, ReleaseKind>;

	  public:
		/// @brief Constructs a default `GainReductionFET`
		/// (zeroed shared state, rise time = 0.2ms
		GainReductionFET() noexcept {
	#ifdef TESTING_GAIN_REDUCTION_FET
			apex::utils::Logger::LogMessage("Creating Gain Reduction FET");
	#endif
			GainReduction::mRiseTimeSeconds = DEFAULT_RISE_TIME;
			GainReduction::mRiseCoefficient
				= calculateRiseCoefficient(GainReduction::mState->getSampleRate());
		}

		/// @brief Constructs a `GainReductonFET` with the given shared state and rise time
		///
		/// @param state - The shared state
		/// @param riseTimeSeconds - The rise time, in seconds
		explicit GainReductionFET(DynamicsState* state,
								  FloatType riseTimeSeconds = DEFAULT_RISE_TIME) noexcept
			: GainReduction(state, riseTimeSeconds) {
	#ifdef TESTING_GAIN_REDUCTION_FET
			apex::utils::Logger::LogMessage("Creating Gain Reduction FET");
	#endif
		}

		/// @brief Move constructs the given `GainReductionFET`
		///
		/// @param reduction - The `GainReductionFET` to move
		GainReductionFET(GainReductionFET&& reduction) noexcept = default;
		~GainReductionFET() noexcept override = default;

		/// @brief Calculates the adjusted gain reduction based on this `GainReductionFET`'s
		/// parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return  - The adjusted gain reduction
		[[nodiscard]] auto
		adjustedGainReduction(Decibels gainReduction) noexcept -> Decibels override {
	#ifdef TESTING_GAIN_REDUCTION_FET
			apex::utils::Logger::LogMessage(
				"Gain Reduction FET Calculating Adjusted Gain Reduction");
	#endif
			auto sign = narrow_cast<FloatType>(1.0);
			if(gainReduction < narrow_cast<FloatType>(0.0)) {
				sign = -sign;
				gainReduction *= sign;
			}
			FloatType gainReductionStep = waveshapers::softSaturation<FloatType>(
				narrow_cast<FloatType>(gainReduction.getLinear()),
				SLEW_RATE_AMOUNT,
				SLEW_RATE_SLOPE);

			GainReduction::mCurrentGainReduction
				= sign
				  * (GainReduction::mCurrentGainReduction * GainReduction::mRiseCoefficient
					 + (narrow_cast<FloatType>(1.0) - GainReduction::mRiseCoefficient)
						   * narrow_cast<FloatType>(Decibels::fromLinear(gainReductionStep)));

			return Decibels::fromLinear(
				waveshapers::softSaturation<FloatType>(sign * GainReduction::mCurrentGainReduction,
													   WAVE_SHAPER_AMOUNT,
													   WAVE_SHAPER_SLOPE));
		}

		auto operator=(GainReductionFET&& reduction) noexcept -> GainReductionFET& = default;

	  private:
		/// The "amount" for the `softSaturation` WaveShaper
		static const constexpr FloatType WAVE_SHAPER_AMOUNT = narrow_cast<FloatType>(-0.2);
		/// The "slope" for the `softSaturation` WaveShaper
		static const constexpr FloatType WAVE_SHAPER_SLOPE = narrow_cast<FloatType>(0.25);
		/// The "amount" for the slew rate WaveShaper
		static const constexpr FloatType SLEW_RATE_AMOUNT = narrow_cast<FloatType>(0.4);
		/// The "slope" for the slew rate WaveShaper
		static const constexpr FloatType SLEW_RATE_SLOPE = narrow_cast<FloatType>(0.4);
		/// The offset for the slew rate WaveShaper
		static const constexpr FloatType SLEW_RATE_OFFSET = narrow_cast<FloatType>(0.1);
		/// The default rise time
		static const constexpr FloatType DEFAULT_RISE_TIME = narrow_cast<FloatType>(1.5e-9);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionFET)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION_FET
