#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "../../base/StandardIncludes.h"
#include "EQBand.h"

namespace apex::dsp {
	/// @brief Basic Parallel Equalizer band
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class ParallelEQBand : public EQBand<FloatType> {
	  private:
		using EQBand = EQBand<FloatType>;
		using Processor = Processor<FloatType>;

	  public:
		/// @brief Creates a default `ParallelEQBand`
		ParallelEQBand() noexcept {
			setGainDB(EQBand::mGain);
		}

		/// @brief Creates a `ParallelEQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The band type to use
		ParallelEQBand(Hertz frequency,
					   FloatType q,
					   Decibels gainDB,
					   Hertz sampleRate,
					   BandType type) noexcept
			: EQBand(frequency, q, gainDB, sampleRate, type) {
			setGainDB(EQBand::mGain);
		}

		/// @brief Move constructs a `ParallelEQBand` from the given one
		///
		/// @param band - The `ParallelEQBand` to move
		ParallelEQBand(ParallelEQBand&& band) noexcept = default;
		~ParallelEQBand() noexcept override = default;

		/// @brief Sets the gain of this `ParallelEQBand` to the given value
		///
		/// @param gainDB - The new gain, in Decibels
		inline auto setGainDB(Decibels gainDB) noexcept -> void override {
			auto one = narrow_cast<FloatType>(1.0);
			if(EQBand::mType < BandType::Allpass) {
				EQBand::mGain = gainDB;
				mGainActual = EQBand::mGain;
				EQBand::mGainProcessor.setGainDecibels(EQBand::mGain);
				for(auto& channel : EQBand::mFilter) {
					channel.setGainDB(EQBand::mGain);
				}
				for(auto& channel : EQBand::mFilters) {
					for(auto& filt : channel) {
						filt.setGainDB(EQBand::mGain);
					}
				}
			}
			else if(EQBand::mType < BandType::LowShelf) {
				EQBand::mGain = gainDB;
				mGainActual = EQBand::mGain;
				EQBand::mGainProcessor.setGainDecibels(EQBand::mGain);
				for(auto& channel : EQBand::mFilter) {
					channel.setGainDB(EQBand::mGain);
				}
			}
			else {
				mGainActual = gainDB;
				// if gain is negative, we need to subtract that amount of linear gain from the
				// input to get the desired gain: linear 0db (eg 1.0) - linear gain level = amount
				// of linear signal to be removed
				if(gainDB < narrow_cast<FloatType>(0.0)) {
					EQBand::mGain = Decibels::fromLinear(one - gainDB.getLinear());
				}
				// otherwise, the gain is the amount of linear gain we need to add to the input:
				// linear gain level - linear 0db (eg 1.0) = amount of linear signal to be added
				else {
					EQBand::mGain = math::Decibels::linearToDecibels(gainDB.getLinear() - one);
				}

				EQBand::mGainProcessor.setGainDecibels(EQBand::mGain);
				for(auto& channel : EQBand::mFilter) {
					channel.setGainDB(EQBand::mGain);
				}
			}
		}

		/// @brief Returns the gain of this `ParallelEQBand`
		///
		/// @return - The current gain, in Decibels
		[[nodiscard]] inline auto getGainDB() const noexcept -> Decibels override {
			return mGainActual;
		}

		/// @brief Applies this `ParallelEQBand` to the given input value
		///
		/// @param input - The input value to apply EQ to
		///
		/// @return - The processed value
		[[nodiscard]] inline auto processMono(FloatType input) noexcept -> FloatType override {
			auto x = input;
			if(EQBand::mType < BandType::Allpass) {
				for(auto& filt : EQBand::mFilters.at(Processor::MONO)) {
					x = filt.process(x);
				}
				x = EQBand::mGainProcessor.process(x);
			}
			else if(EQBand::mType < BandType::LowShelf) {
				x = EQBand::mFilter.at(Processor::MONO).process(x);
				x = EQBand::mGainProcessor.process(x);
			}
			else {
				x = EQBand::mFilter.at(Processor::MONO).process(x);
				x = EQBand::mGainProcessor.process(x);
				if(mGainActual < narrow_cast<FloatType>(0.0)) {
					x = input - x;
				}
				else {
					x = input + x;
				}
			}
			return x;
		}

		/// @brief Applies this `ParallelEQBand` to the array of given input values
		///
		/// @param input - The array of input values to apply EQ to
		/// @param output - The processed values
		inline auto
		processMono(Span<FloatType> input, Span<FloatType> output) noexcept -> void override {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = process(input.at(i));
			}
		}

