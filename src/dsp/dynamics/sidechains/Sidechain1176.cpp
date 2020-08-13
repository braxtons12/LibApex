#include "Sidechain1176.h"

namespace apex::dsp {
	/// @brief Constructs a `Sidechain1176` with the following defaults:
	/// * attack: 390uS
	/// * release: 525mS
	/// * ratio: 4
	/// * sampleRate: 44100Hz
	///
	/// Threshold and knee width are ratio-dependent semi-fixed values
	Sidechain1176<float>::Sidechain1176() noexcept {
		mState.setAttack(DEFAULT_ATTACK_SECONDS);
		mState.setRelease(DEFAULT_RELEASE_SECONDS);
		mState.setRatio(DEFAULT_RATIO);
		mState.setThreshold(THRESHOLD_RATIO_4_TO_1);
		mState.setKneeWidth(KNEE_RATIO_4_TO_1);
	}

	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto Sidechain1176<float>::process(float input) noexcept -> Decibels {
		return processFeedForwardReturnToZero(input);
	}

	/// @brief Sets the attack to the given value
	/// Valid values are in [20uS (20 microseconds), 800uS (800 microseconds)]
	///
	/// @param attackMS - The attack time in milliseconds
	auto Sidechain1176<float>::setAttackTime(float attackMS) noexcept -> void {
		float attackSeconds = attackMS * MS_TO_SECS_MULT;
		jassert(attackSeconds > MIN_ATTACK_SECONDS && attackSeconds < MAX_ATTACK_SECONDS);

		mState.setAttack(attackSeconds);
	}

