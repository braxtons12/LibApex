#include "GainReductionVCA.h"

#include "../../WaveShaper.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a default `GainReductionVCA` (sample rate = 44100)
		GainReductionVCA<float>::GainReductionVCA() noexcept
			: GainReduction<float>()
			{
				mRiseTimeSeconds = 0.0004f;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
			}

		/// @brief Constructs a `GainReductionVCA` with the given sample rate
		///
		/// @param sampleRate - The sample rate to use
		GainReductionVCA<float>::GainReductionVCA(size_t sampleRate) noexcept
			: GainReduction<float>(sampleRate)
			{
				mRiseTimeSeconds = 0.0004f;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * sampleRate + 0.5f);
			}

		GainReductionVCA<float>::~GainReductionVCA() noexcept {

		}

		/// @brief Resets this `GainReductionVCA` to an initial state
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReductionVCA<float>::reset(float currentGainReduction) noexcept {
			mCurrentSample = 0;
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Calculates the adjusted gain reduction based on this `GainReductionVCA`'s parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return - The adjusted gain reduction
		float GainReductionVCA<float>::adjustedGainReduction(float gainReduction) noexcept {
			float samplesToTransition = static_cast<float>(mNumSamplesToTransitionGain);
			if(gainReduction < mCurrentGainReduction) samplesToTransition *= 2.0f;
			if(mCurrentSample > samplesToTransition) mCurrentSample = 0;
			float gainReductionStep = (gainReduction - mCurrentGainReduction)
				/ static_cast<float>(samplesToTransition - mCurrentSample);
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
		void GainReductionVCA<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
		}

		/// @brief Constructs a default `GainReductionVCA` (sample rate = 44100)
		GainReductionVCA<double>::GainReductionVCA() noexcept
			: GainReduction<double>()
			{
				mRiseTimeSeconds = 0.0004;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
			}

		/// @brief Constructs a `GainReductionVCA` with the given sample rate
		///
		/// @param sampleRate - The sample rate to use
		GainReductionVCA<double>::GainReductionVCA(size_t sampleRate) noexcept
			: GainReduction<double>(sampleRate)
			{
				mRiseTimeSeconds = 0.004;
				mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * sampleRate + 0.5);
			}

		GainReductionVCA<double>::~GainReductionVCA() noexcept {

		}

		/// @brief Resets this `GainReductionVCA` to an initial state
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReductionVCA<double>::reset(double currentGainReduction) noexcept {
			mCurrentSample = 0;
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Calculates the adjusted gain reduction based on this `GainReductionVCA`'s parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return - The adjusted gain reduction
		double GainReductionVCA<double>::adjustedGainReduction(double gainReduction) noexcept {
			double samplesToTransition = static_cast<double>(mNumSamplesToTransitionGain);
			if(gainReduction < mCurrentGainReduction) samplesToTransition *= 2.0;
			if(mCurrentSample > samplesToTransition) mCurrentSample = 0;
			double gainReductionStep = (gainReduction - mCurrentGainReduction)
				/ static_cast<double>(samplesToTransition - mCurrentSample);
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
		void GainReductionVCA<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
		}
	}
}
