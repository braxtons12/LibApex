#include "EQBand.h"

namespace apex::dsp {
	/// @brief Creates an `EQBand` with the given parameters
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param q - The Q to use
	/// @param gainDB - The gain to use, in Decibels
	/// @param sampleRate - The sample rate to use, in Hertz
	/// @param type - The type of band
	EQBand<float>::EQBand(Hertz frequency,
						  float q,
						  Decibels gainDB,
						  Hertz sampleRate,
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
			mGainProcessor = Gain<float>(mGain);
		}

		mFilters.resize(mOrder);
		createFilters();
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
	auto EQBand<float>::process(Span<float> input) noexcept -> void {
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
	EQBand<double>::EQBand(Hertz frequency,
						   double q,
						   Decibels gainDB,
						   Hertz sampleRate,
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
			mGainProcessor = Gain<double>(mGain);
		}

		mFilters.resize(mOrder);
		createFilters();
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
	auto EQBand<double>::process(Span<double> input) noexcept -> void {
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
