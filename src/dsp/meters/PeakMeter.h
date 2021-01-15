#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Meter.h"

namespace apex::dsp {
	using math::Exponentials;
	using math::General;
	/// @brief Basic RMS Meter
	///
	/// @tparam FloatType - The floating point type to perform operations with
	/// @tparam Enable - disables the type of T is not a floating point type
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class PeakMeter final : public Meter<FloatType> {
	  public:
		/// @brief Constructs a default `PeakMeter`
		PeakMeter() noexcept = default;

		/// @brief Constructs an `PeakMeter` with the given sample rate
		///
		/// @param sampleRate - The sample rate to use
		explicit PeakMeter(Hertz sampleRate) noexcept : mSampleRate(sampleRate) {
			mAttackCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAttackSeconds * narrow_cast<FloatType>(mSampleRate)));
			mReleaseCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mReleaseCoeff * narrow_cast<FloatType>(mSampleRate)));
		}

		/// @brief Move contructs the given `PeakMeter`
		///
		/// @param meter - The `PeakMeter` to move
		PeakMeter(PeakMeter&& meter) noexcept = default;
		~PeakMeter() noexcept final = default;

		/// @brief Sets the sample rate to the given value
		///
		/// @param SampleRate - The new sample rate
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void final {
			mSampleRate = sampleRate;
			mAttackCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAttackSeconds * narrow_cast<FloatType>(mSampleRate)));
			mReleaseCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mReleaseCoeff * narrow_cast<FloatType>(mSampleRate)));
		}

		/// @brief Resets the meter to an initial state
		inline auto reset() noexcept -> void final {
			mCurrentLevel = narrow_cast<FloatType>(0.0);
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(FloatType input) noexcept -> void final {
			auto in = General<FloatType>::fabs(input);
			if(in > mCurrentLevel) {
				mCurrentLevel = mAttackCoeff * mCurrentLevel
								+ (narrow_cast<FloatType>(1.0) - mAttackCoeff) * in;
			}
			else {
				mCurrentLevel *= mReleaseCoeff;
			}
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(Span<FloatType> input) noexcept -> void final {
			for(auto& in : input) {
				update(in);
			}
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(Span<const FloatType> input) noexcept -> void final {
			for(auto& in : input) {
				update(in);
			}
		}

		/// @brief Returns the current linear level of the meter
		///
		/// @return - The linear level
		[[nodiscard]] inline auto getLevel() const noexcept -> FloatType final {
			return mCurrentLevel;
		}

		/// @brief Returns the current level of the meter, in Decibels
		///
		/// @return - The level in Decibels
		[[nodiscard]] inline auto getLevelDB() const noexcept -> Decibels final {
			return Decibels::fromLinear(mCurrentLevel);
		}

		auto operator=(PeakMeter&& meter) noexcept -> PeakMeter& = default;

	  private:
		Hertz mSampleRate = 44100_Hz;
		static const constexpr FloatType mAttackSeconds = narrow_cast<FloatType>(0.001);
		static const constexpr FloatType mReleaseSeconds = narrow_cast<FloatType>(0.3);
		FloatType mAttackCoeff = Exponentials<FloatType>::exp(
			narrow_cast<FloatType>(-1.0) / (mAttackSeconds * narrow_cast<FloatType>(mSampleRate)));
		FloatType mReleaseCoeff = Exponentials<FloatType>::exp(
			narrow_cast<FloatType>(-1.0) / (mReleaseSeconds * narrow_cast<FloatType>(mSampleRate)));
		FloatType mCurrentLevel = narrow_cast<FloatType>(0.0);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakMeter)
	};
} // namespace apex::dsp