		/// @brief Applies this `ParallelEQBand` to the array of given input values
		///
		/// @param input - The array of input values to apply EQ to
		/// @param output - The processed values
		inline auto
		processMono(Span<const FloatType> input, Span<FloatType> output) noexcept -> void override {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = process(input.at(i));
			}
		}

		/// @brief Applies this `ParallelEQBand` to the given arrays of input values
		///
		/// @param inputLeft - The left channel sample to apply EQ to
		/// @param inputRight - The right channel sample to apply EQ to
		///
		/// @return - The processed values
		inline auto processStereo(FloatType inputLeft, FloatType inputRight) noexcept
			-> std::tuple<FloatType, FloatType> override {
			auto left = inputLeft;
			auto right = inputRight;
			if(EQBand::mType < BandType::Allpass) {
				for(auto& filt : EQBand::mFilters.at(Processor::LEFT)) {
					left = filt.process(left);
				}
				for(auto& filt : EQBand::mFilters.at(Processor::RIGHT)) {
					right = filt.process(right);
				}
				left = EQBand::mGainProcessor.process(left);
				right = EQBand::mGainProcessor.process(right);
			}
			else if(EQBand::mType < BandType::LowShelf) {
				left = EQBand::mFilter.at(Processor::LEFT).process(left);
				right = EQBand::mFilter.at(Processor::LEFT).process(right);
				left = EQBand::mGainProcessor.process(left);
				right = EQBand::mGainProcessor.process(right);
			}
			else {
				left = EQBand::mFilter.at(Processor::MONO).process(left);
				right = EQBand::mFilter.at(Processor::MONO).process(right);
				left = EQBand::mGainProcessor.process(left);
				right = EQBand::mGainProcessor.process(right);
				if(mGainActual < narrow_cast<FloatType>(0.0)) {
					left = inputLeft - left;
					right = inputRight - right;
				}
				else {
					left = inputLeft + left;
					right = inputRight + right;
				}
			}
			return {left, right};
		}

		/// @brief Applies this `ParallelEQBand` to the given arrays of input values
		///
		/// @param inputLeft - The left channel to apply EQ to
		/// @param inputRight - The right channel to apply EQ to
		/// @param outputRight - The left channel processed values
		/// @param outputRight - The right channel processed values
		inline auto processStereo(Span<FloatType> inputLeft,
								  Span<FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void override {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [left, right] = processStereo(inputLeft.at(i), inputRight.at(i));

				outputLeft.at(i) = left;
				outputRight.at(i) = right;
			}
		}

		/// @brief Applies this `ParallelEQBand` to the given arrays of input values
		///
		/// @param inputLeft - The left channel to apply EQ to
		/// @param inputRight - The right channel to apply EQ to
		/// @param outputRight - The left channel processed values
		/// @param outputRight - The right channel processed values
		inline auto processStereo(Span<const FloatType> inputLeft,
								  Span<const FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void override {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [left, right] = processStereo(inputLeft.at(i), inputRight.at(i));

				outputLeft.at(i) = left;
				outputRight.at(i) = right;
			}
		}

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto
		getMagnitudeForFrequency(Hertz frequency) const noexcept -> FloatType override {
			auto x = narrow_cast<FloatType>(1.0);
			if(EQBand::mType < BandType::Allpass) {
				for(const auto& filt : EQBand::mFilters.at(Processor::LEFT)) {
					x *= filt.getMagnitudeForFrequency(frequency);
				}
			}
			else if(EQBand::mType < BandType::LowShelf) {
				x = EQBand::mFilter.at(Processor::LEFT).getMagnitudeForFrequency(frequency);
			}
			else if(EQBand::mType == BandType::LowShelf) {
				BiQuadFilter<FloatType> filt
					= BiQuadFilter<FloatType>::MakeLowShelf(EQBand::mFrequency,
															EQBand::mQ,
															EQBand::mGain,
															EQBand::mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else if(EQBand::mType == BandType::HighShelf) {
				BiQuadFilter<FloatType> filt
					= BiQuadFilter<FloatType>::MakeHighShelf(EQBand::mFrequency,
															 EQBand::mQ,
															 EQBand::mGain,
															 EQBand::mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else if(EQBand::mType == BandType::Bell) {
				BiQuadFilter<FloatType> filt
					= BiQuadFilter<FloatType>::MakeBell(EQBand::mFrequency,
														EQBand::mQ,
														EQBand::mGain,
														EQBand::mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			else {
				BiQuadFilter<FloatType> filt
					= BiQuadFilter<FloatType>::MakeAnalogBell(EQBand::mFrequency,
															  EQBand::mQ,
															  EQBand::mGain,
															  EQBand::mSampleRate);
				x = filt.getMagnitudeForFrequency(frequency);
			}
			return x;
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] inline auto
		getDecibelMagnitudeForFrequency(Hertz frequency) const noexcept -> Decibels override {
			return Decibels::fromLinear(getMagnitudeForFrequency(frequency));
		}

		/// @brief Calculates the linear magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		inline auto
		getMagnitudesForFrequencies(Span<const Hertz> frequencies,
									Span<FloatType> magnitudes) const noexcept -> void override {
			jassert(frequencies.size() == magnitudes.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				magnitudes.at(frequency) = getMagnitudeForFrequency(frequencies.at(frequency));
			}
		}

		/// @brief Calculates the decibel magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		inline auto getDecibelMagnitudesForFrequencies(Span<const Hertz> frequencies,
													   Span<Decibels> magnitudes) const noexcept
			-> void override {
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
		[[nodiscard]] auto
		getPhaseForFrequency(Hertz frequency) const noexcept -> Radians override {
			Radians x = 0.0_rad;
			if(EQBand::mType < BandType::Allpass) {
				for(const auto& filt : EQBand::mFilters.at(Processor::LEFT)) {
					x += filt.getPhaseForFrequency(frequency);
				}
			}
			else if(EQBand::mType < BandType::LowShelf) {
				x = EQBand::mFilter.at(Processor::LEFT).getPhaseForFrequency(frequency);
			}
			else {
				/// TODO: This might not be the accurate way to do this,
				/// but for now it's what we have

				// Parallel bands should have phase-shift as the average between
				// the shift caused by the filter and the original, right?
				x = EQBand::mFilter.at(Processor::LEFT).getPhaseForFrequency(frequency)
					* narrow_cast<FloatType>(0.5);
			}
			return x;
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		[[nodiscard]] inline auto
		getDegreesPhaseForFrequency(Hertz frequency) const noexcept -> float override {
			return narrow_cast<FloatType>(getPhaseForFrequency(frequency))
				   * narrow_cast<FloatType>(180.0) / Constants<FloatType>::pi;
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		inline auto getPhasesForFrequencies(Span<const Hertz> frequencies,
											Span<Radians> phases) const noexcept -> void override {
			jassert(frequencies.size() == phases.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				phases.at(frequency) = getPhaseForFrequency(frequencies.at(frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		inline auto
		getDegreesPhasesForFrequencies(Span<const Hertz> frequencies,
									   Span<float> phases) const noexcept -> void override {
			jassert(frequencies.size() == phases.size());
			auto size = frequencies.size();
			for(auto frequency = 0U; frequency < size; ++frequency) {
				phases.at(frequency) = getDegreesPhaseForFrequency(frequencies.at(frequency));
			}
		}

		auto operator=(ParallelEQBand&& band) noexcept -> ParallelEQBand& = default;

	  protected:
		/// the actual gain, before acounting for changes necessary for EQing in parallel
		Decibels mGainActual = 0.0_dB;

		/// @brief Creates necessary filter(s) for this EQ band
		inline auto createFilters() noexcept -> void override {
			EQBand::createFilters();
			if(EQBand::mType == BandType::LowShelf) {
				for(auto& channel : EQBand::mFilter) {
					channel = BiQuadFilter<FloatType>::MakeLowpass(EQBand::mFrequency,
																   EQBand::mQ,
																   EQBand::mSampleRate);
				}
			}
			else if(EQBand::mType == BandType::HighShelf) {
				for(auto& channel : EQBand::mFilter) {
					channel = BiQuadFilter<FloatType>::MakeHighpass(EQBand::mFrequency,
																	EQBand::mQ,
																	EQBand::mSampleRate);
				}
			}
			else if(EQBand::mType == BandType::Bell) {
				for(auto& channel : EQBand::mFilter) {
					channel = BiQuadFilter<FloatType>::MakeBandpass(EQBand::mFrequency,
																	EQBand::mQ,
																	EQBand::mSampleRate);
				}
			}
			else if(EQBand::mType == BandType::AnalogBell) {
				for(auto& channel : EQBand::mFilter) {
					channel = BiQuadFilter<FloatType>::MakeBandpass(
						EQBand::mFrequency,
						EQBand::mQ
							* Exponentials<FloatType>::pow10(narrow_cast<FloatType>(mGainActual)
															 / narrow_cast<FloatType>(40.0)),
						EQBand::mSampleRate);
				}
			}
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParallelEQBand)
	};
} // namespace apex::dsp
