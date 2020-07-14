#include "Gain.h"

#include <vector>

namespace apex {
	namespace dsp {

		/// @brief Constructs a `Gain` with the given gain value.
		///
		/// @param gain - The gain value to use
		/// @param gainIsDecibels - Whether the gain value is in Decibels
		Gain<float>::Gain(float gain, bool gainIsDecibels) noexcept {
			if(gainIsDecibels) {
				mGainDecibels = gain;
				mGainLinear = math::Decibels::decibelsToLinear(gain);
			}
			else {
				mGainLinear = gain;
				mGainDecibels = math::Decibels::linearToDecibels(gain);
			}
		}

		/// @brief Sets the gain of this `Gain` to be the given linear value
		///
		/// @param gain - The linear gain value to set this `Gain` to
		void Gain<float>::setGainLinear(float gain) noexcept {
			mGainLinear = gain;
			mGainDecibels = math::Decibels::linearToDecibels(gain);
		}

		/// @brief Returns the currently set linear gain value
		///
		/// @return The linear gain value
		float Gain<float>::getGainLinear() const noexcept {
			return mGainLinear;
		}

		/// @brief Sets the gain of this `Gain` to be the given Decibel value
		///
		/// @param gainDecibels - The Decibel gain value to set this `Gain` to
		void Gain<float>::setGainDecibels(float gainDecibels) noexcept {
			mGainDecibels = gainDecibels;
			mGainLinear = math::Decibels::decibelsToLinear(gainDecibels);
		}

		/// @brief Returns the currently set gain value, in Decibels
		///
		/// @return The gain value, in Decibels
		float Gain<float>::getGainDecibels() const noexcept {
			return mGainDecibels;
		}

		/// @brief Applies this `Gain` to the input
		///
		/// @param input - The input to apply the gain to
		///
		/// @return The resulting value after applying the gain
		float Gain<float>::process(float input) noexcept {
			return input * mGainLinear;
		}

		/// @brief Applies this `Gain` to the pair of input values
		///
		/// @param inputL - The left input to apply the gain to
		/// @param inputR - The right input to apply the gain to
		///
		/// @return - The resulting pair of values after applying the gain
		std::tuple<float, float> Gain<float>::process(float inputL, float inputR) const noexcept {
			return std::tuple<float, float>(inputL * mGainLinear, inputR * mGainLinear);
		}

		/// @brief Applies this `Gain` to the array of input values, in place
		///
		/// @param input - The array of input values to apply gain to
		/// @param numSamples - The number of samples
		void Gain<float>::process(float* input, size_t numSamples) noexcept {
#if JUCE_USE_SIMD
			using juce::dsp::SIMDRegister;
			SIMDRegister<float> reg;
			size_t size = SIMDRegister<float>::size();
			for(size_t sample = 0; sample < numSamples; sample += size) {
				reg = SIMDRegister<float>::fromRawArray(&input[sample]);
				reg *= mGainLinear;
				reg.copyToRawArray(&input[sample]);
			}
#else //JUCE_USE_SIMD
			for(size_t sample = 0; sample < numSamples; ++sample) {
				input[sample] *= mGainLinear;
			}
#endif //JUCE_USE_SIMD
		}

		/// @brief Constructs a `Gain` with the given gain value.
		///
		/// @param gain - The gain value to use
		/// @param gainIsDecibels - Whether the gain value is in Decibels
		Gain<double>::Gain(double gain, bool gainIsDecibels) noexcept {
			if(gainIsDecibels) {
				mGainDecibels = gain;
				mGainLinear = math::Decibels::decibelsToLinear(gain);
			}
			else {
				mGainLinear = gain;
				mGainDecibels = math::Decibels::linearToDecibels(gain);
			}
		}

		/// @brief Sets the gain of this `Gain` to be the given linear value
		///
		/// @param gain - The linear gain value to set this `Gain` to
		void Gain<double>::setGainLinear(double gain) noexcept {
			mGainLinear = gain;
			mGainDecibels = math::Decibels::linearToDecibels(gain);
		}

		/// @brief Returns the currently set linear gain value
		///
		/// @return The linear gain value
		double Gain<double>::getGainLinear() const noexcept {
			return mGainLinear;
		}

		/// @brief Sets the gain of this `Gain` to be the given Decibel value
		///
		/// @param gainDecibels - The Decibel gain value to set this `Gain` to
		void Gain<double>::setGainDecibels(double gainDecibels) noexcept {
			mGainDecibels = gainDecibels;
			mGainLinear = math::Decibels::decibelsToLinear(gainDecibels);
		}

		/// @brief Returns the currently set gain value, in Decibels
		///
		/// @return The gain value, in Decibels
		double Gain<double>::getGainDecibels() const noexcept {
			return mGainDecibels;
		}

		/// @brief Applies this `Gain` to the input
		///
		/// @param input - The input to apply the gain to
		///
		/// @return The resulting value after applying the gain
		double Gain<double>::process(double input) noexcept {
			return input * mGainLinear;
		}

		/// @brief Applies this `Gain` to the pair of input values
		///
		/// @param inputL - The left input to apply the gain to
		/// @param inputR - The right input to apply the gain to
		///
		/// @return - The resulting pair of values after applying the gain
		std::tuple<double, double> Gain<double>::process(double inputL, double inputR) const noexcept {
			return std::tuple<double, double>(inputL * mGainLinear, inputR * mGainLinear);
		}

		/// @brief Applies this `Gain` to the array of input values, in place
		///
		/// @param input - The array of input values to apply gain to
		/// @param numSamples - The number of samples
		void Gain<double>::process(double* input, size_t numSamples) noexcept {
#if JUCE_USE_SIMD
			using juce::dsp::SIMDRegister;
			SIMDRegister<double> reg;
			size_t size = SIMDRegister<double>::size();
			for(size_t sample = 0; sample < numSamples; sample += size) {
				reg = SIMDRegister<double>::fromRawArray(&input[sample]);
				reg *= mGainLinear;
				reg.copyToRawArray(&input[sample]);
			}
#else //JUCE_USE_SIMD
			for(size_t sample = 0; sample < numSamples; ++sample) {
				input[sample] *= mGainLinear;
			}
#endif //JUCE_USE_SIMD
		}
	}
}
