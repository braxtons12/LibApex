/// @brief Contains base implementatin for an RMS Level Detector Component of a Dynamic Range
/// Processor's Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and
/// Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "LevelDetector.h"

namespace apex::dsp {
	/// @brief Base RMS Level Detector used for the level detection portion of a
	/// Dynamic Range Processor's Sidechain
	///
	/// @tparam FloatType - The floating point type to back operations, either float or double
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class LevelDetectorRMS : public LevelDetector<FloatType> {
	  private:
		using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;
		using LevelDetector = LevelDetector<FloatType>;

	  public:
		LevelDetectorRMS() noexcept = default;

		/// @brief Constructs a `LevelDetector` of the given type
		/// with the given shared state
		///
		/// @param state - The shared state
		/// @param type - The detector type
		explicit LevelDetectorRMS(DynamicsState* state,
								  DetectorType type = DetectorType::NonCorrected) noexcept
			: LevelDetector(state, type) {
			LevelDetector::mType = type;
			LevelDetector::mState->template registerCallback<FloatType, DynamicsField::Attack>(
				[this](FloatType attack) { this->setAttackTime(attack); });
			LevelDetector::mState->template registerCallback<FloatType, DynamicsField::Release>(
				[this](FloatType release) { this->setReleaseTime(release); });
			LevelDetector::mState->template registerCallback<Hertz, DynamicsField::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
		}

		/// @brief Move contructs an `RMSLevelDetector` from the given one
		///
		/// @param detector - The `RMSLevelDetector` to move
		LevelDetectorRMS(LevelDetectorRMS&& detector) noexcept = default;
		~LevelDetectorRMS() noexcept override = default;

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		[[nodiscard]] auto process(float input) noexcept -> float override {
			auto xn = LevelDetector::process(input);
			auto y2n
				= mRMSCoeff * mYSquared1 + (narrow_cast<FloatType>(1.0) - mRMSCoeff) * (xn * xn);
			auto yn = General<FloatType>::sqrt(y2n);
			mYSquared1 = y2n;
			return yn;
		}

		/// @brief Resets this level detector to an initial state
		auto reset() noexcept -> void override {
			LevelDetector::reset();
			mYSquared1 = narrow_cast<FloatType>(0.0);
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseSeconds - The new release time, in seconds
		auto setReleaseTime(float releaseSeconds) noexcept -> void override {
			LevelDetector::setReleaseTime(releaseSeconds);
			mRMSSeconds = releaseSeconds * 2.0F;
			mRMSCoeff = calculateRMSCoefficient(LevelDetector::mState->getSampleRate());
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		auto setSampleRate(Hertz sampleRate) noexcept -> void override {
			LevelDetector::setSampleRate(sampleRate);
			mRMSCoeff = calculateRMSCoefficient(sampleRate);
		}

		auto operator=(LevelDetectorRMS&& detector) noexcept -> LevelDetectorRMS& = default;

	  protected:
		// y[n-1] squared
		FloatType mYSquared1 = narrow_cast<FloatType>(0.0);
		FloatType mRMSSeconds = narrow_cast<FloatType>(0.1);
		FloatType mRMSCoeff = calculateRMSCoefficient(LevelDetector::mState->getSampleRate());

		[[nodiscard]] virtual inline auto
		calculateRMSCoefficient(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0) / (mRMSSeconds * narrow_cast<FloatType>(sampleRate)));
		}

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
	};
} // namespace apex::dsp
