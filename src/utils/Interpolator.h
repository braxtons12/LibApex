#pragma once

#include <type_traits>
// for std::ignore
#include <gsl/gsl>
#include <tuple>

#include "../math/Exponentials.h"
#include "Concepts.h"
#include "OptionAndResult.h"

namespace apex::utils {
	using concepts::FloatingPoint;
	/// @brief The possible kinds of value interpolation possible by `Interpolator`
	enum class InterpolationType
	{
		Linear = 0,
		Logarithmic,
		Exponential
	};

	/// @brief Interpolates from a starting value to a final value in the way prescribed by `Type`
	/// @note `T` must be a floating point type
	/// @note The start value must be in [0, target value] and the target value must be > 0
	///
	/// @tparam T - The floating point type to perform operations in
	/// @tparam Type - The type of interpolation
	template<FloatingPoint T, InterpolationType Type = InterpolationType::Linear>
	class Interpolator {
	  public:
		static constexpr T DEFAULT_TARGET_VALUE = gsl::narrow_cast<T>(1.0);
		static constexpr T DEFAULT_INITIAL_VALUE = gsl::narrow_cast<T>(0.0);
		static constexpr T DEFAULT_TRANSITION_LENGTH = gsl::narrow_cast<T>(0.1);
		static constexpr size_t DEFAULT_SAMPLE_RATE = 44100ULL;

		/// @brief Creates a default `Interpolator`
		constexpr Interpolator() noexcept = default;

		/// @brief Creates an `Interpolator` with the given target value
		///
		/// @param targetValue - The target final value to interpolate to
		constexpr explicit Interpolator(T targetValue) noexcept
			: mTargetValue(targetValue), mLinearTransitionStep((mTargetValue - mInitialValue)
															   / (gsl::narrow_cast<T>(mSampleRate)
																  * mTransitionLengthSeconds)),
																  mSamplesToTransition(getSamplesToTransition()) {
		}

		/// @brief Creates an `Interpolator` with the given target value and sample rate
		///
		/// @param targetValue - The target final value to interpolate to
		/// @param sampleRate - The sample rate to process at
		constexpr explicit Interpolator(T targetValue, size_t sampleRate) noexcept
			: mSampleRate(sampleRate), mTargetValue(targetValue),
			  mLinearTransitionStep(
				  (mTargetValue - mInitialValue)
				  / (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds)),
				  mSamplesToTransition(getSamplesToTransition()) {
		}

		/// @brief Creates an `Interpolator` with the given target and initial values, and sample
		/// rate
		///
		/// @param targetValue - The target final value to interpolate to
		/// @param initialValue - The initial value to start interpolation from
		/// @param sampleRate - The sample rate to process at
		constexpr Interpolator(T targetValue, T initialValue, size_t sampleRate) noexcept
			: mSampleRate(sampleRate), mTargetValue(targetValue), mCurrentValue(initialValue),
			  mInitialValue(mCurrentValue),
			  mLinearTransitionStep(
				  (mTargetValue - mInitialValue)
				  / (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds)),
				  mSamplesToTransition(getSamplesToTransition()) {
		}

		/// @brief Creates an `Interpolator` with the given target and initial values, time to
		/// interpolate over, and sample rate
		///
		/// @param targetValue - The target final value to interpolate to
		/// @param initialValue - The initial value to start interpolation from
		/// @param transitionLengthSeconds - The transition time to interpolate over
		/// @param sampleRate - The sample rate to process at
		constexpr Interpolator(T targetValue,
							   T initialValue,
							   T transitionLengthSeconds,
							   size_t sampleRate) noexcept
			: mSampleRate(sampleRate), mTargetValue(targetValue), mCurrentValue(initialValue),
			  mInitialValue(mCurrentValue), mTransitionLengthSeconds(transitionLengthSeconds),
			  mLinearTransitionStep(
				  (mTargetValue - mInitialValue)
				  / (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds)),
				  mSamplesToTransition(getSamplesToTransition()) {
		}
		constexpr Interpolator(const Interpolator& interpolator) noexcept = default;
		constexpr Interpolator(Interpolator&& interpolator) noexcept = default;
		~Interpolator() noexcept = default;

		/// @brief Gets the next value in the interpolation sequence
		/// If `currentSample` is given, gets the value in the sequence for the given sample index
		/// @note This is only available for `Interpolator`s of type
		/// `InterpolationType::Logarithmic` or `InterpolationType::Exponential`
		/// @param currentSample - The (optional) sample in the sequence to get the value for
		///
		/// @return - The interpolation value
		inline auto getNextValue(Option<size_t> currentSample = None()) noexcept -> T {
			if(currentSample.isSome()) {
				mCurrentTransitionSample = currentSample.unwrap();
			}

			if(mCurrentTransitionSample <= mSamplesToTransition) {
				mCurrentValue = interpolate(mCurrentTransitionSample);
				mCurrentTransitionSample++;
			}

			return mCurrentValue;
		}

