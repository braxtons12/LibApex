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
	template<typename T, typename AttackKind, typename ReleaseKind>
	class GainReductionFET : public GainReduction<T, AttackKind, ReleaseKind> {
	  protected:
		using Field = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>;

	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
		static_assert((std::is_floating_point<AttackKind>::value
					   && std::is_same<T, AttackKind>::value)
						  || std::is_enum<AttackKind>::value,
					  "AttackKind must be the same floating point type as T, or an enum");
		static_assert((std::is_floating_point<ReleaseKind>::value
					   && std::is_same<T, ReleaseKind>::value)
						  || std::is_enum<ReleaseKind>::value,
					  "ReleaseKind must be the same floating point type as T, or an enum");

		/// @brief Constructs a default `GainReductionFET`
		/// (zeroed shared state, rise time = 0.2ms
		GainReductionFET() noexcept : GainReduction<T, AttackKind, ReleaseKind>() {
	#ifdef TESTING_GAIN_REDUCTION_FET
			apex::utils::Logger::LogMessage("Creating Gain Reduction FET");
	#endif
			this->mRiseTimeSeconds = DEFAULT_RISE_TIME;
			this->mRiseCoefficient = gsl::narrow_cast<T>(Exponentials<double>::exp(
				-1.0
				/ (this->mRiseTimeSeconds * static_cast<double>(this->mState->getSampleRate()))));
		}

		/// @brief Constructs a `GainReductonFET` with the given shared state and rise time
		///
		/// @param state - The shared state
		/// @param riseTimeSeconds - The rise time, in seconds
		explicit GainReductionFET(DynamicsState* state,
								  T riseTimeSeconds = DEFAULT_RISE_TIME) noexcept
			: GainReduction<T, AttackKind, ReleaseKind>(state, riseTimeSeconds) {
	#ifdef TESTING_GAIN_REDUCTION_FET
			apex::utils::Logger::LogMessage("Creating Gain Reduction FET");
	#endif
		}

		/// @brief Move constructs the given `GainReductionFET`
		///
		/// @param reduction - The `GainReductionFET` to move
		GainReductionFET(
			GainReductionFET<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
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
			T sign = 1.0;
			if(gainReduction < 0.0) {
				sign = -sign;
				gainReduction *= sign;
			}
			T gainReductionStep
				= waveshapers::softSaturation(gsl::narrow_cast<T>(gainReduction.getLinear()),
											  SLEW_RATE_AMOUNT,
											  SLEW_RATE_SLOPE);

			this->mCurrentGainReduction
				= sign
				  * (this->mCurrentGainReduction * this->mRiseCoefficient
					 + (gsl::narrow_cast<T>(1.0) - this->mRiseCoefficient)
						   * gsl::narrow_cast<T>(Decibels::fromLinear(gainReductionStep)));

			return Decibels::fromLinear(
				waveshapers::softSaturation(gsl::narrow_cast<T>(sign * this->mCurrentGainReduction),
											WAVE_SHAPER_AMOUNT,
											WAVE_SHAPER_SLOPE));
		}

		auto operator=(GainReductionFET<T, AttackKind, ReleaseKind>&& reduction) noexcept
			-> GainReductionFET<T, AttackKind, ReleaseKind>& = default;

	  private:
		/// The "amount" for the `softSaturation` WaveShaper
		static const constexpr T WAVE_SHAPER_AMOUNT = static_cast<T>(-0.2);
		/// The "slope" for the `softSaturation` WaveShaper
		static const constexpr T WAVE_SHAPER_SLOPE = static_cast<T>(0.25);
		/// The "amount" for the slew rate WaveShaper
		static const constexpr T SLEW_RATE_AMOUNT = static_cast<T>(0.4);
		/// The "slope" for the slew rate WaveShaper
		static const constexpr T SLEW_RATE_SLOPE = static_cast<T>(0.4);
		/// The offset for the slew rate WaveShaper
		static const constexpr T SLEW_RATE_OFFSET = static_cast<T>(0.1);
		/// The default rise time
		static const constexpr T DEFAULT_RISE_TIME = static_cast<T>(0.0002);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionFET)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION_FET
