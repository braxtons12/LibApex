#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	/// @brief Interface specifying necessary behaviors of an audio processor
	///
	/// @tparam T - The floating point type backing operations
	template<typename T>
	class Processor {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type)");
		Processor() noexcept = default;

		Processor(Processor<T>&& proc) noexcept = default;

		virtual ~Processor() noexcept = default;

		/// @brief Processes the input value
		///
		/// @param input - The input value to process
		///
		/// @return - The processed value
		[[nodiscard]] virtual auto process(T input) noexcept -> T = 0;

		/// @brief Processes the array of input values, in place
		///
		/// @param input - The input values to process
		virtual auto process(Span<T> input) noexcept -> void = 0;

		/// @brief Resets the processor to an initial state
		virtual auto reset() noexcept -> void = 0;

		auto operator=(Processor<T>&& proc) noexcept -> Processor<T>& = default;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
	};
} // namespace apex::dsp
