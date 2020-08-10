#include "EQBand.h"

namespace apex::dsp {
	/// @brief Creates an `EQBand` with the given parameters
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param q - The Q to use
	/// @param gainDB - The gain to use, in Decibels
	/// @param sampleRate - The sample rate to use, in Hertz
	/// @param type - The type of band
	EQBand<float>::EQBand(float frequency,
						  float q,
						  float gainDB,
						  size_t sampleRate,
						  BandType type) noexcept
		: mType(type), mFrequency(frequency), mQ(q), mGain(gainDB), mSampleRate(sampleRate),
		  mFilter(BiQuadFilter<float>::MakeAllpass()) {
		if(mType < BandType::Allpass) {
			switch(static_cast<size_t>(mType) % 4) {
				case 0: mOrder = 1; break;
				case 1: mOrder = 2; break;
				case 2: mOrder = 4; break;
				case 3: mOrder = 8; break;
			}
			mGainProcessor = Gain<float>(mGain, true);
		}

		mFilters.resize(mOrder);
		createFilters();
	}

	/// @brief Sets the frequency of this `EQBand` to the given value
	///
	/// @param frequency - The new frequency, in Hertz
	auto EQBand<float>::setFrequency(float frequency) noexcept -> void {
		mFrequency = frequency;
		mFilter.setFrequency(mFrequency);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setFrequency(mFrequency);
			}
		}
	}

	/// @brief Returns the frequency of this `EQBand`
	///
	/// @return - The current frequency, in Hertz
	auto EQBand<float>::getFrequency() const noexcept -> float {
		return mFrequency;
	}

	/// @brief Sets the Q of this `EQBand` to the given value
	///
	/// @param q - The new Q
	auto EQBand<float>::setQ(float q) noexcept -> void {
		mQ = q;
		mFilter.setQ(mQ);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setQ(mQ);
			}
		}
	}

	/// @brief Returns the Q of this `EQBand`
	///
	/// @return - The current Q
	auto EQBand<float>::getQ() const noexcept -> float {
		return mQ;
	}

	/// @brief Sets the gain of this `EQBand`
	///
	/// @param gainDB - The new gain, in Decibels
	auto EQBand<float>::setGainDB(float gainDB) noexcept -> void {
		mGain = gainDB;
		mFilter.setGainDB(mGain);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setGainDB(mGain);
			}
		}
	}

	/// @brief Returns the gain of this `EQBand`
	///
	/// @return - The current gain, in Decibels
	auto EQBand<float>::getGainDB() const noexcept -> float {
		return mGain;
	}

	/// @brief Sets the sample rate of this `EQBand` to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	auto EQBand<float>::setSampleRate(size_t sampleRate) noexcept -> void {
		mSampleRate = sampleRate;
		mFilter.setSampleRate(mSampleRate);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setSampleRate(mSampleRate);
			}
		}
	}

	/// @brief Returns the sample rate of this `EQBand`
	///
	/// @return - The current sample rate, in Hertz
	auto EQBand<float>::getSampleRate() const noexcept -> size_t {
		return mSampleRate;
	}

	/// @brief Sets the type of this `EQBand` to the given value
	///
	/// @param type - The new type
	auto EQBand<float>::setBandType(BandType type) noexcept -> void {
		mType = type;
		if(mType < BandType::Allpass) {
			switch(static_cast<size_t>(mType) % 4) {
				case 0: mOrder = 1; break;
				case 1: mOrder = 2; break;
				case 2: mOrder = 4; break;
				case 3: mOrder = 8; break;
			}
			mFilters.resize(mOrder);
		}
		createFilters();
	}

	/// @brief Returns the type of this `EQBand`
	///
	/// @return - The current type
	auto EQBand<float>::getBandType() const noexcept -> EQBand<float>::BandType {
		return mType;
	}

	/// @brief Applies this `EQBand` to the given input value
	///
	/// @param input - The input to apply EQ to
	///
	/// @return - The processed value
	auto EQBand<float>::process(float input) noexcept -> float {
		float x = 0.0F;
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				x = filt.process(x);
			}
			x = mGainProcessor.process(x);
		}
		else {
			x = mFilter.process(input);
			if(mType == BandType::Allpass || mType == BandType::Notch) {
				x = mGainProcessor.process(x);
			}
		}
		return x;
	}

	/// @brief Applies this `EQBand` to the given array of input values, in place
	///
	/// @param input - The input values to apply EQ to
	auto EQBand<float>::process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void {
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.process(input);
			}
			mGainProcessor.process(input);
		}
		else {
			mFilter.process(input);
		}
	}

	/// @brief Resets this `EQBand` to an initial state
	void EQBand<float>::reset() noexcept {
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.reset();
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
	inline auto EQBand<float>::getMagnitudeForFrequency(float frequency) const noexcept -> float {
		float x = 1.0F;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x *= filt.getMagnitudeForFrequency(frequency);
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
	inline auto
	EQBand<float>::getDecibelMagnitudeForFrequency(float frequency) const noexcept -> float {
		return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
	}

	/// @brief Calculates the linear magnitude response of this filter for the given array of
	/// frequencies and stores them in `magnitudes`
	///
	/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
	/// @param magnitudes - The array to store the magnitudes in
	inline auto EQBand<float>::getMagnitudesForFrequencies(
		gsl::span<float, gsl::dynamic_extent> frequencies,
		gsl::span<float, gsl::dynamic_extent> magnitudes) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(magnitudes, frequency)
				= getMagnitudeForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Calculates the decibel magnitude response of this filter for the given array of
	/// frequencies and stores them in `magnitudes`
	///
	/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
	/// @param magnitudes - The array to store the magnitudes in
	inline auto EQBand<float>::getDecibelMagnitudesForFrequencies(
		gsl::span<float, gsl::dynamic_extent> frequencies,
		gsl::span<float, gsl::dynamic_extent> magnitudes) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(magnitudes, frequency)
				= getDecibelMagnitudeForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Calculates the phase response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the phase response for, in Hertz
	///
	/// @return - The phase response, in radians, at the given frequency
	inline auto EQBand<float>::getPhaseForFrequency(float frequency) const noexcept -> float {
		float x = 0.0F;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x += filt.getPhaseForFrequency(frequency);
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
	inline auto
	EQBand<float>::getDegreesPhaseForFrequency(float frequency) const noexcept -> float {
		return getPhaseForFrequency(frequency) * 180.0F / math::pif;
	}

	/// @brief Calculates the phase response of this filter for the given array of frequencies and
	/// stores it in `phases`
	///
	/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
	/// @param phases - The array to store the phases (in radians) in
	inline auto EQBand<float>::getPhasesForFrequencies(
		gsl::span<float, gsl::dynamic_extent> frequencies,
		gsl::span<float, gsl::dynamic_extent> phases) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(phases, frequency) = getPhaseForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Calculates the phase response of this filter for the given array of frequencies and
	/// stores it in `phases`
	///
	/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
	/// @param phases - The array to store the phases (in degrees) in
	inline auto EQBand<float>::getDegreesPhasesForFrequencies(
		gsl::span<float, gsl::dynamic_extent> frequencies,
		gsl::span<float, gsl::dynamic_extent> phases) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(phases, frequency)
				= getDegreesPhaseForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Returns the shifted frequency for the Nth filter stage in
	/// a multi-order filter
	///
	/// @param filterIndex - The filter stage to calculate the shift for
	///
	/// @return - The shifted frequency
	auto EQBand<float>::frequencyShift(size_t filterIndex) const noexcept -> float {
		float shiftMultiplier = 0.25F * filterIndex;
		if(mType < BandType::Highpass12DB) {
			float nextOctFreq = mFrequency * 2.0F;
			return mFrequency + (shiftMultiplier * (nextOctFreq - mFrequency));
		}
		else {
			float nextOctFreq = mFrequency / 2.0F;
			return mFrequency - (shiftMultiplier * (nextOctFreq - mFrequency));
		}
	}

	/// @brief Creates the necessary filter(s) for this `EQBand`
	auto EQBand<float>::createFilters() noexcept -> void {

		switch(mType) {
			case BandType::Lowpass12DB:
				{
					mFilter = BiQuadFilter<float>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
					}
				}
				break;
			case BandType::Lowpass24DB:
				{
					mFilter = BiQuadFilter<float>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
					}
				}
				break;
			case BandType::Lowpass48DB:
				{
					mFilter = BiQuadFilter<float>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
					}
				}
				break;
			case BandType::Lowpass96DB:
				{
					mFilter = BiQuadFilter<float>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
					}
				}
				break;
			case BandType::Highpass12DB:
				{
					mFilter = BiQuadFilter<float>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Highpass24DB:
				{
					mFilter = BiQuadFilter<float>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Highpass48DB:
				{
					mFilter = BiQuadFilter<float>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Highpass96DB:
				{
					mFilter = BiQuadFilter<float>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<float>::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Bandpass12DB:
				{
					mFilter = BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Bandpass24DB:
				{
					mFilter = BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Bandpass48DB:
				{
					mFilter = BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Bandpass96DB:
				{
					mFilter = BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Allpass:
				{
					mFilter = BiQuadFilter<float>::MakeAllpass(mFrequency, mQ, mSampleRate);
				}
				break;
			case BandType::Notch:
				{
					mFilter = BiQuadFilter<float>::MakeNotch(mFrequency, mQ, mSampleRate);
				}
				break;
			case BandType::LowShelf:
				{
					mFilter = BiQuadFilter<float>::MakeLowShelf(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
			case BandType::HighShelf:
				{
					mFilter
						= BiQuadFilter<float>::MakeHighShelf(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
			case BandType::Bell:
				{
					mFilter = BiQuadFilter<float>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
			case BandType::AnalogBell:
				{
					mFilter
						= BiQuadFilter<float>::MakeAnalogBell(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
		}
	}

	/// @brief Creates an `EQBand` with the given parameters
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param q - The Q to use
	/// @param gainDB - The gain to use, in Decibels
	/// @param sampleRate - The sample rate to use, in Hertz
	/// @param type - The type of band
	EQBand<double>::EQBand(double frequency,
						   double q,
						   double gainDB,
						   size_t sampleRate,
						   BandType type) noexcept
		: mType(type), mFrequency(frequency), mQ(q), mGain(gainDB), mSampleRate(sampleRate),
		  mFilter(BiQuadFilter<double>::MakeAllpass()) {
		if(mType < BandType::Allpass) {
			switch(static_cast<size_t>(mType) % 4) {
				case 0: mOrder = 1; break;
				case 1: mOrder = 2; break;
				case 2: mOrder = 4; break;
				case 3: mOrder = 8; break;
			}
			mGainProcessor = Gain<double>(mGain, true);
		}

		mFilters.resize(mOrder);
		createFilters();
	}

	/// @brief Sets the frequency of this `EQBand` to the given value
	///
	/// @param frequency - The new frequency, in Hertz
	auto EQBand<double>::setFrequency(double frequency) noexcept -> void {
		mFrequency = frequency;
		mFilter.setFrequency(mFrequency);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setFrequency(mFrequency);
			}
		}
	}

	/// @brief Returns the frequency of this `EQBand`
	///
	/// @return - The current frequency, in Hertz
	auto EQBand<double>::getFrequency() const noexcept -> double {
		return mFrequency;
	}

	/// @brief Sets the Q of this `EQBand` to the given value
	///
	/// @param q - The new Q
	auto EQBand<double>::setQ(double q) noexcept -> void {
		mQ = q;
		mFilter.setQ(mQ);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setQ(mQ);
			}
		}
	}

	/// @brief Returns the Q of this `EQBand`
	///
	/// @return - The current Q
	auto EQBand<double>::getQ() const noexcept -> double {
		return mQ;
	}

	/// @brief Sets the gain of this `EQBand`
	///
	/// @param gainDB - The new gain, in Decibels
	auto EQBand<double>::setGainDB(double gainDB) noexcept -> void {
		mGain = gainDB;
		mGainProcessor.setGainDecibels(mGain);
		mFilter.setGainDB(mGain);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setGainDB(mGain);
			}
		}
	}

	/// @brief Returns the gain of this `EQBand`
	///
	/// @return - The current gain, in Decibels
	auto EQBand<double>::getGainDB() const noexcept -> double {
		return mGain;
	}

	/// @brief Sets the sample rate of this `EQBand` to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	auto EQBand<double>::setSampleRate(size_t sampleRate) noexcept -> void {
		mSampleRate = sampleRate;
		mFilter.setSampleRate(mSampleRate);
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.setSampleRate(mSampleRate);
			}
		}
	}

	/// @brief Returns the sample rate of this `EQBand`
	///
	/// @return - The current sample rate, in Hertz
	auto EQBand<double>::getSampleRate() const noexcept -> size_t {
		return mSampleRate;
	}

	/// @brief Sets the type of this `EQBand` to the given value
	///
	/// @param type - The new type
	void EQBand<double>::setBandType(BandType type) noexcept {
		mType = type;
		if(mType < BandType::Allpass) {
			switch(static_cast<size_t>(mType) % 4) {
				case 0: mOrder = 1; break;
				case 1: mOrder = 2; break;
				case 2: mOrder = 4; break;
				case 3: mOrder = 8; break;
			}
			mFilters.resize(mOrder);
		}
		createFilters();
	}

	/// @brief Returns the type of this `EQBand`
	///
	/// @return - The current type
	auto EQBand<double>::getBandType() const noexcept -> EQBand<double>::BandType {
		return mType;
	}

	/// @brief Applies this `EQBand` to the given input value
	///
	/// @param input - The input to apply EQ to
	///
	/// @return - The processed value
	auto EQBand<double>::process(double input) noexcept -> double {
		double x = 0.0;
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				x = filt.process(x);
			}
			x = mGainProcessor.process(x);
		}
		else {
			x = mFilter.process(input);
			if(mType == BandType::Allpass || mType == BandType::Notch) {
				x = mGainProcessor.process(x);
			}
		}
		return x;
	}

	/// @brief Applies this `EQBand` to the given array of input values, in place
	///
	/// @param input - The input values to apply EQ to
	auto EQBand<double>::process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void {
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.process(input);
			}
			mGainProcessor.process(input);
		}
		else {
			mFilter.process(input);
		}
	}

	/// @brief Resets this `EQBand` to an initial state
	auto EQBand<double>::reset() noexcept -> void {
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.reset();
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
	inline auto
	EQBand<double>::getMagnitudeForFrequency(double frequency) const noexcept -> double {
		double x = 1.0;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x *= filt.getMagnitudeForFrequency(frequency);
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
	inline auto
	EQBand<double>::getDecibelMagnitudeForFrequency(double frequency) const noexcept -> double {
		return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
	}

	/// @brief Calculates the linear magnitude response of this filter for the given array of
	/// frequencies and stores them in `magnitudes`
	///
	/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
	/// @param magnitudes - The array to store the magnitudes in
	inline auto EQBand<double>::getMagnitudesForFrequencies(
		gsl::span<double, gsl::dynamic_extent> frequencies,
		gsl::span<double, gsl::dynamic_extent> magnitudes) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(magnitudes, frequency)
				= getMagnitudeForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Calculates the decibel magnitude response of this filter for the given array of
	/// frequencies and stores them in `magnitudes`
	///
	/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
	/// @param magnitudes - The array to store the magnitudes in
	inline auto EQBand<double>::getDecibelMagnitudesForFrequencies(
		gsl::span<double, gsl::dynamic_extent> frequencies,
		gsl::span<double, gsl::dynamic_extent> magnitudes) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(magnitudes, frequency)
				= getDecibelMagnitudeForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Calculates the phase response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the phase response for, in Hertz
	///
	/// @return - The phase response, in radians, at the given frequency
	inline auto EQBand<double>::getPhaseForFrequency(double frequency) const noexcept -> double {
		double x = 0.0;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x += filt.getPhaseForFrequency(frequency);
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
	inline auto
	EQBand<double>::getDegreesPhaseForFrequency(double frequency) const noexcept -> double {
		return getPhaseForFrequency(frequency) * 180.0 / math::pi;
	}

	/// @brief Calculates the phase response of this filter for the given array of frequencies and
	/// stores it in `phases`
	///
	/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
	/// @param phases - The array to store the phases (in radians) in
	inline auto EQBand<double>::getPhasesForFrequencies(
		gsl::span<double, gsl::dynamic_extent> frequencies,
		gsl::span<double, gsl::dynamic_extent> phases) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(phases, frequency) = getPhaseForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Calculates the phase response of this filter for the given array of frequencies and
	/// stores it in `phases`
	///
	/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
	/// @param phases - The array to store the phases (in degrees) in
	inline auto EQBand<double>::getDegreesPhasesForFrequencies(
		gsl::span<double, gsl::dynamic_extent> frequencies,
		gsl::span<double, gsl::dynamic_extent> phases) const noexcept -> void {
		auto size = static_cast<gsl::index>(frequencies.size());
		for(gsl::index frequency = 0; frequency < size; ++frequency) {
			gsl::at(phases, frequency)
				= getDegreesPhaseForFrequency(gsl::at(frequencies, frequency));
		}
	}

	/// @brief Returns the shifted frequency for the Nth filter stage in
	/// a multi-order filter
	///
	/// @param filterIndex - The filter stage to calculate the shift for
	///
	/// @return - The shifted frequency
	auto EQBand<double>::frequencyShift(size_t filterIndex) const noexcept -> double {
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
	auto EQBand<double>::createFilters() noexcept -> void {

		switch(mType) {
			case BandType::Lowpass12DB:
				{
					mFilter = BiQuadFilter<double>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeLowpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Lowpass24DB:
				{
					mFilter = BiQuadFilter<double>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeLowpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Lowpass48DB:
				{
					mFilter = BiQuadFilter<double>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeLowpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Lowpass96DB:
				{
					mFilter = BiQuadFilter<double>::MakeLowpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeLowpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
					}
				}
				break;
			case BandType::Highpass12DB:
				{
					mFilter = BiQuadFilter<double>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeHighpass(frequencyShift(ord),
																		   mQ,
																		   mSampleRate);
					}
				}
				break;
			case BandType::Highpass24DB:
				{
					mFilter = BiQuadFilter<double>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeHighpass(frequencyShift(ord),
																		   mQ,
																		   mSampleRate);
					}
				}
				break;
			case BandType::Highpass48DB:
				{
					mFilter = BiQuadFilter<double>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeHighpass(frequencyShift(ord),
																		   mQ,
																		   mSampleRate);
					}
				}
				break;
			case BandType::Highpass96DB:
				{
					mFilter = BiQuadFilter<double>::MakeHighpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord] = BiQuadFilter<double>::MakeHighpass(frequencyShift(ord),
																		   mQ,
																		   mSampleRate);
					}
				}
				break;
			case BandType::Bandpass12DB:
				{
					mFilter = BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Bandpass24DB:
				{
					mFilter = BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Bandpass48DB:
				{
					mFilter = BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Bandpass96DB:
				{
					mFilter = BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					for(size_t ord = 0; ord < mOrder; ++ord) {
						mFilters[ord]
							= BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
					}
				}
				break;
			case BandType::Allpass:
				{
					mFilter = BiQuadFilter<double>::MakeAllpass(mFrequency, mQ, mSampleRate);
				}
				break;
			case BandType::Notch:
				{
					mFilter = BiQuadFilter<double>::MakeNotch(mFrequency, mQ, mSampleRate);
				}
				break;
			case BandType::LowShelf:
				{
					mFilter
						= BiQuadFilter<double>::MakeLowShelf(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
			case BandType::HighShelf:
				{
					mFilter
						= BiQuadFilter<double>::MakeHighShelf(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
			case BandType::Bell:
				{
					mFilter = BiQuadFilter<double>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
			case BandType::AnalogBell:
				{
					mFilter
						= BiQuadFilter<double>::MakeAnalogBell(mFrequency, mQ, mGain, mSampleRate);
				}
				break;
		}
	}
} // namespace apex::dsp
