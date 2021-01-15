#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	/// @brief Base Meter interface
	///
	/// @tparam FloatType - The floating point type to perform operations with
	/// @tparam Enable - disables the type of T is not a floating point type
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Meter {
	  public:
		Meter() noexcept = default;
		virtual ~Meter() noexcept = default;
		/// @brief Sets the sample rate to the given value
		///
		/// @param SampleRate - The new sample rate
		virtual auto setSampleRate(Hertz SampleRate) noexcept -> void = 0;

		/// @brief Resets the meter to an initial state
		virtual auto reset() noexcept -> void = 0;

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		virtual auto update(FloatType input) noexcept -> void = 0;

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		virtual auto update(Span<FloatType> input) noexcept -> void = 0;

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		virtual auto update(Span<const FloatType> input) noexcept -> void = 0;

		/// @brief Returns the current linear level of the meter
		///
		/// @return - The linear level
		[[nodiscard]] virtual auto getLevel() const noexcept -> FloatType = 0;

		/// @brief Returns the current level of the meter, in Decibels
		///
		/// @return - The level in Decibels
		[[nodiscard]] virtual auto getLevelDB() const noexcept -> Decibels = 0;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter)
	};
} // namespace apex::dsp
