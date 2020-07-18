#pragma once

#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../../base/StandardIncludes.h"
#include "../../WaveShaper.h"

namespace apex {
	namespace dsp {
		/// @brief Class for calculating gain reduction values adjusted to roughly model FET topology behavior
		///
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainReductionFET : public GainReduction<T, AttackKind, ReleaseKind> {
				protected:
					typedef typename DynamicsState<T, AttackKind, ReleaseKind>::Field Field;
					typedef typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind> DynamicsState;

				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
					static_assert(std::is_floating_point<AttackKind>::value ||
							std::is_enum<AttackKind>::value,
							"AttacKind must be a floating point type or an enum");
					static_assert(std::is_floating_point<ReleaseKind>::value ||
							std::is_enum<ReleaseKind>::value,
							"ReleaseKind must be a floating point type or an enum");

					/// @brief Constructs a default `GainReductionFET`
					/// (zeroed shared state, rise time = 0.2ms
					GainReductionFET() noexcept
						: GainReduction<T, AttackKind, ReleaseKind>()
						{
							this->mRiseTimeSeconds = DEFAULT_RISE_TIME;
							this->mNumSamplesToTransitionGain = static_cast<size_t>(
									this->mRiseTimeSeconds * this->mState->getSampleRate() + 0.5
									);
						}

					/// @brief Constructs a `GainReductonFET` with the given shared state and rise time
					///
					/// @param state - The shared state
					/// @param riseTimeSeconds - The rise time, in seconds
					GainReductionFET(DynamicsState* state, T riseTimeSeconds = DEFAULT_RISE_TIME)
						noexcept
						: GainReduction<T, AttackKind, ReleaseKind>(state, riseTimeSeconds)
						{

						}

					/// @brief Move constructs the given `GainReductionFET`
					///
					/// @param reduction - The `GainReductionFET` to move
					GainReductionFET(GainReductionFET<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
					virtual ~GainReductionFET() noexcept override = default;

					/// @brief Calculates the adjusted gain reduction based on this `GainReductionFET`'s parameters
					///
					/// @param gainReduction - The gain reduction determined by the gain computer
					///
					/// @return  - The adjusted gain reduction
					T adjustedGainReduction(T gainReduction) noexcept override {
						if(this->mCurrentSample > this->mNumSamplesToTransitionGain)
							this->mCurrentSample = 0;

						T gainReductionStep = (gainReduction - this->mCurrentGainReduction)
							/ static_cast<T>(this->mNumSamplesToTransitionGain - this->mCurrentSample);

						if(math::fabs(gainReductionStep) - 0.001 > 0.0) {
							gainReductionStep = waveshapers::softSaturation(
									this->mCurrentGainReduction +
									(gainReductionStep > 0.0 ? -SLEW_RATE_OFFSET : SLEW_RATE_OFFSET),
									SLEW_RATE_AMOUNT,
									SLEW_RATE_SLOPE);
						}

						this->mCurrentGainReduction += gainReductionStep;
						this->mCurrentSample++;

						return waveshapers::softSaturation(
								this->mCurrentGainReduction,
								WAVE_SHAPER_AMOUNT,
								WAVE_SHAPER_SLOPE);
					}

					GainReductionFET<T, AttackKind, ReleaseKind>& operator=(
							GainReductionFET<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;

				private:
					///The "amount" for the `softSaturation` WaveShaper
					static const constexpr T WAVE_SHAPER_AMOUNT = -0.2;
					///The "slope" for the `softSaturation` WaveShaper
					static const constexpr T WAVE_SHAPER_SLOPE = 0.25;
					///The "amount" for the slew rate WaveShaper
					static const constexpr T SLEW_RATE_AMOUNT = 0.4;
					///The "slope" for the slew rate WaveShaper
					static const constexpr T SLEW_RATE_SLOPE = 0.4;
					///The offset for the slew rate WaveShaper
					static const constexpr T SLEW_RATE_OFFSET = 0.1;
					///The default rise time
					static const constexpr T DEFAULT_RISE_TIME = 0.0002;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionFET)
			};
	}
}
