#include "GainReduction.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a default `GainReduction` (sample rate = 44100, slew rate = 0)
		GainReduction<float>::GainReduction() {

		}

		/// @brief Constructs a `GainReduction` with the given sample rate and a slew rate of 0
		///
		/// @param sampleRate - The sample rate to use
		GainReduction<float>::GainReduction(size_t sampleRate)
			: mSampleRate(sampleRate)
		{
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * sampleRate + 0.5f);
		}

		/// @brief Constructs a `GainReduction` with the given slew rate and a sample rate of 44100
		///
		/// @param riseTimeSeconds - The slew rate to use
		GainReduction<float>::GainReduction(float riseTimeSeconds)
			: mRiseTimeSeconds(riseTimeSeconds)
		{
			mNumSamplesToTransitionGain = static_cast<size_t>(riseTimeSeconds * mSampleRate + 0.5f);
		}

		/// @brief Constructs a `GainReduction` with the given sample rate and slew rate
		///
		/// @param sampleRate - The sample rate to use
		/// @param riseTimeSeconds - The slew rate to use
		GainReduction<float>::GainReduction(size_t sampleRate, float riseTimeSeconds)
			: mSampleRate(sampleRate), mRiseTimeSeconds(riseTimeSeconds)
		{
			mNumSamplesToTransitionGain = static_cast<size_t>(riseTimeSeconds * sampleRate + 0.5f);
		}

		GainReduction<float>::~GainReduction() {

		}

		/// @brief Resets this `GainReduction` to an initial state.
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReduction<float>::reset(float currentGainReduction) {
			mCurrentGainReduction = currentGainReduction;
			mCurrentSample = 0;
		}

		/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
		///
		/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
		/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
		///
		/// @return The adjusted gain reduction
		float GainReduction<float>::adjustedGainReduction(float actualGainReduction, float idealGainReduction) {
			juce::ignoreUnused(idealGainReduction);
			if(mCurrentSample > mNumSamplesToTransitionGain) mCurrentSample = 0;
			float gainReductionStep = (actualGainReduction - mCurrentGainReduction)
				/ static_cast<float>(mNumSamplesToTransitionGain - mCurrentSample);
			mCurrentGainReduction += gainReductionStep;
			mCurrentSample++;
			return mCurrentGainReduction;
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		void GainReduction<float>::setSampleRate(size_t sampleRate) {
			mSampleRate = sampleRate;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
		}

		/// @brief Sets the slew rate to use for calculations to the given value
		///
		/// @param seconds - The new slew rate
		void GainReduction<float>::setRiseTimeSeconds(float seconds) {
			mRiseTimeSeconds = seconds;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5f);
		}

		/// @brief Constructs a default `GainReduction` (sample rate = 44100, slew rate = 0)
		GainReduction<double>::GainReduction() {

		}

		/// @brief Constructs a `GainReduction` with the given sample rate and a slew rate of 0
		///
		/// @param sampleRate - The sample rate to use
		GainReduction<double>::GainReduction(size_t sampleRate)
			: mSampleRate(sampleRate)
		{
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * sampleRate + 0.5);
		}

		/// @brief Constructs a `GainReduction` with the given slew rate and a sample rate of 44100
		///
		/// @param riseTimeSeconds - The slew rate to use
		GainReduction<double>::GainReduction(double riseTimeSeconds)
			: mRiseTimeSeconds(riseTimeSeconds)
		{
			mNumSamplesToTransitionGain = static_cast<size_t>(riseTimeSeconds * mSampleRate + 0.5);
		}

		/// @brief Constructs a `GainReduction` with the given sample rate and slew rate
		///
		/// @param sampleRate - The sample rate to use
		/// @param riseTimeSeconds - The slew rate to use
		GainReduction<double>::GainReduction(size_t sampleRate, double riseTimeSeconds)
			: mSampleRate(sampleRate), mRiseTimeSeconds(riseTimeSeconds)
		{
			mNumSamplesToTransitionGain = static_cast<size_t>(riseTimeSeconds * sampleRate + 0.5);
		}

		GainReduction<double>::~GainReduction() {

		}

		/// @brief Resets this `GainReduction` to an initial state.
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		void GainReduction<double>::reset(double currentGainReduction) {
			mCurrentGainReduction = currentGainReduction;
			mCurrentSample = 0;
		}

		/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
		///
		/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
		/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
		///
		/// @return The adjusted gain reduction
		double GainReduction<double>::adjustedGainReduction(double actualGainReduction, double idealGainReduction) {
			juce::ignoreUnused(idealGainReduction);
			if(mCurrentSample > mNumSamplesToTransitionGain) mCurrentSample = 0;
			double gainReductionStep = (actualGainReduction - mCurrentGainReduction)
				/ static_cast<double>(mNumSamplesToTransitionGain - mCurrentSample);
			mCurrentGainReduction += gainReductionStep;
			mCurrentSample++;
			return actualGainReduction;
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		void GainReduction<double>::setSampleRate(size_t sampleRate) {
			mSampleRate = sampleRate;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
		}

		/// @brief Sets the slew rate to use for calculations to the given value
		///
		/// @param seconds - The new slew rate
		void GainReduction<double>::setRiseTimeSeconds(double seconds) {
			mRiseTimeSeconds = seconds;
			mNumSamplesToTransitionGain = static_cast<size_t>(mRiseTimeSeconds * mSampleRate + 0.5);
		}
	}
}