		/// @brief Resets the `Interpolator`.
		/// - If `initialValue` is given, the `Interpolator` will start at the given one instead of
		/// `DEFAULT_INITIAL_VALUE`
		/// - If `transitionLengthSeconds` is also given, the `Interpolator` will interpolate over
		/// that length of time instead of the current one
		///
		/// @param initialValue - The initial value to start interpolation from
		/// @param transitionLengthSeconds - The transition time to interpolate over
		inline auto
		reset(Option<T> initialValue, Option<T> transitionLengthSeconds) noexcept -> void {
			if(initialValue.isSome()) {
				mCurrentValue = initialValue.unwrap();
				mInitialValue = mCurrentValue;
			}
			else {
				mCurrentValue = DEFAULT_INITIAL_VALUE;
				mInitialValue = mCurrentValue;
			}

			if(transitionLengthSeconds.isSome()) {
				mTransitionLengthSeconds = transitionLengthSeconds.unwrap();
			}

			mLinearTransitionStep = (mTargetValue - mCurrentValue)
									/ (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds);
			mSamplesToTransition = getSamplesToTransition();
			mCurrentTransitionSample = 0ULL;
		}

		/// @brief Sets the target value to interpolate to, to the given one
		///
		/// @param targetValue - The value to interpolate to
		constexpr inline auto setTarget(T targetValue) noexcept -> void {
			mTargetValue = targetValue;
			mInitialValue = mCurrentValue;
			mLinearTransitionStep = (mTargetValue - mInitialValue)
									/ (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds);
			mSamplesToTransition = getSamplesToTransition();
		}

		/// @brief Sets the sample rate to use for interpolation
		///
		/// @param sampleRate - The sample rate to use for interpolation
		inline auto setSampleRate(size_t sampleRate) noexcept -> void {
			mSampleRate = sampleRate;
			mLinearTransitionStep = (mTargetValue - mInitialValue)
									/ (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds);
			mSamplesToTransition = getSamplesToTransition();
			mCurrentTransitionSample = 0ULL;
		}

		constexpr auto
		operator=(const Interpolator& interpolator) noexcept -> Interpolator& = default;
		constexpr auto operator=(Interpolator&& interpolator) noexcept -> Interpolator& = default;

	  private:
		static constexpr T LOG_TIME_FACTOR = gsl::narrow_cast<T>(5.0);
		static constexpr T EXP_TIME_FACTOR = gsl::narrow_cast<T>(0.693);

		size_t mSampleRate = DEFAULT_SAMPLE_RATE;
		T mTargetValue = DEFAULT_TARGET_VALUE;
		T mCurrentValue = DEFAULT_INITIAL_VALUE;
		T mInitialValue = DEFAULT_INITIAL_VALUE;
		T mTransitionLengthSeconds = DEFAULT_TRANSITION_LENGTH;
		T mLinearTransitionStep = (mTargetValue - mInitialValue)
								  / (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds);
		size_t mSamplesToTransition = getSamplesToTransition();
		size_t mCurrentTransitionSample = 0;

		/// @brief Gets the interpolated value for the given sample
		///
		/// @param sample - The sample to get the interpolated value for
		///
		/// @return The interpolated value
		inline auto interpolate(size_t sample) noexcept -> T {
			if constexpr(Type == InterpolationType::Linear) {
				return linearInterpolation(sample);
			}
			else if constexpr(Type == InterpolationType::Logarithmic) {
				return logInterpolation(sample);
			}
			else {
				return expInterpolation(sample);
			}
		}

		inline auto linearInterpolation(size_t sample) noexcept -> T {
			return mInitialValue + (mLinearTransitionStep * gsl::narrow_cast<T>(sample));
		}

		/// @brief Performs "logarithmic" interpolation.
		/// In reality, this is a pseudo logarithmic shape:
		///
		/// y = transition_factor *
		/// [1 + (peak_factor - 1) * (-e^{1 - t / (transtionLength / time_factor) } ) ]
		///
		/// where t = sample / sampleRate
		///
		/// which has a  horizontal asymptote at  ~ the target value and more pronouced slope
		///
		/// @param sample - The sample to get the interpolation value for
		///
		/// @return - The interpolated value
		inline auto logInterpolation(size_t sample) noexcept -> T {
			return (mInitialValue - mTargetValue)
					   * math::Exponentials<T>::exp(
						   -gsl::narrow_cast<T>(sample)
						   / (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds
							  / LOG_TIME_FACTOR))
				   + mTargetValue;
		}

		/// @brief Performs exponential interpolation:
		///
		/// y = e^{t / (transitionLength / time_factor) } - 1
		///
		/// where t = sample / sampleRate
		///
		/// @param sample The sample to get the interpolation value for
		///
		/// @return - The interpolated value
		inline auto expInterpolation(size_t sample) noexcept -> T {
			return (mTargetValue - mInitialValue)
					   * (math::Exponentials<T>::exp(
							  gsl::narrow_cast<T>(sample)
							  / (gsl::narrow_cast<T>(mSampleRate) * mTransitionLengthSeconds
								 / EXP_TIME_FACTOR))
						  - gsl::narrow_cast<T>(1.0))
				   + mInitialValue;
		}

		/// @brief Gets the number of samples necessary to fully complete the interpolation sequence
		///
		/// @return The number of samples in the interpolation sequence
		inline auto getSamplesToTransition() noexcept -> size_t {
			return gsl::narrow_cast<size_t>(gsl::narrow_cast<T>(mSampleRate)
											* mTransitionLengthSeconds);
		}
	};

} // namespace apex::utils
