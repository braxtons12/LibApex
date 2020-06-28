#include "Gain.h"

#include <vector>

namespace apex {
	namespace dsp {

		/// @brief Constructs a default `Gain` with an initial linear value of 1.0
		Gain<float>::Gain() {

		}

		/// @brief Constructs a `Gain` with the given gain value.
		///
		/// @param gain - The gain value to use
		/// @param gainIsDecibels - Whether the gain value is in Decibels
		Gain<float>::Gain(float gain, bool gainIsDecibels) {
			if(gainIsDecibels) {
				mGainDecibels = gain;
				mGainLinear = math::Decibels::decibelsToLinear(gain);
			}
			else {
				mGainLinear = gain;
				mGainDecibels = math::Decibels::linearToDecibels(gain);
			}
		}

		Gain<float>::~Gain() {

		}

		/// @brief Sets the gain of this `Gain` to be the given linear value
		///
		/// @param gain - The linear gain value to set this `Gain` to
		void Gain<float>::setGainLinear(float gain) {
			mGainLinear = gain;
			mGainDecibels = math::Decibels::linearToDecibels(gain);
		}

		/// @brief Returns the currently set linear gain value
		///
		/// @return The linear gain value
		float Gain<float>::getGainLinear() const {
			return mGainLinear;
		}

		/// @brief Sets the gain of this `Gain` to be the given Decibel value
		///
		/// @param gainDecibels - The Decibel gain value to set this `Gain` to
		void Gain<float>::setGainDecibels(float gainDecibels) {
			mGainDecibels = gainDecibels;
			mGainLinear = math::Decibels::decibelsToLinear(gainDecibels);
		}

		/// @brief Returns the currently set gain value, in Decibels
		///
		/// @return The gain value, in Decibels
		float Gain<float>::getGainDecibels() const {
			return mGainDecibels;
		}

		/// @brief Applies this `Gain` to the input
		///
		/// @param input - The input to apply the gain to
		///
		/// @return The resulting value after applying the gain
		float Gain<float>::process(float input) const {
			return input * mGainLinear;
		}

		/// @brief Applies this `Gain` to the input, in place
		///
		/// @param input - The input to apply the gain to
		void Gain<float>::process(float& input) const {
			input *= mGainLinear;
		}

		/// @brief Applies this `Gain` to the pair of input values
		///
		/// @param inputL - The left input to apply the gain to
		/// @param inputR - The right input to apply the gain to
		///
		/// @return - The resulting pair of values after applying the gain
		std::tuple<float, float> Gain<float>::process(float inputL, float inputR) const {
			return std::tuple<float, float>(inputL * mGainLinear, inputR * mGainLinear);
		}

		/// @brief Applies this `Gain` to the pair of input values, in place
		///
		/// @param inputL - The left input to apply the gain to
		/// @param inputR - The right input to apply the gain to
		void Gain<float>::process(float& inputL, float& inputR) const {
			inputL *= mGainLinear;
			inputR *= mGainLinear;
		}

		/// @brief Applies this `Gain` to the block of input values, in place
		///
		/// @param input - The block of input values to apply the gain to
		/// @param numChannels - The number of channels in the input block
		/// @param numSamples - The number of samples in the input block
		void Gain<float>::processBlock(float** input, size_t numChannels, size_t numSamples) const {
			///TODO: test if this optimizatino is good?
#if JUCE_USE_SIMD
			using juce::dsp::SIMDRegister;
			std::vector<SIMDRegister<float>> channelRegs =
				std::vector<SIMDRegister<float>>(numChannels);
			size_t size = SIMDRegister<float>::size();
			for(size_t sample = 0; sample < numSamples; sample += size) {
				for(size_t channel = 0; channel < numChannels; ++channel) {
					channelRegs[channel] =
						SIMDRegister<float>::fromRawArray(&input[channel][sample]);
					channelRegs[channel] *= mGainLinear;
					channelRegs[channel].copyToRawArray(&input[channel][sample]);
				}
			}
#else
			for(size_t channel = 0; channel < numChannels; ++channel) {
				for(size_t sample = 0; sample < numSamples; ++sample) {
					input[channel][sample] *= mGainLinear;
				}
			}
#endif //JUCE_USE_SIMD
		}

