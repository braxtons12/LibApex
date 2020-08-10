#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "GainStage.h"

namespace apex::dsp {
	/// @brief Roughly models a VCA-Style gain stage
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class GainStageVCA : public GainStage<T> {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageVCA)
	};

	/// @brief Roughly models a VCA-Style gain stage
	template<>
	class GainStageVCA<float> : public GainStage<float> {
	  public:
		/// @brief Constructs a default `GainStageVCA`
		GainStageVCA() noexcept = default;

		/// @brief Move constructs the given `GainStageVCA`
		///
		/// @param stage - The `GainStageVCA` to move
		GainStageVCA(GainStageVCA<float>&& stage) noexcept = default;
		~GainStageVCA() noexcept override = default;

		/// @brief Processes the input through the gain stage
		///
		/// @param input - The input to process
		///
		/// @return - The processed output
		auto process(float input) noexcept -> float override;

		auto operator=(GainStageVCA<float>&& stage) noexcept -> GainStageVCA<float>& = default;

	  private:
		static const constexpr float SATURATION_AMOUNT = 0.5F;
		static const constexpr float SATURATION_SLOPE = 0.3F;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageVCA)
	};

	/// @brief Roughly models a VCA-Style gain stage
	template<>
	class GainStageVCA<double> : public GainStage<double> {
	  public:
		/// @brief Constructs a default `GainStageVCA`
		GainStageVCA() noexcept = default;

		/// @brief Move constructs the given `GainStageVCA`
		///
		/// @param stage - The `GainStageVCA` to move
		GainStageVCA(GainStageVCA<double>&& stage) noexcept = default;
		~GainStageVCA() noexcept override = default;

		/// @brief Processes the input through the gain stage
		///
		/// @param input - The input to process
		///
		/// @return - The processed output
		auto process(double input) noexcept -> double override;

		auto operator=(GainStageVCA<double>&& stage) noexcept -> GainStageVCA<double>& = default;

	  private:
		static const constexpr double SATURATION_AMOUNT = 0.5;
		static const constexpr double SATURATION_SLOPE = 0.3;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageVCA)
	};
} // namespace apex::dsp
