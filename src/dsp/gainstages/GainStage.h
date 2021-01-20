#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	///@brief Base Interface describing behavior of a `GainStage`
	///
	///@tparam T - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class GainStage {
	  public:
		///@brief Constructs a default `GainStage`
		GainStage() noexcept = default;

		///@brief Move constructs a `GainStage` from the given one
		///
		///@param stage - The `GainStage` to move
		GainStage(GainStage&& stage) noexcept = default;
		virtual ~GainStage() noexcept = default;

		///@brief Processes the input according to this `GainStage`'s parameters
		///
		///@param input - The input to process
		///
		///@return - The processed output
		[[nodiscard]] virtual auto process(FloatType input) noexcept -> FloatType {
			return input;
		}

		/// @brief Processes the input according to this `GainStage`'s parameters'
		///
		/// @param input - The input to process
		/// @param output - The processed output
		virtual auto process(Span<FloatType> input, Span<FloatType> output) noexcept -> void {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = input.at(i);
			}
		}

		/// @brief Processes the input according to this `GainStage`'s parameters'
		///
		/// @param input - The input to process
		/// @param output - The processed output
		virtual auto process(Span<const FloatType> input, Span<FloatType> output) noexcept -> void {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = input.at(i);
			}
		}

		auto operator=(GainStage&& stage) noexcept -> GainStage& = default;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStage)
	};
} // namespace apex::dsp
