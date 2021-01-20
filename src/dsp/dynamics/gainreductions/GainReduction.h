#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

#ifndef GAIN_REDUCTION
	#define GAIN_REDUCTION

namespace apex::dsp {
	/// @brief Base class for calculating gain reduction values adjusted to match different topology
	/// behaviors. Base implementation only performs basic slew-rate adjustments.
	///
	/// @tparam FloatType - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicState`
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class GainReduction {
	  protected:
		using DynamicsState = DynamicsState<FloatType, AttackKind, ReleaseKind>;

	  public:
		/// @brief Constructs a default `GainReduction` - zeroed state, zero rise time
		GainReduction() noexcept = default;

		/// @brief Constructs a `GainReduction` with the given shared state and rise time
		///
		/// @param state - Non-Owning pointer to the shared dynamics state
		/// @param riseTimeSeconds - The rise time, in seconds
		explicit GainReduction(DynamicsState* state,
							   FloatType riseTimeSeconds = narrow_cast<FloatType>(0.0)) noexcept
			: mState(state), mRiseTimeSeconds(riseTimeSeconds),
			  mRiseCoefficient(calculateRiseCoefficient(state->getSampleRate())) {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Creating Base Gain Reduction");
	#endif
			mState->template registerCallback<Hertz, DynamicsField::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
		}

		/// @brief Move constructs the given `GainReduction`
		///
		/// @param reduction - The `GainReduction` to move
		GainReduction(GainReduction&& reduction) noexcept = default;
		virtual ~GainReduction() noexcept = default;

		/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return The adjusted gain reduction
		[[nodiscard]] virtual inline auto
		adjustedGainReduction(Decibels gainReduction) noexcept -> Decibels {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage(
				"Base Gain Reduction Calculating Adjusted Gain Reduction");
	#endif
			auto sign = narrow_cast<FloatType>(1.0);
			if(gainReduction < narrow_cast<FloatType>(0.0)) {
				sign = -sign;
				gainReduction *= sign;
			}

			mCurrentGainReduction = sign
									* (mRiseCoefficient * mCurrentGainReduction
									   + (narrow_cast<FloatType>(1.0) - mRiseCoefficient)
											 * narrow_cast<FloatType>(gainReduction));
			return mCurrentGainReduction;
		}

		/// @brief Resets this `GainReduction` to an initial state.
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		virtual inline auto
		reset(FloatType currentGainReduction = narrow_cast<FloatType>(0.0)) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Resetting");
	#endif
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		virtual inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Updating Sample Rate");
	#endif
			mRiseCoefficient = calculateRiseCoefficient(sampleRate);
		}

		/// @brief Sets the slew rate to use for calculations to the given value
		///
		/// @param seconds - The new slew rate
		virtual inline auto setRiseTimeSeconds(FloatType seconds) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Updating Rise Time");
	#endif
			mRiseTimeSeconds = seconds;
			mRiseCoefficient = calculateRiseCoefficient(mState->getSampleRate());
		}

		/// @brief Sets the shared state to the given one
		///
		/// @param state - The shared state to use
		virtual inline auto setState(DynamicsState* state) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Updating Dynamics State");
	#endif
			mState = state;
			mState->template registerCallback<Hertz, DynamicsField::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
		}

		auto operator=(GainReduction&& reduction) noexcept -> GainReduction& = default;

	  protected:
		/// prevent us from having to `new` or use a smart pointer
		/// just to get our zeroed state
		DynamicsState DEFAULT_STATE = DynamicsState();
		DynamicsState* mState = &DEFAULT_STATE;
		/// The slew rate
		FloatType mRiseTimeSeconds = narrow_cast<FloatType>(1e-9);
		/// The LPF coefficient for rise time
		FloatType mRiseCoefficient = narrow_cast<FloatType>(0.1);
		/// The current gain reduction value
		Decibels mCurrentGainReduction = narrow_cast<FloatType>(0.0);

		inline virtual auto calculateRiseCoefficient(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mRiseTimeSeconds * narrow_cast<FloatType>(sampleRate)));
		}

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReduction)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION
