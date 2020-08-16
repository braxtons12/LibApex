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
	class PeakMeter final : public Meter<T, Enable> {
	  public:
		/// @brief Constructs a default `PeakMeter`
		PeakMeter() noexcept = default;

		/// @brief Constructs an `PeakMeter` with the given sample rate
		///
		/// @param sampleRate - The sample rate to use
		explicit PeakMeter(Hertz sampleRate) noexcept : mSampleRate(sampleRate) {
			mAttackCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate))));
			mReleaseCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mReleaseCoeff * static_cast<double>(mSampleRate))));
		}

		/// @brief Move contructs the given `PeakMeter`
		///
		/// @param meter - The `PeakMeter` to move
		PeakMeter(PeakMeter<T, Enable>&& meter) noexcept = default;
		~PeakMeter() noexcept final = default;

		/// @brief Sets the sample rate to the given value
		///
		/// @param SampleRate - The new sample rate
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void final {
			mSampleRate = sampleRate;
			mAttackCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate))));
			mReleaseCoeff = gsl::narrow_cast<T>(
				math::exp(-1.0 / (mReleaseCoeff * static_cast<double>(mSampleRate))));
		}

		/// @brief Resets the meter to an initial state
		inline auto reset() noexcept -> void final {
			mCurrentLevel = gsl::narrow_cast<T>(0.0);
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(T input) noexcept -> void final {
			T in = gsl::narrow_cast<T>(math::fabs(input));
			if(in > mCurrentLevel) {
				mCurrentLevel = mAttackCoeff * mCurrentLevel + (1.0 - mAttackCoeff) * in;
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

		auto operator=(PeakMeter<T, Enable>&& meter) noexcept -> PeakMeter<T, Enable>& = default;

	  private:
		Hertz mSampleRate = 44100_Hz;
		static const constexpr T mAttackSeconds = gsl::narrow_cast<T>(0.001);
		static const constexpr T mReleaseSeconds = gsl::narrow_cast<T>(0.5);
		T mAttackCoeff = gsl::narrow_cast<T>(0.9775794253);
		T mReleaseCoeff = gsl::narrow_cast<T>(0.9999546496);

		T mCurrentLevel = gsl::narrow_cast<T>(0.0);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakMeter)
	};
} // namespace apex::dsp
