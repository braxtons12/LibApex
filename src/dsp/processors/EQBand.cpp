#include "EQBand.h"

namespace apex {
	namespace dsp {

		/// @brief Creates a default `EQBand`
		EQBand<float>::EQBand() noexcept {
			mFilter = std::move(BiQuadFilter<float>::MakeBell(mFrequency, mQ, mGain, mSampleRate));
			mFilters.resize(0);
			mGainProcessor = std::move(Gain<float>(mGain, true));
		}

		/// @brief Creates an `EQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The type of band
		EQBand<float>::EQBand(float frequency, float q, float gainDB, size_t sampleRate, BandType type) noexcept
			: Processor<float>(),
			mType(type),
			mFrequency(frequency),
			mQ(q),
			mGain(gainDB),
			mSampleRate(sampleRate),
			mFilter(std::move(BiQuadFilter<float>::MakeAllpass()))
			{
				if(mType < BandType::Allpass) {
					if(mType % 4 == 0) mOrder = 1;
					if(mType % 4 == 1) mOrder = 2;
					if(mType % 4 == 2) mOrder = 4;
					if(mType % 4 == 3) mOrder = 8;
					mGainProcessor = std::move(Gain<float>(mGain, true));
				}

				mFilters.resize(mOrder);
				createFilters();
			}

		/// @brief Move constructs an `EQBand` from the given one
		///
		/// @param band - The `EQBand` to move
		EQBand<float>::EQBand(const EQBand<float>&& band) noexcept {
			mType = band.mType;
			mFrequency = band.mFrequency;
			mQ = band.mQ;
			mGain = band.mGain;
			mSampleRate = band.mSampleRate;
			mFilter = std::move(band.mFilter);
			mOrder = band.mOrder;
			mGainProcessor = std::move(band.mGainProcessor);
			mFilters.resize(band.mFilters.size());
			for(size_t i = 0; i < mFilters.size(); ++i) {
				mFilters[i] = std::move(band.mFilters[i]);
			}
		}

		EQBand<float>::~EQBand() noexcept {

		}

