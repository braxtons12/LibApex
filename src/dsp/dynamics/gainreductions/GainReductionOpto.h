#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "../../../base/StandardIncludes.h"
#include "../../WaveShaper.h"
#include "GainReduction.h"

#ifndef GAIN_REDUCTION_OPTICAL
	#define GAIN_REDUCTION_OPTICAL

namespace apex::dsp {
	/// @brief Class for calculating gain reduction values adjusted to roughly model Optical
	/// topology behavior
	///
	/// @tparam T - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<typename T, typename AttackKind, typename ReleaseKind>
	class GainReductionOptical : public GainReduction<T, AttackKind, ReleaseKind> {
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

		/// @brief Constructs a default `GainReductionOptical`
		/// (zeroed shared state)
		GainReductionOptical() noexcept : GainReduction<T, AttackKind, ReleaseKind>() {
	#ifdef TESTING_GAIN_REDUCTION_OPTO
			apex::utils::Logger::LogMessage("Creating Gain Reduction Opto");
	#endif
		}

		/// @brief Contructs a `GainReductionOptical` with the given shared state
		///
		/// @param state - The shared state
		explicit GainReductionOptical(DynamicsState* state) noexcept {
			this->mState = state;
			this->mState->template registerCallback<size_t, Field::SampleRate>(
				[this](size_t sampleRate) { this->setSampleRate(sampleRate); });
	#ifdef TESTING_GAIN_REDUCTION_OPTO
			apex::utils::Logger::LogMessage("Creating Gain Reduction Opto");
	#endif
		}

		/// @brief Move constructs the given `GainReductionOptical`
		///
		/// @param reduction - The `GainReductionOptical` to move
		GainReductionOptical(
			GainReductionOptical<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
		~GainReductionOptical() noexcept override = default;

		/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s
		/// parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return - The adjusted gain reduction
		[[nodiscard]] auto
		adjustedGainReduction(Decibels gainReduction) noexcept -> Decibels override {
	#ifdef TESTING_GAIN_REDUCTION_OPTO
			apex::utils::Logger::LogMessage(
				"Gain Reduction Opto Calculating Adjusted Gain Reduction");
	#endif
			Decibels oldGainReduction = this->mCurrentGainReduction;
			Decibels coefficient = gainReduction;
			auto coefficientIndex
				= static_cast<size_t>(coefficient * static_cast<T>(NUM_COEFFICIENTS_PER_STEP));

			if(coefficientIndex > (NUM_COEFFICIENTS - 1)) {
				coefficientIndex = NUM_COEFFICIENTS - 1;
			}

			if(gainReduction > this->mCurrentGainReduction) {
				this->mCurrentGainReduction
					= (mAttackCoefficients[coefficientIndex] * oldGainReduction)
					  + (static_cast<T>(1.0) - mAttackCoefficients[coefficientIndex])
							* gainReduction;
			}
			else {
				this->mCurrentGainReduction
					= (mReleaseCoefficients[coefficientIndex] * oldGainReduction)
					  + (static_cast<T>(1.0) - mReleaseCoefficients[coefficientIndex])
							* gainReduction;
			}

			return waveshapers::softSaturation(this->mCurrentGainReduction,
											   WAVE_SHAPER_AMOUNT,
											   WAVE_SHAPER_SLOPE);
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		auto setSampleRate(Hertz sampleRate) noexcept -> void override {
			for(size_t coefficient = 0; coefficient < NUM_COEFFICIENTS; ++coefficient) {
				Decibels decibel
					= static_cast<T>(coefficient) / static_cast<T>(NUM_COEFFICIENTS_PER_STEP);
				T resistance
					= static_cast<T>(510.0) / static_cast<T>(static_cast<T>(3.0) + decibel);
				T attackSeconds = (resistance / static_cast<T>(10.0)) / static_cast<T>(1000.0);
				T releaseSeconds = resistance / static_cast<T>(1000.0);

				T sampleRateFloat = static_cast<T>(sampleRate);

				mAttackCoefficients[coefficient] = static_cast<T>(math::exp(
					math::ln(0.27) / static_cast<double>(attackSeconds * sampleRateFloat)));
				mReleaseCoefficients[coefficient] = static_cast<T>(math::exp(
					math::ln(0.27) / static_cast<double>(releaseSeconds * sampleRateFloat)));
			}
		}

		auto operator=(GainReductionOptical<T, AttackKind, ReleaseKind>&& reduction) noexcept
			-> GainReductionOptical<T, AttackKind, ReleaseKind>& = default;

	  private:
		/// The number of steps in decibels to store coefficients for
		static const constexpr size_t NUM_DB_STEPS = 48;
		/// The number of coefficients making up each decibel step
		static const constexpr size_t NUM_COEFFICIENTS_PER_STEP = 2;
		/// The total number of coefficients
		static const constexpr size_t NUM_COEFFICIENTS = NUM_DB_STEPS * NUM_COEFFICIENTS_PER_STEP;
		/// The "amount" to use for the `softSaturation` wave shaper
		static const constexpr T WAVE_SHAPER_AMOUNT = static_cast<T>(0.2);
		/// The "slope" to use for the `softSaturation` wave shaper
		static const constexpr T WAVE_SHAPER_SLOPE = static_cast<T>(0.2);
		/// The attack response coefficients
		std::vector<T> mAttackCoefficients = std::vector<T>(NUM_COEFFICIENTS);
		/// The release response coefficients
		std::vector<T> mReleaseCoefficients = std::vector<T>(NUM_COEFFICIENTS);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionOptical)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION_OPTICAL
