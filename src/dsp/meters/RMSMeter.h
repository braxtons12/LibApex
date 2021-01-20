#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Meter.h"

namespace apex::dsp {
	/// @brief Basic RMS Meter
	///
	/// @tparam FloatType - The floating point type to perform operations with
	/// @tparam Enable - disables the type of T is not a floating point type
	template<typename FloatType, std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class RMSMeter final : public Meter<FloatType> {
	  public:
		/// @brief Constructs a default `RMSMeter`
		RMSMeter() noexcept = default;

		/// @brief Constructs an `RMSMeter` with the given sample rate
		///
		/// @param sampleRate - The sample rate to use
		explicit RMSMeter(Hertz sampleRate) noexcept : mSampleRate(sampleRate) {
			mAveragingCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAveragingLengthSeconds * narrow_cast<FloatType>(mSampleRate)));
			mAttackCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAttackSeconds * narrow_cast<FloatType>(mSampleRate)));
			mReleaseCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mReleaseSeconds * narrow_cast<FloatType>(mSampleRate)));
		}

		/// @brief Move contructs the given `RMSMeter`
		///
		/// @param meter - The `RMSMeter` to move
		RMSMeter(RMSMeter&& meter) noexcept = default;
		~RMSMeter() noexcept final = default;

		/// @brief Sets the sample rate to the given value
		///
		/// @param SampleRate - The new sample rate
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void final {
			mSampleRate = sampleRate;
			mAveragingCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAveragingLengthSeconds * narrow_cast<FloatType>(mSampleRate)));
			mAttackCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAttackSeconds * narrow_cast<FloatType>(mSampleRate)));
			mReleaseCoeff = Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mReleaseSeconds * narrow_cast<FloatType>(mSampleRate)));
		}

		/// @brief Resets the meter to an initial state
		inline auto reset() noexcept -> void final {
			mCurrentLevel = narrow_cast<FloatType>(0.0);
			mY2N1 = narrow_cast<FloatType>(0.0);
			mAverageN1 = narrow_cast<FloatType>(0.0);
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(FloatType input) noexcept -> void final {
			FloatType y2nAverage
				= mAveragingCoeff * mAverageN1
				  + (narrow_cast<FloatType>(1.0) - mAveragingCoeff) * (input * input);
			mAverageN1 = y2nAverage;
			FloatType yn = General<FloatType>::sqrt(y2nAverage);
			if(yn > mCurrentLevel) {
				mCurrentLevel = mAttackCoeff * mCurrentLevel
								+ (narrow_cast<FloatType>(1.0) - mAttackCoeff) * yn;
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

		/// @brief Updates the meter with the given input
		///
		/// @param inputLeft - The left channel input to meter
		/// @param inputRight - The right channel input to meter
		inline auto update(FloatType inputLeft, FloatType inputRight) noexcept -> void final {
			update(narrow_cast<FloatType>(0.5) * (inputLeft + inputRight));
		}

		/// @brief Updates the meter with the given input
		///
		/// @param inputLeft - The left channel input to meter
		/// @param inputRight - The right channel input to meter
		inline auto
		update(Span<FloatType> inputLeft, Span<FloatType> inputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				update(inputLeft.at(i), inputRight.at(i));
			}
		}

		/// @brief Updates the meter with the given input
		///
		/// @param inputLeft - The left channel input to meter
		/// @param inputRight - The right channel input to meter
		inline auto update(Span<const FloatType> inputLeft,
						   Span<const FloatType> inputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				update(inputLeft.at(i), inputRight.at(i));
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

		auto operator=(RMSMeter&& meter) noexcept -> RMSMeter& = default;

	  private:
		Hertz mSampleRate = 44100_Hz;
		static const constexpr FloatType mAveragingLengthSeconds = narrow_cast<FloatType>(0.3);
		static const constexpr FloatType mAttackSeconds = narrow_cast<FloatType>(0.01);
		static const constexpr FloatType mReleaseSeconds = narrow_cast<FloatType>(0.3);
		FloatType mAveragingCoeff = Exponentials<FloatType>::exp(
			narrow_cast<FloatType>(-1.0)
			/ (mAveragingLengthSeconds * narrow_cast<FloatType>(mSampleRate)));
		FloatType mAttackCoeff = Exponentials<FloatType>::exp(
			narrow_cast<FloatType>(-1.0) / (mAttackSeconds * narrow_cast<FloatType>(mSampleRate)));
		FloatType mReleaseCoeff = Exponentials<FloatType>::exp(
			narrow_cast<FloatType>(-1.0) / (mReleaseSeconds * narrow_cast<FloatType>(mSampleRate)));

		FloatType mCurrentLevel = narrow_cast<FloatType>(0.0);

		FloatType mY2N1 = narrow_cast<FloatType>(0.0);
		FloatType mAverageN1 = narrow_cast<FloatType>(0.0);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RMSMeter)
	};
} // namespace apex::dsp
