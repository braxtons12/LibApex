#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "../../base/StandardIncludes.h"
#include "../filters/BiQuadFilter.h"
#include "Gain.h"
#include "Processor.h"

namespace apex::dsp {
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

	/// @brief Basic Equalizer Band
	///
	/// @tparam T - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class EQBand : public Processor<FloatType> {
	  private:
		using Processor = Processor<FloatType>;
		using BiQuadFilter = BiQuadFilter<FloatType>;

	  public:
		/// @brief Creates a default `EQBand`
		EQBand() noexcept {
			for(auto& channel : mFilter) {
				channel = BiQuadFilter::MakeBell(mFrequency, mQ, mGain, mSampleRate);
			}
		}

		/// @brief Creates an `EQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The type of band
		EQBand(Hertz frequency,
			   FloatType q,
			   Decibels gainDB,
			   Hertz sampleRate,
			   BandType type) noexcept
			: mType(type), mFrequency(frequency), mQ(q), mGain(gainDB), mSampleRate(sampleRate) {
			if(mType < BandType::Allpass) {
				switch(static_cast<size_t>(mType) % 4) {
					case 0: mOrder = 1; break;
					case 1: mOrder = 2; break;
					case 2: mOrder = 4; break;
					case 3: mOrder = 8; break;
				}
				mGainProcessor = Gain<FloatType>(mGain);
			}
			for(auto& channel : mFilters) {
				channel.resize(mOrder);
			}
			createFilters();
		}

		/// @brief Move constructs an `EQBand` from the given one
		///
		/// @param band - The `EQBand` to move
		EQBand(EQBand&& band) noexcept = default;
		~EQBand() noexcept override = default;

