#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	enum class ProcessorChannel
	{
		Mono = 0,
		Left = 0,
		Right
	};

	/// @brief Interface specifying necessary behaviors of an audio processor
	///
	/// @tparam T - The floating point type backing operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Processor {
	  public:
		Processor() noexcept = default;

		Processor(Processor&& proc) noexcept = default;

		virtual ~Processor() noexcept = default;

		/// @brief Processes the input value
		///
		/// @param input - The input value to process
		///
		/// @return - The processed value
		[[nodiscard]] auto processMono(FloatType input) noexcept -> FloatType;

		/// @brief Processes the array of input values
		///
		/// @param input - The input values to process
		/// @param output - The processed values
		auto processMono(Span<FloatType> input, Span<FloatType> output) noexcept -> void;

		/// @brief Processes the array of input values
		///
		/// @param input - The input values to process
		/// @param output - The processed values
		auto processMono(Span<const FloatType> input, Span<FloatType> output) noexcept -> void;

		/// @brief Processes the array of input values
		///
		/// @param inputLeft - The left channel input value to process
		/// @param inputRight - The right channel input value to process
		///
		/// @return - The processed values
		[[nodiscard]] virtual auto processStereo(FloatType inputLeft, FloatType inputRight) noexcept
			-> std::tuple<FloatType, FloatType> = 0;

		/// @brief Processes the array of input values
		///
		/// @param inputLeft - The left channel input value to process
		/// @param inputRight - The right channel input value to process
		/// @param outputRight - The left channel processed value
		/// @param outputRight - The right channel processed value
		virtual auto processStereo(Span<FloatType> inputLeft,
								   Span<FloatType> inputRight,
								   Span<FloatType> outputLeft,
								   Span<FloatType> outputRight) noexcept -> void
			= 0;

		/// @brief Processes the array of input values
		///
		/// @param inputLeft - The left channel input value to process
		/// @param inputRight - The right channel input value to process
		/// @param outputRight - The left channel processed value
		/// @param outputRight - The right channel processed value
		virtual auto processStereo(Span<const FloatType> inputLeft,
								   Span<const FloatType> inputRight,
								   Span<FloatType> outputLeft,
								   Span<FloatType> outputRight) noexcept -> void
			= 0;

		/// @brief Resets the processor to an initial state
		virtual auto reset() noexcept -> void = 0;

		auto operator=(Processor&& proc) noexcept -> Processor& = default;

	  protected:
		static constexpr size_t MONO = static_cast<size_t>(ProcessorChannel::Mono);
		static constexpr size_t LEFT = static_cast<size_t>(ProcessorChannel::Left);
		static constexpr size_t RIGHT = static_cast<size_t>(ProcessorChannel::Right);
		static constexpr size_t MAX_CHANNELS = RIGHT + 1U;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
	};
} // namespace apex::dsp
