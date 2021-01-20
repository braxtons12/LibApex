#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "../WaveShaper.h"
#include "GainStage.h"

namespace apex::dsp {
	/// @brief Roughly models a FET-style gain stage
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class GainStageFET final : public GainStage<FloatType> {
	  public:
		/// @brief Constructs a default `GainStageFET`
		GainStageFET() noexcept = default;

		/// @brief Move constructs the given `GainStageFET`
		///
		/// @param stage - The `GainStageFET` to move
		GainStageFET(GainStageFET&& stage) noexcept = default;
		~GainStageFET() noexcept final = default;

		/// @brief Processes the input through the gain stage
		///
		/// @param input - The input to process
		///
		/// @return - The processed output
		[[nodiscard]] inline auto process(FloatType input) noexcept -> FloatType final {
			return waveshapers::softSaturation<FloatType>(input,
														  SATURATION_AMOUNT,
														  SATURATION_SLOPE);
		}

		/// @brief Processes the input according to this `GainStage`'s parameters'
		///
		/// @param input - The input to process
		/// @param output - The processed output
		inline auto process(Span<FloatType> input, Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = waveshapers::softSaturation<FloatType>(input.at(i),
																	  SATURATION_AMOUNT,
																	  SATURATION_SLOPE);
			}
		}

		/// @brief Processes the input according to this `GainStage`'s parameters'
		///
		/// @param input - The input to process
		/// @param output - The processed output
		inline auto
		process(Span<const FloatType> input, Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = waveshapers::softSaturation<FloatType>(input.at(i),
																	  SATURATION_AMOUNT,
																	  SATURATION_SLOPE);
			}
		}

		auto operator=(GainStageFET&& stage) noexcept -> GainStageFET& = default;

	  private:
		static const constexpr FloatType SATURATION_AMOUNT = narrow_cast<FloatType>(0.5);
		static const constexpr FloatType SATURATION_SLOPE = narrow_cast<FloatType>(0.7);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
	};
} // namespace apex::dsp
