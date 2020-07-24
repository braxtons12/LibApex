#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Processor.h"

namespace apex::dsp {
	/// @brief Class used for managing, storing, and applying a specifc gain.
	///
	/// @tparam T - The floating point type to operate with, either `float` or `double`
	template<typename T>
		class Gain : public Processor<T> {
			public:
				static_assert(std::is_floating_point<T>::value, "Gain must be instantiated with a floating point template parameter (float or double)");

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
		};

	/// @brief Class used for managing, storing, and applying a specifc gain; operating with `float`s
	template<>
		class Gain<float> : public Processor<float> {
			public:

				/// @brief Constructs a default `Gain` with an initial linear value of 1.0
				Gain() noexcept = default;

				/// @brief Constructs a `Gain` with the given gain value.
				///
				/// @param gain - The gain value to use
				/// @param gainIsDecibels - Whether the gain value is in Decibels
				explicit Gain(float gain, bool gainIsDecibels = false) noexcept;

				/// @brief Move contructs the given `Gain`
				///
				/// @param gain - The `Gain` to move
				Gain(Gain<float>&& gain) noexcept = default;
				~Gain() noexcept override = default;

				/// @brief Sets the gain of this `Gain` to be the given linear value
				///
				/// @param gain - The linear gain value to set this `Gain` to
				auto setGainLinear(float gain) noexcept -> void;

				/// @brief Returns the currently set linear gain value
				///
				/// @return The linear gain value
				[[nodiscard]]
					auto getGainLinear() const noexcept -> float;

				/// @brief Sets the gain of this `Gain` to be the given Decibel value
				///
				/// @param gainDecibels - The Decibel gain value to set this `Gain` to
				auto setGainDecibels(float gainDecibels) noexcept -> void;

				/// @brief Returns the currently set gain value, in Decibels
				///
				/// @return The gain value, in Decibels
				[[nodiscard]]
					auto getGainDecibels() const noexcept -> float;

				/// @brief Applies this `Gain` to the input
				///
				/// @param input - The input to apply the gain to
				///
				/// @return The resulting value after applying the gain
				[[nodiscard]]
					auto process(float input) noexcept -> float override;

				/// @brief Applies this `Gain` to the array of input values, in place
				///
				/// @param input - The array of input values to apply gain to
				void process(gsl::span<float, gsl::dynamic_extent> input) noexcept override;

				auto reset() noexcept -> void override {

				}

				auto operator=(Gain<float>&& gain) noexcept -> Gain<float>& = default;

			private:
				///The linear gain value
				float mGainLinear = 1.0F;
				///The gain value, in Decibels
				float mGainDecibels = 0.0F;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
		};

	/// @brief Class used for managing, storing, and applying a specifc gain; operating with `double`s
	template<>
		class Gain<double> : public Processor<double> {
			public:

				/// @brief Constructs a default `Gain` with an initial linear value of 1.0
				Gain() noexcept = default;

				/// @brief Constructs a `Gain` with the given gain value.
				///
				/// @param gain - The gain value to use
				/// @param gainIsDecibels - Whether the gain value is in Decibels
				explicit Gain(double gain, bool gainIsDecibels = false) noexcept;

				/// @brief Move constructs the given `Gain`
				///
				/// @param gain - The `Gain` to move
				Gain(Gain<double>&& gain) noexcept = default;
				~Gain() noexcept override = default;

				/// @brief Sets the gain of this `Gain` to be the given linear value
				///
				/// @param gain - The linear gain value to set this `Gain` to
				auto setGainLinear(double gain) noexcept -> void;

				/// @brief Returns the currently set linear gain value
				///
				/// @return The linear gain value
				[[nodiscard]]
					auto getGainLinear() const noexcept -> double;

				/// @brief Sets the gain of this `Gain` to be the given Decibel value
				///
				/// @param gainDecibels - The Decibel gain value to set this `Gain` to
				auto setGainDecibels(double gainDecibels) noexcept -> void;

				/// @brief Returns the currently set gain value, in Decibels
				///
				/// @return The gain value, in Decibels
				[[nodiscard]]
					auto getGainDecibels() const noexcept -> double;

				/// @brief Applies this `Gain` to the input
				///
				/// @param input - The input to apply the gain to
				///
				/// @return The resulting value after applying the gain
				[[nodiscard]]
					auto process(double input) noexcept -> double override;

				/// @brief Applies this `Gain` to the array of input values, in place
				///
				/// @param input - The array of input values to apply gain to
				auto process(gsl::span<double, gsl::dynamic_extent> input)
					noexcept -> void override;

				auto reset() noexcept -> void override {

				}

				auto operator=(Gain<double>&& gain) noexcept -> Gain<double>& = default;

			private:
				///The linear gain value
				double mGainLinear = 1.0;
				///The gain value, in Decibels
				double mGainDecibels = 0.0;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
		};
} //namespace apex::dsp
