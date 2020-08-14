#include "Gain.h"

#include <vector>

namespace apex::dsp {
	/// @brief Constructs a `Gain` with the given gain value.
	///
	/// @param gain - The gain value to use
	/// @param gainIsDecibels - Whether the gain value is in Decibels
	Gain<float>::Gain(Decibels gain) noexcept {
		mGainDecibels = gain;
		mGainLinear = gsl::narrow_cast<float>(gain.getLinear());
	}

	/// @brief Applies this `Gain` to the array of input values, in place
	///
	/// @param input - The array of input values to apply gain to
	auto Gain<float>::process(Span<float> input) noexcept -> void {
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
#else // JUCE_USE_SIMD
		for(auto& in : input) {
			in *= mGainLinear;
		}
#endif // JUCE_USE_SIMD
	}

	/// @brief Constructs a `Gain` with the given gain value.
	///
	/// @param gain - The gain value to use
	/// @param gainIsDecibels - Whether the gain value is in Decibels
	Gain<double>::Gain(Decibels gain) noexcept {
		mGainDecibels = gain;
		mGainLinear = gain.getLinear();
	}

	/// @brief Applies this `Gain` to the array of input values, in place
	///
	/// @param input - The array of input values to apply gain to
	auto Gain<double>::process(Span<double> input) noexcept -> void {
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
#else // JUCE_USE_SIMD
		for(auto& in : input) {
			in *= mGainLinear;
		}
#endif // JUCE_USE_SIMD
	}
} // namespace apex::dsp
