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
	/// @tparam T - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicState`
	template<typename T, typename AttackKind, typename ReleaseKind>
	class GainReduction {
	  protected:
		using Field = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>;

	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
		static_assert((std::is_floating_point<AttackKind>::value
					   && std::is_same<T, AttackKind>::value)
						  || std::is_enum<AttackKind>::value,
					  "AttackKind must be the same floating point type as T, or an enum");
		static_assert((std::is_floating_point<ReleaseKind>::value
					   && std::is_same<T, ReleaseKind>::value)
						  || std::is_enum<ReleaseKind>::value,
					  "ReleaseKind must be the same floating point type as T, or an enum");

		/// @brief Constructs a default `GainReduction` - zeroed state, zero rise time
		GainReduction() noexcept = default;

		/// @brief Constructs a `GainReduction` with the given shared state and rise time
		///
		/// @param state - Non-Owning pointer to the shared dynamics state
		/// @param riseTimeSeconds - The rise time, in seconds
		explicit GainReduction(DynamicsState* state, T riseTimeSeconds = 0) noexcept
			: mState(state), mRiseTimeSeconds(riseTimeSeconds) {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Creating Base Gain Reduction");
	#endif
			mState->template registerCallback<Hertz, Field::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
			mRiseCoefficient = gsl::narrow_cast<T>(Exponentials<double>::exp(
				-1.0 / (mRiseTimeSeconds * static_cast<double>(mState->getSampleRate()))));
		}

		/// @brief Move constructs the given `GainReduction`
		///
		/// @param reduction - The `GainReduction` to move
		GainReduction(GainReduction<T, AttackKind, ReleaseKind>&& reduction) noexcept = default;
		virtual ~GainReduction() noexcept = default;

		/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
		///
		/// @param gainReduction - The gain reduction determined by the gain computer
		///
		/// @return The adjusted gain reduction
		[[nodiscard]] virtual auto
		adjustedGainReduction(Decibels gainReduction) noexcept -> Decibels {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage(
				"Base Gain Reduction Calculating Adjusted Gain Reduction");
	#endif
			T sign = 1.0;
			if(gainReduction < 0.0) {
				sign = -sign;
				gainReduction *= sign;
			}

			mCurrentGainReduction = sign
									* (mRiseCoefficient * mCurrentGainReduction
									   + (gsl::narrow_cast<T>(1.0) - mRiseCoefficient)
											 * gsl::narrow_cast<T>(gainReduction));
			return mCurrentGainReduction;
		}

		/// @brief Resets this `GainReduction` to an initial state.
		///
		/// @param currentGainReduction - The gain reduction to use as the initial value
		inline virtual auto reset(T currentGainReduction = 0) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Resetting");
	#endif
			mCurrentGainReduction = currentGainReduction;
		}

		/// @brief Sets the sample rate to use for calculations to the given value
		///
		/// @param sampleRate - The new sample rate to use
		inline virtual auto setSampleRate(Hertz sampleRate) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Updating Sample Rate");
	#endif
			mRiseCoefficient = gsl::narrow_cast<T>(Exponentials<double>::exp(
				-1.0 / (mRiseTimeSeconds * static_cast<double>(sampleRate))));
		}

		/// @brief Sets the slew rate to use for calculations to the given value
		///
		/// @param seconds - The new slew rate
		inline virtual auto setRiseTimeSeconds(T seconds) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Updating Rise Time");
	#endif
			mRiseTimeSeconds = seconds;
			mRiseCoefficient = gsl::narrow_cast<T>(Exponentials<double>::exp(
				-1.0 / (mRiseTimeSeconds * static_cast<double>(mState->getSampleRate()))));
		}

		/// @brief Sets the shared state to the given one
		///
		/// @param state - The shared state to use
		inline auto setState(DynamicsState* state) noexcept -> void {
	#ifdef TESTING_GAIN_REDUCTION
			apex::utils::Logger::LogMessage("Base Gain Reduction Updating Dynamics State");
	#endif
			mState = state;
		}

		auto operator=(GainReduction<T, AttackKind, ReleaseKind>&& reduction) noexcept
			-> GainReduction<T, AttackKind, ReleaseKind>& = default;

	  protected:
		/// prevent us from having to `new` or use a smart pointer
		/// just to get our zeroed state
		DynamicsState DEFAULT_STATE = DynamicsState();
		DynamicsState* mState = &DEFAULT_STATE;
		T mRiseCoefficient = gsl::narrow_cast<T>(0.1);
		/// The current gain reduction value
		Decibels mCurrentGainReduction = gsl::narrow_cast<T>(0.0);
		/// The slew rate
		T mRiseTimeSeconds = gsl::narrow_cast<T>(0.00001);

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReduction)
	};
} // namespace apex::dsp

#endif // GAIN_REDUCTION
