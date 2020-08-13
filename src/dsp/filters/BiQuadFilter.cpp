#include "BiQuadFilter.h"

#include <array>
#include <complex>

namespace apex::dsp {
	/// @brief Constructs a default `BiQuadFilter`
	BiQuadFilter<float>::BiQuadFilter() noexcept {
		updateCoefficients();
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
