#include "Gain.h"

#include <vector>

namespace apex::dsp {
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
	auto Gain<float>::setGainLinear(float gain) noexcept -> void {
		mGainLinear = gain;
		mGainDecibels = math::Decibels::linearToDecibels(gain);
	}

	/// @brief Returns the currently set linear gain value
	///
	/// @return The linear gain value
	auto Gain<float>::getGainLinear() const noexcept -> float {
		return mGainLinear;
	}

	/// @brief Sets the gain of this `Gain` to be the given Decibel value
	///
	/// @param gainDecibels - The Decibel gain value to set this `Gain` to
	auto Gain<float>::setGainDecibels(float gainDecibels) noexcept -> void {
		mGainDecibels = gainDecibels;
		mGainLinear = math::Decibels::decibelsToLinear(gainDecibels);
	}

	/// @brief Returns the currently set gain value, in Decibels
	///
	/// @return The gain value, in Decibels
	auto Gain<float>::getGainDecibels() const noexcept -> float {
		return mGainDecibels;
	}

	/// @brief Applies this `Gain` to the input
	///
	/// @param input - The input to apply the gain to
	///
	/// @return The resulting value after applying the gain
	auto Gain<float>::process(float input) noexcept -> float{
		return input * mGainLinear;
	}

	/// @brief Applies this `Gain` to the array of input values, in place
	///
	/// @param input - The array of input values to apply gain to
	auto Gain<float>::process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void {
#if JUCE_USE_SIMD
		using juce::dsp::SIMDRegister;
		SIMDRegister<float> reg = SIMDRegister<float>();
		size_t size = SIMDRegister<float>::size();
		size_t leftovers = input.size() % size;
		for(size_t sample = 0; sample < leftovers; ++sample) {
			gsl::at(input, static_cast<gsl::index>(sample)) *= mGainLinear;
		}
		for(size_t sample = leftovers; sample < input.size(); sample += size) {
			reg = SIMDRegister<float>::fromRawArray(input.subspan(sample).data());
			reg *= mGainLinear;
			reg.copyToRawArray(input.subspan(sample).data());
		}
#else //JUCE_USE_SIMD
		for(auto& in : input) {
			in *= mGainLinear;
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
	auto Gain<double>::setGainLinear(double gain) noexcept -> void {
		mGainLinear = gain;
		mGainDecibels = math::Decibels::linearToDecibels(gain);
	}

	/// @brief Returns the currently set linear gain value
	///
	/// @return The linear gain value
	auto Gain<double>::getGainLinear() const noexcept -> double {
		return mGainLinear;
	}

	/// @brief Sets the gain of this `Gain` to be the given Decibel value
	///
	/// @param gainDecibels - The Decibel gain value to set this `Gain` to
	auto Gain<double>::setGainDecibels(double gainDecibels) noexcept -> void {
		mGainDecibels = gainDecibels;
		mGainLinear = math::Decibels::decibelsToLinear(gainDecibels);
	}

	/// @brief Returns the currently set gain value, in Decibels
	///
	/// @return The gain value, in Decibels
	auto Gain<double>::getGainDecibels() const noexcept -> double {
		return mGainDecibels;
	}

	/// @brief Applies this `Gain` to the input
	///
	/// @param input - The input to apply the gain to
	///
	/// @return The resulting value after applying the gain
	auto Gain<double>::process(double input) noexcept -> double {
		return input * mGainLinear;
	}

	/// @brief Applies this `Gain` to the array of input values, in place
	///
	/// @param input - The array of input values to apply gain to
	auto Gain<double>::process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void {
#if JUCE_USE_SIMD
		using juce::dsp::SIMDRegister;
		SIMDRegister<double> reg = SIMDRegister<double>();
		size_t size = SIMDRegister<double>::size();
		size_t leftovers = input.size() % size;
		for(size_t sample = 0; sample < leftovers; ++sample) {
			gsl::at(input, static_cast<gsl::index>(sample)) *= mGainLinear;
		}
		for(size_t sample = leftovers; sample < input.size(); sample += size) {
			reg = SIMDRegister<double>::fromRawArray(input.subspan(sample).data());
			reg *= mGainLinear;
			reg.copyToRawArray(input.subspan(sample).data());
		}
#else //JUCE_USE_SIMD
		for(auto& in : input) {
			in *= mGainLinear;
		}
#endif //JUCE_USE_SIMD
	}
} //namespace apex::dsp
