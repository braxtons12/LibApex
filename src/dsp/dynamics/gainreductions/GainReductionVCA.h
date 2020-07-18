#pragma once

#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../../base/StandardIncludes.h"
#include "../../WaveShaper.h"

#ifndef GAIN_REDUCTION_VCA
#define GAIN_REDUCTION_VCA

namespace apex {
	namespace dsp {
		/// @brief Class for calculating gain reduction values adjusted to roughly model VCA topology behavior
		///
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainReductionVCA : public GainReduction<T, AttackKind, ReleaseKind> {
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

					/// @brief Constructs a default `GainReductionVCA`
					/// (zeroed shared state, rise time = 0.4ms)
					GainReductionVCA() noexcept
						: GainReduction<T, AttackKind, ReleaseKind>()
						{
							this->mRiseTimeSeconds = DEFAULT_RISE_TIME;
							this->mNumSamplesToTransitionGain = static_cast<size_t>(
									this->mRiseTimeSeconds * this->mState->getSampleRate() + 0.5
									);
						}

					/// @brief Constructs a `GainReductionVCA` with the given shared state and rise time
					///
					/// @param state - The shared state
					/// @param riseTimeSeconds - The rise time, in seconds
					GainReductionVCA(DynamicsState* state, T riseTimeSeconds = DEFAULT_RISE_TIME)
						noexcept
						: GainReduction<T, AttackKind, ReleaseKind>(state, riseTimeSeconds)
						{

						}

					/// @brief Move constructs the given `GainReductionVCA`
					///
					/// @param reduction - The `GainReductionVCA` to use
					GainReductionVCA(GainReductionVCA<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
					virtual ~GainReductionVCA() noexcept override = default;

					/// @brief Calculates the adjusted gain reduction based on this `GainReductionVCA`'s parameters
					///
					/// @param gainReduction - The gain reduction determined by the gain computer
					///
					/// @return - The adjusted gain reduction
					T adjustedGainReduction(T gainReduction) noexcept override {
						T samplesToTransition =	static_cast<T>(this->mNumSamplesToTransitionGain);

						if(gainReduction < this->mCurrentGainReduction) samplesToTransition *= 2.0f;

						if(this->mCurrentSample > samplesToTransition) this->mCurrentSample = 0;

						T gainReductionStep = (gainReduction - this->mCurrentGainReduction)
							/ static_cast<T>(samplesToTransition - this->mCurrentSample);

						this->mCurrentGainReduction += gainReductionStep;
						this->mCurrentSample++;

						return waveshapers::softSaturation(
								this->mCurrentGainReduction,
								WAVE_SHAPER_AMOUNT,
								WAVE_SHAPER_SLOPE);
					}

					GainReductionVCA<T, AttackKind, ReleaseKind>& operator=(
							GainReductionVCA<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;

				private:
					///The "amount" for the `softSaturation` wave shaper
					static const constexpr T WAVE_SHAPER_AMOUNT = 0.2;
					///The "slope" for the `softSaturation` wave shaper
					static const constexpr T WAVE_SHAPER_SLOPE = 0.4;
					///The default rise time
					static const constexpr T DEFAULT_RISE_TIME = 0.0004;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionVCA)
			};
	}
}

#endif //GAIN_REDUCTION_VCA
