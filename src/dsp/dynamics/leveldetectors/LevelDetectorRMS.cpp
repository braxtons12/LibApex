#include "LevelDetectorRMS.h"

namespace apex::dsp {
	/// @brief Constructs a `LevelDetector` of the given type
	/// with the given shared state
	///
	/// @param state - The shared state
	/// @param type - The detector type
	LevelDetectorRMS<float>::LevelDetectorRMS(DynamicsState* state, DetectorType type) noexcept
		: LevelDetector<float>(state, type),
		mState(state)
		{
			mType = type;
			mState->registerCallback<float, DynamicsState::Field::Attack>(
					[this](float attack) {
					this->setAttackTime(attack);
					});
			mState->registerCallback<float, DynamicsState::Field::Release>(
					[this](float release) {
					this->setReleaseTime(release);
					});
			mState->registerCallback<size_t, DynamicsState::Field::SampleRate>(
					[this](size_t sampleRate) {
					this->setSampleRate(sampleRate);
					});
		}

	/// @brief Generates the detected level from the given input
	///
	/// @param input - The input to detect on
	///
	/// @return - The detected level
	auto LevelDetectorRMS<float>::process(float input) noexcept -> float {
		float xn = LevelDetector<float>::process(input);
		float y2n = mRMSCoeff * mYSquared1 + (1.0F - mRMSCoeff) * (xn * xn);
		float yn = math::sqrtf(y2n);
		mYSquared1 = y2n;
		return yn;
	}

	/// @brief Resets this level detector to an initial state
	auto LevelDetectorRMS<float>::reset() noexcept -> void {
		LevelDetector<float>::reset();
		mYSquared1 = 0.0F;
	}

	/// @brief Sets the attack time to the given value
	///
	/// @param attackSeconds - The new attack time, in seconds
	auto LevelDetectorRMS<float>::setAttackTime(float attackSeconds) noexcept -> void {
		LevelDetector<float>::setAttackTime(attackSeconds);
	}

	/// @brief Sets the release time to the given value
	///
	/// @param releaseSeconds - The new release time, in seconds
	auto LevelDetectorRMS<float>::setReleaseTime(float releaseSeconds) noexcept -> void {
		LevelDetector<float>::setReleaseTime(releaseSeconds);
		mRMSSeconds = releaseSeconds * 2.0F;
		mRMSCoeff = math::expf(-1.0F / (mRMSSeconds *
					static_cast<float>(mState->getSampleRate())));
	}

	/// @brief Sets the sample rate to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	auto LevelDetectorRMS<float>::setSampleRate(size_t sampleRate) noexcept -> void {
		LevelDetector<float>::setSampleRate(sampleRate);
		mRMSCoeff = math::expf(-1.0F / (mRMSSeconds * static_cast<float>(sampleRate)));
	}

	/// @brief Constructs a `LevelDetector` of the given type
	/// with the given shared state
	///
	/// @param state - The shared state
	/// @param type - The detector type
	LevelDetectorRMS<double>::LevelDetectorRMS(DynamicsState* state, DetectorType type) noexcept
		: LevelDetector<double>(state, type),
		mState(state)
		{
			mType = type;
			mState->registerCallback<double, DynamicsState::Field::Attack>(
					[this](double attack) {
					this->setAttackTime(attack);
					});
			mState->registerCallback<double, DynamicsState::Field::Release>(
					[this](double release) {
					this->setReleaseTime(release);
					});
			mState->registerCallback<size_t, DynamicsState::Field::SampleRate>(
					[this](size_t sampleRate) {
					this->setSampleRate(sampleRate);
					});
		}

	/// @brief Generates the detected level from the given input
	///
	/// @param input - The input to detect on
	///
	/// @return - The detected level
	auto LevelDetectorRMS<double>::process(double input) noexcept -> double {
		double xn = LevelDetector<double>::process(input);
		double y2n = mRMSCoeff * mYSquared1 + (1.0 - mRMSCoeff) * (xn * xn);
		double yn = math::sqrt(y2n);
		mYSquared1 = y2n;
		return yn;
	}

	/// @brief Resets this level detector to an initial state
	auto LevelDetectorRMS<double>::reset() noexcept -> void {
		LevelDetector<double>::reset();
		mYSquared1 = 0.0;
	}

	/// @brief Sets the attack time to the given value
	///
	/// @param attackSeconds - The new attack time, in seconds
	auto LevelDetectorRMS<double>::setAttackTime(double attackSeconds) noexcept -> void {
		LevelDetector<double>::setAttackTime(attackSeconds);
	}

	/// @brief Sets the release time to the given value
	///
	/// @param releaseSeconds - The new release time, in seconds
	auto LevelDetectorRMS<double>::setReleaseTime(double releaseSeconds) noexcept -> void {
		LevelDetector<double>::setReleaseTime(releaseSeconds);
		mRMSSeconds = releaseSeconds * 2.0;
		mRMSCoeff = math::exp(-1.0 / (mRMSSeconds *
					static_cast<double>(mState->getSampleRate())));
	}

	/// @brief Sets the sample rate to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	auto LevelDetectorRMS<double>::setSampleRate(size_t sampleRate) noexcept -> void {
		LevelDetector<double>::setSampleRate(sampleRate);
		mRMSCoeff = math::exp(-1.0 / (mRMSSeconds * static_cast<double>(sampleRate)));
	}
} //namespace apex::dsp
