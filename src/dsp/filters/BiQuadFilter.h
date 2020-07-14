#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Basic BiQuad Filter implementation
		///
		/// @see https://www.musicdsp.org/en/latest/Filters/197-rbj-audio-eq-cookbook.html
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class BiQuadFilter {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiQuadFilter)
			};

		/// @brief Basic BiQuad Filter implementation
		///
		/// @see https://www.musicdsp.org/en/latest/Filters/197-rbj-audio-eq-cookbook.html
		template<>
			class BiQuadFilter<float> {
				public:

					/// @brief Constructs a default `BiQuadFilter`
					BiQuadFilter() noexcept;

					/// @brief Move contructs a `BiQuadFilter` from the given one
					///
					/// @param filt - The `BiQuadFilter<float>` to move
					BiQuadFilter(BiQuadFilter<float>&& filt) noexcept = default;
					~BiQuadFilter() noexcept = default;

					/// @brief The different possible BiQuad Filter types
					enum FilterType {
						Lowpass = 0,
						Highpass,
						Bandpass,
						Allpass,
						Notch,
						LowShelf,
						HighShelf,
						Bell,
						AnalogBell
					};

					/// @brief Creates a default lowpass BiQuad Filter
					///
					/// @return - A lowpass BiQuad Filter
					static BiQuadFilter<float> MakeLowpass() noexcept;

					/// @brief Creates a lowpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowpass BiQuad Filter
					static BiQuadFilter<float> MakeLowpass(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a lowpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowpass BiQuad Filter
					static BiQuadFilter<float> MakeLowpass(float frequency, float q = 0.7f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default highpass BiQuad filter
					///
					/// @return - A highpass BiQuad Filter
					static BiQuadFilter<float> MakeHighpass() noexcept;

					/// @brief Creates a highpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highpass BiQuad Filter
					static BiQuadFilter<float> MakeHighpass(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a highpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highpass BiQuad Filter
					static BiQuadFilter<float> MakeHighpass(float frequency, float q = 0.7f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default bandpass BiQuad Filter
					///
					/// @return - A bandpass BiQuad Filter
					static BiQuadFilter<float> MakeBandpass() noexcept;

					/// @brief Creates a bandpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bandpass BiQuad Filter
					static BiQuadFilter<float> MakeBandpass(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a bandpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bandpass BiQuad Filter
					static BiQuadFilter<float> MakeBandpass(float frequency, float q = 0.7f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default allpass BiQuad Filter
					///
					/// @return - An allpass BiQuad Filter
					static BiQuadFilter<float> MakeAllpass() noexcept;

					/// @brief Creates an allpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An allpass BiQuad Filter
					static BiQuadFilter<float> MakeAllpass(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates an allpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An allpass BiQuad Filter
					static BiQuadFilter<float> MakeAllpass(float frequency, float q = 0.7f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default notch BiQuad Filter
					///
					/// @return - A notch BiQuad Filter
					static BiQuadFilter<float> MakeNotch() noexcept;

					/// @brief Creates a notch BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A notch BiQuad Filter
					static BiQuadFilter<float> MakeNotch(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a notch BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A notch BiQuad Filter
					static BiQuadFilter<float> MakeNotch(float frequency, float q = 0.7f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default lowshelf BiQuad Filter
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<float> MakeLowShelf() noexcept;

					/// @brief Creates a lowshelf BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<float> MakeLowShelf(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a lowshelf BiQuad Filter with the given frequency, gain,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<float> MakeLowShelf(float frequency, float gain = 0.0f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a lowshelf BiQuad Filter with the given frequency, q,
					/// gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<float> MakeLowShelf(float frequency, float q = 0.7f,
							float gain = 0.0f, size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default highshelf BiQuad Filter
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<float> MakeHighShelf() noexcept;

					/// @brief Creates a highshelf BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<float> MakeHighShelf(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a highshelf BiQuad Filter with the given frequency, gain,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<float> MakeHighShelf(float frequency, float gain = 0.0f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a highshelf BiQuad Filter with the given frequency, q,
					/// gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<float> MakeHighShelf(float frequency, float q = 0.7f,
							float gain = 0.0f, size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default bell-shape BiQuad Filter
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeBell() noexcept;

					/// @brief Creates a bell-shape BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeBell(float frequency, size_t sampleRate = 44100) noexcept;

					/// @brief Creates a bell-shape BiQuad Filter with the given frequency, gain,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeBell(float frequency, float gain = 0.0f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a bell-shape BiQuad Filter with the given frequency, q,
					/// gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeBell(float frequency, float q = 0.7f,
							float gain = 0.0f, size_t sampleRate = 44100) noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter
					///
					/// @return - An analog-style bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeAnalogBell() noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
					/// frequency and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An analog-style bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeAnalogBell(float frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
					/// frequency, gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An analog-style bell-shape BiQuad Filter
					static BiQuadFilter<float> MakeAnalogBell(float frequency, float gain = 0.0f,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
					/// frequency, q, gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return
					static BiQuadFilter<float> MakeAnalogBell(float frequency, float q = 0.7f,
							float gain = 0.0f, size_t sampleRate = 44100) noexcept;

					/// @brief Sets the frequency of this filter to the given value
					///
					/// @param frequency - The new frequency, in Hertz
					void setFrequency(float frequency) noexcept;

					/// @brief Returns the frequency of this filter
					///
					/// @return - The frequency, in Hertz
					float getFrequency() const noexcept;

					/// @brief Sets the Q of this filter to the given value
					///
					/// @param q - The new Q
					void setQ(float q) noexcept;

					/// @brief Returns the Q of this filter
					///
					/// @return  - The Q
					float getQ() const noexcept;

					/// @brief Sets the gain of this filter to the given value
					///
					/// @param gain - The new gain, in Decibels
					void setGainDB(float gain) noexcept;

					/// @brief Returns the gain of this filter
					///
					/// @return - The gain, in Decibels
					float getGainDB() const noexcept;

					/// @brief Sets the sample rate of this filter to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the sample rate of this filter
					///
					/// @return - The sample rate, in Hertz
					size_t getSampleRate() const noexcept;

					/// @brief Applies this filter to the given input value
					///
					/// @param input - The input value to apply filtering to
					///
					/// @return - The filtered value
					float process(float input) noexcept;

					/// @brief Applies this filter to the array of given input values, in place
					///
					/// @param input - The array of input values to filter
					/// @param numSamples - The number of samples in the array
					void process(float* input, size_t numSamples) noexcept;

					/// @brief Resets this filter to an initial state
					void reset() noexcept;

					/// @brief Calculates the magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					float getMagnitudeForFrequency(float frequency) const noexcept;

					/// @brief Calculates the magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					void getMagnitudesForFrequencies(float* frequencies, float* magnitudes,
							size_t numFrequencies) const noexcept;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response at the given frequency
					float getPhaseForFrequency(float frequency) const noexcept;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					void getPhasesForFrequencies(float* frequencies, float* phases,
							size_t numFrequencies) const noexcept;

					BiQuadFilter<float>& operator=(BiQuadFilter<float>&& filt) noexcept = default;

				private:
					float mB0 = 0.0f;
					float mB1 = 0.0f;
					float mB2 = 0.0f;
					float mA0 = 0.0f;
					float mA1 = 0.0f;
					float mA2 = 0.0f;

					float mY1 = 0.0f;
					float mY2 = 0.0f;
					float mX1 = 0.0f;
					float mX2 = 0.0f;

					FilterType mType = FilterType::Bell;
					float mFrequency = 1000.0f;
					float mQ = 0.7f;
					float mGain = 0.0f;
					size_t mSampleRate = 44100;

					BiQuadFilter(float frequency, float q, float gain,
							size_t sampleRate, FilterType type) noexcept;

					/// @brief Updates the coefficients of this filter
					void updateCoefficients() noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiQuadFilter)
			};

		/// @brief Basic BiQuad Filter implementation
		///
		/// @see https://www.musicdsp.org/en/latest/Filters/197-rbj-audio-eq-cookbook.html
		template<>
			class BiQuadFilter<double> {
				public:

					/// @brief Constructs a default `BiQuadFilter`
					BiQuadFilter() noexcept;

					/// @brief Move contructs a `BiQuadFilter` from the given one
					///
					/// @param filt - The `BiQuadFilter` to move
					BiQuadFilter(BiQuadFilter<double>&& filt) noexcept = default;
					~BiQuadFilter() noexcept = default;

					/// @brief The different possible BiQuad Filter types
					enum FilterType {
						Lowpass = 0,
						Highpass,
						Bandpass,
						Allpass,
						Notch,
						LowShelf,
						HighShelf,
						Bell,
						AnalogBell
					};

					/// @brief Creates a default lowpass BiQuad Filter
					///
					/// @return - A lowpass BiQuad Filter
					static BiQuadFilter<double> MakeLowpass() noexcept;

					/// @brief Creates a lowpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowpass BiQuad Filter
					static BiQuadFilter<double> MakeLowpass(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a lowpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowpass BiQuad Filter
					static BiQuadFilter<double> MakeLowpass(double frequency, double q = 0.7,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default highpass BiQuad filter
					///
					/// @return - A highpass BiQuad Filter
					static BiQuadFilter<double> MakeHighpass() noexcept;

					/// @brief Creates a highpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highpass BiQuad Filter
					static BiQuadFilter<double> MakeHighpass(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a highpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highpass BiQuad Filter
					static BiQuadFilter<double> MakeHighpass(double frequency, double q = 0.7,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default bandpass BiQuad Filter
					///
					/// @return - A bandpass BiQuad Filter
					static BiQuadFilter<double> MakeBandpass() noexcept;

					/// @brief Creates a bandpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bandpass BiQuad Filter
					static BiQuadFilter<double> MakeBandpass(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a bandpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bandpass BiQuad Filter
					static BiQuadFilter<double> MakeBandpass(double frequency, double q = 0.7,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default allpass BiQuad Filter
					///
					/// @return - An allpass BiQuad Filter
					static BiQuadFilter<double> MakeAllpass() noexcept;

					/// @brief Creates an allpass BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An allpass BiQuad Filter
					static BiQuadFilter<double> MakeAllpass(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates an allpass BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An allpass BiQuad Filter
					static BiQuadFilter<double> MakeAllpass(double frequency, double q = 0.7,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default notch BiQuad Filter
					///
					/// @return - A notch BiQuad Filter
					static BiQuadFilter<double> MakeNotch() noexcept;

					/// @brief Creates a notch BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A notch BiQuad Filter
					static BiQuadFilter<double> MakeNotch(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a notch BiQuad Filter with the given frequency, q,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A notch BiQuad Filter
					static BiQuadFilter<double> MakeNotch(double frequency, double q = 0.7,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default lowshelf BiQuad Filter
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<double> MakeLowShelf() noexcept;

					/// @brief Creates a lowshelf BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<double> MakeLowShelf(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a lowshelf BiQuad Filter with the given frequency, gain,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<double> MakeLowShelf(double frequency, double gain = 0.0,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a lowshelf BiQuad Filter with the given frequency, q,
					/// gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A lowshelf BiQuad Filter
					static BiQuadFilter<double> MakeLowShelf(double frequency, double q = 0.7, double gain = 0.0,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default highshelf BiQuad Filter
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<double> MakeHighShelf() noexcept;

					/// @brief Creates a highshelf BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<double> MakeHighShelf(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a highshelf BiQuad Filter with the given frequency, gain,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<double> MakeHighShelf(double frequency, double gain = 0.0,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a highshelf BiQuad Filter with the given frequency, q,
					/// gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A highshelf BiQuad Filter
					static BiQuadFilter<double> MakeHighShelf(double frequency, double q = 0.7,
							double gain = 0.0, size_t sampleRate = 44100) noexcept;

					/// @brief Creates a default bell-shape BiQuad Filter
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeBell() noexcept;

					/// @brief Creates a bell-shape BiQuad Filter with the given frequency
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeBell(double frequency, size_t sampleRate = 44100) noexcept;

					/// @brief Creates a bell-shape BiQuad Filter with the given frequency, gain,
					/// and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeBell(double frequency, double gain = 0.0,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates a bell-shape BiQuad Filter with the given frequency, q,
					/// gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - A bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeBell(double frequency, double q = 0.7,
							double gain = 0.0, size_t sampleRate = 44100) noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter
					///
					/// @return - An analog-style bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeAnalogBell() noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
					/// frequency and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An analog-style bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeAnalogBell(double frequency,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
					/// frequency, gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return - An analog-style bell-shape BiQuad Filter
					static BiQuadFilter<double> MakeAnalogBell(double frequency, double gain = 0.0,
							size_t sampleRate = 44100) noexcept;

					/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
					/// frequency, q, gain, and sample rate
					///
					/// @param frequency - The frequency to use, in Hertz
					/// @param q - The Q to use
					/// @param gain - The gain to use, in Decibels
					/// @param sampleRate - The sample rate to use, in Hertz
					///
					/// @return
					static BiQuadFilter<double> MakeAnalogBell(double frequency, double q = 0.7,
							double gain = 0.0, size_t sampleRate = 44100) noexcept;

					/// @brief Sets the frequency of this filter to the given value
					///
					/// @param frequency - The new frequency, in Hertz
					void setFrequency(double frequency) noexcept;

					/// @brief Returns the frequency of this filter
					///
					/// @return - The frequency, in Hertz
					double getFrequency() const noexcept;

					/// @brief Sets the Q of this filter to the given value
					///
					/// @param q - The new Q
					void setQ(double q) noexcept;

					/// @brief Returns the Q of this filter
					///
					/// @return  - The Q
					double getQ() const noexcept;

					/// @brief Sets the gain of this filter to the given value
					///
					/// @param gain - The new gain, in Decibels
					void setGainDB(double gain) noexcept;

					/// @brief Returns the gain of this filter
					///
					/// @return - The gain, in Decibels
					double getGainDB() const noexcept;

					/// @brief Sets the sample rate of this filter to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the sample rate of this filter
					///
					/// @return - The sample rate, in Hertz
					size_t getSampleRate() const noexcept;

					/// @brief Applies this filter to the given input value
					///
					/// @param input - The input value to apply filtering to
					///
					/// @return - The filtered value
					double process(double input) noexcept;

					/// @brief Applies this filter to the array of given input values, in place
					///
					/// @param input - The array of input values to filter
					/// @param numSamples - The number of samples in the array
					void process(double* input, size_t numSamples) noexcept;

					/// @brief Resets this filter to an initial state
					void reset() noexcept;

					/// @brief Calculates the magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					double getMagnitudeForFrequency(double frequency) const noexcept;

					/// @brief Calculates the magnitude response of this filter for the given array of frequencies and stores it in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					void getMagnitudesForFrequencies(double* frequencies, double* magnitudes,
							size_t numFrequencies) const noexcept;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response at the given frequency
					double getPhaseForFrequency(double frequency) const noexcept;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					void getPhasesForFrequencies(double* frequencies, double* phases,
							size_t numFrequencies) const noexcept;

					BiQuadFilter<double>& operator=(BiQuadFilter<double>&& filt) noexcept = default;

				private:
					double mB0 = 0.0;
					double mB1 = 0.0;
					double mB2 = 0.0;
					double mA0 = 0.0;
					double mA1 = 0.0;
					double mA2 = 0.0;

					double mY1 = 0.0;
					double mY2 = 0.0;
					double mX1 = 0.0;
					double mX2 = 0.0;

					FilterType mType = FilterType::Bell;
					double mFrequency = 1000.0;
					double mQ = 0.7;
					double mGain = 0.0;
					size_t mSampleRate = 44100;

					BiQuadFilter(double frequency, double q, double gain,
							size_t sampleRate, FilterType type) noexcept;

					/// @brief Updates the coefficients of this filter
					void updateCoefficients() noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiQuadFilter)
			};
	}
}
