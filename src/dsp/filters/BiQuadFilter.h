#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
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

	/// @brief Basic BiQuad Filter implementation
	///
	/// @see https://www.musicdsp.org/en/latest/Filters/197-rbj-audio-eq-cookbook.html
	///
	/// @tparam FloatType - The floating point type to back operations, either float or double
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class BiQuadFilter {
	  public:
		/// @brief Constructs a default `BiQuadFilter`
		BiQuadFilter() noexcept {
			updateCoefficients();
		}

		/// @brief Move contructs a `BiQuadFilter` from the given one
		///
		/// @param filt - The `BiQuadFilter<float>` to move
		BiQuadFilter(BiQuadFilter&& filt) noexcept = default;
		~BiQuadFilter() noexcept = default;

		/// @brief Creates a default lowpass BiQuad Filter
		///
		/// @return - A lowpass BiQuad Filter
		[[nodiscard]] static inline auto MakeLowpass() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(20.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowpass(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::Lowpass);
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
		MakeLowpass(Hertz frequency,
					FloatType q = narrow_cast<FloatType>(0.7),
					Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, 0.0_dB, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a default highpass BiQuad filter
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto MakeHighpass() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(20.0_Hz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighpass(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::Highpass);
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
		MakeHighpass(Hertz frequency,
					 FloatType q = narrow_cast<FloatType>(0.7),
					 Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, 0.0_dB, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a default bandpass BiQuad Filter
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto MakeBandpass() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(1.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBandpass(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::Bandpass);
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
		MakeBandpass(Hertz frequency,
					 FloatType q = narrow_cast<FloatType>(0.7),
					 Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, 0.0_dB, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a default allpass BiQuad Filter
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto MakeAllpass() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(1.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0F,
										   44.1_kHz,
										   FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAllpass(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::Allpass);
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
		MakeAllpass(Hertz frequency,
					FloatType q = narrow_cast<FloatType>(0.7),
					Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, 0.0_dB, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates a default notch BiQuad Filter
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto MakeNotch() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(4.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		[[nodiscard]] static inline auto
		MakeNotch(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::Notch);
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
		MakeNotch(Hertz frequency,
				  FloatType q = narrow_cast<FloatType>(0.7),
				  Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, 0.0_dB, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a default lowshelf BiQuad Filter
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto MakeLowShelf() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(300.0_Hz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeLowShelf(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::LowShelf);
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
		MakeLowShelf(Hertz frequency, Decibels gain = 0.0_dB, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   gain,
										   sampleRate,
										   FilterType::LowShelf);
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
		MakeLowShelf(Hertz frequency,
					 FloatType q = narrow_cast<FloatType>(0.7),
					 Decibels gain = 0.0_dB,
					 Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a default highshelf BiQuad Filter
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto MakeHighShelf() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(8.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		[[nodiscard]] static inline auto
		MakeHighShelf(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::HighShelf);
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
		MakeHighShelf(Hertz frequency, Decibels gain = 0.0_dB, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   gain,
										   sampleRate,
										   FilterType::HighShelf);
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
		MakeHighShelf(Hertz frequency,
					  FloatType q = narrow_cast<FloatType>(0.7),
					  Decibels gain = 0.0_dB,
					  Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a default bell-shape BiQuad Filter
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto MakeBell() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(1.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeBell(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::Bell);
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
		MakeBell(Hertz frequency, Decibels gain = 0.0_dB, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   gain,
										   sampleRate,
										   FilterType::Bell);
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
		MakeBell(Hertz frequency,
				 FloatType q = narrow_cast<FloatType>(0.7),
				 Decibels gain = 0.0_dB,
				 Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto MakeAnalogBell() noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(1.0_kHz,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   44.1_kHz,
										   FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		[[nodiscard]] static inline auto
		MakeAnalogBell(Hertz frequency, Hertz sampleRate = 44.1_kHz) noexcept
			-> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   0.0_dB,
										   sampleRate,
										   FilterType::AnalogBell);
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
		MakeAnalogBell(Hertz frequency,
					   Decibels gain = 0.0_dB,
					   Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency,
										   narrow_cast<FloatType>(0.7),
										   gain,
										   sampleRate,
										   FilterType::AnalogBell);
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
		MakeAnalogBell(Hertz frequency,
					   FloatType q = narrow_cast<FloatType>(0.7),
					   Decibels gain = 0.0_dB,
					   Hertz sampleRate = 44.1_kHz) noexcept -> BiQuadFilter<FloatType> {
			return BiQuadFilter<FloatType>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Sets the frequency of this filter to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline auto setFrequency(Hertz frequency) noexcept -> void {
			mFrequency = frequency;
			updateCoefficients();
		}

		/// @brief Returns the frequency of this filter
		///
		/// @return - The frequency, in Hertz
		[[nodiscard]] inline auto getFrequency() const noexcept -> Hertz {
			return mFrequency;
		}

		/// @brief Sets the Q of this filter to the given value
		///
		/// @param q - The new Q
		inline auto setQ(FloatType q) noexcept -> void {
			jassert(q > narrow_cast<FloatType>(0.0));
			mQ = q;
			updateCoefficients();
		}

		/// @brief Returns the Q of this filter
		///
		/// @return  - The Q
		[[nodiscard]] inline auto getQ() const noexcept -> FloatType {
			return mQ;
		}

		/// @brief Sets the gain of this filter to the given value
		///
		/// @param gain - The new gain, in Decibels
		inline auto setGainDB(Decibels gain) noexcept -> void {
			mGain = gain;
			updateCoefficients();
		}

		/// @brief Returns the gain of this filter
		///
		/// @return - The gain, in Decibels
		[[nodiscard]] inline auto getGainDB() const noexcept -> Decibels {
			return mGain;
		}

		/// @brief Sets the sample rate of this filter to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Returns the sample rate of this filter
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		/// @brief Applies this filter to the given input value
		///
		/// @param input - The input value to apply filtering to
		///
		/// @return - The filtered value
		[[nodiscard]] inline auto process(FloatType input) noexcept -> FloatType {
			auto yn = input * mB0 / mA0 + mX1 * mB1 / mA0 + mX2 * mB2 / mA0 - mY1 * mA1 / mA0
					  - mY2 * mA2 / mA0;

			mX2 = mX1;
			mX1 = input;
			mY2 = mY1;
			mY1 = yn;

			return yn;
		}

		/// @brief Applies this filter to the array of given input values, in place
		///
		/// @param input - The array of input values to filter
		/// @param output - The array to store the output values in
		inline auto process(Span<FloatType> input, Span<FloatType> output) noexcept -> void {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = process(input.at(i));
			}
		}

		/// @brief Applies this filter to the array of given input values, in place
		///
		/// @param input - The array of input values to filter
		/// @param output - The array to store the output values in
		inline auto process(Span<const FloatType> input, Span<FloatType> output) noexcept -> void {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = process(input.at(i));
			}
		}

		/// @brief Resets this filter to an initial state
		inline auto reset() noexcept -> void {
			mY1 = narrow_cast<FloatType>(0.0);
			mY2 = narrow_cast<FloatType>(0.0);
			mX1 = narrow_cast<FloatType>(0.0);
			mX2 = narrow_cast<FloatType>(0.0);
		}

		/// @brief Calculates the magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] inline auto
		getMagnitudeForFrequency(Hertz frequency) const noexcept -> FloatType {
			auto one = narrow_cast<FloatType>(1.0);
			constexpr std::complex<FloatType> j(narrow_cast<FloatType>(0.0), one);
			const size_t order = 2;
			const std::array<FloatType, 5> coefficients
				= {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

			jassert(frequency >= 0.0_Hz
					&& frequency
						   <= narrow_cast<FloatType>(mSampleRate) * narrow_cast<FloatType>(0.5));

			std::complex<FloatType> numerator = narrow_cast<FloatType>(0.0);
			std::complex<FloatType> denominator = one;
			std::complex<FloatType> factor = one;
			std::complex<FloatType> jw
				= std::exp(-Constants<FloatType>::twoPi * narrow_cast<FloatType>(frequency) * j
						   / narrow_cast<FloatType>(mSampleRate));

			for(size_t n = 0; n <= order; ++n) {
				numerator += coefficients.at(n) * factor;
				factor *= jw;
			}

			factor = jw;

			for(size_t n = order + 1; n <= 2 * order; ++n) {
				denominator += coefficients.at(n) * factor;
				factor *= jw;
			}

			return std::abs(numerator / denominator);
		}

		/// @brief Calculates the magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @tparam numFrequencies - The number of frequencies in the `frequencies` array
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		template<size_t numFrequencies>
		inline auto getMagnitudesForFrequencies(Span<const Hertz> frequencies,
												Span<FloatType> magnitudes) const noexcept -> void {
			auto size = narrow_cast<gsl::index>(numFrequencies);
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				magnitudes.at(frequency) = getMagnitudeForFrequency(frequencies.at(frequency));
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response at the given frequency
		[[nodiscard]] inline auto getPhaseForFrequency(Hertz frequency) const noexcept -> Radians {
			auto one = narrow_cast<FloatType>(1.0);
			constexpr std::complex<FloatType> j(narrow_cast<FloatType>(0.0), one);
			const size_t order = 2;
			const std::array<FloatType, 5> coefficients
				= {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

			jassert(frequency >= 0.0_Hz
					&& frequency
						   <= narrow_cast<FloatType>(mSampleRate) * narrow_cast<FloatType>(0.5));

			std::complex<FloatType> numerator = narrow_cast<FloatType>(0.0);
			std::complex<FloatType> denominator = one;
			std::complex<FloatType> factor = one;
			std::complex<FloatType> jw
				= std::exp(-Constants<FloatType>::twoPi * gsl::narrow_cast<FloatType>(frequency) * j
						   / narrow_cast<FloatType>(mSampleRate));

			for(size_t n = 0; n <= order; ++n) {
				numerator += coefficients.at(n) * factor;
				factor *= jw;
			}

			factor = jw;

			for(size_t n = order + 1; n <= 2 * order; ++n) {
				denominator += coefficients.at(n) * factor;
				factor *= jw;
			}

			return std::arg(numerator / denominator);
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @tparam numFrequencies - The number of frequencies in the `frequencies` array
		//
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases in
		template<size_t numFrequencies>
		auto
		getPhasesForFrequencies(Span<const Hertz> frequencies, Span<Radians> phases) const noexcept
			-> void {
			auto size = narrow_cast<gsl::index>(numFrequencies);
			for(gsl::index frequency = 0; frequency < size; ++frequency) {
				phases.at(frequency) = getPhaseForFrequency(frequencies.at(frequency));
			}
		}

		auto operator=(BiQuadFilter&& filt) noexcept -> BiQuadFilter& = default;

	  private:
		FloatType mB0 = narrow_cast<FloatType>(0.0);
		FloatType mB1 = narrow_cast<FloatType>(0.0);
		FloatType mB2 = narrow_cast<FloatType>(0.0);
		FloatType mA0 = narrow_cast<FloatType>(0.0);
		FloatType mA1 = narrow_cast<FloatType>(0.0);
		FloatType mA2 = narrow_cast<FloatType>(0.0);

		FloatType mY1 = narrow_cast<FloatType>(0.0);
		FloatType mY2 = narrow_cast<FloatType>(0.0);
		FloatType mX1 = narrow_cast<FloatType>(0.0);
		FloatType mX2 = narrow_cast<FloatType>(0.0);

		FilterType mType = FilterType::Bell;
		Hertz mFrequency = 1.0_kHz;
		FloatType mQ = narrow_cast<FloatType>(0.7);
		Decibels mGain = 0.0_dB;
		Hertz mSampleRate = 44.1_kHz;

		BiQuadFilter(Hertz frequency,
					 FloatType q,
					 Decibels gain,
					 Hertz sampleRate,
					 FilterType type) noexcept
			: mType(type), mFrequency(frequency), mQ(q), mGain(gain), mSampleRate(sampleRate) {
			updateCoefficients();
		}

		/// @brief Updates the coefficients of this filter
		inline auto updateCoefficients() noexcept -> void {
			auto one = narrow_cast<FloatType>(1.0);
			auto two = narrow_cast<FloatType>(2.0);
			auto w0 = narrow_cast<FloatType>(two * Constants<FloatType>::pi * mFrequency
											 / narrow_cast<FloatType>(mSampleRate));
			auto cosw0 = Trig<FloatType>::cos(w0);
			auto sinw0 = Trig<FloatType>::sin(w0);
			auto alpha = sinw0 / (two * mQ);
			auto a = Exponentials<FloatType>::pow10(narrow_cast<FloatType>(mGain)
													/ narrow_cast<FloatType>(40.0));
			auto twoSqrtAAlpha = narrow_cast<FloatType>(0.0);

			if(mType == FilterType::AnalogBell) {
				alpha = sinw0 / (two * mQ * a);
			}

			if(mType == FilterType::LowShelf || mType == FilterType::HighShelf) {
				twoSqrtAAlpha = two * General<FloatType>::sqrt(a) * alpha;
			}

			switch(mType) {
				case FilterType::Lowpass:
					{
						mB0 = (one - cosw0) / two;
						mB1 = one - cosw0;
						mB2 = mB0;
						mA0 = one + alpha;
						mA1 = -two * cosw0;
						mA2 = one - alpha;
					}
					break;
				case FilterType::Highpass:
					{
						mB0 = (one + cosw0) / two;
						mB1 = -(one + cosw0);
						mB2 = mB0;
						mA0 = one + alpha;
						mA1 = -two * cosw0;
						mA2 = one - alpha;
					}
					break;
				case FilterType::Bandpass:
					{
						mB0 = alpha;
						mB1 = 0.0F;
						mB2 = -alpha;
						mA0 = one + alpha;
						mA1 = -two * cosw0;
						mA2 = one - alpha;
					}
					break;
				case FilterType::Allpass:
					{
						mB0 = one - alpha;
						mB1 = -two * cosw0;
						mB2 = one + alpha;
						mA0 = mB2;
						mA1 = mB1;
						mA2 = mB0;
					}
					break;
				case FilterType::Notch:
					{
						mB0 = one;
						mB1 = -two * cosw0;
						mB2 = one;
						mA0 = one + alpha;
						mA1 = mB1;
						mA2 = one - alpha;
					}
					break;
				case FilterType::LowShelf:
					{
						mB0 = a * ((a + one) - (a - one) * cosw0 + twoSqrtAAlpha);
						mB1 = two * a * ((a - one) + (a + one) * cosw0);
						mB2 = a * ((a + one) - (a - one) * cosw0 - twoSqrtAAlpha);
						mA0 = (a + one) - (a - one) * cosw0 + twoSqrtAAlpha;
						mA1 = -two * ((a - one) - (a + one) * cosw0);
						mA2 = (a + one) - (a - one) * cosw0 - twoSqrtAAlpha;
					}
					break;
				case FilterType::HighShelf:
					{
						mB0 = a * ((a + one) - (a - one) * cosw0 + twoSqrtAAlpha);
						mB1 = -two * a * ((a - one) + (a + one) * cosw0);
						mB2 = a * ((a + one) - (a - one) * cosw0 - twoSqrtAAlpha);
						mA0 = (a + one) - (a - one) * cosw0 + twoSqrtAAlpha;
						mA1 = two * ((a - one) - (a + one) * cosw0);
						mA2 = (a + one) - (a - one) * cosw0 - twoSqrtAAlpha;
					}
					break;
				case FilterType::Bell:
					{
						mB0 = one + alpha * a;
						mB1 = -two * cosw0;
						mB2 = one - alpha * a;
						mA0 = one + alpha / a;
						mA1 = mB1;
						mA2 = one - alpha / a;
					}
					break;
				case FilterType::AnalogBell:
					{
						mB0 = one + alpha * a;
						mB1 = -two * cosw0;
						mB2 = one - alpha * a;
						mA0 = one + alpha / a;
						mA1 = mB1;
						mA2 = one - alpha / a;
					}
					break;
			}
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiQuadFilter)
	};
} // namespace apex::dsp