		/// @brief Sets the frequency of this `EQBand` to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		void EQBand<float>::setFrequency(float frequency) noexcept {
			mFrequency = frequency;
			mFilter.setFrequency(mFrequency);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setFrequency(mFrequency);
				}
			}
		}

		/// @brief Returns the frequency of this `EQBand`
		///
		/// @return - The current frequency, in Hertz
		float EQBand<float>::getFrequency() const noexcept {
			return mFrequency;
		}

		/// @brief Sets the Q of this `EQBand` to the given value
		///
		/// @param q - The new Q
		void EQBand<float>::setQ(float q) noexcept {
			mQ = q;
			mFilter.setQ(mQ);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setQ(mQ);
				}
			}
		}

		/// @brief Returns the Q of this `EQBand`
		///
		/// @return - The current Q
		float EQBand<float>::getQ() const noexcept {
			return mQ;
		}

		/// @brief Sets the gain of this `EQBand`
		///
		/// @param gainDB - The new gain, in Decibels
		void EQBand<float>::setGainDB(float gainDB) noexcept {
			mGain = gainDB;
			mFilter.setGainDB(mGain);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setGainDB(mGain);
				}
			}
		}

		/// @brief Returns the gain of this `EQBand`
		///
		/// @return - The current gain, in Decibels
		float EQBand<float>::getGainDB() const noexcept {
			return mGain;
		}

		/// @brief Sets the sample rate of this `EQBand` to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void EQBand<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mFilter.setSampleRate(mSampleRate);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setSampleRate(mSampleRate);
				}
			}
		}

		/// @brief Returns the sample rate of this `EQBand`
		///
		/// @return - The current sample rate, in Hertz
		size_t EQBand<float>::getSampleRate() const noexcept {
			return mSampleRate;
		}

		/// @brief Sets the type of this `EQBand` to the given value
		///
		/// @param type - The new type
		void EQBand<float>::setBandType(BandType type) noexcept {
			mType = type;
			if(mType < BandType::Allpass) {
				if(mType % 4 == 0) mOrder = 1;
				if(mType % 4 == 1) mOrder = 2;
				if(mType % 4 == 2) mOrder = 4;
				if(mType % 4 == 3) mOrder = 8;
				mFilters.resize(mOrder);
			}
			createFilters();
		}

		/// @brief Returns the type of this `EQBand`
		///
		/// @return - The current type
		EQBand<float>::BandType EQBand<float>::getBandType() const noexcept {
			return mType;
		}

		/// @brief Applies this `EQBand` to the given input value
		///
		/// @param input - The input to apply EQ to
		///
		/// @return - The processed value
		float EQBand<float>::process(float input) noexcept {
			float x = 0.0f;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x = mFilters[ord].process(input);
				}
				x = mGainProcessor.process(x);
			}
			else {
				x = mFilter.process(input);
				if(mType == BandType::Allpass || mType == BandType::Notch)
					x = mGainProcessor.process(x);
			}
			return x;
		}

		/// @brief Applies this `EQBand` to the given array of input values, in place
		///
		/// @param input - The input values to apply EQ to
		/// @param numSamples - The number of input samples
		void EQBand<float>::process(float* input, size_t numSamples) noexcept {
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].process(input, numSamples);
				}
				mGainProcessor.process(input, numSamples);
			}
			else {
				mFilter.process(input, numSamples);
			}
		}

		/// @brief Resets this `EQBand` to an initial state
		void EQBand<float>::reset() noexcept {
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].reset();
				}
			}
			else {
				mFilter.reset();
			}
		}

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline float EQBand<float>::getMagnitudeForFrequency(float frequency) const noexcept {
			float x = 1.0f;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x *= mFilters[ord].getMagnitudeForFrequency(frequency);
				}
			}
			else {
				x = mFilter.getMagnitudeForFrequency(frequency);
			}
			return x;
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline float EQBand<float>::getDecibelMagnitudeForFrequency(float frequency) const noexcept {
			return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void EQBand<float>::getMagnitudesForFrequencies(float* frequencies, float* magnitudes,
				size_t numFrequencies) const noexcept
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
		inline void EQBand<float>::getDecibelMagnitudesForFrequencies(float* frequencies,
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
		inline float EQBand<float>::getPhaseForFrequency(float frequency) const noexcept {
			float x = 0.0f;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x += mFilters[ord].getPhaseForFrequency(frequency);
				}
			}
			else {
				x = mFilter.getPhaseForFrequency(frequency);
			}
			return x;

		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		inline float EQBand<float>::getDegreesPhaseForFrequency(float frequency) const noexcept {
			return getPhaseForFrequency(frequency) * 180.0f / math::pif;
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void EQBand<float>::getPhasesForFrequencies(float* frequencies, float* phases,
				size_t numFrequencies) const noexcept
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
		inline void EQBand<float>::getDegreesPhasesForFrequencies(float* frequencies,
				float* phases, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getDegreesPhaseForFrequency(frequencies[frequency]);
			}
		}

		EQBand<float> EQBand<float>::operator=(const EQBand<float>&& band) noexcept {
			return EQBand<float>(std::move(band));
		}

		/// @brief Returns the shifted frequency for the Nth filter stage in
		/// a multi-order filter
		///
		/// @param filterIndex - The filter stage to calculate the shift for
		///
		/// @return - The shifted frequency
		float EQBand<float>::frequencyShift(size_t filterIndex) const noexcept {
			float shiftMultiplier = 0.25f * filterIndex;
			if(mType < BandType::Highpass12DB) {
				float nextOctFreq = mFrequency * 2.0f;
				return mFrequency + (shiftMultiplier * (nextOctFreq - mFrequency));
			}
			else {
				float nextOctFreq = mFrequency / 2.0f;
				return mFrequency - (shiftMultiplier * (nextOctFreq - mFrequency));
			}
		}

		/// @brief Creates the necessary filter(s) for this `EQBand`
		void EQBand<float>::createFilters() noexcept {

			switch(mType) {
				case Lowpass12DB: {
									  mFilter = std::move(
											  BiQuadFilter<float>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<float>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Lowpass24DB: {
									  mFilter = std::move(
											  BiQuadFilter<float>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<float>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Lowpass48DB: {
									  mFilter = std::move(
											  BiQuadFilter<float>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<float>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Lowpass96DB: {
									  mFilter = std::move(
											  BiQuadFilter<float>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<float>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Highpass12DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Highpass24DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Highpass48DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Highpass96DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass12DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass24DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass48DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass96DB: {
									   mFilter = std::move(
											   BiQuadFilter<float>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<float>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Allpass: {
								  mFilter = std::move(
										  BiQuadFilter<float>::MakeAllpass(
											  mFrequency,
											  mQ,
											  mSampleRate
											  )
										  );
							  }
							  break;
				case Notch: {
								mFilter = std::move(
										BiQuadFilter<float>::MakeNotch(
											mFrequency,
											mQ,
											mSampleRate
											)
										);
							}
							break;
				case LowShelf: {
								   mFilter = std::move(
										   BiQuadFilter<float>::MakeLowShelf(
											   mFrequency,
											   mQ,
											   mGain,
											   mSampleRate
											   )
										   );
							   }
							   break;
				case HighShelf: {
									mFilter = std::move(
											BiQuadFilter<float>::MakeHighShelf(
												mFrequency,
												mQ,
												mGain,
												mSampleRate
												)
											);
								}
								break;
				case Bell: {
							   mFilter = std::move(
									   BiQuadFilter<float>::MakeBell(
										   mFrequency,
										   mQ,
										   mGain,
										   mSampleRate
										   )
									   );
						   }
						   break;
				case AnalogBell: {
									 mFilter = std::move(
											 BiQuadFilter<float>::MakeAnalogBell(
												 mFrequency,
												 mQ,
												 mGain,
												 mSampleRate
												 )
											 );
								 }
								 break;
			}
		}

		/// @brief Creates a default `EQBand`
		EQBand<double>::EQBand() noexcept {
			mFilter = std::move(BiQuadFilter<double>::MakeBell(mFrequency, mQ, mGain, mSampleRate));
			mFilters.resize(0);
			mGainProcessor = std::move(Gain<double>(mGain, true));
		}

		/// @brief Creates an `EQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The type of band
		EQBand<double>::EQBand(double frequency, double q, double gainDB, size_t sampleRate, BandType type) noexcept
			: Processor<double>(),
			mType(type),
			mFrequency(frequency),
			mQ(q),
			mGain(gainDB),
			mSampleRate(sampleRate),
			mFilter(std::move(BiQuadFilter<double>::MakeAllpass()))
			{
				if(mType < BandType::Allpass) {
					if(mType % 4 == 0) mOrder = 1;
					if(mType % 4 == 1) mOrder = 2;
					if(mType % 4 == 2) mOrder = 4;
					if(mType % 4 == 3) mOrder = 8;
					mGainProcessor = std::move(Gain<double>(mGain, true));
				}

				mFilters.resize(mOrder);
				createFilters();
			}

		/// @brief Move constructs an `EQBand` from the given one
		///
		/// @param band - The `EQBand` to move
		EQBand<double>::EQBand(const EQBand<double>&& band) noexcept {
			mType = band.mType;
			mFrequency = band.mFrequency;
			mQ = band.mQ;
			mGain = band.mGain;
			mSampleRate = band.mSampleRate;
			mFilter = std::move(band.mFilter);
			mOrder = band.mOrder;
			mGainProcessor = std::move(band.mGainProcessor);
			mFilters.resize(band.mFilters.size());
			for(size_t i = 0; i < mFilters.size(); ++i) {
				mFilters[i] = std::move(band.mFilters[i]);
			}
		}

		EQBand<double>::~EQBand() noexcept {

		}

		/// @brief Sets the frequency of this `EQBand` to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		void EQBand<double>::setFrequency(double frequency) noexcept {
			mFrequency = frequency;
			mFilter.setFrequency(mFrequency);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setFrequency(mFrequency);
				}
			}
		}

		/// @brief Returns the frequency of this `EQBand`
		///
		/// @return - The current frequency, in Hertz
		double EQBand<double>::getFrequency() const noexcept {
			return mFrequency;
		}

		/// @brief Sets the Q of this `EQBand` to the given value
		///
		/// @param q - The new Q
		void EQBand<double>::setQ(double q) noexcept {
			mQ = q;
			mFilter.setQ(mQ);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setQ(mQ);
				}
			}
		}

		/// @brief Returns the Q of this `EQBand`
		///
		/// @return - The current Q
		double EQBand<double>::getQ() const noexcept {
			return mQ;
		}

		/// @brief Sets the gain of this `EQBand`
		///
		/// @param gainDB - The new gain, in Decibels
		void EQBand<double>::setGainDB(double gainDB) noexcept {
			mGain = gainDB;
			mGainProcessor.setGainDecibels(mGain);
			mFilter.setGainDB(mGain);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setGainDB(mGain);
				}
			}
		}

		/// @brief Returns the gain of this `EQBand`
		///
		/// @return - The current gain, in Decibels
		double EQBand<double>::getGainDB() const noexcept {
			return mGain;
		}

		/// @brief Sets the sample rate of this `EQBand` to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void EQBand<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mFilter.setSampleRate(mSampleRate);
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].setSampleRate(mSampleRate);
				}
			}
		}

		/// @brief Returns the sample rate of this `EQBand`
		///
		/// @return - The current sample rate, in Hertz
		size_t EQBand<double>::getSampleRate() const noexcept {
			return mSampleRate;
		}

		/// @brief Sets the type of this `EQBand` to the given value
		///
		/// @param type - The new type
		void EQBand<double>::setBandType(BandType type) noexcept {
			mType = type;
			if(mType < BandType::Allpass) {
				if(mType % 4 == 0) mOrder = 1;
				if(mType % 4 == 1) mOrder = 2;
				if(mType % 4 == 2) mOrder = 4;
				if(mType % 4 == 3) mOrder = 8;
				mFilters.resize(mOrder);
			}
			createFilters();
		}

		/// @brief Returns the type of this `EQBand`
		///
		/// @return - The current type
		EQBand<double>::BandType EQBand<double>::getBandType() const noexcept {
			return mType;
		}

		/// @brief Applies this `EQBand` to the given input value
		///
		/// @param input - The input to apply EQ to
		///
		/// @return - The processed value
		double EQBand<double>::process(double input) noexcept {
			double x = 0.0;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x = mFilters[ord].process(input);
				}
				x = mGainProcessor.process(x);
			}
			else {
				x = mFilter.process(input);
				if(mType == BandType::Allpass || mType == BandType::Notch)
					x = mGainProcessor.process(x);
			}
			return x;
		}

		/// @brief Applies this `EQBand` to the given array of input values, in place
		///
		/// @param input - The input values to apply EQ to
		/// @param numSamples - The number of input samples
		void EQBand<double>::process(double* input, size_t numSamples) noexcept {
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].process(input, numSamples);
				}
				mGainProcessor.process(input, numSamples);
			}
			else {
				mFilter.process(input, numSamples);
			}
		}

		/// @brief Resets this `EQBand` to an initial state
		void EQBand<double>::reset() noexcept {
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					mFilters[ord].reset();
				}
			}
			else {
				mFilter.reset();
			}
		}

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline double EQBand<double>::getMagnitudeForFrequency(double frequency) const noexcept {
			double x = 1.0;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x *= mFilters[ord].getMagnitudeForFrequency(frequency);
				}
			}
			else {
				x = mFilter.getMagnitudeForFrequency(frequency);
			}
			return x;
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		inline double EQBand<double>::getDecibelMagnitudeForFrequency(double frequency) const noexcept {
			return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void EQBand<double>::getMagnitudesForFrequencies(double* frequencies,
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
		inline void EQBand<double>::getDecibelMagnitudesForFrequencies(double* frequencies,
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
		inline double EQBand<double>::getPhaseForFrequency(double frequency) const noexcept {
			double x = 0.0;
			if(mType < BandType::Allpass) {
				for(size_t ord = 0; ord < mOrder; ++ord) {
					x += mFilters[ord].getPhaseForFrequency(frequency);
				}
			}
			else {
				x = mFilter.getPhaseForFrequency(frequency);
			}
			return x;

		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		inline double EQBand<double>::getDegreesPhaseForFrequency(double frequency) const noexcept {
			return getPhaseForFrequency(frequency) * 180.0 / math::pi;
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		inline void EQBand<double>::getPhasesForFrequencies(double* frequencies, double* phases,
				size_t numFrequencies) const noexcept
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
		inline void EQBand<double>::getDegreesPhasesForFrequencies(double* frequencies,
				double* phases, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getDegreesPhaseForFrequency(frequencies[frequency]);
			}
		}

		EQBand<double> EQBand<double>::operator=(const EQBand<double>&& band) noexcept {
			return EQBand<double>(std::move(band));
		}

		/// @brief Returns the shifted frequency for the Nth filter stage in
		/// a multi-order filter
		///
		/// @param filterIndex - The filter stage to calculate the shift for
		///
		/// @return - The shifted frequency
		double EQBand<double>::frequencyShift(size_t filterIndex) const noexcept {
			double shiftMultiplier = 0.25 * filterIndex;
			if(mType < BandType::Highpass12DB) {
				double nextOctFreq = mFrequency * 2.0;
				return mFrequency + (shiftMultiplier * (nextOctFreq - mFrequency));
			}
			else {
				double nextOctFreq = mFrequency / 2.0;
				return mFrequency - (shiftMultiplier * (nextOctFreq - mFrequency));
			}
		}

		/// @brief Creates the necessary filter(s) for this `EQBand`
		void EQBand<double>::createFilters() noexcept {

			switch(mType) {
				case Lowpass12DB: {
									  mFilter = std::move(
											  BiQuadFilter<double>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<double>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Lowpass24DB: {
									  mFilter = std::move(
											  BiQuadFilter<double>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<double>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Lowpass48DB: {
									  mFilter = std::move(
											  BiQuadFilter<double>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<double>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Lowpass96DB: {
									  mFilter = std::move(
											  BiQuadFilter<double>::MakeLowpass(
												  mFrequency,
												  mQ,
												  mSampleRate
												  )
											  );
									  for(size_t ord = 0; ord < mOrder; ++ord) {
										  mFilters[ord] = std::move(
												  BiQuadFilter<double>::MakeLowpass(
													  frequencyShift(ord),
													  mQ,
													  mSampleRate
													  )
												  );
									  }
								  }
								  break;
				case Highpass12DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Highpass24DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Highpass48DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Highpass96DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeHighpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeHighpass(
													   frequencyShift(ord),
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass12DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass24DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass48DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Bandpass96DB: {
									   mFilter = std::move(
											   BiQuadFilter<double>::MakeBandpass(
												   mFrequency,
												   mQ,
												   mSampleRate
												   )
											   );
									   for(size_t ord = 0; ord < mOrder; ++ord) {
										   mFilters[ord] = std::move(
												   BiQuadFilter<double>::MakeBandpass(
													   mFrequency,
													   mQ,
													   mSampleRate
													   )
												   );
									   }
								   }
								   break;
				case Allpass: {
								  mFilter = std::move(
										  BiQuadFilter<double>::MakeAllpass(
											  mFrequency,
											  mQ,
											  mSampleRate
											  )
										  );
							  }
							  break;
				case Notch: {
								mFilter = std::move(
										BiQuadFilter<double>::MakeNotch(
											mFrequency,
											mQ,
											mSampleRate
											)
										);
							}
							break;
				case LowShelf: {
								   mFilter = std::move(
										   BiQuadFilter<double>::MakeLowShelf(
											   mFrequency,
											   mQ,
											   mGain,
											   mSampleRate
											   )
										   );
							   }
							   break;
				case HighShelf: {
									mFilter = std::move(
											BiQuadFilter<double>::MakeHighShelf(
												mFrequency,
												mQ,
												mGain,
												mSampleRate
												)
											);
								}
								break;
				case Bell: {
							   mFilter = std::move(
									   BiQuadFilter<double>::MakeBell(
										   mFrequency,
										   mQ,
										   mGain,
										   mSampleRate
										   )
									   );
						   }
						   break;
				case AnalogBell: {
									 mFilter = std::move(
											 BiQuadFilter<double>::MakeAnalogBell(
												 mFrequency,
												 mQ,
												 mGain,
												 mSampleRate
												 )
											 );
								 }
								 break;
			}
		}
	}
}
