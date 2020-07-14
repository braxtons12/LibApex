#include "BiQuadFilter.h"

#include <complex>

namespace apex {
	namespace dsp {

		/// @brief Constructs a default `BiQuadFilter`
		BiQuadFilter<float>::BiQuadFilter() noexcept {
			updateCoefficients();
		}

		/// @brief Creates a default lowpass BiQuad Filter
		///
		/// @return - A lowpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowpass() noexcept {
			return BiQuadFilter<float>(20000.0f, 0.7f, 0.0f, 44100, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowpass(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowpass(float frequency,
				float q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, 0.0f, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a default highpass BiQuad filter
		///
		/// @return - A highpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighpass() noexcept {
			return BiQuadFilter<float>(20.0f, 0.7f, 0.0f, 44100, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighpass(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighpass(float frequency,
				float q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, 0.0f, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a default bandpass BiQuad Filter
		///
		/// @return - A bandpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBandpass() noexcept {
			return BiQuadFilter<float>(2000.0f, 0.7f, 0.0f, 44100, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBandpass(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBandpass(float frequency,
				float q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, 0.0f, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a default allpass BiQuad Filter
		///
		/// @return - An allpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAllpass() noexcept {
			return BiQuadFilter<float>(2000.0f, 0.7f, 0.0f, 44100, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAllpass(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAllpass(float frequency,
				float q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, 0.0f, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates a default notch BiQuad Filter
		///
		/// @return - A notch BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeNotch() noexcept {
			return BiQuadFilter<float>(4000.0f, 0.7f, 0.0f, 44100, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeNotch(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency, q,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The Q to use
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeNotch(float frequency,
				float q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, 0.0f, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a default lowshelf BiQuad Filter
		///
		/// @return - A lowshelf BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowShelf() noexcept {
			return BiQuadFilter<float>(300.0f, 0.7f, 0.0f, 44100, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowShelf(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowShelf(float frequency,
				float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, gain, sampleRate, FilterType::LowShelf);
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
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeLowShelf(float frequency,
				float q, float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a default highshelf BiQuad Filter
		///
		/// @return - A highshelf BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighShelf() noexcept {
			return BiQuadFilter<float>(3000.0f, 0.7f, 0.0f, 44100, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighShelf(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighShelf(float frequency,
				float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, gain, sampleRate, FilterType::HighShelf);
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
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeHighShelf(float frequency,
				float q, float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a default bell-shape BiQuad Filter
		///
		/// @return - A bell-shape BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBell() noexcept {
			return BiQuadFilter<float>(1000.0f, 0.7f, 0.0f, 44100, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBell(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency, gain,
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBell(float frequency,
				float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, gain, sampleRate, FilterType::Bell);
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
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeBell(float frequency,
				float q, float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAnalogBell() noexcept {
			return BiQuadFilter<float>(1000.0f, 0.7f, 0.0f, 44100, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAnalogBell(float frequency,
				size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, 0.0f, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency, gain, and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param gain - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAnalogBell(float frequency,
				float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, 0.7f, gain, sampleRate, FilterType::AnalogBell);
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
		inline BiQuadFilter<float> BiQuadFilter<float>::MakeAnalogBell(float frequency,
				float q, float gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<float>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Sets the frequency of this filter to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline void BiQuadFilter<float>::setFrequency(float frequency) noexcept {
			mFrequency = frequency;
			updateCoefficients();
		}

		/// @brief Returns the frequency of this filter
		///
		/// @return - The frequency, in Hertz
		inline float BiQuadFilter<float>::getFrequency() const  noexcept {
			return mFrequency;
		}

		/// @brief Sets the Q of this filter to the given value
		///
		/// @param q - The new Q
		inline void BiQuadFilter<float>::setQ(float q)  noexcept {
			mQ = q;
			updateCoefficients();
		}

		/// @brief Returns the Q of this filter
		///
		/// @return  - The Q
		inline float BiQuadFilter<float>::getQ() const noexcept {
			return mQ;
		}

		/// @brief Sets the gain of this filter to the given value
		///
		/// @param gain - The new gain, in Decibels
		inline void BiQuadFilter<float>::setGainDB(float gain) noexcept {
			mGain = gain;
			updateCoefficients();
		}

		/// @brief Returns the gain of this filter
		///
		/// @return - The gain, in Decibels
		inline float BiQuadFilter<float>::getGainDB() const noexcept {
			return mGain;
		}

		/// @brief Sets the sample rate of this filter to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline void BiQuadFilter<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Returns the sample rate of this filter
		///
		/// @return - The sample rate, in Hertz
		inline size_t BiQuadFilter<float>::getSampleRate() const noexcept {
			return mSampleRate;
		}

		/// @brief Applies this filter to the given input value
		///
		/// @param input - The input value to apply filtering to
		///
		/// @return - The filtered value
		float BiQuadFilter<float>::process(float input) noexcept {
			float yn = input * mB0 / mA0 + mX1 * mB1 / mA0 + mX2 * mB2 / mA0
				- mY1 * mA1 / mA0 - mY2 * mA2 / mA0;

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
		void BiQuadFilter<float>::process(float* input, size_t numSamples) noexcept {
			for(size_t n = 0; n < numSamples; ++n) {
				input[n] = process(input[n]);
			}
		}

		/// @brief Resets this filter to an initial state
		void BiQuadFilter<float>::reset() noexcept {
			mB0 = 0.0f;
			mB1 = 0.0f;
			mB2 = 0.0f;
			mA0 = 0.0f;
			mA1 = 0.0f;
			mA2 = 0.0f;

			mY1 = 0.0f;
			mY2 = 0.0f;
			mX1 = 0.0f;
			mX2 = 0.0f;
		}

		/// @brief Calculates the magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		float BiQuadFilter<float>::getMagnitudeForFrequency(float frequency) const noexcept {
			constexpr std::complex<float> j(0.0f, 1.0f);
			const size_t order = 2;
			const float coefficients[5] = {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

			jassert(frequency >= 0 && frequency <= mSampleRate * 0.5f);

			std::complex<float> numerator = 0.0f;
			std::complex<float> denominator = 1.0f;
			std::complex<float> factor = 1.0f;
			std::complex<float> jw = std::exp(-math::twoPif * frequency * j / static_cast<float>(mSampleRate));

			for(size_t n = 0; n <= order; ++n) {
				numerator += coefficients[n] * factor;
				factor *= jw;
			}

			factor = jw;

			for(size_t n = order + 1; n <= 2 * order; ++n) {
				denominator += coefficients[n] * factor;
				factor *= jw;
			}

			return std::abs(numerator / denominator);
		}

		/// @brief Calculates the magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude respones for
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		void BiQuadFilter<float>::getMagnitudesForFrequencies(float* frequencies,
				float* magnitudes, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				magnitudes[frequency] = getMagnitudeForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response at the given frequency
		float BiQuadFilter<float>::getPhaseForFrequency(float frequency) const noexcept {
			constexpr std::complex<float> j(0.0f, 1.0f);
			const size_t order = 2;
			const float coefficients[5] = {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

			jassert(frequency >= 0 && frequency <= mSampleRate * 0.5f);

			std::complex<float> numerator = 0.0f;
			std::complex<float> denominator = 1.0f;
			std::complex<float> factor = 1.0f;
			std::complex<float> jw = std::exp(-math::twoPif * frequency * j / static_cast<float>(mSampleRate));

			for(size_t n = 0; n <= order; ++n) {
				numerator += coefficients[n] * factor;
				factor *= jw;
			}

			factor = jw;

			for(size_t n = order + 1; n <= 2 * order; ++n) {
				denominator += coefficients[n] * factor;
				factor *= jw;
			}

			return std::arg(numerator / denominator);
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		void BiQuadFilter<float>::getPhasesForFrequencies(float* frequencies, float* phases,
				size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getPhaseForFrequency(frequencies[frequency]);
			}
		}

		BiQuadFilter<float>::BiQuadFilter(float frequency, float q, float gain,
				size_t sampleRate, FilterType type) noexcept
			: mType(type),
			mFrequency(frequency),
			mQ(q),
			mGain(gain),
			mSampleRate(sampleRate)
			{
				updateCoefficients();
			}

		/// @brief Updates the coefficients of this filter
		void BiQuadFilter<float>::updateCoefficients() noexcept {
			float w0 = 2.0f * math::pif * mFrequency / mSampleRate;
			float cosw0 = math::cosf(w0);
			float sinw0 = math::sinf(w0);
			float alpha = sinw0 / (2.0f * mQ);
			float a = math::pow10f(mGain / 40.0f);
			float twoSqrtAAlpha = 0.0f;

			if(mType == FilterType::AnalogBell) {
				alpha = sinw0 / (2.0f * mQ * a);
			}

			if(mType == FilterType::LowShelf || mType == FilterType::HighShelf) {
				twoSqrtAAlpha = 2.0f * math::sqrtf(a) * alpha;
			}

			switch(mType) {
				case FilterType::Lowpass: {
											  mB0 = (1.0f - cosw0) / 2.0f;
											  mB1 = 1.0f - cosw0;
											  mB2 = mB0;
											  mA0 = 1.0f + alpha;
											  mA1 = -2.0f * cosw0;
											  mA2 = 1.0f - alpha;
										  }
										  break;
				case FilterType::Highpass: {
											   mB0 = (1.0f + cosw0) / 2.0f;
											   mB1 = -(1.0f + cosw0);
											   mB2 = mB0;
											   mA0 = 1.0f + alpha;
											   mA1 = -2.0f * cosw0;
											   mA2 = 1.0f - alpha;
										   }
										   break;
				case FilterType::Bandpass: {
											   mB0 = alpha;
											   mB1 = 0.0f;
											   mB2 = -alpha;
											   mA0 = 1.0f + alpha;
											   mA1 = -2.0f * cosw0;
											   mA2 = 1.0f - alpha;
										   }
										   break;
				case FilterType::Allpass: {
											  mB0 = 1.0f - alpha;
											  mB1 = -2.0f * cosw0;
											  mB2 = 1.0f + alpha;
											  mA0 = 1.0f + alpha;
											  mA1 = -2.0f * cosw0;
											  mA2 = 1.0f - alpha;
										  }
										  break;
				case FilterType::Notch: {
											mB0 = 1.0f;
											mB1 = -2.0f * cosw0;
											mB2 = 1.0f;
											mA0 = 1.0f + alpha;
											mA1 = -2.0f * cosw0;
											mA2 = 1.0f - alpha;
										}
										break;
				case FilterType::LowShelf: {
											   mB0 = a * ( (a + 1.0f) - (a - 1.0f) * cosw0 + twoSqrtAAlpha);
											   mB1 = 2.0f * a * ( (a - 1.0f) + (a + 1.0f) * cosw0);
											   mB2 = a * ( (a + 1.0f) - (a - 1.0f) * cosw0 - twoSqrtAAlpha);
											   mA0 = (a + 1.0f) - (a - 1.0f) * cosw0 + twoSqrtAAlpha;
											   mA1 = -2.0f * ( (a - 1.0f) - (a + 1.0f) * cosw0);
											   mA2 = (a + 1.0f) - (a - 1.0f) * cosw0 - twoSqrtAAlpha;
										   }
										   break;
				case FilterType::HighShelf: {
												mB0 = a * ( (a + 1.0f) - (a - 1.0f) * cosw0 + twoSqrtAAlpha);
												mB1 = -2.0f * a * ( (a - 1.0f) + (a + 1.0f) * cosw0);
												mB2 = a * ( (a + 1.0f) - (a - 1.0f) * cosw0 - twoSqrtAAlpha);
												mA0 = (a + 1.0f) - (a - 1.0f) * cosw0 + twoSqrtAAlpha;
												mA1 = 2.0f * ( (a - 1.0f) - (a + 1.0f) * cosw0);
												mA2 = (a + 1.0f) - (a - 1.0f) * cosw0 - twoSqrtAAlpha;
											}
											break;
				case FilterType::Bell: {
										   mB0 = 1.0f + alpha * a;
										   mB1 = -2.0f * cosw0;
										   mB2 = 1.0f - alpha * a;
										   mA0 = 1.0f + alpha / a;
										   mA1 = -2.0f * cosw0;
										   mA2 = 1.0f - alpha / a;
									   }
									   break;
				case FilterType::AnalogBell: {
												 mB0 = 1.0f + alpha * a;
												 mB1 = -2.0f * cosw0;
												 mB2 = 1.0f - alpha * a;
												 mA0 = 1.0f + alpha / a;
												 mA1 = -2.0f * cosw0;
												 mA2 = 1.0f - alpha / a;
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowpass() noexcept {
			return BiQuadFilter<double>(20000.0, 0.7, 0.0, 44100, FilterType::Lowpass);
		}

		/// @brief Creates a lowpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowpass(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowpass(double frequency,
				double q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Lowpass);
		}

		/// @brief Creates a default highpass BiQuad filter
		///
		/// @return - A highpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighpass() noexcept {
			return BiQuadFilter<double>(20.0, 0.7, 0.0, 44100, FilterType::Highpass);
		}

		/// @brief Creates a highpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighpass(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighpass(double frequency,
				double q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Highpass);
		}

		/// @brief Creates a default bandpass BiQuad Filter
		///
		/// @return - A bandpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBandpass() noexcept {
			return BiQuadFilter<double>(2000.0, 0.7, 0.0, 44100, FilterType::Bandpass);
		}

		/// @brief Creates a bandpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bandpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBandpass(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBandpass(double frequency,
				double q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Bandpass);
		}

		/// @brief Creates a default allpass BiQuad Filter
		///
		/// @return - An allpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAllpass() noexcept {
			return BiQuadFilter<double>(2000.0, 0.7, 0.0, 44100, FilterType::Allpass);
		}

		/// @brief Creates an allpass BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An allpass BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAllpass(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAllpass(double frequency,
				double q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Allpass);
		}

		/// @brief Creates a default notch BiQuad Filter
		///
		/// @return - A notch BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeNotch() noexcept {
			return BiQuadFilter<double>(4000.0, 0.7, 0.0, 44100, FilterType::Notch);
		}

		/// @brief Creates a notch BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A notch BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeNotch(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeNotch(double frequency,
				double q, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, 0.0, sampleRate, FilterType::Notch);
		}

		/// @brief Creates a default lowshelf BiQuad Filter
		///
		/// @return - A lowshelf BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowShelf() noexcept {
			return BiQuadFilter<double>(300.0, 0.7, 0.0, 44100, FilterType::LowShelf);
		}

		/// @brief Creates a lowshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A lowshelf BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowShelf(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowShelf(double frequency,
				double gain, size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeLowShelf(double frequency,
				double q, double gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::LowShelf);
		}

		/// @brief Creates a default highshelf BiQuad Filter
		///
		/// @return - A highshelf BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighShelf() noexcept {
			return BiQuadFilter<double>(3000.0, 0.7, 0.0, 44100, FilterType::HighShelf);
		}

		/// @brief Creates a highshelf BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A highshelf BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighShelf(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighShelf(double frequency,
				double gain, size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeHighShelf(double frequency,
				double q, double gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::HighShelf);
		}

		/// @brief Creates a default bell-shape BiQuad Filter
		///
		/// @return - A bell-shape BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBell() noexcept {
			return BiQuadFilter<double>(1000.0, 0.7, 0.0, 44100, FilterType::Bell);
		}

		/// @brief Creates a bell-shape BiQuad Filter with the given frequency
		/// and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - A bell-shape BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBell(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBell(double frequency,
				double gain, size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeBell(double frequency,
				double q, double gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::Bell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAnalogBell() noexcept {
			return BiQuadFilter<double>(1000.0, 0.7, 0.0, 44100, FilterType::AnalogBell);
		}

		/// @brief Creates an analog-style bell-shape BiQuad Filter with the given
		/// frequency and sample rate
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param sampleRate - The sample rate to use, in Hertz
		///
		/// @return - An analog-style bell-shape BiQuad Filter
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAnalogBell(double frequency,
				size_t sampleRate) noexcept
		{
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAnalogBell(double frequency,
				double gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, 0.7f, gain, sampleRate, FilterType::AnalogBell);
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
		inline BiQuadFilter<double> BiQuadFilter<double>::MakeAnalogBell(double frequency,
				double q, double gain, size_t sampleRate) noexcept
		{
			return BiQuadFilter<double>(frequency, q, gain, sampleRate, FilterType::AnalogBell);
		}

		/// @brief Sets the frequency of this filter to the given value
		///
		/// @param frequency - The new frequency, in Hertz
		inline void BiQuadFilter<double>::setFrequency(double frequency) noexcept {
			mFrequency = frequency;
			updateCoefficients();
		}

		/// @brief Returns the frequency of this filter
		///
		/// @return - The frequency, in Hertz
		inline double BiQuadFilter<double>::getFrequency() const noexcept {
			return mFrequency;
		}

		/// @brief Sets the Q of this filter to the given value
		///
		/// @param q - The new Q
		inline void BiQuadFilter<double>::setQ(double q) noexcept {
			mQ = q;
			updateCoefficients();
		}

		/// @brief Returns the Q of this filter
		///
		/// @return  - The Q
		inline double BiQuadFilter<double>::getQ() const noexcept {
			return mQ;
		}

		/// @brief Sets the gain of this filter to the given value
		///
		/// @param gain - The new gain, in Decibels
		inline void BiQuadFilter<double>::setGainDB(double gain) noexcept {
			mGain = gain;
			updateCoefficients();
		}

		/// @brief Returns the gain of this filter
		///
		/// @return - The gain, in Decibels
		inline double BiQuadFilter<double>::getGainDB() const noexcept {
			return mGain;
		}

		/// @brief Sets the sample rate of this filter to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline void BiQuadFilter<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Returns the sample rate of this filter
		///
		/// @return - The sample rate, in Hertz
		inline size_t BiQuadFilter<double>::getSampleRate() const noexcept {
			return mSampleRate;
		}

		/// @brief Applies this filter to the given input value
		///
		/// @param input - The input value to apply filtering to
		///
		/// @return - The filtered value
		double BiQuadFilter<double>::process(double input) noexcept {
			double yn = input * mB0 / mA0 + mX1 * mB1 / mA0 + mX2 * mB2 / mA0
				- mY1 * mA1 / mA0 - mY2 * mA2 / mA0;

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
		void BiQuadFilter<double>::process(double* input, size_t numSamples) noexcept {
			for(size_t n = 0; n < numSamples; ++n) {
				input[n] = process(input[n]);
			}
		}

		/// @brief Resets this filter to an initial state
		void BiQuadFilter<double>::reset() noexcept {
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
		double BiQuadFilter<double>::getMagnitudeForFrequency(double frequency) const noexcept {
			constexpr std::complex<double> j(0.0, 1.0);
			const size_t order = 2;
			const double coefficients[5] = {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

			jassert(frequency >= 0 && frequency <= mSampleRate * 0.5);

			std::complex<double> numerator = 0.0;
			std::complex<double> denominator = 1.0;
			std::complex<double> factor = 1.0;
			std::complex<double> jw = std::exp(-math::twoPi * frequency *
					j / static_cast<double>(mSampleRate));

			for(size_t n = 0; n <= order; ++n) {
				numerator += coefficients[n] * factor;
				factor *= jw;
			}

			factor = jw;

			for(size_t n = order + 1; n <= 2 * order; ++n) {
				denominator += coefficients[n] * factor;
				factor *= jw;
			}

			return std::abs(numerator / denominator);
		}

		/// @brief Calculates the magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude respones for
		/// @param magnitudes - The array to store the magnitudes in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		void BiQuadFilter<double>::getMagnitudesForFrequencies(double* frequencies,
				double* magnitudes, size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				magnitudes[frequency] = getMagnitudeForFrequency(frequencies[frequency]);
			}
		}

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response at the given frequency
		double BiQuadFilter<double>::getPhaseForFrequency(double frequency) const noexcept {
			constexpr std::complex<double> j(0.0, 1.0);
			const size_t order = 2;
			const double coefficients[5] = {mB0 / mA0, mB1 / mA0, mB2 / mA0, mA1 / mA0, mA2 / mA0};

			jassert(frequency >= 0 && frequency <= mSampleRate * 0.5f);

			std::complex<double> numerator = 0.0;
			std::complex<double> denominator = 1.0;
			std::complex<double> factor = 1.0;
			std::complex<double> jw = std::exp(-math::twoPi * frequency * j / static_cast<double>(mSampleRate));

			for(size_t n = 0; n <= order; ++n) {
				numerator += coefficients[n] * factor;
				factor *= jw;
			}

			factor = jw;

			for(size_t n = order + 1; n <= 2 * order; ++n) {
				denominator += coefficients[n] * factor;
				factor *= jw;
			}

			return std::arg(numerator / denominator);
		}

		/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases in
		/// @param numFrequencies - The number of frequencies in the `frequencies` array
		void BiQuadFilter<double>::getPhasesForFrequencies(double* frequencies, double* phases,
				size_t numFrequencies) const noexcept
		{
			for(size_t frequency = 0; frequency < numFrequencies; ++frequency) {
				phases[frequency] = getPhaseForFrequency(frequencies[frequency]);
			}
		}

		BiQuadFilter<double>::BiQuadFilter(double frequency, double q, double gain,
				size_t sampleRate, FilterType type) noexcept
			: mType(type),
			mFrequency(frequency),
			mQ(q),
			mGain(gain),
			mSampleRate(sampleRate)
			{
				updateCoefficients();
			}

		/// @brief Updates the coefficients of this filter
		void BiQuadFilter<double>::updateCoefficients() noexcept {
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
				case FilterType::Lowpass: {
											  mB0 = (1.0 - cosw0) / 2.0;
											  mB1 = 1.0 - cosw0;
											  mB2 = mB0;
											  mA0 = 1.0 + alpha;
											  mA1 = -2.0 * cosw0;
											  mA2 = 1.0 - alpha;
										  }
										  break;
				case FilterType::Highpass: {
											   mB0 = (1.0 + cosw0) / 2.0;
											   mB1 = -(1.0 + cosw0);
											   mB2 = mB0;
											   mA0 = 1.0 + alpha;
											   mA1 = -2.0 * cosw0;
											   mA2 = 1.0 - alpha;
										   }
										   break;
				case FilterType::Bandpass: {
											   mB0 = alpha;
											   mB1 = 0.0;
											   mB2 = -alpha;
											   mA0 = 1.0 + alpha;
											   mA1 = -2.0 * cosw0;
											   mA2 = 1.0 - alpha;
										   }
										   break;
				case FilterType::Allpass: {
											  mB0 = 1.0 - alpha;
											  mB1 = -2.0 * cosw0;
											  mB2 = 1.0 + alpha;
											  mA0 = 1.0 + alpha;
											  mA1 = -2.0 * cosw0;
											  mA2 = 1.0 - alpha;
										  }
										  break;
				case FilterType::Notch: {
											mB0 = 1.0;
											mB1 = -2.0 * cosw0;
											mB2 = 1.0;
											mA0 = 1.0 + alpha;
											mA1 = -2.0 * cosw0;
											mA2 = 1.0 - alpha;
										}
										break;
				case FilterType::LowShelf: {
											   mB0 = a * ( (a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha);
											   mB1 = 2.0 * a * ( (a - 1.0) + (a + 1.0) * cosw0);
											   mB2 = a * ( (a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha);
											   mA0 = (a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha;
											   mA1 = -2.0 * ( (a - 1.0) - (a + 1.0) * cosw0);
											   mA2 = (a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha;
										   }
										   break;
				case FilterType::HighShelf: {
												mB0 = a * ( (a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha);
												mB1 = -2.0 * a * ( (a - 1.0) + (a + 1.0) * cosw0);
												mB2 = a * ( (a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha);
												mA0 = (a + 1.0) - (a - 1.0) * cosw0 + twoSqrtAAlpha;
												mA1 = 2.0 * ( (a - 1.0) - (a + 1.0) * cosw0);
												mA2 = (a + 1.0) - (a - 1.0) * cosw0 - twoSqrtAAlpha;
											}
											break;
				case FilterType::Bell: {
										   mB0 = 1.0 + alpha * a;
										   mB1 = -2.0 * cosw0;
										   mB2 = 1.0 - alpha * a;
										   mA0 = 1.0 + alpha / a;
										   mA1 = -2.0 * cosw0;
										   mA2 = 1.0 - alpha / a;
									   }
									   break;
				case FilterType::AnalogBell: {
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
	}
}
