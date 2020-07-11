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

		/// @brief Move constructs a `ParallelEQBand` from the given one
		///
		/// @param band - The `ParallelEQBand` to move
		ParallelEQBand<float>::ParallelEQBand(const ParallelEQBand<float>&& band) noexcept
			: EQBand<float>(std::move(band))
			{
				setGainDB(mGain);
			}

		ParallelEQBand<float>::~ParallelEQBand() noexcept {

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

		ParallelEQBand<float> ParallelEQBand<float>::operator=(const ParallelEQBand<float>&& band) noexcept {
			return ParallelEQBand<float>(std::move(band));
		}

		/// @brief Creates necessary filter(s) for this EQ band
		void ParallelEQBand<float>::createFilters() noexcept {
			EQBand<float>::createFilters();
			if(mType == BandType::LowShelf) {
				mFilter = std::move(
						BiQuadFilter<float>::MakeLowpass(
							mFrequency,
							mQ,
							mSampleRate
							)
						);
			}
			else if(mType == BandType::HighShelf) {
				mFilter = std::move(
						BiQuadFilter<float>::MakeHighpass(
							mFrequency,
							mQ,
							mSampleRate
							)
						);
			}
			else if(mType == BandType::Bell) {
				mFilter = std::move(
						BiQuadFilter<float>::MakeBandpass(
							mFrequency,
							mQ,
							mSampleRate
							)
						);
			}
			else if(mType == BandType::AnalogBell) {
				mFilter = std::move(
						BiQuadFilter<float>::MakeBandpass(
							mFrequency,
							mQ * math::pow10f(mGainActual / 40.0f),
							mSampleRate
							)
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

		/// @brief Move constructs a `ParallelEQBand` from the given one
		///
		/// @param band - The `ParallelEQBand` to move
		ParallelEQBand<double>::ParallelEQBand(const ParallelEQBand<double>&& band) noexcept
			: EQBand<double>(std::move(band))
			{
				setGainDB(mGain);
			}

		ParallelEQBand<double>::~ParallelEQBand() noexcept {

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

		ParallelEQBand<double> ParallelEQBand<double>::operator=(const ParallelEQBand<double>&& band) noexcept {
			return ParallelEQBand<double>(std::move(band));
		}

		/// @brief Creates necessary filter(s) for this EQ band
		void ParallelEQBand<double>::createFilters() noexcept {
			EQBand<double>::createFilters();
			if(mType == BandType::LowShelf) {
				mFilter = std::move(
						BiQuadFilter<double>::MakeLowpass(
							mFrequency,
							mQ,
							mSampleRate
							)
						);
			}
			else if(mType == BandType::HighShelf) {
				mFilter = std::move(
						BiQuadFilter<double>::MakeHighpass(
							mFrequency,
							mQ,
							mSampleRate
							)
						);
			}
			else if(mType == BandType::Bell) {
				mFilter = std::move(
						BiQuadFilter<double>::MakeBandpass(
							mFrequency,
							mQ,
							mSampleRate
							)
						);
			}
			else if(mType == BandType::AnalogBell) {
				mFilter = std::move(
						BiQuadFilter<double>::MakeBandpass(
							mFrequency,
							mQ * math::pow10(mGainActual / 40.0),
							mSampleRate
							)
						);
			}
		}
	}
}
