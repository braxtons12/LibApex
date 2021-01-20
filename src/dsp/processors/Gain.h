#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Processor.h"

namespace apex::dsp {
	/// @brief Class used for managing, storing, and applying a specifc gain
	///
	/// @tparam FloatType - The floating point type to operate with, either `float` or `double`
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Gain final : public Processor<FloatType> {
	  public:
		/// @brief Constructs a default `Gain` with an initial linear value of 1.0
		Gain() noexcept = default;

		/// @brief Constructs a `Gain` with the given gain value.
		///
		/// @param gain - The gain value to use
		/// @param gainIsDecibels - Whether the gain value is in Decibels
		explicit Gain(Decibels gain) noexcept
			: mGainDecibels(gain), mGainLinear(narrow_cast<FloatType>(gain.getLinear())) {
		}

		/// @brief Move contructs the given `Gain`
		///
		/// @param gain - The `Gain` to move
		Gain(Gain&& gain) noexcept = default;
		~Gain() noexcept final = default;

		/// @brief Sets the gain of this `Gain` to be the given linear value
		///
		/// @param gain - The linear gain value to set this `Gain` to
		inline auto setGainLinear(FloatType gain) noexcept -> void {
#ifdef TESTING_GAIN
			Logger::LogMessage("Gain: Setting Gain linearly");
#endif
			mGainLinear = gain;
			mGainDecibels = Decibels::fromLinear(gain);
		}

		/// @brief Returns the currently set linear gain value
		///
		/// @return The linear gain value
		[[nodiscard]] inline auto getGainLinear() const noexcept -> FloatType {
			return mGainLinear;
		}

		/// @brief Sets the gain of this `Gain` to be the given Decibel value
		///
		/// @param gainDecibels - The Decibel gain value to set this `Gain` to
		inline auto setGainDecibels(Decibels gainDecibels) noexcept -> void {
#ifdef TESTING_GAIN
			Logger::LogMessage("Gain: Setting Gain Decibels-ly");
#endif
			mGainDecibels = gainDecibels;
			mGainLinear = narrow_cast<FloatType>(gainDecibels.getLinear());
		}

		/// @brief Returns the currently set gain value, in Decibels
		///
		/// @return The gain value, in Decibels
		[[nodiscard]] inline auto getGainDecibels() const noexcept -> Decibels {
			return mGainDecibels;
		}

		/// @brief Applies this `Gain` to the input
		///
		/// @param input - The input to apply the gain to
		///
		/// @return The resulting value after applying the gain
		[[nodiscard]] inline auto processMono(FloatType input) noexcept -> FloatType final {
#ifdef TESTING_GAIN
			Logger::LogMessage("Gain: Processing single input");
#endif
			return input * mGainLinear;
		}

		/// @brief Applies this `Gain` to the array of input values
		///
		/// @param input - The array of input values to apply gain to
		/// @param output - The processed values
		inline auto
		processMono(Span<FloatType> input, Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = processMono(input.at(i));
			}
		}

		/// @brief Applies this `Gain` to the array of input values
		///
		/// @param input - The array of input values to apply gain to
		/// @param output - The processed values
		inline auto
		processMono(Span<const FloatType> input, Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = processMono(input.at(i));
			}
		}

		/// @brief Applies this `Gain` to the input values
		///
		/// @param inputLeft - The left channel sample to apply gain to
		/// @param inputRight - The right channel sample to apply gain to
		///
		/// @return The pair of processed values
		inline auto processStereo(FloatType inputLeft, FloatType inputRight) noexcept
			-> std::tuple<FloatType, FloatType> final {
			return {inputLeft * mGainLinear, inputRight * mGainLinear};
		}

		/// @brief Applies this `Gain` to the input values
		///
		/// @param inputLeft - The left channel samples to apply gain to
		/// @param inputRight - The right channel samples to apply gain to
		/// @param outputLeft - The left channel processed samples
		/// @param outputRight - The right channel processed samples
		inline auto processStereo(Span<FloatType> inputLeft,
								  Span<FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [left, right] = processStereo(inputLeft.at(i), inputRight.at(i));

				outputLeft.at(i) = left;
				outputRight.at(i) = right;
			}
		}

		/// @brief Applies this `Gain` to the input values
		///
		/// @param inputLeft - The left channel samples to apply gain to
		/// @param inputRight - The right channel samples to apply gain to
		/// @param outputLeft - The left channel processed samples
		/// @param outputRight - The right channel processed samples
		inline auto processStereo(Span<const FloatType> inputLeft,
								  Span<const FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [left, right] = processStereo(inputLeft.at(i), inputRight.at(i));

				outputLeft.at(i) = left;
				outputRight.at(i) = right;
			}
		}

		auto reset() noexcept -> void final {
#ifdef TESTING_GAIN
			Logger::LogMessage("Gain: Resetting");
#endif
		}

		auto operator=(Gain&& gain) noexcept -> Gain& = default;

	  private:
		/// The linear gain value
		FloatType mGainLinear = narrow_cast<FloatType>(1.0);
		/// The gain value, in Decibels
		Decibels mGainDecibels = 0.0_dB;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
	};
} // namespace apex::dsp