		/// @brief Constructs a default `Gain` with an initial linear value of 1.0
		Gain<double>::Gain() {

		}

		/// @brief Constructs a `Gain` with the given gain value.
		///
		/// @param gain - The gain value to use
		/// @param gainIsDecibels - Whether the gain value is in Decibels
		Gain<double>::Gain(double gain, bool gainIsDecibels) {
			if(gainIsDecibels) {
				mGainDecibels = gain;
				mGainLinear = math::Decibels::decibelsToLinear(gain);
			}
			else {
				mGainLinear = gain;
				mGainDecibels = math::Decibels::linearToDecibels(gain);
			}
		}

		Gain<double>::~Gain() {

		}

		/// @brief Sets the gain of this `Gain` to be the given linear value
		///
		/// @param gain - The linear gain value to set this `Gain` to
		void Gain<double>::setGainLinear(double gain) {
			mGainLinear = gain;
			mGainDecibels = math::Decibels::linearToDecibels(gain);
		}

		/// @brief Returns the currently set linear gain value
		///
		/// @return The linear gain value
		double Gain<double>::getGainLinear() const {
			return mGainLinear;
		}

		/// @brief Sets the gain of this `Gain` to be the given Decibel value
		///
		/// @param gainDecibels - The Decibel gain value to set this `Gain` to
		void Gain<double>::setGainDecibels(double gainDecibels) {
			mGainDecibels = gainDecibels;
			mGainLinear = math::Decibels::decibelsToLinear(gainDecibels);
		}

		/// @brief Returns the currently set gain value, in Decibels
		///
		/// @return The gain value, in Decibels
		double Gain<double>::getGainDecibels() const {
			return mGainDecibels;
		}

		/// @brief Applies this `Gain` to the input
		///
		/// @param input - The input to apply the gain to
		///
		/// @return The resulting value after applying the gain
		double Gain<double>::process(double input) const {
			return input * mGainLinear;
		}

		/// @brief Applies this `Gain` to the input, in place
		///
		/// @param input - The input to apply the gain to
		void Gain<double>::process(double& input) const {
			input *= mGainLinear;
		}

		/// @brief Applies this `Gain` to the pair of input values
		///
		/// @param inputL - The left input to apply the gain to
		/// @param inputR - The right input to apply the gain to
		///
		/// @return - The resulting pair of values after applying the gain
		std::tuple<double, double> Gain<double>::process(double inputL, double inputR) const {
			return std::tuple<double, double>(inputL * mGainLinear, inputR * mGainLinear);
		}

		/// @brief Applies this `Gain` to the pair of input values, in place
		///
		/// @param inputL - The left input to apply the gain to
		/// @param inputR - The right input to apply the gain to
		void Gain<double>::process(double& inputL, double& inputR) const {
			inputL *= mGainLinear;
			inputR *= mGainLinear;
		}

		/// @brief Applies this `Gain` to the block of input values, in place
		///
		/// @param input - The block of input values to apply the gain to
		/// @param numChannels - The number of channels in the input block
		/// @param numSamples - The number of samples in the input block
		void Gain<double>::processBlock(double** input, size_t numChannels, size_t numSamples) const {
			///TODO: test if this optimization is good?
#if JUCE_USE_SIMD
			using juce::dsp::SIMDRegister;
			std::vector<SIMDRegister<double>> channelRegs =
				std::vector<SIMDRegister<double>>(numChannels);
			size_t size = SIMDRegister<double>::size();
			for(size_t sample = 0; sample < numSamples; sample += size) {
				for(size_t channel = 0; channel < numChannels; ++channel) {
					channelRegs[channel] =
						SIMDRegister<double>::fromRawArray(&input[channel][sample]);
					channelRegs[channel] *= mGainLinear;
					channelRegs[channel].copyToRawArray(&input[channel][sample]);
				}
			}
#else
			for(size_t channel = 0; channel < numChannels; ++channel) {
				for(size_t sample = 0; sample < numSamples; ++sample) {
					input[channel][sample] *= mGainLinear;
				}
			}
#endif //JUCE_USE_SIMD
		}
	}
}