		/// @brief Sets the frequency of this `EQBand` to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline auto setFrequency(Hertz frequency) noexcept -> void {
			mFrequency = frequency;
			for(auto& channel : mFilter) {
				channel.setFrequency(mFrequency);
			}
			if(mType < BandType::Allpass) {
				for(auto& channel : mFilters) {
					for(auto& filter : channel) {
						filter.setFrequency(mFrequency);
					}
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
		inline auto setQ(FloatType q) noexcept -> void {
			jassert(q > narrow_cast<FloatType>(0.0));
			mQ = q;
			for(auto& channel : mFilter) {
				channel.setQ(mQ);
			}
			if(mType < BandType::Allpass) {
				for(auto& channel : mFilters) {
					for(auto& filter : channel) {
						filter.setQ(mQ);
					}
				}
			}
		}

		/// @brief Returns the Q of this `EQBand`
		///
		/// @return - The current Q
		[[nodiscard]] inline auto getQ() const noexcept -> FloatType {
			return mQ;
		}

		/// @brief Sets the gain of this `EQBand`
		///
		/// @param gainDB - The new gain, in Decibels
		virtual inline auto setGainDB(Decibels gainDB) noexcept -> void {
			mGain = gainDB;
			for(auto& channel : mFilter) {
				channel.setGainDB(mGain);
			}
			if(mType < BandType::Allpass) {
				for(auto& channel : mFilters) {
					for(auto& filter : channel) {
						filter.setGainDB(mGain);
					}
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
			for(auto& channel : mFilter) {
				channel.setSampleRate(mSampleRate);
			}
			if(mType < BandType::Allpass) {
				for(auto& channel : mFilters) {
					for(auto& filter : channel) {
						filter.setSampleRate(mSampleRate);
					}
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
				for(auto& channel : mFilters) {
					channel.resize(mOrder);
				}
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
		[[nodiscard]] inline auto processMono(FloatType input) noexcept -> FloatType override {
			auto x = input;
			if(mType < BandType::Allpass) {
				for(auto& filter : mFilters.at(Processor::MONO)) {
					x = filter.process(x);
				}
				x = mGainProcessor.process(x);
			}
			else {
				x = mFilter.at(Processor::MONO).process(input);
				if(mType == BandType::Allpass || mType == BandType::Notch) {
					x = mGainProcessor.process(x);
				}
			}
			return x;
		}

		/// @brief Applies this `EQBand` to the given array of input values
		///
		/// @param input - The input values to apply EQ to
		/// @param output - The processed values
		inline auto
		processMono(Span<FloatType> input, Span<FloatType> output) noexcept -> void override {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = processMono(input.at(i));
			}
		}

		/// @brief Applies this `EQBand` to the given array of input values
		///
		/// @param input - The input values to apply EQ to
		/// @param output - The processed values
		inline auto
		processMono(Span<const FloatType> input, Span<FloatType> output) noexcept -> void override {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = processMono(input.at(i));
			}
		}

		/// @brief Applies this `EQBand` to the given arrays of input values
		///
		/// @param inputLeft - The left channel sample to apply EQ to
		/// @param inputRight - The right channel sample to apply EQ to
		///
		/// @return - The processed values
		inline auto processStereo(FloatType inputLeft, FloatType inputRight) noexcept
			-> std::tuple<FloatType, FloatType> {
			auto left = inputLeft;
			auto right = inputRight;
			if(mType < BandType::Allpass) {
				for(auto& filter : mFilters.at(Processor::LEFT)) {
					left = filter.process(left);
				}
				for(auto& filter : mFilters.at(Processor::RIGHT)) {
					right = filter.process(right);
				}
				left = mGainProcessor.process(left);
				right = mGainProcessor.process(right);
			}
			else {
				left = mFilter.at(Processor::LEFT).process(left);
				right = mFilter.at(Processor::RIGHT).process(right);
				if(mType == BandType::Allpass || mType == BandType::Notch) {
					left = mGainProcessor.process(left);
					right = mGainProcessor.process(right);
				}
			}
			return {left, right};
		}

		/// @brief Applies this `EQBand` to the given arrays of input values
		///
		/// @param inputLeft - The left channel to apply EQ to
		/// @param inputRight - The right channel to apply EQ to
		/// @param outputRight - The left channel processed values
		/// @param outputRight - The right channel processed values
		inline auto processStereo(Span<FloatType> inputLeft,
								  Span<FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [left, right] = processStereo(inputLeft.at(i), inputRight.at(i));
				outputLeft.at(i) = left;
				outputRight.at(i) = right;
			}
		}

		/// @brief Applies this `EQBand` to the given arrays of input values
		///
		/// @param inputLeft - The left channel to apply EQ to
		/// @param inputRight - The right channel to apply EQ to
		/// @param outputRight - The left channel processed values
		/// @param outputRight - The right channel processed values
		inline auto processStereo(Span<const FloatType> inputLeft,
								  Span<const FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [left, right] = processStereo(inputLeft.at(i), inputRight.at(i));
				outputLeft.at(i) = left;
				outputRight.at(i) = right;
			}
		}

		/// @brief Resets this `EQBand` to an initial state
		inline auto reset() noexcept -> void override {
			if(mType < BandType::Allpass) {
				for(auto& channel : mFilters) {
					for(auto& filter : channel) {
						filter.reset();
					}
				}
			}
			else {
				for(auto& channel : mFilter) {
					channel.reset();
				}
			}
		}

		/// @brief Calculates the linear magnitude response of this filter for the given
		/// frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] virtual inline auto
		getMagnitudeForFrequency(Hertz frequency) const noexcept -> FloatType {
			auto x = narrow_cast<FloatType>(1.0);
			if(mType < BandType::Allpass) {
				for(const auto& channel : mFilters.at(Processor::MONO)) {
					x *= channel.getMagnitudeForFrequency(frequency);
				}
			}
			else {
				x = mFilter.at(Processor::MONO).getMagnitudeForFrequency(frequency);
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
			return Decibels::fromLinear(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		virtual inline auto
		getMagnitudesForFrequencies(Span<const Hertz> frequencies,
									Span<FloatType> magnitudes) const noexcept -> void {
			jassert(frequencies.size() == magnitudes.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				magnitudes.at(frequency) = getMagnitudeForFrequency(frequencies.at(frequency));
			}
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given array
		/// of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		virtual inline auto
		getDecibelMagnitudesForFrequencies(Span<const Hertz> frequencies,
										   Span<Decibels> magnitudes) const noexcept -> void {
			jassert(frequencies.size() == magnitudes.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				magnitudes.at(frequency)
					= getDecibelMagnitudeForFrequency(frequencies.at(frequency));
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
				for(const auto& channel : mFilters.at(Processor::MONO)) {
					x += channel.getPhaseForFrequency(frequency);
				}
			}
			else {
				x = mFilter.at(Processor::MONO).getPhaseForFrequency(frequency);
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
			return narrow_cast<FloatType>(getPhaseForFrequency(frequency))
				   * narrow_cast<FloatType>(180.0) / Constants<FloatType>::pi;
		}

		/// @brief Calculates the phase response of this filter for the given array of
		/// frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		virtual inline auto getPhasesForFrequencies(Span<const Hertz> frequencies,
													Span<Radians> phases) const noexcept -> void {
			jassert(frequencies.size() == phases.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				phases.at(frequency) = getPhaseForFrequency(frequencies.at(frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given array of
		/// frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		virtual inline auto
		getDegreesPhasesForFrequencies(Span<const Hertz> frequencies,
									   Span<float> phases) const noexcept -> void {
			jassert(frequencies.size() == phases.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				phases.at(frequency) = getDegreesPhaseForFrequency(frequencies.at(frequency));
			}
		}

		auto operator=(EQBand&& band) noexcept -> EQBand& = default;

	  protected:
		BandType mType = BandType::Bell;
		Hertz mFrequency = 1.0_kHz;
		FloatType mQ = narrow_cast<FloatType>(0.7);
		Decibels mGain = 0.0_dB;
		/// Only used for "____pass" type filters
		Gain<FloatType> mGainProcessor = Gain<FloatType>(mGain);
		Hertz mSampleRate = 44.1_kHz;
		size_t mOrder = 1;
		std::array<BiQuadFilter, Processor::MAX_CHANNELS> mFilter
			= std::array<BiQuadFilter, Processor::MAX_CHANNELS>();
		std::array<std::vector<BiQuadFilter>, Processor::MAX_CHANNELS> mFilters
			= std::array<std::vector<BiQuadFilter>, Processor::MAX_CHANNELS>();

		/// @brief Returns the shifted frequency for the Nth filter stage in
		/// a multi-order filter
		///
		/// @param filterIndex - The filter stage to calculate the shift for
		///
		/// @return - The shifted frequency
		[[nodiscard]] inline auto frequencyShift(size_t filterIndex) const noexcept -> Hertz {
			auto shiftMultiplier
				= narrow_cast<FloatType>(0.25) * narrow_cast<FloatType>(filterIndex);
			auto two = narrow_cast<FloatType>(2.0);
			if(mType < BandType::Highpass12DB) {
				Hertz nextOctFreq = mFrequency * two;
				return mFrequency + (shiftMultiplier * (nextOctFreq - mFrequency));
			}
			else {
				Hertz nextOctFreq = mFrequency / two;
				return mFrequency - (shiftMultiplier * (nextOctFreq - mFrequency));
			}
		}

		// clang-format off

		/// @brief Creates the necessary filter(s) for this `EQBand`
		virtual inline auto createFilters() noexcept -> void { /// NOLINT(readability-function-cognitive-complexity): This is an edge case that we'll allow. While the structure of the function would indicate a large congitive load, the actual code content is pretty simple
			// clang-format on
			switch(mType) {
				case BandType::Lowpass12DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeLowpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
							}
						}
					}
					break;
				case BandType::Lowpass24DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeLowpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
							}
						}
					}
					break;
				case BandType::Lowpass48DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeLowpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
							}
						}
					}
					break;
				case BandType::Lowpass96DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeLowpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeLowpass(frequencyShift(ord),
																		 mQ,
																		 mSampleRate);
							}
						}
					}
					break;
				case BandType::Highpass12DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeHighpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
							}
						}
					}
					break;
				case BandType::Highpass24DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeHighpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
							}
						}
					}
					break;
				case BandType::Highpass48DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeHighpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
							}
						}
					}
					break;
				case BandType::Highpass96DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeHighpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord] = BiQuadFilter::MakeHighpass(frequencyShift(ord),
																		  mQ,
																		  mSampleRate);
							}
						}
					}
					break;
				case BandType::Bandpass12DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord]
									= BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
							}
						}
					}
					break;
				case BandType::Bandpass24DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord]
									= BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
							}
						}
					}
					break;
				case BandType::Bandpass48DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord]
									= BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
							}
						}
					}
					break;
				case BandType::Bandpass96DB:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
						}
						for(auto& channel : mFilters) {
							for(size_t ord = 0; ord < mOrder; ++ord) {
								channel[ord]
									= BiQuadFilter::MakeBandpass(mFrequency, mQ, mSampleRate);
							}
						}
					}
					break;
				case BandType::Allpass:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeAllpass(mFrequency, mQ, mSampleRate);
						}
					}
					break;
				case BandType::Notch:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeNotch(mFrequency, mQ, mSampleRate);
						}
					}
					break;
				case BandType::LowShelf:
					{
						for(auto& channel : mFilter) {
							channel
								= BiQuadFilter::MakeLowShelf(mFrequency, mQ, mGain, mSampleRate);
						}
					}
					break;
				case BandType::HighShelf:
					{
						for(auto& channel : mFilter) {
							channel
								= BiQuadFilter::MakeHighShelf(mFrequency, mQ, mGain, mSampleRate);
						}
					}
					break;
				case BandType::Bell:
					{
						for(auto& channel : mFilter) {
							channel = BiQuadFilter::MakeBell(mFrequency, mQ, mGain, mSampleRate);
						}
					}
					break;
				case BandType::AnalogBell:
					{
						for(auto& channel : mFilter) {
							channel
								= BiQuadFilter::MakeAnalogBell(mFrequency, mQ, mGain, mSampleRate);
						}
					}
					break;
			}
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
	};
} // namespace apex::dsp
