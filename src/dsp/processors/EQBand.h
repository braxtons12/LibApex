#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "../../base/StandardIncludes.h"
#include "../filters/BiQuadFilter.h"
#include "Gain.h"
#include "Processor.h"

namespace apex::dsp {
	/// @brief Basic Equalizer Band
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class EQBand : public Processor<T> {
	  public:
		static_assert(std::is_floating_point<T>::value,
					  "T must be a floating point type (float or double)");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
	};

	/// @brief Basic Equalizer Band
	template<>
	class EQBand<float> : public Processor<float> {
	  public:
		/// @brief The different possible band types
		enum class BandType
		{
			Lowpass12DB = 0,
			Lowpass24DB = 1,
			Lowpass48DB = 2,
			Lowpass96DB = 3,
			Highpass12DB = 4,
			Highpass24DB = 5,
			Highpass48DB = 6,
			Highpass96DB = 7,
			Bandpass12DB = 8,
			Bandpass24DB = 9,
			Bandpass48DB = 10,
			Bandpass96DB = 11,
			Allpass = 12,
			Notch = 13,
			LowShelf = 14,
			HighShelf = 15,
			Bell = 16,
			AnalogBell = 17
		};

		/// @brief Creates a default `EQBand`
		EQBand() noexcept = default;

		/// @brief Creates an `EQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The type of band
		EQBand(Hertz frequency, float q, Decibels gainDB, Hertz sampleRate, BandType type) noexcept;

		/// @brief Move constructs an `EQBand` from the given one
		///
		/// @param band - The `EQBand` to move
		EQBand(EQBand<float>&& band) noexcept = default;
		~EQBand() noexcept override = default;

		/// @brief Sets the frequency of this `EQBand` to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline auto setFrequency(Hertz frequency) noexcept -> void {
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
		[[nodiscard]] inline auto getFrequency() const noexcept -> Hertz {
			return mFrequency;
		}

