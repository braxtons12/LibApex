#include "GainReductionOpto.h"

#include "../../WaveShaper.h"
#include "../../../math/Exponentials.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a default `GainReductionOptical` (sampleRate = 44100)
		GainReductionOptical<float>::GainReductionOptical() noexcept
			: GainReduction<float>()
			{
				for(size_t coefficient = 0; coefficient < mNUM_COEFFICIENTS; ++coefficient) {
					float decibel = static_cast<float>(coefficient) / static_cast<float>(mNUM_COEFFICIENTS_PER_STEP);
					float resistance = 510.0f / (3.0f + decibel);
					float attackSeconds = (resistance / 10.0f) / 1000.0f;
					float releaseSeconds = resistance / 1000.0f;

					float sampleRate = static_cast<float>(mSampleRate);
					mAttackCoefficients[coefficient] = (math::expf(math::logf(0.27f)
								/ (attackSeconds * sampleRate)));
					mReleaseCoefficients[coefficient] = (math::expf(math::logf(0.27f)
								/ (releaseSeconds * sampleRate)));
				}
			}

		/// @brief Constructs a `GainReductionOptical` with he given sample rate
		///
		/// @param sampleRate
		GainReductionOptical<float>::GainReductionOptical(size_t sampleRate) noexcept
			: GainReduction<float>(sampleRate)
			{
				for(size_t coefficient = 0; coefficient < mNUM_COEFFICIENTS; ++coefficient) {
					float decibel = static_cast<float>(coefficient) / static_cast<float>(mNUM_COEFFICIENTS_PER_STEP);
					float resistance = 510.0f / (3.0f + decibel);
					float attackSeconds = (resistance / 10.0f) / 1000.0f;
					float releaseSeconds = resistance / 1000.0f;

					float sampleRateFloat = static_cast<float>(sampleRate);
					mAttackCoefficients[coefficient] = (math::expf(math::logf(0.27f)
								/ (attackSeconds * sampleRateFloat)));
					mReleaseCoefficients[coefficient] = (math::expf(math::logf(0.27f)
								/ (releaseSeconds * sampleRateFloat)));
				}
			}

		GainReductionOptical<float>::~GainReductionOptical() noexcept {

		}

		/// @brief Resets this `GainReductionOptical` to an initial state
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReductionOptical<float>::reset(float currentGainReduction) noexcept {
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s parameters
		///
		/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
		/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
		///
		/// @return - The adjusted gain reduction
		float GainReductionOptical<float>::adjustedGainReduction(float actualGainReduction, float idealGainReduction) noexcept {
			juce::ignoreUnused(idealGainReduction);
			float oldGainReduction = mCurrentGainReduction;
			float coefficient = actualGainReduction;
			size_t coefficientIndex = static_cast<size_t>(coefficient * static_cast<float>(mNUM_COEFFICIENTS_PER_STEP));

			if(coefficientIndex > (mNUM_COEFFICIENTS - 1)) {
				coefficientIndex = mNUM_COEFFICIENTS - 1;
			}

			if(actualGainReduction > mCurrentGainReduction) {
				mCurrentGainReduction = (mAttackCoefficients[coefficientIndex] * oldGainReduction)
					+ (1.0f - mAttackCoefficients[coefficientIndex]) * actualGainReduction;
			}
			else {
				mCurrentGainReduction = (mReleaseCoefficients[coefficientIndex] * oldGainReduction)
					+ (1.0f - mReleaseCoefficients[coefficientIndex]) * actualGainReduction;
			}

			return waveshapers::softSaturation(
					mCurrentGainReduction,
					mWAVE_SHAPER_AMOUNT,
					mWAVE_SHAPER_SLOPE);
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		void GainReductionOptical<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			for(size_t coefficient = 0; coefficient < mNUM_COEFFICIENTS; ++coefficient) {
				float decibel = static_cast<float>(coefficient) / static_cast<float>(mNUM_COEFFICIENTS_PER_STEP);
				float resistance = 510.0f / (3.0f + decibel);
				float attackSeconds = (resistance / 10.0f) / 1000.0f;
				float releaseSeconds = resistance / 1000.0f;

				float sampleRateFloat = static_cast<float>(sampleRate);
				mAttackCoefficients[coefficient] = (math::expf(math::logf(0.27f)
							/ (attackSeconds * sampleRateFloat)));
				mReleaseCoefficients[coefficient] = (math::expf(math::logf(0.27f)
							/ (releaseSeconds * sampleRateFloat)));
			}
		}

		/// @brief Constructs a default `GainReductionOptical` (sampleRate = 44100)
		GainReductionOptical<double>::GainReductionOptical() noexcept
			: GainReduction<double>()
			{
				for(size_t coefficient = 0; coefficient < mNUM_COEFFICIENTS; ++coefficient) {
					double decibel = static_cast<double>(coefficient) / static_cast<double>(mNUM_COEFFICIENTS_PER_STEP);
					double resistance = 510.0 / (3.0 + decibel);
					double attackSeconds = (resistance / 10.0) / 1000.0;
					double releaseSeconds = resistance / 1000.0;

					double sampleRate = static_cast<float>(mSampleRate);
					mAttackCoefficients[coefficient] = (math::exp(math::log(0.27)
								/ (attackSeconds * sampleRate)));
					mReleaseCoefficients[coefficient] = (math::exp(math::log(0.27)
								/ (releaseSeconds * sampleRate)));
				}
			}

		/// @brief Constructs a `GainReductionOptical` with he given sample rate
		///
		/// @param sampleRate
		GainReductionOptical<double>::GainReductionOptical(size_t sampleRate) noexcept
			: GainReduction<double>(sampleRate)
			{
				for(size_t coefficient = 0; coefficient < mNUM_COEFFICIENTS; ++coefficient) {
					double decibel = static_cast<double>(coefficient) / static_cast<double>(mNUM_COEFFICIENTS_PER_STEP);
					double resistance = 510.0 / (3.0 + decibel);
					double attackSeconds = (resistance / 10.0) / 1000.0;
					double releaseSeconds = resistance / 1000.0;

					double sampleRateDouble = static_cast<float>(sampleRate);
					mAttackCoefficients[coefficient] = (math::exp(math::log(0.27)
								/ (attackSeconds * sampleRateDouble)));
					mReleaseCoefficients[coefficient] = (math::exp(math::log(0.27)
								/ (releaseSeconds * sampleRateDouble)));
				}
			}

		GainReductionOptical<double>::~GainReductionOptical() noexcept {

		}

		/// @brief Resets this `GainReductionOptical` to an initial state
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReductionOptical<double>::reset(double currentGainReduction) noexcept {
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s parameters
		///
		/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
		/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
		///
		/// @return - The adjusted gain reduction
		double GainReductionOptical<double>::adjustedGainReduction(double actualGainReduction, double idealGainReduction) noexcept {
			juce::ignoreUnused(idealGainReduction);
			double oldGainReduction = mCurrentGainReduction;
			double coefficient = actualGainReduction;
			size_t coefficientIndex = static_cast<size_t>(coefficient * static_cast<double>(mNUM_COEFFICIENTS_PER_STEP));

			if(coefficientIndex > (mNUM_COEFFICIENTS - 1)) {
				coefficientIndex = mNUM_COEFFICIENTS - 1;
			}

			if(actualGainReduction > mCurrentGainReduction) {
				mCurrentGainReduction = (mAttackCoefficients[coefficientIndex] * oldGainReduction)
					+ (1.0 - mAttackCoefficients[coefficientIndex]) * actualGainReduction;
			}
			else {
				mCurrentGainReduction = (mReleaseCoefficients[coefficientIndex] * oldGainReduction)
					+ (1.0 - mReleaseCoefficients[coefficientIndex]) * actualGainReduction;
			}

			return waveshapers::softSaturation(
					mCurrentGainReduction,
					mWAVE_SHAPER_AMOUNT,
					mWAVE_SHAPER_SLOPE);
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		void GainReductionOptical<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			for(size_t coefficient = 0; coefficient < mNUM_COEFFICIENTS; ++coefficient) {
				double decibel = static_cast<double>(coefficient) / static_cast<double>(mNUM_COEFFICIENTS_PER_STEP);
				double resistance = 510.0 / (3.0 + decibel);
				double attackSeconds = (resistance / 10.0) / 1000.0;
				double releaseSeconds = resistance / 1000.0;

				double sampleRateDouble = static_cast<float>(sampleRate);
				mAttackCoefficients[coefficient] = (math::exp(math::log(0.27)
							/ (attackSeconds * sampleRateDouble)));
				mReleaseCoefficients[coefficient] = (math::exp(math::log(0.27)
							/ (releaseSeconds * sampleRateDouble)));
			}
		}
	}
}
