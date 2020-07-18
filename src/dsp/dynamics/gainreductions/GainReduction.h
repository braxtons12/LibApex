#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

namespace apex {
	namespace dsp {
		/// @brief Base class for calculating gain reduction values adjusted to match different topology behaviors.
		/// Base implementation only performs basic slew-rate adjustments.
		///
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainReduction {
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

					/// @brief Constructs a default `GainReduction` - zeroed state, zero rise time
					GainReduction() noexcept = default;

					/// @brief Constructs a `GainReduction` with the given shared state and rise time
					///
					/// @param state - Non-Owning pointer to the shared dynamics state
					/// @param riseTimeSeconds - The rise time, in seconds
					GainReduction(DynamicsState* state, T riseTimeSeconds = 0) noexcept
						: mState(state),
						mRiseTimeSeconds(riseTimeSeconds)
						{
							mState->registerCallback(Field::SampleRate,
									&GainReduction<T, AttackKind, ReleaseKind>::setSampleRate);
						}

					/// @brief Move constructs the given `GainReduction`
					///
					/// @param reduction - The `GainReduction` to move
					GainReduction(GainReduction<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
					virtual ~GainReduction() noexcept = default;

					/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
					///
					/// @param gainReduction - The gain reduction determined by the gain computer
					///
					/// @return The adjusted gain reduction
					virtual T adjustedGainReduction(T gainReduction) noexcept {
						if(mCurrentSample > mNumSamplesToTransitionGain) mCurrentSample = 0;

						if(mNumSamplesToTransitionGain != 0) {
							T gainReductionStep = (gainReduction - mCurrentGainReduction)
								/ static_cast<T>(mNumSamplesToTransitionGain - mCurrentSample);
							mCurrentGainReduction += gainReductionStep;
							mCurrentSample++;
						}
						else mCurrentGainReduction = gainReduction;
						return mCurrentGainReduction;
					}

					/// @brief Resets this `GainReduction` to an initial state.
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					inline virtual void reset(T currentGainReduction = 0) noexcept {
						mCurrentGainReduction = currentGainReduction;
						mCurrentSample = 0;
					}

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					inline virtual void setSampleRate(size_t sampleRate) noexcept {
						mNumSamplesToTransitionGain = static_cast<size_t>(
								sampleRate * mRiseTimeSeconds + 0.5
								);
					}

					/// @brief Sets the slew rate to use for calculations to the given value
					///
					/// @param seconds - The new slew rate
					inline virtual void setRiseTimeSeconds(T seconds) noexcept {
						mRiseTimeSeconds = seconds;
						mNumSamplesToTransitionGain = static_cast<size_t>(
								mState->getSampleRate() * mRiseTimeSeconds + 0.5
								);
					}

					/// @brief Sets the shared state to the given one
					///
					/// @param state - The shared state to use
					inline void setState(DynamicsState* state) noexcept {
						mState = state;
					}


				protected:
					///prevent us from having to `new` or use a smart pointer
					///just to get our zeroed state
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					///The current sample in the slew
					size_t mCurrentSample = 0;
					///The number of samples the slew takes to complete
					size_t mNumSamplesToTransitionGain = 0;
					///The current gain reduction value
					T mCurrentGainReduction = 0.0;
					///The slew rate
					T mRiseTimeSeconds = 0.0;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReduction)
			};
	}
}