		/// @brief Sets the Q of this `EQBand` to the given value
		///
		/// @param q - The new Q
		inline auto setQ(float q) noexcept -> void {
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
		[[nodiscard]] inline auto getQ() const noexcept -> float {
			return mQ;
		}

		/// @brief Sets the gain of this `EQBand`
		///
		/// @param gainDB - The new gain, in Decibels
		virtual inline auto setGainDB(Decibels gainDB) noexcept -> void {
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
		[[nodiscard]] virtual inline auto getGainDB() const noexcept -> Decibels {
			return mGain;
		}

		/// @brief Sets the sample rate of this `EQBand` to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
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
		[[nodiscard]] inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		/// @brief Sets the type of this `EQBand` to the given value
		///
		/// @param type - The new type
		inline auto setBandType(BandType type) noexcept -> void {
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
		[[nodiscard]] inline auto getBandType() const noexcept -> BandType {
			return mType;
		}

		/// @brief Applies this `EQBand` to the given input value
		///
		/// @param input - The input to apply EQ to
		///
		/// @return - The processed value
		[[nodiscard]] auto process(float input) noexcept -> float override;

		/// @brief Applies this `EQBand` to the given array of input values, in place
		///
		/// @param input - The input values to apply EQ to
		auto process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void override;

		/// @brief Resets this `EQBand` to an initial state
		auto reset() noexcept -> void override;

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] virtual inline auto
		getMagnitudeForFrequency(Hertz frequency) const noexcept -> float {
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
		[[nodiscard]] virtual inline auto
		getDecibelMagnitudeForFrequency(Hertz frequency) const noexcept -> Decibels {
			return Decibels::fromLinear(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		virtual inline auto
		getMagnitudesForFrequencies(gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
									gsl::span<float, gsl::dynamic_extent> magnitudes) const noexcept
			-> void {
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
		virtual inline auto getDecibelMagnitudesForFrequencies(
			gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
			gsl::span<Decibels, gsl::dynamic_extent> magnitudes) const noexcept -> void {
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
		[[nodiscard]] virtual inline auto
		getPhaseForFrequency(Hertz frequency) const noexcept -> Radians {
			Radians x = 0.0_rad;
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
		[[nodiscard]] virtual inline auto
		getDegreesPhaseForFrequency(Hertz frequency) const noexcept -> float {
			return gsl::narrow_cast<float>(getPhaseForFrequency(frequency)) * 180.0F / math::pif;
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		virtual inline auto
		getPhasesForFrequencies(gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
								gsl::span<Radians, gsl::dynamic_extent> phases) const noexcept
			-> void {
			auto size = static_cast<gsl::index>(frequencies.size());
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(phases, frequency) = getPhaseForFrequency(gsl::at(frequencies, frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		virtual inline auto
		getDegreesPhasesForFrequencies(gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
									   gsl::span<float, gsl::dynamic_extent> phases) const noexcept
			-> void {
			auto size = static_cast<gsl::index>(frequencies.size());
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(phases, frequency)
					= getDegreesPhaseForFrequency(gsl::at(frequencies, frequency));
			}
		}

		auto operator=(EQBand<float>&& band) noexcept -> EQBand<float>& = default;

	  protected:
		BandType mType = BandType::Bell;
		Hertz mFrequency = 1000.0_Hz;
		float mQ = 1.0F;
		Decibels mGain = 0.0_dB;
		/// Only used for "____pass" type filters
		Gain<float> mGainProcessor = Gain<float>(mGain);
		Hertz mSampleRate = 44100_Hz;
		size_t mOrder = 1;
		BiQuadFilter<float> mFilter
			= BiQuadFilter<float>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
		std::vector<BiQuadFilter<float>> mFilters = std::vector<BiQuadFilter<float>>();

		/// @brief Returns the shifted frequency for the Nth filter stage in
		/// a multi-order filter
		///
		/// @param filterIndex - The filter stage to calculate the shift for
		///
		/// @return - The shifted frequency
		[[nodiscard]] inline auto frequencyShift(size_t filterIndex) const noexcept -> Hertz {
			float shiftMultiplier = 0.25F * filterIndex;
			if(mType < BandType::Highpass12DB) {
				Hertz nextOctFreq = mFrequency * 2.0F;
				return mFrequency + (shiftMultiplier * (nextOctFreq - mFrequency));
			}
			else {
				Hertz nextOctFreq = mFrequency / 2.0F;
				return mFrequency - (shiftMultiplier * (nextOctFreq - mFrequency));
			}
		}

		/// @brief Creates the necessary filter(s) for this `EQBand`
		virtual auto createFilters() noexcept -> void;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
	};

	/// @brief Basic Equalizer Band
	template<>
	class EQBand<double> : public Processor<double> {
	  public:
		/// @brief The different possible band types
		enum class BandType
		{
			Lowpass12DB = 0,
			Lowpass24DB = 1,
			Lowpass48DB = 2,
			Lowpass96DB = 3,
			Highpass12DB = 4,
			Highpass24DB = 5,
			Highpass48DB = 6,
			Highpass96DB = 7,
			Bandpass12DB = 8,
			Bandpass24DB = 9,
			Bandpass48DB = 10,
			Bandpass96DB = 11,
			Allpass = 12,
			Notch = 13,
			LowShelf = 14,
			HighShelf = 15,
			Bell = 16,
			AnalogBell = 17
		};

		/// @brief Creates a default `EQBand`
		EQBand() noexcept = default;

		/// @brief Creates an `EQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The type of band
		EQBand(Hertz frequency,
			   double q,
			   Decibels gainDB,
			   Hertz sampleRate,
			   BandType type) noexcept;

		/// @brief Move constructs an `EQBand` from the given one
		///
		/// @param band - The `EQBand` to move
		EQBand(EQBand<double>&& band) noexcept = default;
		~EQBand() noexcept override = default;

		/// @brief Sets the frequency of this `EQBand` to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline auto setFrequency(Hertz frequency) noexcept -> void {
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
		[[nodiscard]] inline auto getFrequency() const noexcept -> Hertz {
			return mFrequency;
		}

		/// @brief Sets the Q of this `EQBand` to the given value
		///
		/// @param q - The new Q
		inline auto setQ(double q) noexcept -> void {
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
		[[nodiscard]] inline auto getQ() const noexcept -> double {
			return mQ;
		}

		/// @brief Sets the gain of this `EQBand`
		///
		/// @param gainDB - The new gain, in Decibels
		virtual inline auto setGainDB(Decibels gainDB) noexcept -> void {
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
		[[nodiscard]] virtual inline auto getGainDB() const noexcept -> Decibels {
			return mGain;
		}

		/// @brief Sets the sample rate of this `EQBand` to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
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
		[[nodiscard]] inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		/// @brief Sets the type of this `EQBand` to the given value
		///
		/// @param type - The new type
		inline auto setBandType(BandType type) noexcept -> void {
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
		[[nodiscard]] inline auto getBandType() const noexcept -> BandType {
			return mType;
		}

		/// @brief Applies this `EQBand` to the given input value
		///
		/// @param input - The input to apply EQ to
		///
		/// @return - The processed value
		[[nodiscard]] auto process(double input) noexcept -> double override;

		/// @brief Applies this `EQBand` to the given array of input values, in place
		///
		/// @param input - The input values to apply EQ to
		auto process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void override;

		/// @brief Resets this `EQBand` to an initial state
		auto reset() noexcept -> void override;

		/// @brief Calculates the linear magnitude response of this filter for the given
		/// frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] virtual inline auto
		getMagnitudeForFrequency(Hertz frequency) const noexcept -> double {
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

		/// @brief Calculates the decibel magnitude response of this filter for the given
		/// frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] virtual inline auto
		getDecibelMagnitudeForFrequency(Hertz frequency) const noexcept -> Decibels {
			return math::Decibels::linearToDecibels(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array
		/// of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in
		/// Hertz
		/// @param magnitudes - The array to store the magnitudes in
		virtual inline auto getMagnitudesForFrequencies(
			gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
			gsl::span<double, gsl::dynamic_extent> magnitudes) const noexcept -> void {
			auto size = static_cast<gsl::index>(frequencies.size());
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(magnitudes, frequency)
					= getMagnitudeForFrequency(gsl::at(frequencies, frequency));
			}
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given array
		/// of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in
		/// Hertz
		/// @param magnitudes - The array to store the magnitudes in
		virtual inline auto getDecibelMagnitudesForFrequencies(
			gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
			gsl::span<Decibels, gsl::dynamic_extent> magnitudes) const noexcept -> void {
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
		[[nodiscard]] virtual inline auto
		getPhaseForFrequency(Hertz frequency) const noexcept -> Radians {
			Radians x = 0.0_rad;
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
		[[nodiscard]] virtual inline auto
		getDegreesPhaseForFrequency(Hertz frequency) const noexcept -> double {
			return static_cast<double>(getPhaseForFrequency(frequency)) * 180.0 / math::pi;
		}

		/// @brief Calculates the phase response of this filter for the given array of
		/// frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		virtual inline auto
		getPhasesForFrequencies(gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
								gsl::span<Radians, gsl::dynamic_extent> phases) const noexcept
			-> void {
			auto size = static_cast<gsl::index>(frequencies.size());
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(phases, frequency) = getPhaseForFrequency(gsl::at(frequencies, frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given array of
		/// frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		virtual inline auto
		getDegreesPhasesForFrequencies(gsl::span<const Hertz, gsl::dynamic_extent> frequencies,
									   gsl::span<double, gsl::dynamic_extent> phases) const noexcept
			-> void {
			auto size = static_cast<gsl::index>(frequencies.size());
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(phases, frequency)
					= getDegreesPhaseForFrequency(gsl::at(frequencies, frequency));
			}
		}

		auto operator=(EQBand<double>&& band) noexcept -> EQBand<double>& = default;

	  protected:
		BandType mType = BandType::Bell;
		Hertz mFrequency = 1000.0_Hz;
		double mQ = 1.0;
		Decibels mGain = 0.0_dB;
		/// Only used for "____pass" type filters
		Gain<double> mGainProcessor = Gain<double>(mGain);
		Hertz mSampleRate = 44100_Hz;
		size_t mOrder = 1;
		BiQuadFilter<double> mFilter
			= BiQuadFilter<double>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
		std::vector<BiQuadFilter<double>> mFilters = std::vector<BiQuadFilter<double>>();

		/// @brief Returns the shifted frequency for the Nth filter stage in
		/// a multi-order filter
		///
		/// @param filterIndex - The filter stage to calculate the shift for
		///
		/// @return - The shifted frequency
		[[nodiscard]] inline auto frequencyShift(size_t filterIndex) const noexcept -> Hertz {
			double shiftMultiplier = 0.25 * filterIndex;
			if(mType < BandType::Highpass12DB) {
				Hertz nextOctFreq = mFrequency * 2.0;
				return mFrequency + (shiftMultiplier * (nextOctFreq - mFrequency));
			}
			else {
				Hertz nextOctFreq = mFrequency / 2.0;
				return mFrequency - (shiftMultiplier * (nextOctFreq - mFrequency));
			}
		}

		/// @brief Creates the necessary filter(s) for this `EQBand`
		virtual auto createFilters() noexcept -> void;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
	};
} // namespace apex::dsp
