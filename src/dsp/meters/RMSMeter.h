#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Meter.h"

namespace apex::dsp {
	/// @brief Basic RMS Meter
	///
	/// @tparam T - The floating point type to perform operations with
	/// @tparam Enable - disables the type of T is not a floating point type
	template<typename T, typename Enable = std::enable_if_t<std::is_floating_point_v<T>, float>>
	class RMSMeter final : public Meter<T, Enable> {
	  public:
		/// @brief Constructs a default `RMSMeter`
		RMSMeter() noexcept = default;

		/// @brief Constructs an `RMSMeter` with the given sample rate
		///
		/// @param sampleRate - The sample rate to use
		explicit RMSMeter(Hertz sampleRate) noexcept : mSampleRate(sampleRate) {
			mAveragingCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mAveragingLengthSeconds * static_cast<double>(mSampleRate))));
			mAttackCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate))));
			mReleaseCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mReleaseSeconds * static_cast<double>(mSampleRate))));
		}

		/// @brief Move contructs the given `RMSMeter`
		///
		/// @param meter - The `RMSMeter` to move
		RMSMeter(RMSMeter<T, Enable>&& meter) noexcept = default;
		~RMSMeter() noexcept final = default;

		/// @brief Sets the sample rate to the given value
		///
		/// @param SampleRate - The new sample rate
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void final {
			mSampleRate = sampleRate;
			mAveragingCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mAveragingLengthSeconds * static_cast<double>(mSampleRate))));
			mAttackCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate))));
			mReleaseCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mReleaseSeconds * static_cast<double>(mSampleRate))));
		}

		/// @brief Resets the meter to an initial state
		inline auto reset() noexcept -> void final {
			mCurrentLevel = gsl::narrow_cast<T>(0.0);
			mY2N1 = gsl::narrow_cast<T>(0.0);
			mAverageN1 = gsl::narrow_cast<T>(0.0);
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(T input) noexcept -> void final {
			T y2nAverage = mAveragingCoeff * mAverageN1 + (1.0 - mAveragingCoeff) * (input * input);
			mAverageN1 = y2nAverage;
			T yn = gsl::narrow_cast<T>(math::sqrt(y2nAverage));
			if(yn > mCurrentLevel) {
				mCurrentLevel = mAttackCoeff * mCurrentLevel + (1.0 - mAttackCoeff) * yn;
			}
			else {
				mCurrentLevel *= mReleaseCoeff;
			}
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(Span<T> input) noexcept -> void final {
			for(auto& in : input) {
				update(in);
			}
		}

		/// @brief Returns the current linear level of the meter
		///
		/// @return - The linear level
		[[nodiscard]] inline auto getLevel() const noexcept -> T final {
			return mCurrentLevel;
		}

		/// @brief Returns the current level of the meter, in Decibels
		///
		/// @return - The level in Decibels
		[[nodiscard]] inline auto getLevelDB() const noexcept -> Decibels final {
			return Decibels::fromLinear(mCurrentLevel);
		}

		auto operator=(RMSMeter<T, Enable>&& meter) noexcept -> RMSMeter<T, Enable>& = default;

	  private:
		Hertz mSampleRate = 44100_Hz;
		static const constexpr T mAveragingLengthSeconds = gsl::narrow_cast<T>(0.3);
		static const constexpr T mAttackSeconds = gsl::narrow_cast<T>(0.01);
		static const constexpr T mReleaseSeconds = gsl::narrow_cast<T>(1.0);
		T mAveragingCoeff = gsl::narrow_cast<T>(0.9999244171);
		T mAttackCoeff = gsl::narrow_cast<T>(0.9977349953);
		T mReleaseCoeff = gsl::narrow_cast<T>(0.9999773245);

		T mCurrentLevel = gsl::narrow_cast<T>(0.0);

		T mY2N1 = gsl::narrow_cast<T>(0.0);
		T mAverageN1 = gsl::narrow_cast<T>(0.0);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RMSMeter)
	};
} // namespace apex::dsp
