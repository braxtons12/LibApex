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
		explicit PeakMeter(Hertz sampleRate) noexcept
			: mSampleRate(sampleRate), mAttackCoeff(calculateAttackCoeff(sampleRate)),
			  mReleaseCoeff(calculateReleaseCoeff(sampleRate)) {
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
			mAttackCoeff = calculateAttackCoeff(mSampleRate);
			mReleaseCoeff = calculateReleaseCoeff(mSampleRate);
		}

		/// @brief Resets the meter to an initial state
		inline auto reset() noexcept -> void final {
			mCurrentLevel = narrow_cast<FloatType>(0.0);
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(FloatType input) noexcept -> void final {
			auto in = General<FloatType>::abs(input);
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
			for(auto&& in : input) {
				update(in);
			}
		}

		/// @brief Updates the meter with the given input
		///
		/// @param input - The input to meter
		inline auto update(Span<const FloatType> input) noexcept -> void final {
			for(auto&& in : input) {
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

		auto operator=(PeakMeter&& meter) noexcept -> PeakMeter& = default;

	  private:
		Hertz mSampleRate = 44100_Hz;
		static const constexpr FloatType mAttackSeconds = narrow_cast<FloatType>(0.001);
		static const constexpr FloatType mReleaseSeconds = narrow_cast<FloatType>(0.3);
		FloatType mAttackCoeff = calculateAttackCoeff(mSampleRate);
		FloatType mReleaseCoeff = calculateReleaseCoeff(mSampleRate);
		FloatType mCurrentLevel = narrow_cast<FloatType>(0.0);

		[[nodiscard]] inline auto calculateAttackCoeff(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mAttackSeconds * narrow_cast<FloatType>(sampleRate)));
		}

		[[nodiscard]] inline auto calculateReleaseCoeff(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mReleaseSeconds * narrow_cast<FloatType>(sampleRate)));
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakMeter)
	};
} // namespace apex::dsp
