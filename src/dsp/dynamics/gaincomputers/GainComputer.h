// clang-format on
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

#ifndef GAIN_COMPUTER
	#define GAIN_COMPUTER

namespace apex::dsp {
	/// @brief  Base Gain Computer behaviors for use in a dynamic range processor's `Sidechain`
	///
	/// @tparam FloatType - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainComputer {
	  protected:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, AttackKind, ReleaseKind>;

	  public:
		/// @brief Constructs a `GainComputer` with zeroed shared state
		GainComputer() noexcept = default;

		/// @brief Constructs a `GainComputer` with the given shared state
		///
		/// @param state - The shared state
		explicit GainComputer(DynamicsState* state) noexcept : mState(state) {
	#ifdef TESTING_GAIN_COMPUTER
			apex::utils::Logger::LogMessage("Creating Base Gain Computer");
	#endif
		}

		/// @brief Move constructs the given `GainComputer`
		///
		/// @param computer - The `GainComputer` to move
		GainComputer(GainComputer&& computer) noexcept = default;
		virtual ~GainComputer() noexcept = default;

		/// @brief Calculates the target gain reduction value
		///
		/// @param input - The input to calculate gain reduction for
		///
		/// @return - The target gain reduction
		[[nodiscard]] virtual auto process(Decibels input) noexcept -> Decibels = 0;

		auto operator=(GainComputer&& computer) noexcept -> GainComputer& = default;

	  protected:
		static const constexpr DynamicsState DEFAULT_STATE = DynamicsState();
		DynamicsState* mState = &DEFAULT_STATE;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputer)
	};
} // namespace apex::dsp

#endif // GAIN_COMPUTER