	/// @brief Returns the attack
	///
	/// @return - The attack time in milliseconds
	auto Sidechain1176<float>::getAttackTime() const noexcept -> float {
		return mState.getAttack() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the release to the given value
	/// Valid values are in [50mS (50 milliseconds), 1.1s (1.1 seconds)]
	///
	/// @param releaseMS - The release time in milliseconds
	auto Sidechain1176<float>::setReleaseTime(float releaseMS) noexcept -> void {
		float releaseSeconds = releaseMS * MS_TO_SECS_MULT;
		jassert(releaseSeconds > MIN_RELEASE_SECONDS && releaseSeconds < MAX_RELEASE_SECONDS);

		mState.setRelease(releaseSeconds);
	}

	/// @brief Returns the release
	///
	/// @return - The release time in milliseconds
	auto Sidechain1176<float>::getReleaseTime() const noexcept -> float {
		return mState.getRelease() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the ratio to the given value
	///
	/// @param ratio - The ratio
	auto Sidechain1176<float>::setRatio(Ratio ratio) noexcept -> void {
		mRatio = ratio;
		switch(mRatio) {
			case Ratio::FourToOne:
				{
					mState.setRatio(RATIO_4_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_4_TO_1);
					mState.setKneeWidth(KNEE_RATIO_4_TO_1);
				}
				break;
			case Ratio::EightToOne:
				{
					mState.setRatio(RATIO_8_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_8_TO_1);
					mState.setKneeWidth(KNEE_RATIO_8_TO_1);
				}
				break;
			case Ratio::TwelveToOne:
				{
					mState.setRatio(RATIO_12_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_12_TO_1);
					mState.setKneeWidth(KNEE_RATIO_12_TO_1);
				}
				break;
			case Ratio::TwentyToOne:
				{
					mState.setRatio(RATIO_20_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_20_TO_1);
					mState.setKneeWidth(KNEE_RATIO_20_TO_1);
				}
				break;
			case Ratio::AllButtonsIn:
				{
					mState.setRatio(RATIO_ALL_BUTTONS);
					mState.setThreshold(THRESHOLD_RATIO_ALL_BUTTONS);
					mState.setKneeWidth(KNEE_RATIO_ALL_BUTTONS);
				}
				break;
		}
	}

	/// @brief Sets the DynamicsType
	///
	/// @param type - The dynamics type
	auto Sidechain1176<float>::setDynamicsType(DynamicsType type) noexcept -> void {
		mDynamicsType = type;
		if(mDynamicsType == DynamicsType::Compressor) {
			mGainComputer = &mCompressorComputer;
		}
		else {
			mGainComputer = &mExpanderComputer;
		}
	}

	auto Sidechain1176<float>::processFeedForwardReturnToZero(float input) noexcept -> Decibels {
		float rectified = math::fabsf(input);
		Decibels detectedDB = Decibels::fromLinear(mLevelDetector.process(rectified));
		Decibels outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB = outputDB - detectedDB;
		return mGainReductionDB;
	}

	/// @brief Constructs a `Sidechain1176` with the following defaults:
	/// * attack: 390uS
	/// * release: 525mS
	/// * ratio: 4
	/// * sampleRate: 44100Hz
	///
	/// Threshold and knee width are ratio-dependent semi-fixed values
	Sidechain1176<double>::Sidechain1176() noexcept {
		mState.setAttack(DEFAULT_ATTACK_SECONDS);
		mState.setRelease(DEFAULT_RELEASE_SECONDS);
		mState.setRatio(DEFAULT_RATIO);
		mState.setThreshold(THRESHOLD_RATIO_4_TO_1);
		mState.setKneeWidth(KNEE_RATIO_4_TO_1);
	}

	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto Sidechain1176<double>::process(double input) noexcept -> Decibels {
		return processFeedForwardReturnToZero(input);
	}

	/// @brief Sets the attack to the given value
	/// Valid values are in [20uS (20 microseconds), 800uS (800 microseconds)]
	///
	/// @param attackMS - The attack time in milliseconds
	auto Sidechain1176<double>::setAttackTime(double attackMS) noexcept -> void {
		double attackSeconds = attackMS * MS_TO_SECS_MULT;
		jassert(attackSeconds > MIN_ATTACK_SECONDS && attackSeconds < MAX_ATTACK_SECONDS);

		mState.setAttack(attackSeconds);
	}

	/// @brief Returns the attack
	///
	/// @return - The attack time in milliseconds
	auto Sidechain1176<double>::getAttackTime() const noexcept -> double {
		return mState.getAttack() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the release to the given value
	/// Valid values are in [50mS (50 milliseconds), 1.1s (1.1 seconds)]
	///
	/// @param releaseMS - The release time in milliseconds
	auto Sidechain1176<double>::setReleaseTime(double releaseMS) noexcept -> void {
		double releaseSeconds = releaseMS * MS_TO_SECS_MULT;
		jassert(releaseSeconds > MIN_RELEASE_SECONDS && releaseSeconds < MAX_RELEASE_SECONDS);

		mState.setRelease(releaseSeconds);
	}

	/// @brief Returns the release
	///
	/// @return - The release time in milliseconds
	auto Sidechain1176<double>::getReleaseTime() const noexcept -> double {
		return mState.getRelease() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the ratio to the given value
	///
	/// @param ratio - The ratio
	auto Sidechain1176<double>::setRatio(Ratio ratio) noexcept -> void {
		mRatio = ratio;
		switch(mRatio) {
			case Ratio::FourToOne:
				{
					mState.setRatio(RATIO_4_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_4_TO_1);
					mState.setKneeWidth(KNEE_RATIO_4_TO_1);
				}
				break;
			case Ratio::EightToOne:
				{
					mState.setRatio(RATIO_8_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_8_TO_1);
					mState.setKneeWidth(KNEE_RATIO_8_TO_1);
				}
				break;
			case Ratio::TwelveToOne:
				{
					mState.setRatio(RATIO_12_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_12_TO_1);
					mState.setKneeWidth(KNEE_RATIO_12_TO_1);
				}
				break;
			case Ratio::TwentyToOne:
				{
					mState.setRatio(RATIO_20_TO_1);
					mState.setThreshold(THRESHOLD_RATIO_20_TO_1);
					mState.setKneeWidth(KNEE_RATIO_20_TO_1);
				}
				break;
			case Ratio::AllButtonsIn:
				{
					mState.setRatio(RATIO_ALL_BUTTONS);
					mState.setThreshold(THRESHOLD_RATIO_ALL_BUTTONS);
					mState.setKneeWidth(KNEE_RATIO_ALL_BUTTONS);
				}
				break;
		}
	}

	/// @brief Sets the DynamicsType
	///
	/// @param type - The dynamics type
	auto Sidechain1176<double>::setDynamicsType(DynamicsType type) noexcept -> void {
		mDynamicsType = type;
		if(mDynamicsType == DynamicsType::Compressor) {
			mGainComputer = &mCompressorComputer;
		}
		else {
			mGainComputer = &mExpanderComputer;
		}
	}

	auto Sidechain1176<double>::processFeedForwardReturnToZero(double input) noexcept -> Decibels {
		double rectified = math::fabs(input);
		Decibels detectedDB = Decibels::fromLinear(mLevelDetector.process(rectified));
		Decibels outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB = outputDB - detectedDB;
		return mGainReductionDB;
	}
} // namespace apex::dsp
