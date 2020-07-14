#include "ParallelEQBand.h"

namespace apex {
	namespace dsp {

		/// @brief Creates a default `ParallelEQBand`
		ParallelEQBand<float>::ParallelEQBand() noexcept
			: EQBand<float>()
			{
				setGainDB(mGain);
			}

		/// @brief Creates a `ParallelEQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The band type to use
		ParallelEQBand<float>::ParallelEQBand(float frequency, float q, float gainDB,
				size_t sampleRate, BandType type) noexcept
			: EQBand<float>(frequency, q, gainDB, sampleRate, type)
			{
				setGainDB(mGain);
			}

		/// @brief Sets the gain of this `ParallelEQBand` to the given value
		///
		/// @param gainDB - The new gain, in Decibels
		void ParallelEQBand<float>::setGainDB(float gainDB) noexcept {
			if(mType < BandType::Allpass) {
				mGain = gainDB;
				mGainActual = mGain;
				mGainProcessor.setGainDecibels(mGain);
				mFilter.setGainDB(mGain);
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setGainDB(mGain);
				}
			}
			else if(mType < BandType::LowShelf) {
				mGain = gainDB;
				mGainActual = mGain;
				mGainProcessor.setGainDecibels(mGain);
				mFilter.setGainDB(mGain);
			}
			else {
				mGainActual = gainDB;
				//if gain is negative, we need to subtract that amount of linear gain from the input
				//to get the desired gain:
				//linear 0db (eg 1.0) - linear gain level = amount of linear signal to be removed
				if(gainDB < 0.0f) mGain = math::Decibels::linearToDecibels(
						1.0f - math::Decibels::decibelsToLinear(gainDB)
						);
				//otherwise, the gain is the amount of linear gain we need to add to the input:
				//linear gain level - linear 0db (eg 1.0) = amount of linear signal to be added
				else mGain = math::Decibels::linearToDecibels(
						math::Decibels::decibelsToLinear(gainDB) - 1.0f
						);

				mGainProcessor.setGainDecibels(mGain);
				mFilter.setGainDB(mGain);
			}
		}

		/// @brief Returns the gain of this `ParallelEQBand`
		///
		/// @return - The current gain, in Decibels
		float ParallelEQBand<float>::getGainDB() const noexcept {
			return mGainActual;
		}

