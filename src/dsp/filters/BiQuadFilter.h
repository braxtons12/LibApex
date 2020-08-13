#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	/// @brief Basic BiQuad Filter implementation
	///
	/// @see https://www.musicdsp.org/en/latest/Filters/197-rbj-audio-eq-cookbook.html
	///
	/// @tparam T - The floating point type to back operations, either float or double
	template<typename T>
	class BiQuadFilter {
	  public:
		static_assert(std::is_floating_point<T>::value,
					  "T must be a floating point type (float or double)");

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
		enum class FilterType
		{
			Lowpass,
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
		[[nodiscard]] static inline auto MakeLowpass() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(20000.0F, 0.7F, 0.0F, 44100, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowpass(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowpass(float frequency, float q = 0.7F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a default highpass BiQuad filter
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto MakeHighpass() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(20.0F, 0.7F, 0.0F, 44100, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighpass(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighpass(float frequency, float q = 0.7F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a default bandpass BiQuad Filter
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto MakeBandpass() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(2000.0F, 0.7F, 0.0F, 44100, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBandpass(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBandpass(float frequency, float q = 0.7F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a default allpass BiQuad Filter
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto MakeAllpass() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(2000.0F, 0.7F, 0.0F, 44100, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAllpass(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAllpass(float frequency, float q = 0.7F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates a default notch BiQuad Filter
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto MakeNotch() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(4000.0F, 0.7F, 0.0F, 44100, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto
		MakeNotch(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto
		MakeNotch(float frequency, float q = 0.7F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a default lowshelf BiQuad Filter
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto MakeLowShelf() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(300.0F, 0.7F, 0.0F, 44100, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(float frequency, float gain = 0.0F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency, q,
		/// gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(float frequency,
					 float q = 0.7F,
					 float gain = 0.0F,
					 size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a default highshelf BiQuad Filter
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto MakeHighShelf() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(3000.0F, 0.7F, 0.0F, 44100, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighShelf(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighShelf(float frequency, float gain = 0.0F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency, q,
		/// gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static auto
		MakeHighShelf(float frequency,
					  float q = 0.7F,
					  float gain = 0.0F,
					  size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a default bell-shape BiQuad Filter
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto MakeBell() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(1000.0F, 0.7F, 0.0F, 44100, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(float frequency, float gain = 0.0F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency, q,
		/// gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(float frequency,
				 float q = 0.7F,
				 float gain = 0.0F,
				 size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto MakeAnalogBell() noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(1000.0F, 0.7F, 0.0F, 44100, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAnalogBell(float frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, 0.0F, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency, gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAnalogBell(float frequency, float gain = 0.0F, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, 0.7F, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency, q, gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return
		[[nodiscard]] static inline auto
		MakeAnalogBell(float frequency,
					   float q = 0.7F,
					   float gain = 0.0F,
					   size_t sampleRate = 44100) noexcept -> BiQuadFilter<float> {
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Sets the frequency of this filter to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline auto setFrequency(float frequency) noexcept -> void {
			mFrequency = frequency;
			updateCoefficients();
		}

		/// @brief Returns the frequency of this filter
		///
		/// @return - The frequency, in Hertz
		[[nodiscard]] inline auto getFrequency() const noexcept -> float {
			return mFrequency;
		}

		/// @brief Sets the Q of this filter to the given value
		///
		/// @param q - The new Q
		inline auto setQ(float q) noexcept -> void {
			mQ = q;
			updateCoefficients();
		}

		/// @brief Returns the Q of this filter
		///
		/// @return  - The Q
		[[nodiscard]] inline auto getQ() const noexcept -> float {
			return mQ;
		}

		/// @brief Sets the gain of this filter to the given value
		///
		/// @param gain - The new gain, in Decibels
		inline auto setGainDB(float gain) noexcept -> void {
			mGain = gain;
			updateCoefficients();
		}

		/// @brief Returns the gain of this filter
		///
		/// @return - The gain, in Decibels
		[[nodiscard]] inline auto getGainDB() const noexcept -> float {
			return mGain;
		}

		/// @brief Sets the sample rate of this filter to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline auto setSampleRate(size_t sampleRate) noexcept -> void {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Returns the sample rate of this filter
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] inline auto getSampleRate() const noexcept -> size_t {
			return mSampleRate;
		}

		/// @brief Applies this filter to the given input value
		///
		/// @param input - The input value to apply filtering to
		///
		/// @return - The filtered value
		[[nodiscard]] auto process(float input) noexcept -> float;

		/// @brief Applies this filter to the array of given input values, in place
		///
		/// @param input - The array of input values to filter
		auto process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void;

		/// @brief Resets this filter to an initial state
		auto reset() noexcept -> void;

		/// @brief Calculates the magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto getMagnitudeForFrequency(float frequency) const noexcept -> float;

		/// @brief Calculates the magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @tparam numFrequencies - The number of frequencies in the `frequencies` array
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		template<size_t numFrequencies>
		auto getMagnitudesForFrequencies(gsl::span<const float, numFrequencies> frequencies,
										 gsl::span<float, numFrequencies> magnitudes) const noexcept
			-> void {
			auto size = static_cast<gsl::index>(numFrequencies);
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(magnitudes, frequency)
					= getMagnitudeForFrequency(gsl::at(frequencies, frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response at the given frequency
		[[nodiscard]] auto getPhaseForFrequency(float frequency) const noexcept -> float;

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @tparam numFrequencies - The number of frequencies in the `frequencies` array
		//
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases in
		template<size_t numFrequencies>
		auto
		getPhasesForFrequencies(gsl::span<const float, numFrequencies> frequencies,
								gsl::span<float, numFrequencies> phases) const noexcept -> void {
			auto size = static_cast<gsl::index>(numFrequencies);
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(phases, frequency) = getPhaseForFrequency(gsl::at(frequencies, frequency));
			}
		}

		auto operator=(BiQuadFilter<float>&& filt) noexcept -> BiQuadFilter<float>& = default;

	  private:
		float mB0 = 0.0F;
		float mB1 = 0.0F;
		float mB2 = 0.0F;
		float mA0 = 0.0F;
		float mA1 = 0.0F;
		float mA2 = 0.0F;

		float mY1 = 0.0F;
		float mY2 = 0.0F;
		float mX1 = 0.0F;
		float mX2 = 0.0F;

		FilterType mType = FilterType::Bell;
		float mFrequency = 1000.0F;
		float mQ = 0.7F;
		float mGain = 0.0F;
		size_t mSampleRate = 44100;

		BiQuadFilter(float frequency,
					 float q,
					 float gain,
					 size_t sampleRate,
					 FilterType type) noexcept;

		/// @brief Updates the coefficients of this filter
		auto updateCoefficients() noexcept -> void;

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
		enum FilterType
		{
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
		[[nodiscard]] static inline auto MakeLowpass() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(20000.0, 0.7, 0.0, 44100, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowpass(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowpass(double frequency, double q = 0.7, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a default highpass BiQuad filter
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto MakeHighpass() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(20.0, 0.7, 0.0, 44100, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighpass(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighpass(double frequency, double q = 0.7, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a default bandpass BiQuad Filter
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto MakeBandpass() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(2000.0, 0.7, 0.0, 44100, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBandpass(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBandpass(double frequency, double q = 0.7, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a default allpass BiQuad Filter
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto MakeAllpass() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(2000.0, 0.7, 0.0, 44100, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAllpass(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAllpass(double frequency, double q = 0.7, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates a default notch BiQuad Filter
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto MakeNotch() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(4000.0, 0.7, 0.0, 44100, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto
		MakeNotch(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto
		MakeNotch(double frequency, double q = 0.7, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a default lowshelf BiQuad Filter
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto MakeLowShelf() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(300.0, 0.7, 0.0, 44100, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(double frequency, double gain = 0.0, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency, q,
		/// gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(double frequency,
					 double q = 0.7,
					 double gain = 0.0,
					 size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a default highshelf BiQuad Filter
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto MakeHighShelf() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(3000.0, 0.7, 0.0, 44100, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighShelf(double frequency, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighShelf(double frequency, double gain = 0.0, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency, q,
		/// gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighShelf(double frequency,
					  double q = 0.7,
					  double gain = 0.0,
					  size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a default bell-shape BiQuad Filter
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto MakeBell() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(1000.0, 0.7, 0.0, 44100, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(double frequency, size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(double frequency, double gain = 0.0, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency, q,
		/// gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(double frequency,
				 double q = 0.7,
				 double gain = 0.0,
				 size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto MakeAnalogBell() noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(1000.0, 0.7, 0.0, 44100, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAnalogBell(double frequency, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, 0.0, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency, gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAnalogBell(double frequency, double gain = 0.0, size_t sampleRate = 44100) noexcept
			-> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, 0.7, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency, q, gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return
		[[nodiscard]] static inline auto
		MakeAnalogBell(double frequency,
					   double q = 0.7,
					   double gain = 0.0,
					   size_t sampleRate = 44100) noexcept -> BiQuadFilter<double> {
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Sets the frequency of this filter to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline auto setFrequency(double frequency) noexcept -> void {
			mFrequency = frequency;
			updateCoefficients();
		}

		/// @brief Returns the frequency of this filter
		///
		/// @return - The frequency, in Hertz
		[[nodiscard]] inline auto getFrequency() const noexcept -> double {
			return mFrequency;
		}

		/// @brief Sets the Q of this filter to the given value
		///
		/// @param q - The new Q
		inline auto setQ(double q) noexcept -> void {
			mQ = q;
			updateCoefficients();
		}

		/// @brief Returns the Q of this filter
		///
		/// @return  - The Q
		[[nodiscard]] inline auto getQ() const noexcept -> double {
			return mQ;
		}

		/// @brief Sets the gain of this filter to the given value
		///
		/// @param gain - The new gain, in Decibels
		inline auto setGainDB(double gain) noexcept -> void {
			mGain = gain;
			updateCoefficients();
		}

		/// @brief Returns the gain of this filter
		///
		/// @return - The gain, in Decibels
		[[nodiscard]] inline auto getGainDB() const noexcept -> double {
			return mGain;
		}

		/// @brief Sets the sample rate of this filter to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline auto setSampleRate(size_t sampleRate) noexcept -> void {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Returns the sample rate of this filter
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] inline auto getSampleRate() const noexcept -> size_t {
			return mSampleRate;
		}

		/// @brief Applies this filter to the given input value
		///
		/// @param input - The input value to apply filtering to
		///
		/// @return - The filtered value
		[[nodiscard]] auto process(double input) noexcept -> double;

		/// @brief Applies this filter to the array of given input values, in place
		///
		/// @param input - The array of input values to filter
		/// @param numSamples - The number of samples in the array
		auto process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void;

		/// @brief Resets this filter to an initial state
		auto reset() noexcept -> void;

		/// @brief Calculates the magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto getMagnitudeForFrequency(double frequency) const noexcept -> double;

		/// @brief Calculates the magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @tparam numFrequencies - The number of frequencies in the `frequencies` array
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		template<size_t numFrequencies>
		auto
		getMagnitudesForFrequencies(gsl::span<const double, numFrequencies> frequencies,
									gsl::span<double, numFrequencies> magnitudes) const noexcept
			-> void {
			auto size = static_cast<gsl::index>(numFrequencies);
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(magnitudes, frequency)
					= getMagnitudeForFrequency(gsl::at(frequencies, frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response at the given frequency
		[[nodiscard]] auto getPhaseForFrequency(double frequency) const noexcept -> double;

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @tparam numFrequencies - The number of frequencies in the `frequencies` array
		//
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases in
		template<size_t numFrequencies>
		auto
		getPhasesForFrequencies(gsl::span<const double, numFrequencies> frequencies,
								gsl::span<double, numFrequencies> phases) const noexcept -> void {
			auto size = static_cast<gsl::index>(numFrequencies);
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				gsl::at(phases, frequency) = getPhaseForFrequency(gsl::at(frequencies, frequency));
			}
		}

		auto operator=(BiQuadFilter<double>&& filt) noexcept -> BiQuadFilter<double>& = default;

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

		BiQuadFilter(double frequency,
					 double q,
					 double gain,
					 size_t sampleRate,
					 FilterType type) noexcept;

		/// @brief Updates the coefficients of this filter
		auto updateCoefficients() noexcept -> void;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiQuadFilter)
	};
} // namespace apex::dsp
