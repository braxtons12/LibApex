#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "GainComputer.h"

#ifndef GAIN_COMPUTER_EXPANDER
	#define GAIN_COMPUTER_EXPANDER

namespace apex::dsp {
	/// @brief Gain Computer for expanders
	///
	/// @tparam FloatType - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainComputerExpander final : public GainComputer<FloatType, AttackKind, ReleaseKind> {
	  protected:
		using DynamicsState = DynamicsState<FloatType, AttackKind, ReleaseKind>;
		using GainComputer = GainComputer<FloatType, AttackKind, ReleaseKind>;

	  public:
	#ifdef TESTING_GAIN_COMPUTER_EXPANDER
		/// @brief Constructs a `GainComputerExpander` with zeroed shared state
		GainComputerExpander() noexcept {
			apex::utils::Logger::LogMessage("Creating Expander Gain Computer");
		}
	#else
		/// @brief Constructs a `GainComputerExpander` with zeroed shared state
		GainComputerExpander() noexcept = default;
	#endif

		/// @brief Constructs a `GainComputerExpander` with the given shared state
		///
		/// @param state - The shared state
		explicit GainComputerExpander(DynamicsState* state) noexcept : GainComputer(state) {
	#ifdef TESTING_GAIN_COMPUTER_EXPANDER
			apex::utils::Logger::LogMessage("Creating Expander Gain Computer");
	#endif
		}

		/// @brief Move constructs the given `GainComputerExpander`
		///
		/// @param computer - The `GainComputerExpander` to move
		GainComputerExpander(GainComputerExpander&& computer) noexcept = default;
		~GainComputerExpander() final = default;

		/// @brief Calculates the target gain reduction value
		///
		/// @param input - The input to calculate gain reduction for
		///
		/// @return - The target gain reduction
		[[nodiscard]] inline auto process(Decibels input) noexcept -> Decibels final {
	#ifdef TESTING_GAIN_COMPUTER_EXPANDER
			apex::utils::Logger::LogMessage("Gain Computer Expander Calculating Expanded Output");
	#endif
			constexpr auto one = narrow_cast<FloatType>(1.0);
			constexpr auto two = narrow_cast<FloatType>(2.0);
			Decibels threshold = GainComputer::mState->getThreshold();
			auto ratio = GainComputer::mState->getRatio();
			Decibels kneeWidth = GainComputer::mState->getKneeWidth();

			auto twoXMinusT = two * (input - GainComputer::mState->getThreshold());
			if(twoXMinusT < -kneeWidth) {
				return threshold + (input - threshold) * ratio;
			}
			else if(twoXMinusT > kneeWidth) {
				return input;
			}
			else {
				return input
					   + (one - ratio)
							 * Exponentials<FloatType>::pow2(
								 narrow_cast<FloatType>(input - threshold - kneeWidth / two))
							 / (two * kneeWidth);
			}
		}
		auto operator=(GainComputerExpander&& computer) noexcept -> GainComputerExpander& = default;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
	};
} // namespace apex::dsp

#endif // GAIN_COMPUTER_EXPANDER
