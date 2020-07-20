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
					using Field = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>::Field;
					using DynamicsState = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>;

				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
					static_assert((std::is_floating_point<AttackKind>::value &&
								std::is_same<T, AttackKind>::value) ||
							std::is_enum<AttackKind>::value,
							"AttackKind must be the same floating point type as T, or an enum");
					static_assert((std::is_floating_point<ReleaseKind>::value &&
								std::is_same<T, ReleaseKind>::value) ||
							std::is_enum<ReleaseKind>::value,
							"ReleaseKind must be the same floating point type as T, or an enum");

					/// @brief Constructs a default `GainReductionVCA`
					/// (zeroed shared state, rise time = 0.4ms)
					GainReductionVCA() noexcept
						: GainReduction<T, AttackKind, ReleaseKind>()
						{
							this->mRiseTimeSeconds = DEFAULT_RISE_TIME;
							this->mNumSamplesToTransitionGain = static_cast<size_t>(
									this->mRiseTimeSeconds * this->mState->getSampleRate() +
									static_cast<T>(0.5)
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

						if(gainReduction < this->mCurrentGainReduction) samplesToTransition *=
							static_cast<T>(2.0);

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
					static const constexpr T WAVE_SHAPER_AMOUNT = static_cast<T>(0.2);
					///The "slope" for the `softSaturation` wave shaper
					static const constexpr T WAVE_SHAPER_SLOPE = static_cast<T>(0.4);
					///The default rise time
					static const constexpr T DEFAULT_RISE_TIME = static_cast<T>(0.0004);

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionVCA)
			};
	}
}

#endif //GAIN_REDUCTION_VCA
