#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	///@brief Base Interface describing behavior of a `GainStage`
	///
	///@tparam T - The floating point type to back operations
	template<typename T>
	class GainStage {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

		///@brief Constructs a default `GainStage`
		GainStage() noexcept = default;

		///@brief Move constructs a `GainStage` from the given one
		///
		///@param stage - The `GainStage` to move
		GainStage(GainStage<T>&& stage) noexcept = default;
		virtual ~GainStage() noexcept = default;

		///@brief Processes the input according to this `GainStage`'s parameters
		///
		///@param input - The input to process
		///
		///@return - The processed output
		[[nodiscard]] virtual auto process(T input) noexcept -> T = 0;

		auto operator=(GainStage<T>&& stage) noexcept -> GainStage<T>& = default;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStage)
	};
} // namespace apex::dsp
