#pragma once

#include <type_traits>
#include <utility>
#include <vector>

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

					/// @brief Creates an `EQBand` with the given parameters
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gainDB - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					/// @param type - The type of band
					EQBand(float frequency, float q, float gainDB, size_t sampleRate, BandType type);
					~EQBand() override;

					/// @brief Sets the frequency of this `EQBand` to the given value
					///
					/// @param frequency - The new frequency, in Hertz
					void setFrequency(float frequency);

					/// @brief Returns the frequency of this `EQBand`
					///
					/// @return - The current frequency, in Hertz
					float getFrequency() const;

					/// @brief Sets the Q of this `EQBand` to the given value
					///
					/// @param q - The new Q
					void setQ(float q);

					/// @brief Returns the Q of this `EQBand`
					///
					/// @return - The current Q
					float getQ() const;

					/// @brief Sets the gain of this `EQBand`
					///
					/// @param gainDB - The new gain, in Decibels
					void setGainDB(float gainDB);

					/// @brief Returns the gain of this `EQBand`
					///
					/// @return - The current gain, in Decibels
					float getGainDB() const;

					/// @brief Sets the sample rate of this `EQBand` to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate);

					/// @brief Returns the sample rate of this `EQBand`
					///
					/// @return - The current sample rate, in Hertz
					size_t getSampleRate() const;

					/// @brief Sets the type of this `EQBand` to the given value
					///
					/// @param type - The new type
					void setBandType(BandType type);

					/// @brief Returns the type of this `EQBand`
					///
					/// @return - The current type
					BandType getBandType() const;

					/// @brief Applies this `EQBand` to the given input value
					///
					/// @param input - The input to apply EQ to
					///
					/// @return - The processed value
					float process(float input) override;

					/// @brief Applies this `EQBand` to the given array of input values, in place
					///
					/// @param input - The input values to apply EQ to
					/// @param numSamples - The number of input samples
					void process(float* input, size_t numSamples) override;

					/// @brief Resets this `EQBand` to an initial state
					void reset() override;

				private:
					BandType mType = BandType::Bell;
					float mFrequency = 1000.0f;
					float mQ = 1.0f;
					float mGain = 0.0f;
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
					float frequencyShift(size_t filterIndex) const;

					/// @brief Creates the necessary filter(s) for this `EQBand`
					void createFilters();

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

					/// @brief Creates an `EQBand` with the given parameters
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gainDB - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					/// @param type - The type of band
					EQBand(double frequency, double q, double gainDB, size_t sampleRate, BandType type);
					~EQBand() override;

					/// @brief Sets the frequency of this `EQBand` to the given value
					///
					/// @param frequency - The new frequency, in Hertz
					void setFrequency(double frequency);

					/// @brief Returns the frequency of this `EQBand`
					///
					/// @return - The current frequency, in Hertz
					double getFrequency() const;

					/// @brief Sets the Q of this `EQBand` to the given value
					///
					/// @param q - The new Q
					void setQ(double q);

					/// @brief Returns the Q of this `EQBand`
					///
					/// @return - The current Q
					double getQ() const;

					/// @brief Sets the gain of this `EQBand`
					///
					/// @param gainDB - The new gain, in Decibels
					void setGainDB(double gainDB);

					/// @brief Returns the gain of this `EQBand`
					///
					/// @return - The current gain, in Decibels
					double getGainDB() const;

					/// @brief Sets the sample rate of this `EQBand` to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate);

					/// @brief Returns the sample rate of this `EQBand`
					///
					/// @return - The current sample rate, in Hertz
					size_t getSampleRate() const;

					/// @brief Sets the type of this `EQBand` to the given value
					///
					/// @param type - The new type
					void setBandType(BandType type);

					/// @brief Returns the type of this `EQBand`
					///
					/// @return - The current type
					BandType getBandType() const;

					/// @brief Applies this `EQBand` to the given input value
					///
					/// @param input - The input to apply EQ to
					///
					/// @return - The processed value
					double process(double input) override;

					/// @brief Applies this `EQBand` to the given array of input values, in place
					///
					/// @param input - The input values to apply EQ to
					/// @param numSamples - The number of input samples
					void process(double* input, size_t numSamples) override;

					/// @brief Resets this `EQBand` to an initial state
					void reset() override;

				private:
					BandType mType = BandType::Bell;
					double mFrequency = 1000.0f;
					double mQ = 1.0f;
					double mGain = 0.0f;
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
					double frequencyShift(size_t filterIndex) const;

					/// @brief Creates the necessary filter(s) for this `EQBand`
					void createFilters();

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQBand)
			};
	}
}
