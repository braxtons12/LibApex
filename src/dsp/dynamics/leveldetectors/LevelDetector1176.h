#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "LevelDetector.h"

namespace apex::dsp {
	/// @brief 1176-style Level Detector for Dynamic Range Processors
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class LevelDetector1176 : public LevelDetector<T> {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
	};

	/// @brief 1176-style Level Detector for Dynamic Range Processors
	template<>
	class LevelDetector1176<float> : public LevelDetector<float> {
	  private:
		using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

	  public:
		/// @brief Constructs a `LevelDetector1176` with zereod shared state
		LevelDetector1176() noexcept;

		/// @brief Constructs a `LevelDetector1176` with the given shared state
		///
		/// @param state - The shared state
		explicit LevelDetector1176(DynamicsState* state) noexcept;

		/// @brief Move constructs a `LevelDetector1176` from the given one
		///
		/// @param detector - The `LevelDetector1176` to move
		LevelDetector1176(LevelDetector1176<float>&& detector) noexcept = default;
		~LevelDetector1176() noexcept override = default;

		auto operator=(LevelDetector1176<float>&& detector) noexcept
			-> LevelDetector1176<float>& = default;

	  private:
		static const constexpr float DEFAULT_ATTACK_SECONDS = 0.00039F;
		static const constexpr float DEFAULT_RELEASE_SECONDS = 0.525F;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
	};

	/// @brief 1176-style Level Detector for Dynamic Range Processors
	template<>
	class LevelDetector1176<double> : public LevelDetector<double> {
	  private:
		using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

	  public:
		/// @brief Constructs a `LevelDetector1176` with zereod shared state
		LevelDetector1176() noexcept;

		/// @brief Constructs a `LevelDetector1176` with the given shared state
		///
		/// @param state - The shared state
		explicit LevelDetector1176(DynamicsState* state) noexcept;

		/// @brief Move constructs a `LevelDetector1176` from the given one
		///
		/// @param detector - The `LevelDetector1176` to move
		LevelDetector1176(LevelDetector1176<double>&& detector) noexcept = default;
		~LevelDetector1176() noexcept override = default;

		auto operator=(LevelDetector1176<double>&& detector) noexcept
			-> LevelDetector1176<double>& = default;

	  private:
		static const constexpr double DEFAULT_ATTACK_SECONDS = 0.00039;
		static const constexpr double DEFAULT_RELEASE_SECONDS = 0.525;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector1176)
	};
} // namespace apex::dsp
