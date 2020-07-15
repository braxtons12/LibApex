#include "GainReductionFET.h"

#include "../../WaveShaper.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a default `GainReductionFET` (sample rate = 44100, slew rate = 0.2ms)
		GainReductionFET<float>::GainReductionFET() noexcept
			: GainReduction<float>()
			{
				mRiseTimeSeconds = 0.0002f;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
			}

		/// @brief Constructs a `GainReductionFET` with the given sample rate and a slew rate of 0.2ms
		///
		/// @param sampleRate - The sample rate to use
		GainReductionFET<float>::GainReductionFET(size_t sampleRate) noexcept
			: GainReduction<float>(sampleRate)
			{
				mRiseTimeSeconds = 0.0002f;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * sampleRate + 0.5f);
			}

		/// @brief Constructs a `GainReductionFET` with the given slew rate and a sample rate of 44100
		///
		/// @param riseTimeSeconds - The slew rate to use
		GainReductionFET<float>::GainReductionFET(float riseTimeSeconds) noexcept
			: GainReduction<float>(riseTimeSeconds)
			{

			}

		/// @brief Constructs a `GainReductionFET` with the given sample rate and slew rate
		///
		/// @param sampleRate - The sample rate to use
		/// @param riseTimeSeconds - The slew rate to use
		GainReductionFET<float>::GainReductionFET(size_t sampleRate, float riseTimeSeconds) noexcept
			: GainReduction<float>(sampleRate, riseTimeSeconds)
			{

			}

		GainReductionFET<float>::~GainReductionFET() noexcept {

		}

		/// @brief Resets this `GainReductionFET` to an initial state
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReductionFET<float>::reset(float currentGainReduction) noexcept {
			mCurrentSample = 0;
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Calculates the adjusted gain reduction based on this `GainReductionFET`'s parameters
		///
		/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
		/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
		///
		/// @return  - The adjusted gain reduction
		float GainReductionFET<float>::adjustedGainReduction(float actualGainReduction, float idealGainReduction) noexcept {
			juce::ignoreUnused(idealGainReduction);
			if(mCurrentSample > mNumSamplesToTransitionGain) mCurrentSample = 0;
			float gainReductionStep = (actualGainReduction - mCurrentGainReduction)
				/ static_cast<float>(mNumSamplesToTransitionGain - mCurrentSample);
			if(math::fabsf(gainReductionStep) - 0.001f > 0.0f) {
				gainReductionStep = waveshapers::softSaturation(
						mCurrentGainReduction +
						(gainReductionStep > 0.0f ? -mSLEW_RATE_OFFSET : mSLEW_RATE_OFFSET),
						mSLEW_RATE_AMOUNT,
						mSLEW_RATE_SLOPE);
			}
			mCurrentGainReduction += gainReductionStep;
			mCurrentSample++;
			return waveshapers::softSaturation(
					mCurrentGainReduction,
					mWAVE_SHAPER_AMOUNT,
					mWAVE_SHAPER_SLOPE);
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		void GainReductionFET<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
		}

		/// @brief Sets the slew rate to use for calculations to the given value
		///
		/// @param seconds - The new slew rate
		void GainReductionFET<float>::setRiseTimeSeconds(float seconds) noexcept {
			mRiseTimeSeconds = seconds;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
		}

		/// @brief Constructs a default `GainReductionFET` (sample rate = 44100, slew rate = 0.2ms)
		GainReductionFET<double>::GainReductionFET() noexcept
			: GainReduction<double>()
			{
				mRiseTimeSeconds = 0.0002;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
			}

		/// @brief Constructs a `GainReductionFET` with the given sample rate and a slew rate of 0.2ms
		///
		/// @param sampleRate - The sample rate to use
		GainReductionFET<double>::GainReductionFET(size_t sampleRate) noexcept
			: GainReduction<double>(sampleRate)
			{
				mRiseTimeSeconds = 0.0002;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * sampleRate + 0.5);
			}

		/// @brief Constructs a `GainReductionFET` with the given slew rate and a sample rate of 44100
		///
		/// @param riseTimeSeconds - The slew rate to use
		GainReductionFET<double>::GainReductionFET(double riseTimeSeconds) noexcept
			: GainReduction<double>(riseTimeSeconds)
			{

			}

		/// @brief Constructs a `GainReductionFET` with the given sample rate and slew rate
		///
		/// @param sampleRate - The sample rate to use
		/// @param riseTimeSeconds - The slew rate to use
		GainReductionFET<double>::GainReductionFET(size_t sampleRate, double riseTimeSeconds) noexcept
			: GainReduction<double>(sampleRate, riseTimeSeconds)
			{

			}

		GainReductionFET<double>::~GainReductionFET() noexcept {

		}

		/// @brief Resets this `GainReductionFET` to an initial state
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReductionFET<double>::reset(double currentGainReduction) noexcept {
			mCurrentSample = 0;
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Calculates the adjusted gain reduction based on this `GainReductionFET`'s parameters
		///
		/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
		/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
		///
		/// @return  - The adjusted gain reduction
		double GainReductionFET<double>::adjustedGainReduction(double actualGainReduction, double idealGainReduction) noexcept {
			juce::ignoreUnused(idealGainReduction);
			if(mCurrentSample > mNumSamplesToTransitionGain) mCurrentSample = 0;
			double gainReductionStep = (actualGainReduction - mCurrentGainReduction)
				/ static_cast<double>(mNumSamplesToTransitionGain - mCurrentSample);
			if(math::fabs(gainReductionStep) - 0.001 > 0.0) {
				gainReductionStep = waveshapers::softSaturation(
						mCurrentGainReduction +
						(gainReductionStep > 0.0 ? -mSLEW_RATE_OFFSET : mSLEW_RATE_OFFSET),
						mSLEW_RATE_AMOUNT,
						mSLEW_RATE_SLOPE);
			}
			mCurrentGainReduction += gainReductionStep;
			mCurrentSample++;
			return waveshapers::softSaturation(
					mCurrentGainReduction,
					mWAVE_SHAPER_AMOUNT,
					mWAVE_SHAPER_SLOPE);
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		void GainReductionFET<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
		}

		/// @brief Sets the slew rate to use for calculations to the given value
		///
		/// @param seconds - The new slew rate
		void GainReductionFET<double>::setRiseTimeSeconds(double seconds) noexcept {
			mRiseTimeSeconds = seconds;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
		}

	}
}
