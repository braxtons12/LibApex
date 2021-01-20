#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "GainComputer.h"

#ifndef GAIN_COMPUTER_COMPRESSOR
	#define GAIN_COMPUTER_COMPRESSOR

namespace apex::dsp {
	/// @brief Gain Computer for compressors
	///
	/// @tparam FloatType - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainComputerCompressor final : public GainComputer<FloatType, AttackKind, ReleaseKind> {
	  protected:
		using DynamicsState = DynamicsState<FloatType, AttackKind, ReleaseKind>;
		using GainComputer = GainComputer<FloatType, AttackKind, ReleaseKind>;

	  public:
	#ifdef TESTING_GAIN_COMPUTER_COMPRESSOR
		/// @brief Constructs a `GainComputerCompressor` with zeroed shared state
		GainComputerCompressor() noexcept {
			apex::utils::Logger::LogMessage("Creating Compressor Gain Computer");
		}
	#else
		/// @brief Constructs a `GainComputerCompressor` with zeroed shared state
		GainComputerCompressor() noexcept = default;
	#endif

		/// @brief Constructs a `GainComputerCompressor` with the given shared state
		///
		/// @param state - The shared state
		explicit GainComputerCompressor(DynamicsState* state) noexcept : GainComputer(state) {
	#ifdef TESTING_GAIN_COMPUTER_COMPRESSOR
			apex::utils::Logger::LogMessage("Creating Compressor Gain Computer");
	#endif
		}

		/// @brief Move constructs the given `GainComputerCompressor`
		///
		/// @param computer - The `GainComputerCompressor` to move
		GainComputerCompressor(GainComputerCompressor&& computer) noexcept = default;
		~GainComputerCompressor() final = default;

		/// @brief Calculates the target gain reduction value
		///
		/// @param input - The input to calculate gain reduction for
		///
		/// @return - The target gain reduction
		[[nodiscard]] inline auto process(Decibels input) noexcept -> Decibels final {
	#ifdef TESTING_GAIN_COMPUTER_COMPRESSOR
			apex::utils::Logger::LogMessage(
				"Gain Computer Compressor Calculating Compressed Output");
	#endif
			constexpr auto one = narrow_cast<FloatType>(1.0);
			constexpr auto two = narrow_cast<FloatType>(2.0);
			Decibels threshold = GainComputer::mState->getThreshold();
			auto ratio = GainComputer::mState->getRatio();
			Decibels kneeWidth = GainComputer::mState->getKneeWidth();

			auto twoXMinusT = two * (input - GainComputer::mState->getThreshold());
			if(twoXMinusT < -kneeWidth) {
				return Decibels(input);
			}
			else if(twoXMinusT > kneeWidth) {
				return threshold + (input - threshold) / ratio;
			}
			else {
				return Decibels(input
								+ ((one / ratio) - one)
									  * Exponentials<FloatType>::pow2(narrow_cast<FloatType>(
										  input - threshold + kneeWidth / two))
									  / (two * kneeWidth));
			}
		}

		auto
		operator=(GainComputerCompressor&& computer) noexcept -> GainComputerCompressor& = default;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
	};
} // namespace apex::dsp

#endif // GAIN_COMPUTER_COMPRESSOR