		/// @brief Applies this `ParallelEQBand` to the given input value
		///
		/// @param input - The input value to apply EQ to
		///
		/// @return - The processed value
		float ParallelEQBand<float>::process(float input) noexcept {
			float x = 0.0f;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x = mFilters[ord].process(input);
				}
				x = mGainProcessor.process(x);
			}
			else if (mType < BandType::LowShelf) {
				x = mFilter.process(input);
				x = mGainProcessor.process(x);
			}
			else {
				x = mFilter.process(input);
				x = mGainProcessor.process(x);
				if(mGainActual < 0.0f) x = input - x;
				else x = input + x;
			}
			return x;
		}

		/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
		///
		/// @param input - The array of input values to apply EQ to
		/// @param numSamples - The number of samples in the array
		void ParallelEQBand<float>::process(float* input, size_t numSamples) noexcept {
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].process(input, numSamples);
				}
				mGainProcessor.process(input, numSamples);
			}
			else if (mType < BandType::LowShelf) {
				mFilter.process(input, numSamples);
				mGainProcessor.process(input, numSamples);
			}
			else {
				float* x = new float[numSamples];
				std::memcpy(x, input, sizeof(float) * numSamples);
				mFilter.process(x, numSamples);
				mGainProcessor.process(x, numSamples);
				if(mGainActual < 0.0f) {
					for(size_t sample = 0; sample < numSamples; ++sample) {
						input[sample] -= x[sample];
					}
				}
				else {
					for(size_t sample = 0; sample < numSamples; ++sample) {
						input[sample] += x[sample];
					}
				}
				delete[] x;
			}
		}

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline float ParallelEQBand<float>::getMagnitudeForFrequency(float frequency) const noexcept {
			float x = 1.0f;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x*= mFilters[ord].getMagnitudeForFrequency(frequency);
				}
			}
			else if(mType < BandType::LowShelf) {
				x = mFilter.getMagnitudeForFrequency(frequency);
			}
			else if(mType == BandType::LowShelf) {
				BiQuadFilter<float> filt = BiQuadFilter<float>::MakeLowShelf(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else if(mType == BandType::HighShelf) {
				BiQuadFilter<float> filt = BiQuadFilter<float>::MakeHighShelf(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else if(mType == BandType::Bell) {
				BiQuadFilter<float> filt = BiQuadFilter<float>::MakeBell(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else {
				BiQuadFilter<float> filt = BiQuadFilter<float>::MakeAnalogBell(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			return x;
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline float ParallelEQBand<float>::getDecibelMagnitudeForFrequency(float frequency) const noexcept {
			return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<float>::getMagnitudesForFrequencies(float* frequencies,
				float* magnitudes, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				magnitudes[frequency] = getMagnitudeForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<float>::getDecibelMagnitudesForFrequencies(float* frequencies,
				float* magnitudes, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				magnitudes[frequency] = getDecibelMagnitudeForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in radians, at the given frequency
		inline float ParallelEQBand<float>::getPhaseForFrequency(float frequency) const noexcept {
			float x = 0.0f;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x += mFilters[ord].getPhaseForFrequency(frequency);
				}
			}
			else if(mType < BandType::LowShelf) {
				x = mFilter.getPhaseForFrequency(frequency);
			}
			else {
				///TODO: This might not be the accurate way to do this,
				///but for now it's what we have

				//Parallel bands should have phase-shift as the average between
				//the shift caused by the filter and the original, right?
				x = mFilter.getPhaseForFrequency(frequency) / 2.0f;
			}
			return x;
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		inline float ParallelEQBand<float>::getDegreesPhaseForFrequency(float frequency) const noexcept {
			return getPhaseForFrequency(frequency) * 180.0f / math::pif;
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<float>::getPhasesForFrequencies(float* frequencies,
				float* phases, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getPhaseForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<float>::getDegreesPhasesForFrequencies(float* frequencies,
				float* phases, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getDegreesPhaseForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Creates necessary filter(s) for this EQ band
		void ParallelEQBand<float>::createFilters() noexcept {
			EQBand<float>::createFilters();
			if(mType == BandType::LowShelf) {
				mFilter = BiQuadFilter<float>::MakeLowpass(
						mFrequency,
						mQ,
						mSampleRate
						);
			}
			else if(mType == BandType::HighShelf) {
				mFilter = BiQuadFilter<float>::MakeHighpass(
						mFrequency,
						mQ,
						mSampleRate
						);
			}
			else if(mType == BandType::Bell) {
				mFilter = BiQuadFilter<float>::MakeBandpass(
						mFrequency,
						mQ,
						mSampleRate
						);
			}
			else if(mType == BandType::AnalogBell) {
				mFilter = BiQuadFilter<float>::MakeBandpass(
						mFrequency,
						mQ * math::pow10f(mGainActual / 40.0f),
						mSampleRate
						);
			}
		}

		/// @brief Creates a default `ParallelEQBand`
		ParallelEQBand<double>::ParallelEQBand() noexcept
			: EQBand<double>()
			{
				setGainDB(mGain);
			}

		/// @brief Creates a `ParallelEQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The band type to use
		ParallelEQBand<double>::ParallelEQBand(double frequency, double q, double gainDB,
				size_t sampleRate, BandType type) noexcept
			: EQBand<double>(frequency, q, gainDB, sampleRate, type)
			{
				setGainDB(mGain);
			}

		/// @brief Sets the gain of this `ParallelEQBand` to the given value
		///
		/// @param gainDB - The new gain, in Decibels
		void ParallelEQBand<double>::setGainDB(double gainDB) noexcept {
			if(mType < BandType::Allpass) {
				mGain = gainDB;
				mGainActual = mGain;
				mGainProcessor.setGainDecibels(mGain);
				mFilter.setGainDB(mGain);
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setGainDB(mGain);
				}
			}
			else if(mType < BandType::LowShelf) {
				mGain = gainDB;
				mGainActual = mGain;
				mGainProcessor.setGainDecibels(mGain);
				mFilter.setGainDB(mGain);
			}
			else {
				mGainActual = gainDB;
				//if gain is negative, we need to subtract that amount of linear gain from the input
				//to get the desired gain:
				//linear 0db (eg 1.0) - linear gain level = amount of linear signal to be removed
				if(gainDB < 0.0) mGain = math::Decibels::linearToDecibels(
						1.0 - math::Decibels::decibelsToLinear(gainDB)
						);
				//otherwise, the gain is the amount of linear gain we need to add to the input:
				//linear gain level - linear 0db (eg 1.0) = amount of linear signal to be added
				else mGain = math::Decibels::linearToDecibels(
						math::Decibels::decibelsToLinear(gainDB) - 1.0
						);

				mGainProcessor.setGainDecibels(mGain);
				mFilter.setGainDB(mGain);
			}
		}

		/// @brief Returns the gain of this `ParallelEQBand`
		///
		/// @return - The current gain, in Decibels
		double ParallelEQBand<double>::getGainDB() const noexcept {
			return mGainActual;
		}

		/// @brief Applies this `ParallelEQBand` to the given input value
		///
		/// @param input - The input value to apply EQ to
		///
		/// @return - The processed value
		double ParallelEQBand<double>::process(double input) noexcept {
			double x = 0.0;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x = mFilters[ord].process(input);
				}
				x = mGainProcessor.process(x);
			}
			else if (mType < BandType::LowShelf) {
				x = mFilter.process(input);
				x = mGainProcessor.process(x);
			}
			else {
				x = mFilter.process(input);
				x = mGainProcessor.process(x);
				if(mGainActual < 0.0) x = input - x;
				else x = input + x;
			}
			return x;
		}

		/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
		///
		/// @param input - The array of input values to apply EQ to
		/// @param numSamples - The number of samples in the array
		void ParallelEQBand<double>::process(double* input, size_t numSamples) noexcept {
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].process(input, numSamples);
				}
				mGainProcessor.process(input, numSamples);
			}
			else if (mType < BandType::LowShelf) {
				mFilter.process(input, numSamples);
				mGainProcessor.process(input, numSamples);
			}
			else {
				double* x = new double[numSamples];
				std::memcpy(x, input, sizeof(double) * numSamples);
				mFilter.process(x, numSamples);
				mGainProcessor.process(x, numSamples);
				if(mGainActual < 0.0) {
					for(size_t sample = 0; sample < numSamples; ++sample) {
						input[sample] -= x[sample];
					}
				}
				else {
					for(size_t sample = 0; sample < numSamples; ++sample) {
						input[sample] += x[sample];
					}
				}
				delete[] x;
			}
		}

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline double ParallelEQBand<double>::getMagnitudeForFrequency(double frequency) const noexcept {
			double x = 1.0;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x*= mFilters[ord].getMagnitudeForFrequency(frequency);
				}
			}
			else if(mType < BandType::LowShelf) {
				x = mFilter.getMagnitudeForFrequency(frequency);
			}
			else if(mType == BandType::LowShelf) {
				BiQuadFilter<double> filt = BiQuadFilter<double>::MakeLowShelf(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else if(mType == BandType::HighShelf) {
				BiQuadFilter<double> filt = BiQuadFilter<double>::MakeHighShelf(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else if(mType == BandType::Bell) {
				BiQuadFilter<double> filt = BiQuadFilter<double>::MakeBell(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else {
				BiQuadFilter<double> filt = BiQuadFilter<double>::MakeAnalogBell(
						mFrequency, mQ, mGain, mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			return x;
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline double ParallelEQBand<double>::getDecibelMagnitudeForFrequency(double frequency) const noexcept {
			return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<double>::getMagnitudesForFrequencies(double* frequencies,
				double* magnitudes, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				magnitudes[frequency] = getMagnitudeForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<double>::getDecibelMagnitudesForFrequencies(double* frequencies,
				double* magnitudes, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				magnitudes[frequency] = getDecibelMagnitudeForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in radians, at the given frequency
		inline double ParallelEQBand<double>::getPhaseForFrequency(double frequency) const noexcept {
			double x = 0.0;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x += mFilters[ord].getPhaseForFrequency(frequency);
				}
			}
			else if(mType < BandType::LowShelf) {
				x = mFilter.getPhaseForFrequency(frequency);
			}
			else {
				///TODO: This might not be the accurate way to do this,
				///but for now it's what we have

				//Parallel bands should have phase-shift as the average between
				//the shift caused by the filter and the original, right?
				x = mFilter.getPhaseForFrequency(frequency) / 2.0;
			}
			return x;
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		inline double ParallelEQBand<double>::getDegreesPhaseForFrequency(double frequency) const noexcept {
			return getPhaseForFrequency(frequency) * 180.0 / math::pi;
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<double>::getPhasesForFrequencies(double* frequencies,
				double* phases, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getPhaseForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void ParallelEQBand<double>::getDegreesPhasesForFrequencies(double* frequencies,
				double* phases, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getDegreesPhaseForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Creates necessary filter(s) for this EQ band
		void ParallelEQBand<double>::createFilters() noexcept {
			EQBand<double>::createFilters();
			if(mType == BandType::LowShelf) {
				mFilter = BiQuadFilter<double>::MakeLowpass(
						mFrequency,
						mQ,
						mSampleRate
						);
			}
			else if(mType == BandType::HighShelf) {
				mFilter = BiQuadFilter<double>::MakeHighpass(
						mFrequency,
						mQ,
						mSampleRate
						);
			}
			else if(mType == BandType::Bell) {
				mFilter = BiQuadFilter<double>::MakeBandpass(
						mFrequency,
						mQ,
						mSampleRate
						);
			}
			else if(mType == BandType::AnalogBell) {
				mFilter = BiQuadFilter<double>::MakeBandpass(
						mFrequency,
						mQ * math::pow10(mGainActual / 40.0),
						mSampleRate
						);
			}
		}
	}
}
