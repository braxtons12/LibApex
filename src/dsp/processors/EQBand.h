#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "Gain.h"
#include "Processor.h"
#include "../filters/BiQuadFilter.h"
#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

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
					enum BandType {
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
					EQBand() noexcept;

					/// @brief Creates an `EQBand` with the given parameters
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gainDB - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					/// @param type - The type of band
					EQBand(float frequency, float q, float gainDB, size_t sampleRate, BandType type) noexcept;

					/// @brief Move constructs an `EQBand` from the given one
					///
					/// @param band - The `EQBand` to move
					EQBand(const EQBand<float>&& band) noexcept;
					~EQBand() noexcept override;

					/// @brief Sets the frequency of this `EQBand` to the given value
					///
					/// @param frequency - The new frequency, in Hertz
					void setFrequency(float frequency) noexcept;

					/// @brief Returns the frequency of this `EQBand`
					///
					/// @return - The current frequency, in Hertz
					float getFrequency() const noexcept;

					/// @brief Sets the Q of this `EQBand` to the given value
					///
					/// @param q - The new Q
					void setQ(float q) noexcept;

					/// @brief Returns the Q of this `EQBand`
					///
					/// @return - The current Q
					float getQ() const noexcept;

					/// @brief Sets the gain of this `EQBand`
					///
					/// @param gainDB - The new gain, in Decibels
					virtual void setGainDB(float gainDB) noexcept;

					/// @brief Returns the gain of this `EQBand`
					///
					/// @return - The current gain, in Decibels
					virtual float getGainDB() const noexcept;

					/// @brief Sets the sample rate of this `EQBand` to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the sample rate of this `EQBand`
					///
					/// @return - The current sample rate, in Hertz
					size_t getSampleRate() const noexcept;

					/// @brief Sets the type of this `EQBand` to the given value
					///
					/// @param type - The new type
					void setBandType(BandType type) noexcept;

					/// @brief Returns the type of this `EQBand`
					///
					/// @return - The current type
					BandType getBandType() const noexcept;

					/// @brief Applies this `EQBand` to the given input value
					///
					/// @param input - The input to apply EQ to
					///
					/// @return - The processed value
					virtual float process(float input) noexcept override;

					/// @brief Applies this `EQBand` to the given array of input values, in place
					///
					/// @param input - The input values to apply EQ to
					/// @param numSamples - The number of input samples
					virtual void process(float* input, size_t numSamples) noexcept override;

					/// @brief Resets this `EQBand` to an initial state
					virtual void reset() noexcept override;

					/// @brief Calculates the linear magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					virtual float getMagnitudeForFrequency(float frequency) const noexcept;

					/// @brief Calculates the decibel magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					float getDecibelMagnitudeForFrequency(float frequency) const noexcept;

					/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getMagnitudesForFrequencies(float* frequencies, float* magnitudes,
							size_t numFrequencies) const noexcept;

					/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					void getDecibelMagnitudesForFrequencies(float* frequencies, float* magnitudes,
							size_t numFrequencies) const noexcept;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response, in radians, at the given frequency
					virtual float getPhaseForFrequency(float frequency) const noexcept;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases (in radians) in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getPhasesForFrequencies(float* frequencies, float* phases,
							size_t numFrequencies) const noexcept;

					EQBand<float> operator=(const EQBand<float>&& band) noexcept;

				protected:
					BandType mType = BandType::Bell;
					float mFrequency = 1000.0f;
					float mQ = 1.0f;
					float mGain = 0.0f;
					///Only used for "____pass" type filters
					Gain<float> mGainProcessor;
					size_t mSampleRate = 44100;
					size_t mOrder = 1;
					BiQuadFilter<float> mFilter;
					std::vector<BiQuadFilter<float>> mFilters;

					/// @brief Returns the shifted frequency for the Nth filter stage in
					/// a multi-order filter
					///
					/// @param filterIndex - The filter stage to calculate the shift for
					///
					/// @return - The shifted frequency
					float frequencyShift(size_t filterIndex) const noexcept;

					/// @brief Creates the necessary filter(s) for this `EQBand`
					virtual void createFilters() noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
			};

		/// @brief Basic Equalizer Band
		template<>
			class EQBand<double> : public Processor<double> {
				public:
					/// @brief The different possible band types
					enum BandType {
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
					EQBand() noexcept;

					/// @brief Creates an `EQBand` with the given parameters
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gainDB - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					/// @param type - The type of band
					EQBand(double frequency, double q, double gainDB, size_t sampleRate, BandType type) noexcept;
					EQBand(const EQBand<double>&& band) noexcept;
					~EQBand() noexcept override;

					/// @brief Sets the frequency of this `EQBand` to the given value
					///
					/// @param frequency - The new frequency, in Hertz
					void setFrequency(double frequency) noexcept;

					/// @brief Returns the frequency of this `EQBand`
					///
					/// @return - The current frequency, in Hertz
					double getFrequency() const noexcept;

					/// @brief Sets the Q of this `EQBand` to the given value
					///
					/// @param q - The new Q
					void setQ(double q) noexcept;

					/// @brief Returns the Q of this `EQBand`
					///
					/// @return - The current Q
					double getQ() const noexcept;

					/// @brief Sets the gain of this `EQBand`
					///
					/// @param gainDB - The new gain, in Decibels
					virtual void setGainDB(double gainDB) noexcept;

					/// @brief Returns the gain of this `EQBand`
					///
					/// @return - The current gain, in Decibels
					virtual double getGainDB() const noexcept;

					/// @brief Sets the sample rate of this `EQBand` to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the sample rate of this `EQBand`
					///
					/// @return - The current sample rate, in Hertz
					size_t getSampleRate() const noexcept;

					/// @brief Sets the type of this `EQBand` to the given value
					///
					/// @param type - The new type
					void setBandType(BandType type) noexcept;

					/// @brief Returns the type of this `EQBand`
					///
					/// @return - The current type
					BandType getBandType() const noexcept;

					/// @brief Applies this `EQBand` to the given input value
					///
					/// @param input - The input to apply EQ to
					///
					/// @return - The processed value
					virtual double process(double input) noexcept override;

					/// @brief Applies this `EQBand` to the given array of input values, in place
					///
					/// @param input - The input values to apply EQ to
					/// @param numSamples - The number of input samples
					virtual void process(double* input, size_t numSamples) noexcept override;

					/// @brief Resets this `EQBand` to an initial state
					virtual void reset() noexcept override;

					/// @brief Calculates the linear magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					virtual double getMagnitudeForFrequency(double frequency) const noexcept;

					/// @brief Calculates the decibel magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					double getDecibelMagnitudeForFrequency(double frequency) const noexcept;

					/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getMagnitudesForFrequencies(double* frequencies, double* magnitudes,
							size_t numFrequencies) const noexcept;

					/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					void getDecibelMagnitudesForFrequencies(double* frequencies, double* magnitudes,
							size_t numFrequencies) const noexcept;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response, in radians, at the given frequency
					virtual double getPhaseForFrequency(double frequency) const noexcept;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases (in radians) in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getPhasesForFrequencies(double* frequencies, double* phases,
							size_t numFrequencies) const noexcept;

					EQBand<double> operator=(const EQBand<double>&& band) noexcept;

				protected:
					BandType mType = BandType::Bell;
					double mFrequency = 1000.0f;
					double mQ = 1.0f;
					double mGain = 0.0f;
					///Only used for "____pass" type filters
					Gain<double> mGainProcessor;
					size_t mSampleRate = 44100;
					size_t mOrder = 1;
					BiQuadFilter<double> mFilter;
					std::vector<BiQuadFilter<double>> mFilters;

					/// @brief Returns the shifted frequency for the Nth filter stage in
					/// a multi-order filter
					///
					/// @param filterIndex - The filter stage to calculate the shift for
					///
					/// @return - The shifted frequency
					double frequencyShift(size_t filterIndex) const noexcept;

					/// @brief Creates the necessary filter(s) for this `EQBand`
					virtual void createFilters() noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
			};
	}
}
