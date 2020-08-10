#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "GainStage.h"

namespace apex::dsp {
	/// @brief Roughly models a FET-style gain stage
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class GainStageFET : public GainStage<T> {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
	};

	/// @brief Roughly models a FET-style gain stage
	template<>
	class GainStageFET<float> : public GainStage<float> {
	  public:
		/// @brief Constructs a default `GainStageFET`
		GainStageFET() noexcept = default;

		/// @brief Move constructs the given `GainStageFET`
		///
		/// @param stage - The `GainStageFET` to move
		GainStageFET(GainStageFET<float>&& stage) noexcept = default;
		~GainStageFET() noexcept override = default;

		/// @brief Processes the input through the gain stage
		///
		/// @param input - The input to process
		///
		/// @return - The processed output
		[[nodiscard]] auto process(float input) noexcept -> float override;

		auto operator=(GainStageFET<float>&& stage) noexcept -> GainStageFET<float>& = default;

	  private:
		static const constexpr float SATURATION_AMOUNT = 0.5F;
		static const constexpr float SATURATION_SLOPE = 0.7F;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
	};

	/// @brief Roughly models a FET-style gain stage
	template<>
	class GainStageFET<double> : public GainStage<double> {
	  public:
		/// @brief Constructs a default `GainStageFET`
		GainStageFET() noexcept = default;

		/// @brief Move constructs the given `GainStageFET`
		///
		/// @param stage - The `GainStageFET` to move
		GainStageFET(GainStageFET<double>&& stage) noexcept = default;
		~GainStageFET() noexcept override = default;

		/// @brief Processes the input through the gain stage
		///
		/// @param input - The input to process
		///
		/// @return - The processed output
		[[nodiscard]] auto process(double input) noexcept -> double override;

		auto operator=(GainStageFET<double>&& stage) noexcept -> GainStageFET<double>& = default;

	  private:
		static const constexpr double SATURATION_AMOUNT = 0.5;
		static const constexpr double SATURATION_SLOPE = 0.7;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
	};
} // namespace apex::dsp
