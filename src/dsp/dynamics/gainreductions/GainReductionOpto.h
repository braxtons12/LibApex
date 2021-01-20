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
	/// @tparam FloatType - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainReductionOptical final : public GainReduction<FloatType, AttackKind, ReleaseKind> {
	  protected:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, AttackKind, ReleaseKind>;
		using GainReduction = GainReduction<FloatType, AttackKind, ReleaseKind>;

	  public:
	#ifdef TESTING_GAIN_REDUCTION_OPTO
		/// @brief Constructs a default `GainReductionOptical`
		/// (zeroed shared state)
		GainReductionOptical() noexcept {
			apex::utils::Logger::LogMessage("Creating Gain Reduction Opto");
		}
	#else
		GainReductionOptical() noexcept = default;
	#endif

		/// @brief Contructs a `GainReductionOptical` with the given shared state
		///
		/// @param state - The shared state
		explicit GainReductionOptical(DynamicsState* state) noexcept {
			GainReduction::mState = state;
			GainReduction::mState->template registerCallback<size_t, DynamicsField::SampleRate>(
				[this](size_t sampleRate) { this->setSampleRate(sampleRate); });
	#ifdef TESTING_GAIN_REDUCTION_OPTO
			apex::utils::Logger::LogMessage("Creating Gain Reduction Opto");
	#endif
		}

		/// @brief Move constructs the given `GainReductionOptical`
		///
		/// @param reduction - The `GainReductionOptical` to move
		GainReductionOptical(GainReductionOptical&& reduction) noexcept = default;
		~GainReductionOptical() noexcept final = default;

		/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s
		/// parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return - The adjusted gain reduction
		[[nodiscard]] inline auto
		adjustedGainReduction(Decibels gainReduction) noexcept -> Decibels final {
	#ifdef TESTING_GAIN_REDUCTION_OPTO
			apex::utils::Logger::LogMessage(
				"Gain Reduction Opto Calculating Adjusted Gain Reduction");
	#endif
			Decibels oldGainReduction = GainReduction::mCurrentGainReduction;
			Decibels coefficient = gainReduction;
			auto coefficientIndex = narrow_cast<size_t>(
				coefficient * narrow_cast<FloatType>(NUM_COEFFICIENTS_PER_STEP));

			if(coefficientIndex > (NUM_COEFFICIENTS - 1)) {
				coefficientIndex = NUM_COEFFICIENTS - 1;
			}

			if(gainReduction > GainReduction::mCurrentGainReduction) {
				GainReduction::mCurrentGainReduction
					= (mAttackCoefficients[coefficientIndex] * oldGainReduction)
					  + (narrow_cast<FloatType>(1.0) - mAttackCoefficients[coefficientIndex])
							* gainReduction;
			}
			else {
				GainReduction::mCurrentGainReduction
					= (mReleaseCoefficients[coefficientIndex] * oldGainReduction)
					  + (narrow_cast<FloatType>(1.0) - mReleaseCoefficients[coefficientIndex])
							* gainReduction;
			}

			return waveshapers::softSaturation<FloatType>(GainReduction::mCurrentGainReduction,
														  WAVE_SHAPER_AMOUNT,
														  WAVE_SHAPER_SLOPE);
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void final {
			for(size_t coefficient = 0; coefficient < NUM_COEFFICIENTS; ++coefficient) {
				Decibels decibel = narrow_cast<FloatType>(coefficient)
								   / narrow_cast<FloatType>(NUM_COEFFICIENTS_PER_STEP);
				auto resistance
					= narrow_cast<FloatType>(510.0) / narrow_cast<FloatType>(3.0 + decibel);
				auto attackSeconds
					= (resistance / narrow_cast<FloatType>(10.0)) / narrow_cast<FloatType>(1000.0);
				auto releaseSeconds = resistance / narrow_cast<FloatType>(1000.0);

				auto sampleRateFloat = narrow_cast<FloatType>(sampleRate);

				mAttackCoefficients[coefficient] = Exponentials<FloatType>::exp(
					Exponentials<FloatType>::ln(narrow_cast<FloatType>(0.27))
					/ (attackSeconds * sampleRateFloat));
				mReleaseCoefficients[coefficient] = Exponentials<FloatType>::exp(
					Exponentials<FloatType>::ln(narrow_cast<FloatType>(0.27))
					/ (releaseSeconds * sampleRateFloat));
			}
		}

		auto
		operator=(GainReductionOptical&& reduction) noexcept -> GainReductionOptical& = default;

	  private:
		/// The number of steps in decibels to store coefficients for
		static const constexpr size_t NUM_DB_STEPS = 48;
		/// The number of coefficients making up each decibel step
		static const constexpr size_t NUM_COEFFICIENTS_PER_STEP = 2;
		/// The total number of coefficients
		static const constexpr size_t NUM_COEFFICIENTS = NUM_DB_STEPS * NUM_COEFFICIENTS_PER_STEP;
		/// The "amount" to use for the `softSaturation` wave shaper
		static const constexpr FloatType WAVE_SHAPER_AMOUNT = narrow_cast<FloatType>(0.2);
		/// The "slope" to use for the `softSaturation` wave shaper
		static const constexpr FloatType WAVE_SHAPER_SLOPE = narrow_cast<FloatType>(0.2);
		/// The attack response coefficients
		std::array<FloatType, NUM_COEFFICIENTS> mAttackCoefficients
			= std::array<FloatType, NUM_COEFFICIENTS>();
		/// The release response coefficients
		std::array<FloatType, NUM_COEFFICIENTS> mReleaseCoefficients
			= std::array<FloatType, NUM_COEFFICIENTS>();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionOptical)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION_OPTICAL
