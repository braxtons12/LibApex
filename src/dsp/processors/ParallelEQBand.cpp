#include "ParallelEQBand.h"

namespace apex::dsp {
	/// @brief Creates a default `ParallelEQBand`
	ParallelEQBand<float>::ParallelEQBand() noexcept {
		setGainDB(mGain);
	}

	/// @brief Creates a `ParallelEQBand` with the given parameters
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param q - The q to use
	/// @param gainDB - The gain to use, in Decibels
	/// @param sampleRate - The sample rate to use, in Hertz
	/// @param type - The band type to use
	ParallelEQBand<float>::ParallelEQBand(float frequency,
										  float q,
										  float gainDB,
										  size_t sampleRate,
										  BandType type) noexcept
		: EQBand<float>(frequency, q, gainDB, sampleRate, type) {
		setGainDB(mGain);
	}

	/// @brief Sets the gain of this `ParallelEQBand` to the given value
	///
	/// @param gainDB - The new gain, in Decibels
	auto ParallelEQBand<float>::setGainDB(float gainDB) noexcept -> void {
		if(mType < BandType::Allpass) {
			mGain = gainDB;
			mGainActual = mGain;
			mGainProcessor.setGainDecibels(mGain);
			mFilter.setGainDB(mGain);
			for(auto& filt : mFilters) {
				filt.setGainDB(mGain);
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
			// if gain is negative, we need to subtract that amount of linear gain from the input
			// to get the desired gain:
			// linear 0db (eg 1.0) - linear gain level = amount of linear signal to be removed
			if(gainDB < 0.0F) {
				mGain = math::Decibels::linearToDecibels(
					1.0F - math::Decibels::decibelsToLinear(gainDB));
			}
			// otherwise, the gain is the amount of linear gain we need to add to the input:
			// linear gain level - linear 0db (eg 1.0) = amount of linear signal to be added
			else {
				mGain = math::Decibels::linearToDecibels(math::Decibels::decibelsToLinear(gainDB)
														 - 1.0F);
			}

			mGainProcessor.setGainDecibels(mGain);
			mFilter.setGainDB(mGain);
		}
	}

	/// @brief Applies this `ParallelEQBand` to the given input value
	///
	/// @param input - The input value to apply EQ to
	///
	/// @return - The processed value
	auto ParallelEQBand<float>::process(float input) noexcept -> float {
		float x = 0.0F;
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				x = filt.process(x);
			}
			x = mGainProcessor.process(x);
		}
		else if(mType < BandType::LowShelf) {
			x = mFilter.process(input);
			x = mGainProcessor.process(x);
		}
		else {
			x = mFilter.process(input);
			x = mGainProcessor.process(x);
			if(mGainActual < 0.0F) {
				x = input - x;
			}
			else {
				x = input + x;
			}
		}
		return x;
	}

	/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
	///
	/// @param input - The array of input values to apply EQ to
	auto
	ParallelEQBand<float>::process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void {
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.process(input);
			}
			mGainProcessor.process(input);
		}
		else if(mType < BandType::LowShelf) {
			mFilter.process(input);
			mGainProcessor.process(input);
		}
		else {
			gsl::owner<float*> x = new float[input.size()];
			std::memcpy(x, input.data(), input.size_bytes());
			gsl::span xspan = gsl::span(x, input.size());
			mFilter.process(xspan);
			mGainProcessor.process(xspan);
			auto size = static_cast<gsl::index>(input.size());
			if(mGainActual < 0.0F) {
				for(gsl::index sample = 0; sample < size; ++sample) {
					gsl::at(input, sample) -= gsl::at(xspan, sample);
				}
			}
			else {
				for(gsl::index sample = 0; sample < size; ++sample) {
					gsl::at(input, sample) += gsl::at(xspan, sample);
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
	auto ParallelEQBand<float>::getMagnitudeForFrequency(float frequency) const noexcept -> float {
		float x = 1.0F;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x *= filt.getMagnitudeForFrequency(frequency);
			}
		}
		else if(mType < BandType::LowShelf) {
			x = mFilter.getMagnitudeForFrequency(frequency);
		}
		else if(mType == BandType::LowShelf) {
			BiQuadFilter<float> filt
				= BiQuadFilter<float>::MakeLowShelf(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		else if(mType == BandType::HighShelf) {
			BiQuadFilter<float> filt
				= BiQuadFilter<float>::MakeHighShelf(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		else if(mType == BandType::Bell) {
			BiQuadFilter<float> filt
				= BiQuadFilter<float>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		else {
			BiQuadFilter<float> filt
				= BiQuadFilter<float>::MakeAnalogBell(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		return x;
	}

	/// @brief Calculates the phase response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the phase response for, in Hertz
	///
	/// @return - The phase response, in radians, at the given frequency
	inline auto
	ParallelEQBand<float>::getPhaseForFrequency(float frequency) const noexcept -> float {
		float x = 0.0F;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x += filt.getPhaseForFrequency(frequency);
			}
		}
		else if(mType < BandType::LowShelf) {
			x = mFilter.getPhaseForFrequency(frequency);
		}
		else {
			/// TODO: This might not be the accurate way to do this,
			/// but for now it's what we have

			// Parallel bands should have phase-shift as the average between
			// the shift caused by the filter and the original, right?
			x = mFilter.getPhaseForFrequency(frequency) / 2.0F;
		}
		return x;
	}

	/// @brief Creates necessary filter(s) for this EQ band
	auto ParallelEQBand<float>::createFilters() noexcept -> void {
		EQBand<float>::createFilters();
		if(mType == BandType::LowShelf) {
			mFilter = BiQuadFilter<float>::MakeLowpass(mFrequency, mQ, mSampleRate);
		}
		else if(mType == BandType::HighShelf) {
			mFilter = BiQuadFilter<float>::MakeHighpass(mFrequency, mQ, mSampleRate);
		}
		else if(mType == BandType::Bell) {
			mFilter = BiQuadFilter<float>::MakeBandpass(mFrequency, mQ, mSampleRate);
		}
		else if(mType == BandType::AnalogBell) {
			mFilter = BiQuadFilter<float>::MakeBandpass(mFrequency,
														mQ * math::pow10f(mGainActual / 40.0F),
														mSampleRate);
		}
	}

	/// @brief Creates a default `ParallelEQBand`
	ParallelEQBand<double>::ParallelEQBand() noexcept {
		setGainDB(mGain);
	}

	/// @brief Creates a `ParallelEQBand` with the given parameters
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param q - The q to use
	/// @param gainDB - The gain to use, in Decibels
	/// @param sampleRate - The sample rate to use, in Hertz
	/// @param type - The band type to use
	ParallelEQBand<double>::ParallelEQBand(double frequency,
										   double q,
										   double gainDB,
										   size_t sampleRate,
										   BandType type) noexcept
		: EQBand<double>(frequency, q, gainDB, sampleRate, type) {
		setGainDB(mGain);
	}

	/// @brief Sets the gain of this `ParallelEQBand` to the given value
	///
	/// @param gainDB - The new gain, in Decibels
	auto ParallelEQBand<double>::setGainDB(double gainDB) noexcept -> void {
		if(mType < BandType::Allpass) {
			mGain = gainDB;
			mGainActual = mGain;
			mGainProcessor.setGainDecibels(mGain);
			mFilter.setGainDB(mGain);
			for(auto& filt : mFilters) {
				filt.setGainDB(mGain);
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
			// if gain is negative, we need to subtract that amount of linear gain from the input
			// to get the desired gain:
			// linear 0db (eg 1.0) - linear gain level = amount of linear signal to be removed
			if(gainDB < 0.0) {
				mGain = math::Decibels::linearToDecibels(
					1.0 - math::Decibels::decibelsToLinear(gainDB));
			}
			// otherwise, the gain is the amount of linear gain we need to add to the input:
			// linear gain level - linear 0db (eg 1.0) = amount of linear signal to be added
			else {
				mGain = math::Decibels::linearToDecibels(math::Decibels::decibelsToLinear(gainDB)
														 - 1.0);
			}

			mGainProcessor.setGainDecibels(mGain);
			mFilter.setGainDB(mGain);
		}
	}

	/// @brief Applies this `ParallelEQBand` to the given input value
	///
	/// @param input - The input value to apply EQ to
	///
	/// @return - The processed value
	auto ParallelEQBand<double>::process(double input) noexcept -> double {
		double x = 0.0;
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				x = filt.process(x);
			}
			x = mGainProcessor.process(x);
		}
		else if(mType < BandType::LowShelf) {
			x = mFilter.process(input);
			x = mGainProcessor.process(x);
		}
		else {
			x = mFilter.process(input);
			x = mGainProcessor.process(x);
			if(mGainActual < 0.0) {
				x = input - x;
			}
			else {
				x = input + x;
			}
		}
		return x;
	}

	/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
	///
	/// @param input - The array of input values to apply EQ to
	auto
	ParallelEQBand<double>::process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void {
		if(mType < BandType::Allpass) {
			for(auto& filt : mFilters) {
				filt.process(input);
			}
			mGainProcessor.process(input);
		}
		else if(mType < BandType::LowShelf) {
			mFilter.process(input);
			mGainProcessor.process(input);
		}
		else {
			gsl::owner<double*> x = new double[input.size()];
			std::memcpy(x, input.data(), input.size_bytes());
			gsl::span xspan = gsl::span(x, input.size());
			mFilter.process(xspan);
			mGainProcessor.process(xspan);
			auto size = static_cast<gsl::index>(input.size());
			if(mGainActual < 0.0) {
				for(gsl::index sample = 0; sample < size; ++sample) {
					gsl::at(input, sample) -= gsl::at(xspan, sample);
				}
			}
			else {
				for(gsl::index sample = 0; sample < size; ++sample) {
					gsl::at(input, sample) += gsl::at(xspan, sample);
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
	inline auto
	ParallelEQBand<double>::getMagnitudeForFrequency(double frequency) const noexcept -> double {
		double x = 1.0;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x *= filt.getMagnitudeForFrequency(frequency);
			}
		}
		else if(mType < BandType::LowShelf) {
			x = mFilter.getMagnitudeForFrequency(frequency);
		}
		else if(mType == BandType::LowShelf) {
			BiQuadFilter<double> filt
				= BiQuadFilter<double>::MakeLowShelf(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		else if(mType == BandType::HighShelf) {
			BiQuadFilter<double> filt
				= BiQuadFilter<double>::MakeHighShelf(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		else if(mType == BandType::Bell) {
			BiQuadFilter<double> filt
				= BiQuadFilter<double>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		else {
			BiQuadFilter<double> filt
				= BiQuadFilter<double>::MakeAnalogBell(mFrequency, mQ, mGain, mSampleRate);
			x = filt.getMagnitudeForFrequency(frequency);
		}
		return x;
	}

	/// @brief Calculates the phase response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the phase response for, in Hertz
	///
	/// @return - The phase response, in radians, at the given frequency
	inline auto
	ParallelEQBand<double>::getPhaseForFrequency(double frequency) const noexcept -> double {
		double x = 0.0;
		if(mType < BandType::Allpass) {
			for(const auto& filt : mFilters) {
				x += filt.getPhaseForFrequency(frequency);
			}
		}
		else if(mType < BandType::LowShelf) {
			x = mFilter.getPhaseForFrequency(frequency);
		}
		else {
			/// TODO: This might not be the accurate way to do this,
			/// but for now it's what we have

			// Parallel bands should have phase-shift as the average between
			// the shift caused by the filter and the original, right?
			x = mFilter.getPhaseForFrequency(frequency) / 2.0;
		}
		return x;
	}

	/// @brief Creates necessary filter(s) for this EQ band
	auto ParallelEQBand<double>::createFilters() noexcept -> void {
		EQBand<double>::createFilters();
		if(mType == BandType::LowShelf) {
			mFilter = BiQuadFilter<double>::MakeLowpass(mFrequency, mQ, mSampleRate);
		}
		else if(mType == BandType::HighShelf) {
			mFilter = BiQuadFilter<double>::MakeHighpass(mFrequency, mQ, mSampleRate);
		}
		else if(mType == BandType::Bell) {
			mFilter = BiQuadFilter<double>::MakeBandpass(mFrequency, mQ, mSampleRate);
		}
		else if(mType == BandType::AnalogBell) {
			mFilter = BiQuadFilter<double>::MakeBandpass(mFrequency,
														 mQ * math::pow10(mGainActual / 40.0),
														 mSampleRate);
		}
	}
} // namespace apex::dsp
