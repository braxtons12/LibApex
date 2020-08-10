#include "BiQuadFilter.h"

#include <array>
#include <complex>

namespace apex::dsp {
	/// @brief Constructs a default `BiQuadFilter`
	BiQuadFilter<float>::BiQuadFilter() noexcept {
		updateCoefficients();
	}

	/// @brief Creates a default lowpass BiQuad Filter
	///
	/// @return - A lowpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeLowpass() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(20000.0F, 0.7F, 0.0F, 44100, FilterType::Lowpass);
	}

	/// @brief Creates a lowpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A lowpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeLowpass(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeLowpass(float frequency, float q, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Lowpass);
	}

	/// @brief Creates a default highpass BiQuad filter
	///
	/// @return - A highpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeHighpass() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(20.0F, 0.7F, 0.0F, 44100, FilterType::Highpass);
	}

	/// @brief Creates a highpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A highpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeHighpass(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeHighpass(float frequency, float q, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Highpass);
	}

	/// @brief Creates a default bandpass BiQuad Filter
	///
	/// @return - A bandpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeBandpass() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(2000.0F, 0.7F, 0.0F, 44100, FilterType::Bandpass);
	}

	/// @brief Creates a bandpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A bandpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeBandpass(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeBandpass(float frequency, float q, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Bandpass);
	}

	/// @brief Creates a default allpass BiQuad Filter
	///
	/// @return - An allpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeAllpass() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(2000.0F, 0.7F, 0.0F, 44100, FilterType::Allpass);
	}

	/// @brief Creates an allpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - An allpass BiQuad Filter
	inline auto BiQuadFilter<float>::MakeAllpass(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeAllpass(float frequency, float q, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Allpass);
	}

	/// @brief Creates a default notch BiQuad Filter
	///
	/// @return - A notch BiQuad Filter
	inline auto BiQuadFilter<float>::MakeNotch() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(4000.0F, 0.7F, 0.0F, 44100, FilterType::Notch);
	}

	/// @brief Creates a notch BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A notch BiQuad Filter
	inline auto BiQuadFilter<float>::MakeNotch(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto BiQuadFilter<float>::MakeNotch(float frequency, float q, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, 0.0F, sampleRate, FilterType::Notch);
	}

	/// @brief Creates a default lowshelf BiQuad Filter
	///
	/// @return - A lowshelf BiQuad Filter
	inline auto BiQuadFilter<float>::MakeLowShelf() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(300.0F, 0.7F, 0.0F, 44100, FilterType::LowShelf);
	}

	/// @brief Creates a lowshelf BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A lowshelf BiQuad Filter
	inline auto BiQuadFilter<float>::MakeLowShelf(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeLowShelf(float frequency, float gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<float>::MakeLowShelf(float frequency,
									  float q,
									  float gain,
									  size_t sampleRate) noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::LowShelf);
	}

	/// @brief Creates a default highshelf BiQuad Filter
	///
	/// @return - A highshelf BiQuad Filter
	inline auto BiQuadFilter<float>::MakeHighShelf() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(3000.0F, 0.7F, 0.0F, 44100, FilterType::HighShelf);
	}

	/// @brief Creates a highshelf BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A highshelf BiQuad Filter
	inline auto BiQuadFilter<float>::MakeHighShelf(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeHighShelf(float frequency, float gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<float>::MakeHighShelf(float frequency,
									   float q,
									   float gain,
									   size_t sampleRate) noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::HighShelf);
	}

	/// @brief Creates a default bell-shape BiQuad Filter
	///
	/// @return - A bell-shape BiQuad Filter
	inline auto BiQuadFilter<float>::MakeBell() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(1000.0F, 0.7F, 0.0F, 44100, FilterType::Bell);
	}

	/// @brief Creates a bell-shape BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A bell-shape BiQuad Filter
	inline auto BiQuadFilter<float>::MakeBell(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeBell(float frequency, float gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<float>::MakeBell(float frequency, float q, float gain, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::Bell);
	}

	/// @brief Creates an analog-style bell-shape BiQuad Filter
	///
	/// @return - An analog-style bell-shape BiQuad Filter
	inline auto BiQuadFilter<float>::MakeAnalogBell() noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(1000.0F, 0.7F, 0.0F, 44100, FilterType::AnalogBell);
	}

	/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
	/// frequency and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - An analog-style bell-shape BiQuad Filter
	inline auto BiQuadFilter<float>::MakeAnalogBell(float frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<float> {
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
	inline auto
	BiQuadFilter<float>::MakeAnalogBell(float frequency, float gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<float>::MakeAnalogBell(float frequency,
										float q,
										float gain,
										size_t sampleRate) noexcept -> BiQuadFilter<float> {
		return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
	}

	/// @brief Sets the frequency of this filter to the given value
	///
	/// @param frequency - The new frequency, in Hertz
	inline auto BiQuadFilter<float>::setFrequency(float frequency) noexcept -> void {
		mFrequency = frequency;
		updateCoefficients();
	}

	/// @brief Returns the frequency of this filter
	///
	/// @return - The frequency, in Hertz
	inline auto BiQuadFilter<float>::getFrequency() const noexcept -> float {
		return mFrequency;
	}

	/// @brief Sets the Q of this filter to the given value
	///
	/// @param q - The new Q
	inline auto BiQuadFilter<float>::setQ(float q) noexcept -> void {
		mQ = q;
		updateCoefficients();
	}

	/// @brief Returns the Q of this filter
	///
	/// @return  - The Q
	inline auto BiQuadFilter<float>::getQ() const noexcept -> float {
		return mQ;
	}

	/// @brief Sets the gain of this filter to the given value
	///
	/// @param gain - The new gain, in Decibels
	inline auto BiQuadFilter<float>::setGainDB(float gain) noexcept -> void {
		mGain = gain;
		updateCoefficients();
	}

	/// @brief Returns the gain of this filter
	///
	/// @return - The gain, in Decibels
	inline auto BiQuadFilter<float>::getGainDB() const noexcept -> float {
		return mGain;
	}

	/// @brief Sets the sample rate of this filter to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	inline auto BiQuadFilter<float>::setSampleRate(size_t sampleRate) noexcept -> void {
		mSampleRate = sampleRate;
		updateCoefficients();
	}

	/// @brief Returns the sample rate of this filter
	///
	/// @return - The sample rate, in Hertz
	inline auto BiQuadFilter<float>::getSampleRate() const noexcept -> size_t {
		return mSampleRate;
	}

	/// @brief Applies this filter to the given input value
	///
	/// @param input - The input value to apply filtering to
	///
	/// @return - The filtered value
	auto BiQuadFilter<float>::process(float input) noexcept -> float {
		float yn = input * mB0 / mA0 + mX1 * mB1 / mA0 + mX2 * mB2 / mA0 - mY1 * mA1 / mA0
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
	auto
	BiQuadFilter<float>::process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void {
		for(auto& in : input) {
			in = process(in);
		}
	}

	/// @brief Resets this filter to an initial state
	auto BiQuadFilter<float>::reset() noexcept -> void {
		mB0 = 0.0F;
		mB1 = 0.0F;
		mB2 = 0.0F;
		mA0 = 0.0F;
		mA1 = 0.0F;
		mA2 = 0.0F;

		mY1 = 0.0F;
		mY2 = 0.0F;
		mX1 = 0.0F;
		mX2 = 0.0F;
	}

	/// @brief Calculates the magnitude response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
	///
	/// @return - The magnitude response at the given frequency
	auto BiQuadFilter<float>::getMagnitudeForFrequency(float frequency) const noexcept -> float {
		constexpr std::complex<float> j(0.0F, 1.0F);
		const size_t order = 2;
		const std::array<float, 5> coefficients
			= {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

		jassert(frequency >= 0 && frequency <= mSampleRate * 0.5F);

		std::complex<float> numerator = 0.0F;
		std::complex<float> denominator = 1.0F;
		std::complex<float> factor = 1.0F;
		std::complex<float> jw
			= std::exp(-math::twoPif * frequency * j / static_cast<float>(mSampleRate));

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

	/// @brief Calculates the phase response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the phase response for, in Hertz
	///
	/// @return - The phase response at the given frequency
	auto BiQuadFilter<float>::getPhaseForFrequency(float frequency) const noexcept -> float {
		constexpr std::complex<float> j(0.0F, 1.0F);
		const size_t order = 2;
		const std::array<float, 5> coefficients
			= {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

		jassert(frequency >= 0 && frequency <= mSampleRate * 0.5F);

		std::complex<float> numerator = 0.0F;
		std::complex<float> denominator = 1.0F;
		std::complex<float> factor = 1.0F;
		std::complex<float> jw
			= std::exp(-math::twoPif * frequency * j / static_cast<float>(mSampleRate));

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

	BiQuadFilter<float>::BiQuadFilter(float frequency,
									  float q,
									  float gain,
									  size_t sampleRate,
									  FilterType type) noexcept
		: mType(type), mFrequency(frequency), mQ(q), mGain(gain), mSampleRate(sampleRate) {
		updateCoefficients();
	}

	/// @brief Updates the coefficients of this filter
	auto BiQuadFilter<float>::updateCoefficients() noexcept -> void {
		float w0 = 2.0F * math::pif * mFrequency / mSampleRate;
		float cosw0 = math::cosf(w0);
		float sinw0 = math::sinf(w0);
		float alpha = sinw0 / (2.0F * mQ);
		float a = math::pow10f(mGain / 40.0F);
		float twoSqrtAAlpha = 0.0F;

		if(mType == FilterType::AnalogBell) {
			alpha = sinw0 / (2.0F * mQ * a);
		}

		if(mType == FilterType::LowShelf || mType == FilterType::HighShelf) {
			twoSqrtAAlpha = 2.0F * math::sqrtf(a) * alpha;
		}

		switch(mType) {
			case FilterType::Lowpass:
				{
					mB0 = (1.0F - cosw0) / 2.0F;
					mB1 = 1.0F - cosw0;
					mB2 = mB0;
					mA0 = 1.0F + alpha;
					mA1 = -2.0F * cosw0;
					mA2 = 1.0F - alpha;
				}
				break;
			case FilterType::Highpass:
				{
					mB0 = (1.0F + cosw0) / 2.0F;
					mB1 = -(1.0F + cosw0);
					mB2 = mB0;
					mA0 = 1.0F + alpha;
					mA1 = -2.0F * cosw0;
					mA2 = 1.0F - alpha;
				}
				break;
			case FilterType::Bandpass:
				{
					mB0 = alpha;
					mB1 = 0.0F;
					mB2 = -alpha;
					mA0 = 1.0F + alpha;
					mA1 = -2.0F * cosw0;
					mA2 = 1.0F - alpha;
				}
				break;
			case FilterType::Allpass:
				{
					mB0 = 1.0F - alpha;
					mB1 = -2.0F * cosw0;
					mB2 = 1.0F + alpha;
					mA0 = mB2;
					mA1 = mB1;
					mA2 = mB0;
				}
				break;
			case FilterType::Notch:
				{
					mB0 = 1.0F;
					mB1 = -2.0F * cosw0;
					mB2 = 1.0F;
					mA0 = 1.0F + alpha;
					mA1 = mB1;
					mA2 = 1.0F - alpha;
				}
				break;
			case FilterType::LowShelf:
				{
					mB0 = a * ((a + 1.0F) - (a - 1.0F) * cosw0 + twoSqrtAAlpha);
					mB1 = 2.0F * a * ((a - 1.0F) + (a + 1.0F) * cosw0);
					mB2 = a * ((a + 1.0F) - (a - 1.0F) * cosw0 - twoSqrtAAlpha);
					mA0 = (a + 1.0F) - (a - 1.0F) * cosw0 + twoSqrtAAlpha;
					mA1 = -2.0F * ((a - 1.0F) - (a + 1.0F) * cosw0);
					mA2 = (a + 1.0F) - (a - 1.0F) * cosw0 - twoSqrtAAlpha;
				}
				break;
			case FilterType::HighShelf:
				{
					mB0 = a * ((a + 1.0F) - (a - 1.0F) * cosw0 + twoSqrtAAlpha);
					mB1 = -2.0F * a * ((a - 1.0F) + (a + 1.0F) * cosw0);
					mB2 = a * ((a + 1.0F) - (a - 1.0F) * cosw0 - twoSqrtAAlpha);
					mA0 = (a + 1.0F) - (a - 1.0F) * cosw0 + twoSqrtAAlpha;
					mA1 = 2.0F * ((a - 1.0F) - (a + 1.0F) * cosw0);
					mA2 = (a + 1.0F) - (a - 1.0F) * cosw0 - twoSqrtAAlpha;
				}
				break;
			case FilterType::Bell:
				{
					mB0 = 1.0F + alpha * a;
					mB1 = -2.0F * cosw0;
					mB2 = 1.0F - alpha * a;
					mA0 = 1.0F + alpha / a;
					mA1 = mB1;
					mA2 = 1.0F - alpha / a;
				}
				break;
			case FilterType::AnalogBell:
				{
					mB0 = 1.0F + alpha * a;
					mB1 = -2.0F * cosw0;
					mB2 = 1.0F - alpha * a;
					mA0 = 1.0F + alpha / a;
					mA1 = mB1;
					mA2 = 1.0F - alpha / a;
				}
				break;
		}
	}

	/// @brief Constructs a default `BiQuadFilter`
	BiQuadFilter<double>::BiQuadFilter() noexcept {
		updateCoefficients();
	}

	/// @brief Creates a default lowpass BiQuad Filter
	///
	/// @return - A lowpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeLowpass() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(20000.0, 0.7, 0.0, 44100, FilterType::Lowpass);
	}

	/// @brief Creates a lowpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A lowpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeLowpass(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeLowpass(double frequency, double q, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Lowpass);
	}

	/// @brief Creates a default highpass BiQuad filter
	///
	/// @return - A highpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeHighpass() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(20.0, 0.7, 0.0, 44100, FilterType::Highpass);
	}

	/// @brief Creates a highpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A highpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeHighpass(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeHighpass(double frequency, double q, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Highpass);
	}

	/// @brief Creates a default bandpass BiQuad Filter
	///
	/// @return - A bandpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeBandpass() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(2000.0, 0.7, 0.0, 44100, FilterType::Bandpass);
	}

	/// @brief Creates a bandpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A bandpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeBandpass(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeBandpass(double frequency, double q, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Bandpass);
	}

	/// @brief Creates a default allpass BiQuad Filter
	///
	/// @return - An allpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeAllpass() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(2000.0, 0.7, 0.0, 44100, FilterType::Allpass);
	}

	/// @brief Creates an allpass BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - An allpass BiQuad Filter
	inline auto BiQuadFilter<double>::MakeAllpass(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeAllpass(double frequency, double q, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Allpass);
	}

	/// @brief Creates a default notch BiQuad Filter
	///
	/// @return - A notch BiQuad Filter
	inline auto BiQuadFilter<double>::MakeNotch() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(4000.0, 0.7, 0.0, 44100, FilterType::Notch);
	}

	/// @brief Creates a notch BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A notch BiQuad Filter
	inline auto BiQuadFilter<double>::MakeNotch(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeNotch(double frequency, double q, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Notch);
	}

	/// @brief Creates a default lowshelf BiQuad Filter
	///
	/// @return - A lowshelf BiQuad Filter
	inline auto BiQuadFilter<double>::MakeLowShelf() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(300.0, 0.7, 0.0, 44100, FilterType::LowShelf);
	}

	/// @brief Creates a lowshelf BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A lowshelf BiQuad Filter
	inline auto BiQuadFilter<double>::MakeLowShelf(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeLowShelf(double frequency, double gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<double>::MakeLowShelf(double frequency,
									   double q,
									   double gain,
									   size_t sampleRate) noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::LowShelf);
	}

	/// @brief Creates a default highshelf BiQuad Filter
	///
	/// @return - A highshelf BiQuad Filter
	inline auto BiQuadFilter<double>::MakeHighShelf() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(3000.0, 0.7, 0.0, 44100, FilterType::HighShelf);
	}

	/// @brief Creates a highshelf BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A highshelf BiQuad Filter
	inline auto BiQuadFilter<double>::MakeHighShelf(double frequency, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<double>::MakeHighShelf(double frequency, double gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<double>::MakeHighShelf(double frequency,
										double q,
										double gain,
										size_t sampleRate) noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::HighShelf);
	}

	/// @brief Creates a default bell-shape BiQuad Filter
	///
	/// @return - A bell-shape BiQuad Filter
	inline auto BiQuadFilter<double>::MakeBell() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(1000.0, 0.7, 0.0, 44100, FilterType::Bell);
	}

	/// @brief Creates a bell-shape BiQuad Filter with the given frequency
	/// and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - A bell-shape BiQuad Filter
	inline auto BiQuadFilter<double>::MakeBell(double frequency, size_t sampleRate) noexcept
		-> BiQuadFilter<double> {
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
	inline auto
	BiQuadFilter<double>::MakeBell(double frequency, double gain, size_t sampleRate) noexcept
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
	inline auto BiQuadFilter<double>::MakeBell(double frequency,
											   double q,
											   double gain,
											   size_t sampleRate) noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::Bell);
	}

	/// @brief Creates an analog-style bell-shape BiQuad Filter
	///
	/// @return - An analog-style bell-shape BiQuad Filter
	inline auto BiQuadFilter<double>::MakeAnalogBell() noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(1000.0, 0.7, 0.0, 44100, FilterType::AnalogBell);
	}

	/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
	/// frequency and sample rate
	///
	/// @param frequency - The frequency to use, in Hertz
	/// @param sampleRate - The sample rate to use, in Hertz
	///
	/// @return - An analog-style bell-shape BiQuad Filter
	inline auto BiQuadFilter<double>::MakeAnalogBell(double frequency, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<double>::MakeAnalogBell(double frequency, double gain, size_t sampleRate) noexcept
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
	inline auto
	BiQuadFilter<double>::MakeAnalogBell(double frequency,
										 double q,
										 double gain,
										 size_t sampleRate) noexcept -> BiQuadFilter<double> {
		return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
	}

	/// @brief Sets the frequency of this filter to the given value
	///
	/// @param frequency - The new frequency, in Hertz
	inline auto BiQuadFilter<double>::setFrequency(double frequency) noexcept -> void {
		mFrequency = frequency;
		updateCoefficients();
	}

	/// @brief Returns the frequency of this filter
	///
	/// @return - The frequency, in Hertz
	inline auto BiQuadFilter<double>::getFrequency() const noexcept -> double {
		return mFrequency;
	}

	/// @brief Sets the Q of this filter to the given value
	///
	/// @param q - The new Q
	inline auto BiQuadFilter<double>::setQ(double q) noexcept -> void {
		mQ = q;
		updateCoefficients();
	}

	/// @brief Returns the Q of this filter
	///
	/// @return  - The Q
	inline auto BiQuadFilter<double>::getQ() const noexcept -> double {
		return mQ;
	}

	/// @brief Sets the gain of this filter to the given value
	///
	/// @param gain - The new gain, in Decibels
	inline auto BiQuadFilter<double>::setGainDB(double gain) noexcept -> void {
		mGain = gain;
		updateCoefficients();
	}

	/// @brief Returns the gain of this filter
	///
	/// @return - The gain, in Decibels
	inline auto BiQuadFilter<double>::getGainDB() const noexcept -> double {
		return mGain;
	}

	/// @brief Sets the sample rate of this filter to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	inline auto BiQuadFilter<double>::setSampleRate(size_t sampleRate) noexcept -> void {
		mSampleRate = sampleRate;
		updateCoefficients();
	}

	/// @brief Returns the sample rate of this filter
	///
	/// @return - The sample rate, in Hertz
	inline auto BiQuadFilter<double>::getSampleRate() const noexcept -> size_t {
		return mSampleRate;
	}

	/// @brief Applies this filter to the given input value
	///
	/// @param input - The input value to apply filtering to
	///
	/// @return - The filtered value
	auto BiQuadFilter<double>::process(double input) noexcept -> double {
		double yn = input * mB0 / mA0 + mX1 * mB1 / mA0 + mX2 * mB2 / mA0 - mY1 * mA1 / mA0
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
	/// @param numSamples - The number of samples in the array
	auto
	BiQuadFilter<double>::process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void {
		for(auto& in : input) {
			in = process(in);
		}
	}

	/// @brief Resets this filter to an initial state
	auto BiQuadFilter<double>::reset() noexcept -> void {
		mB0 = 0.0;
		mB1 = 0.0;
		mB2 = 0.0;
		mA0 = 0.0;
		mA1 = 0.0;
		mA2 = 0.0;

		mY1 = 0.0;
		mY2 = 0.0;
		mX1 = 0.0;
		mX2 = 0.0;
	}

	/// @brief Calculates the magnitude response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
	///
	/// @return - The magnitude response at the given frequency
	auto BiQuadFilter<double>::getMagnitudeForFrequency(double frequency) const noexcept -> double {
		constexpr std::complex<double> j(0.0, 1.0);
		const size_t order = 2;
		const std::array<double, 5> coefficients
			= {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

		jassert(frequency >= 0 && frequency <= mSampleRate * 0.5);

		std::complex<double> numerator = 0.0;
		std::complex<double> denominator = 1.0;
		std::complex<double> factor = 1.0;
		std::complex<double> jw
			= std::exp(-math::twoPi * frequency * j / static_cast<double>(mSampleRate));

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

	/// @brief Calculates the phase response of this filter for the given frequency
	///
	/// @param frequency - The frequency to calculate the phase response for, in Hertz
	///
	/// @return - The phase response at the given frequency
	auto BiQuadFilter<double>::getPhaseForFrequency(double frequency) const noexcept -> double {
		constexpr std::complex<double> j(0.0, 1.0);
		const size_t order = 2;
		const std::array<double, 5> coefficients
			= {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

		jassert(frequency >= 0 && frequency <= mSampleRate * 0.5f);

		std::complex<double> numerator = 0.0;
		std::complex<double> denominator = 1.0;
		std::complex<double> factor = 1.0;
		std::complex<double> jw
			= std::exp(-math::twoPi * frequency * j / static_cast<double>(mSampleRate));

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

	BiQuadFilter<double>::BiQuadFilter(double frequency,
									   double q,
									   double gain,
									   size_t sampleRate,
									   FilterType type) noexcept
		: mType(type), mFrequency(frequency), mQ(q), mGain(gain), mSampleRate(sampleRate) {
		updateCoefficients();
	}

	/// @brief Updates the coefficients of this filter
	auto BiQuadFilter<double>::updateCoefficients() noexcept -> void {
		double w0 = 2.0 * math::pi * mFrequency / mSampleRate;
		double cosw0 = math::cos(w0);
		double sinw0 = math::sin(w0);
		double alpha = sinw0 / (2.0 * mQ);
		double a = math::pow10(mGain / 40.0);
		double twoSqrtAAlpha = 0.0;

		if(mType == FilterType::AnalogBell) {
			alpha = sinw0 / (2.0 * mQ * a);
		}

		if(mType == FilterType::LowShelf || mType == FilterType::HighShelf) {
			twoSqrtAAlpha = 2.0 * math::sqrt(a) * alpha;
		}

		switch(mType) {
			case FilterType::Lowpass:
				{
					mB0 = (1.0 - cosw0) / 2.0;
					mB1 = 1.0 - cosw0;
					mB2 = mB0;
					mA0 = 1.0 + alpha;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha;
				}
				break;
			case FilterType::Highpass:
				{
					mB0 = (1.0 + cosw0) / 2.0;
					mB1 = -(1.0 + cosw0);
					mB2 = mB0;
					mA0 = 1.0 + alpha;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha;
				}
				break;
			case FilterType::Bandpass:
				{
					mB0 = alpha;
					mB1 = 0.0;
					mB2 = -alpha;
					mA0 = 1.0 + alpha;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha;
				}
				break;
			case FilterType::Allpass:
				{
					mB0 = 1.0 - alpha;
					mB1 = -2.0 * cosw0;
					mB2 = 1.0 + alpha;
					mA0 = 1.0 + alpha;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha;
				}
				break;
			case FilterType::Notch:
				{
					mB0 = 1.0;
					mB1 = -2.0 * cosw0;
					mB2 = 1.0;
					mA0 = 1.0 + alpha;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha;
				}
				break;
			case FilterType::LowShelf:
				{
					mB0 = a * ((a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha);
					mB1 = 2.0 * a * ((a - 1.0) + (a + 1.0) * cosw0);
					mB2 = a * ((a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha);
					mA0 = (a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha;
					mA1 = -2.0 * ((a - 1.0) - (a + 1.0) * cosw0);
					mA2 = (a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha;
				}
				break;
			case FilterType::HighShelf:
				{
					mB0 = a * ((a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha);
					mB1 = -2.0 * a * ((a - 1.0) + (a + 1.0) * cosw0);
					mB2 = a * ((a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha);
					mA0 = (a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha;
					mA1 = 2.0 * ((a - 1.0) - (a + 1.0) * cosw0);
					mA2 = (a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha;
				}
				break;
			case FilterType::Bell:
				{
					mB0 = 1.0 + alpha * a;
					mB1 = -2.0 * cosw0;
					mB2 = 1.0 - alpha * a;
					mA0 = 1.0 + alpha / a;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha / a;
				}
				break;
			case FilterType::AnalogBell:
				{
					mB0 = 1.0 + alpha * a;
					mB1 = -2.0 * cosw0;
					mB2 = 1.0 - alpha * a;
					mA0 = 1.0 + alpha / a;
					mA1 = -2.0 * cosw0;
					mA2 = 1.0 - alpha / a;
				}
				break;
		}
	}
} // namespace apex::dsp
