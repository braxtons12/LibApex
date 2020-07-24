#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "Gain.h"
#include "Processor.h"
#include "../filters/BiQuadFilter.h"
#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	/// @brief Basic Equalizer Band
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
		class EQBand : public Processor<T> {
			public:
				static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
		};

	/// @brief Basic Equalizer Band
	template<>
		class EQBand<float> : public Processor<float> {
			public:
				/// @brief The different possible band types
				enum class BandType {
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
				EQBand(float frequency, float q, float gainDB,
						size_t sampleRate, BandType type)
					noexcept;

				/// @brief Move constructs an `EQBand` from the given one
				///
				/// @param band - The `EQBand` to move
				EQBand(EQBand<float>&& band) noexcept = default;
				~EQBand() noexcept override = default;

				/// @brief Sets the frequency of this `EQBand` to the given value
				///
				/// @param frequency - The new frequency, in Hertz
				auto setFrequency(float frequency) noexcept -> void;

				/// @brief Returns the frequency of this `EQBand`
				///
				/// @return - The current frequency, in Hertz
				[[nodiscard]]
					auto getFrequency() const noexcept -> float;

				/// @brief Sets the Q of this `EQBand` to the given value
				///
				/// @param q - The new Q
				auto setQ(float q) noexcept -> void;

				/// @brief Returns the Q of this `EQBand`
				///
				/// @return - The current Q
				[[nodiscard]]
					auto getQ() const noexcept -> float;

				/// @brief Sets the gain of this `EQBand`
				///
				/// @param gainDB - The new gain, in Decibels
				virtual auto setGainDB(float gainDB) noexcept -> void;

				/// @brief Returns the gain of this `EQBand`
				///
				/// @return - The current gain, in Decibels
				[[nodiscard]]
					virtual auto getGainDB() const noexcept -> float;

				/// @brief Sets the sample rate of this `EQBand` to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				auto setSampleRate(size_t sampleRate) noexcept -> void;

				/// @brief Returns the sample rate of this `EQBand`
				///
				/// @return - The current sample rate, in Hertz
				[[nodiscard]]
					auto getSampleRate() const noexcept -> size_t;

				/// @brief Sets the type of this `EQBand` to the given value
				///
				/// @param type - The new type
				auto setBandType(BandType type) noexcept -> void;

				/// @brief Returns the type of this `EQBand`
				///
				/// @return - The current type
				[[nodiscard]]
					auto getBandType() const noexcept -> BandType;

				/// @brief Applies this `EQBand` to the given input value
				///
				/// @param input - The input to apply EQ to
				///
				/// @return - The processed value
				[[nodiscard]]
					auto process(float input) noexcept -> float override;

				/// @brief Applies this `EQBand` to the given array of input values, in place
				///
				/// @param input - The input values to apply EQ to
				auto process(gsl::span<float, gsl::dynamic_extent> input)
					noexcept -> void override;

				/// @brief Resets this `EQBand` to an initial state
				auto reset() noexcept -> void override;

				/// @brief Calculates the linear magnitude response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
				///
				/// @return - The magnitude response at the given frequency
				[[nodiscard]]
					virtual auto getMagnitudeForFrequency(float frequency) const noexcept -> float;

				/// @brief Calculates the decibel magnitude response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
				///
				/// @return - The magnitude response at the given frequency
				[[nodiscard]]
					virtual auto getDecibelMagnitudeForFrequency(float frequency)
						const noexcept -> float;

				/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
				///
				/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
				/// @param magnitudes - The array to store the magnitudes in
				virtual auto getMagnitudesForFrequencies(
						gsl::span<float, gsl::dynamic_extent> frequencies,
						gsl::span<float, gsl::dynamic_extent> magnitudes)
					const noexcept -> void;

				/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
				///
				/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
				/// @param magnitudes - The array to store the magnitudes in
				virtual auto getDecibelMagnitudesForFrequencies(
						gsl::span<float, gsl::dynamic_extent> frequencies,
						gsl::span<float, gsl::dynamic_extent> magnitudes)
					const noexcept -> void;

				/// @brief Calculates the phase response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calculate the phase response for, in Hertz
				///
				/// @return - The phase response, in radians, at the given frequency
				[[nodiscard]]
					virtual auto getPhaseForFrequency(float frequency) const noexcept -> float;

				/// @brief Calculates the phase response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calculate the phase response for, in Hertz
				///
				/// @return - The phase response, in degrees, at the given frequency
				[[nodiscard]]
					virtual auto getDegreesPhaseForFrequency(float frequency) const noexcept -> float;

				/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
				///
				/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
				/// @param phases - The array to store the phases (in radians) in
				virtual auto getPhasesForFrequencies(
						gsl::span<float, gsl::dynamic_extent> frequencies,
						gsl::span<float, gsl::dynamic_extent> phases)
					const noexcept -> void;

				/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
				///
				/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
				/// @param phases - The array to store the phases (in degrees) in
				virtual auto getDegreesPhasesForFrequencies(
						gsl::span<float, gsl::dynamic_extent> frequencies,
						gsl::span<float, gsl::dynamic_extent> phases)
					const noexcept -> void;

				auto operator=(EQBand<float>&& band) noexcept -> EQBand<float>& = default;

			protected:
				BandType mType = BandType::Bell;
				float mFrequency = 1000.0F;
				float mQ = 1.0F;
				float mGain = 0.0F;
				///Only used for "____pass" type filters
				Gain<float> mGainProcessor = Gain<float>(mGain, true);
				size_t mSampleRate = 44100;
				size_t mOrder = 1;
				BiQuadFilter<float> mFilter =
					BiQuadFilter<float>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
				std::vector<BiQuadFilter<float>> mFilters = std::vector<BiQuadFilter<float>>();

				/// @brief Returns the shifted frequency for the Nth filter stage in
				/// a multi-order filter
				///
				/// @param filterIndex - The filter stage to calculate the shift for
				///
				/// @return - The shifted frequency
				[[nodiscard]]
					auto frequencyShift(size_t filterIndex) const noexcept -> float;

				/// @brief Creates the necessary filter(s) for this `EQBand`
				virtual auto createFilters() noexcept -> void;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
		};

	/// @brief Basic Equalizer Band
	template<>
		class EQBand<double> : public Processor<double> {
			public:
				/// @brief The different possible band types
				enum class BandType {
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
				EQBand(double frequency, double q, double gainDB,
						size_t sampleRate, BandType type)
					noexcept;

				/// @brief Move constructs an `EQBand` from the given one
				///
				/// @param band - The `EQBand` to move
				EQBand(EQBand<double>&& band) noexcept = default;
				~EQBand() noexcept override = default;

				/// @brief Sets the frequency of this `EQBand` to the given value
				///
				/// @param frequency - The new frequency, in Hertz
				auto setFrequency(double frequency) noexcept -> void;

				/// @brief Returns the frequency of this `EQBand`
				///
				/// @return - The current frequency, in Hertz
				[[nodiscard]]
					auto getFrequency() const noexcept -> double;

				/// @brief Sets the Q of this `EQBand` to the given value
				///
				/// @param q - The new Q
				auto setQ(double q) noexcept -> void;

				/// @brief Returns the Q of this `EQBand`
				///
				/// @return - The current Q
				[[nodiscard]]
					auto getQ() const noexcept -> double;

				/// @brief Sets the gain of this `EQBand`
				///
				/// @param gainDB - The new gain, in Decibels
				virtual auto setGainDB(double gainDB) noexcept -> void;

				/// @brief Returns the gain of this `EQBand`
				///
				/// @return - The current gain, in Decibels
				[[nodiscard]]
					virtual auto getGainDB() const noexcept -> double;

				/// @brief Sets the sample rate of this `EQBand` to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				auto setSampleRate(size_t sampleRate) noexcept -> void;

				/// @brief Returns the sample rate of this `EQBand`
				///
				/// @return - The current sample rate, in Hertz
				[[nodiscard]]
					auto getSampleRate() const noexcept -> size_t;

				/// @brief Sets the type of this `EQBand` to the given value
				///
				/// @param type - The new type
				auto setBandType(BandType type) noexcept -> void;

				/// @brief Returns the type of this `EQBand`
				///
				/// @return - The current type
				[[nodiscard]]
					auto getBandType() const noexcept -> BandType;

				/// @brief Applies this `EQBand` to the given input value
				///
				/// @param input - The input to apply EQ to
				///
				/// @return - The processed value
				[[nodiscard]]
					auto process(double input) noexcept -> double override;

				/// @brief Applies this `EQBand` to the given array of input values, in place
				///
				/// @param input - The input values to apply EQ to
				auto process(gsl::span<double, gsl::dynamic_extent> input)
					noexcept -> void override;

				/// @brief Resets this `EQBand` to an initial state
				auto reset() noexcept -> void override;

				/// @brief Calculates the linear magnitude response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
				///
				/// @return - The magnitude response at the given frequency
				[[nodiscard]]
					virtual auto getMagnitudeForFrequency(double frequency) const noexcept -> double;

				/// @brief Calculates the decibel magnitude response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
				///
				/// @return - The magnitude response at the given frequency
				[[nodiscard]]
					virtual auto getDecibelMagnitudeForFrequency(double frequency)
						const noexcept -> double;

				/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
				///
				/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
				/// @param magnitudes - The array to store the magnitudes in
				virtual auto getMagnitudesForFrequencies(
						gsl::span<double, gsl::dynamic_extent> frequencies,
						gsl::span<double, gsl::dynamic_extent> magnitudes)
					const noexcept -> void;

				/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
				///
				/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
				/// @param magnitudes - The array to store the magnitudes in
				virtual auto getDecibelMagnitudesForFrequencies(
						gsl::span<double, gsl::dynamic_extent> frequencies,
						gsl::span<double, gsl::dynamic_extent> magnitudes)
					const noexcept -> void;

				/// @brief Calculates the phase response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calculate the phase response for, in Hertz
				///
				/// @return - The phase response, in radians, at the given frequency
				[[nodiscard]]
					virtual auto getPhaseForFrequency(double frequency) const noexcept -> double;

				/// @brief Calculates the phase response of this filter for the given frequency
				///
				/// @param frequency - The frequency to calculate the phase response for, in Hertz
				///
				/// @return - The phase response, in degrees, at the given frequency
				[[nodiscard]]
					virtual auto getDegreesPhaseForFrequency(double frequency)
						const noexcept -> double;

				/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
				///
				/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
				/// @param phases - The array to store the phases (in radians) in
				virtual auto getPhasesForFrequencies(
						gsl::span<double, gsl::dynamic_extent> frequencies,
						gsl::span<double, gsl::dynamic_extent> phases)
					const noexcept -> void;

				/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
				///
				/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
				/// @param phases - The array to store the phases (in degrees) in
				virtual auto getDegreesPhasesForFrequencies(
						gsl::span<double, gsl::dynamic_extent> frequencies,
						gsl::span<double, gsl::dynamic_extent> phases)
					const noexcept -> void;

				auto operator=(EQBand<double>&& band) noexcept -> EQBand<double>& = default;

			protected:
				BandType mType = BandType::Bell;
				double mFrequency = 1000.0;
				double mQ = 1.0;
				double mGain = 0.0;
				///Only used for "____pass" type filters
				Gain<double> mGainProcessor = Gain<double>(mGain, true);
				size_t mSampleRate = 44100;
				size_t mOrder = 1;
				BiQuadFilter<double> mFilter =
					BiQuadFilter<double>::MakeBell(mFrequency, mQ, mGain, mSampleRate);
				std::vector<BiQuadFilter<double>> mFilters = std::vector<BiQuadFilter<double>>();

				/// @brief Returns the shifted frequency for the Nth filter stage in
				/// a multi-order filter
				///
				/// @param filterIndex - The filter stage to calculate the shift for
				///
				/// @return - The shifted frequency
				[[nodiscard]]
					auto frequencyShift(size_t filterIndex) const noexcept -> double;

				/// @brief Creates the necessary filter(s) for this `EQBand`
				virtual auto createFilters() noexcept -> void;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
		};
} //namespace apex::dsp
