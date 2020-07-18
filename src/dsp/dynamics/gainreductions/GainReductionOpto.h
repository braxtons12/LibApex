#pragma once

#include <vector>
#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../../base/StandardIncludes.h"
#include "../../WaveShaper.h"

#ifndef GAIN_REDUCTION_OPTICAL
#define GAIN_REDUCTION_OPTICAL

namespace apex {
	namespace dsp {
		/// @brief Class for calculating gain reduction values adjusted to roughly model Optical topology behavior
		///
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainReductionOptical : public GainReduction<T, AttackKind, ReleaseKind> {
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

					/// @brief Constructs a default `GainReductionOptical`
					/// (zeroed shared state)
					GainReductionOptical() noexcept
						: GainReduction<T, AttackKind, ReleaseKind>()
						{

						}

					/// @brief Contructs a `GainReductionOptical` with the given shared state
					///
					/// @param state - The shared state
					GainReductionOptical(DynamicsState* state) noexcept {
						this->mState = state;
						this->mState->registerCallback(Field::SampleRate,
								&GainReductionOptical<T, AttackKind,ReleaseKind>::setSampleRate);
					}

					/// @brief Move constructs the given `GainReductionOptical`
					///
					/// @param reduction - The `GainReductionOptical` to move
					GainReductionOptical(GainReductionOptical<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
					virtual ~GainReductionOptical() noexcept = default;

					/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s parameters
					///
					/// @param gainReduction - The gain reduction determined by the gain computer
					///
					/// @return - The adjusted gain reduction
					T adjustedGainReduction(T gainReduction) noexcept override {
						T oldGainReduction = this->mCurrentGainReduction;
						T coefficient = gainReduction;
						size_t coefficientIndex = static_cast<size_t>(
								coefficient * static_cast<T>(NUM_COEFFICIENTS_PER_STEP)
								);

						if(coefficientIndex > (NUM_COEFFICIENTS - 1)) {
							coefficientIndex = NUM_COEFFICIENTS - 1;
						}

						if(gainReduction > this->mCurrentGainReduction) {
							this->mCurrentGainReduction =
								(mAttackCoefficients[coefficientIndex] * oldGainReduction)
								+ (1.0 - mAttackCoefficients[coefficientIndex]) * gainReduction;
						}
						else {
							this->mCurrentGainReduction =
								(mReleaseCoefficients[coefficientIndex] * oldGainReduction)
								+ (1.0 - mReleaseCoefficients[coefficientIndex]) * gainReduction;
						}

						return waveshapers::softSaturation(
								this->mCurrentGainReduction,
								WAVE_SHAPER_AMOUNT,
								WAVE_SHAPER_SLOPE);
					}

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) noexcept override {
						for(size_t coefficient = 0; coefficient < NUM_COEFFICIENTS; ++coefficient) {
							T decibel = static_cast<T>(coefficient) /
								static_cast<T>(NUM_COEFFICIENTS_PER_STEP);
							T resistance = 510.0 / (3.0 + decibel);
							T attackSeconds = (resistance / 10.0) / 1000.0;
							T releaseSeconds = resistance / 1000.0;

							T sampleRateFloat = static_cast<T>(sampleRate);
							mAttackCoefficients[coefficient] =
								math::exp(math::log(0.27) / (attackSeconds * sampleRateFloat));
							mReleaseCoefficients[coefficient] =
								math::exp(math::log(0.27) / (releaseSeconds * sampleRateFloat));
						}
					}

					GainReductionOptical<T, AttackKind, ReleaseKind>& operator=(
							GainReductionOptical<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;

				private:
					///The number of steps in decibels to store coefficients for
					static const constexpr size_t NUM_DB_STEPS = 48;
					///The number of coefficients making up each decibel step
					static const constexpr size_t NUM_COEFFICIENTS_PER_STEP = 2;
					///The total number of coefficients
					static const constexpr size_t NUM_COEFFICIENTS = NUM_DB_STEPS * NUM_COEFFICIENTS_PER_STEP;
					///The "amount" to use for the `softSaturation` wave shaper
					static const constexpr T WAVE_SHAPER_AMOUNT = 0.2;
					///The "slope" to use for the `softSaturation` wave shaper
					static const constexpr T WAVE_SHAPER_SLOPE = 0.2;
					///The attack response coefficients
					std::vector<T> mAttackCoefficients = std::vector<T>(NUM_COEFFICIENTS);
					///The release response coefficients
					std::vector<T> mReleaseCoefficients = std::vector<T>(NUM_COEFFICIENTS);

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionOptical)
			};
	}
}

#endif //GAIN_REDUCTION_OPTICAL
