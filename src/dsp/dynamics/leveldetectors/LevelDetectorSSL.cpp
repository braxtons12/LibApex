#include "LevelDetectorSSL.h"

namespace apex::dsp {
	/// @brief Constructs a `LevelDetectorSSL` with the given shared state
	///
	/// @param state - The shared state
	LevelDetectorSSL<float>::LevelDetectorSSL(DynamicsState* state) noexcept : mState(state) {
		mState->setHasAutoRelease(true);
		mState->registerCallback<SSLBusAttackTime, DynamicsState::Field::Attack>(
			[this](SSLBusAttackTime attack) { this->setAttackTime(attack); });
		mState->registerCallback<SSLBusReleaseTime, DynamicsState::Field::Release>(
			[this](SSLBusReleaseTime release) { this->setReleaseTime(release); });
		mState->registerCallback<Hertz, DynamicsState::Field::SampleRate>(
			[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
		mState->registerCallback<bool, DynamicsState::Field::AutoRelease>([this](bool enabled) {
			if(enabled) {
				this->setReleaseTime(SSLBusReleaseTime::Auto);
			}
			else {
				this->setReleaseTime(this->mState->getRelease());
			}
		});
	}

	/// @brief Sets the attack time to the given value
	///
	/// @param attack - The new attack time
	auto LevelDetectorSSL<float>::setAttackTime(SSLBusAttackTime attack) noexcept -> void {
		calculateAttackCoefficients(attack, mState->getSampleRate());
	}

	/// @brief Sets the release time to the given value
	///
	/// @param release - The new release time
	auto LevelDetectorSSL<float>::setReleaseTime(SSLBusReleaseTime release) noexcept -> void {
		calculateReleaseCoefficients(release, mState->getSampleRate());
		if(release == SSLBusReleaseTime::Auto) {
			calculateAttackCoefficients(mState->getAttack(), mState->getSampleRate());
		}
	}

	/// @brief Sets the sample rate to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	auto LevelDetectorSSL<float>::setSampleRate(Hertz sampleRate) noexcept -> void {
		calculateAttackCoefficients(mState->getAttack(), sampleRate);
		calculateReleaseCoefficients(mState->getRelease(), sampleRate);
	}

	/// @brief Generates the detected level from the given input
	///
	/// @param input - The input to detect on
	///
	/// @return - The detected level
	auto LevelDetectorSSL<float>::process(float input) noexcept -> float {
		if(mState->getRelease() != SSLBusReleaseTime::Auto) {
			return LevelDetector<float>::process(input);
		}
		else {
			float y1n = mY1N1
						+ mState->getAttackCoefficient1() * General<>::max(input - mYOut1, 0.0F)
						- mState->getReleaseCoefficient1() * mY1N1;
			float y2n
				= mY2N1 * mState->getAttackCoefficient2() * General<>::max(input - mYOut1, 0.0F)
				  - mState->getReleaseCoefficient2() * mY2N1;
			float yn = y1n + y2n;
			mY1N1 = y1n;
			mY2N1 = y2n;
			mYOut1 = yn;
			return yn;
		}
	}

	auto LevelDetectorSSL<float>::calculateAttackCoefficients(SSLBusAttackTime attack,
															  Hertz sampleRate) noexcept -> void {
		float attackSeconds = 0.0F;
		switch(attack) {
			case SSLBusAttackTime::PointOneMilliseconds:
				attackSeconds = ATTACK_POINT_ONE_MS_S;
				break;
			case SSLBusAttackTime::PointThreeMilliseconds:
				attackSeconds = ATTACK_POINT_THREE_MS_S;
				break;
			case SSLBusAttackTime::OneMilliseconds: attackSeconds = ATTACK_ONE_MS_S; break;
			case SSLBusAttackTime::ThreeMilliseconds: attackSeconds = ATTACK_THREE_MS_S; break;
			case SSLBusAttackTime::TenMilliseconds: attackSeconds = ATTACK_TEN_MS_S; break;
			case SSLBusAttackTime::ThirtyMilliseconds: attackSeconds = ATTACK_THIRTY_MS_S; break;
		}
		mState->setAttackCoefficient1(
			Exponentials<>::exp(-1.0F / (attackSeconds * static_cast<float>(sampleRate))));
		if(mState->getRelease() == SSLBusReleaseTime::Auto) {
			float attack2Seconds = attackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
			mState->setAttackCoefficient2(
				Exponentials<>::exp(-1.0F / (attack2Seconds * static_cast<float>(sampleRate))));
		}
	}

	auto LevelDetectorSSL<float>::calculateReleaseCoefficients(SSLBusReleaseTime release,
															   Hertz sampleRate) noexcept -> void {
		if(release == SSLBusReleaseTime::Auto) {
			float releaseSeconds = 0.0F;
			switch(release) {
				case SSLBusReleaseTime::PointOneSeconds:
					releaseSeconds = RELEASE_POINT_ONE_S;
					break;
				case SSLBusReleaseTime::PointThreeSeconds:
					releaseSeconds = RELEASE_POINT_THREE_S;
					break;
				case SSLBusReleaseTime::PointSixSeconds:
					releaseSeconds = RELEASE_POINT_SIX_S;
					break;
				case SSLBusReleaseTime::OnePointTwoSeconds:
					releaseSeconds = RELEASE_ONE_POINT_TWO_S;
					break;
				case SSLBusReleaseTime::Auto: break;
			}
			calculateAttackCoefficients(mState->getAttack(), sampleRate);
			mState->setReleaseCoefficient1(
				Exponentials<>::exp(-1.0F / (releaseSeconds * static_cast<float>(sampleRate))));
		}
		else {
			mState->setReleaseCoefficient1(
				Exponentials<>::exp(-1.0F / (AUTO_RELEASE1_S * static_cast<float>(sampleRate))));
			mState->setReleaseCoefficient2(
				Exponentials<>::exp(-1.0F / (AUTO_RELEASE2_S * static_cast<float>(sampleRate))));
		}
	}

	/// @brief Constructs a `LevelDetectorSSL` with the given shared state
	///
	/// @param state - The shared state
	LevelDetectorSSL<double>::LevelDetectorSSL(DynamicsState* state) noexcept : mState(state) {
		mState->setHasAutoRelease(true);
		mState->registerCallback<SSLBusAttackTime, DynamicsState::Field::Attack>(
			[this](SSLBusAttackTime attack) { this->setAttackTime(attack); });
		mState->registerCallback<SSLBusReleaseTime, DynamicsState::Field::Release>(
			[this](SSLBusReleaseTime release) { this->setReleaseTime(release); });
		mState->registerCallback<Hertz, DynamicsState::Field::SampleRate>(
			[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
		mState->registerCallback<bool, DynamicsState::Field::AutoRelease>([this](bool enabled) {
			if(enabled) {
				this->setReleaseTime(SSLBusReleaseTime::Auto);
			}
			else {
				this->setReleaseTime(this->mState->getRelease());
			}
		});
	}

	/// @brief Sets the attack time to the given value
	///
	/// @param attack - The new attack time
	auto LevelDetectorSSL<double>::setAttackTime(SSLBusAttackTime attack) noexcept -> void {
		calculateAttackCoefficients(attack, mState->getSampleRate());
	}

	/// @brief Sets the release time to the given value
	///
	/// @param release - The new release time
	auto LevelDetectorSSL<double>::setReleaseTime(SSLBusReleaseTime release) noexcept -> void {
		calculateReleaseCoefficients(release, mState->getSampleRate());
		if(release == SSLBusReleaseTime::Auto) {
			calculateAttackCoefficients(mState->getAttack(), mState->getSampleRate());
		}
	}

	/// @brief Sets the sample rate to the given value
	///
	/// @param sampleRate - The new sample rate, in Hertz
	auto LevelDetectorSSL<double>::setSampleRate(Hertz sampleRate) noexcept -> void {
		calculateAttackCoefficients(mState->getAttack(), sampleRate);
		calculateReleaseCoefficients(mState->getRelease(), sampleRate);
	}

	/// @brief Generates the detected level from the given input
	///
	/// @param input - The input to detect on
	///
	/// @return - The detected level
	auto LevelDetectorSSL<double>::process(double input) noexcept -> double {
		if(mState->getRelease() != SSLBusReleaseTime::Auto) {
			return LevelDetector<double>::process(input);
		}
		else {
			double y1n
				= mY1N1
				  + mState->getAttackCoefficient1() * General<double>::max(input - mYOut1, 0.0)
				  - mState->getReleaseCoefficient1() * mY1N1;
			double y2n = mY2N1 * mState->getAttackCoefficient2()
							 * General<double>::max(input - mYOut1, 0.0)
						 - mState->getReleaseCoefficient2() * mY2N1;
			double yn = y1n + y2n;
			mY1N1 = y1n;
			mY2N1 = y2n;
			mYOut1 = yn;
			return yn;
		}
	}

	auto LevelDetectorSSL<double>::calculateAttackCoefficients(SSLBusAttackTime attack,
															   Hertz sampleRate) noexcept -> void {
		double attackSeconds = 0.0;
		switch(attack) {
			case SSLBusAttackTime::PointOneMilliseconds:
				attackSeconds = ATTACK_POINT_ONE_MS_S;
				break;
			case SSLBusAttackTime::PointThreeMilliseconds:
				attackSeconds = ATTACK_POINT_THREE_MS_S;
				break;
			case SSLBusAttackTime::OneMilliseconds: attackSeconds = ATTACK_ONE_MS_S; break;
			case SSLBusAttackTime::ThreeMilliseconds: attackSeconds = ATTACK_THREE_MS_S; break;
			case SSLBusAttackTime::TenMilliseconds: attackSeconds = ATTACK_TEN_MS_S; break;
			case SSLBusAttackTime::ThirtyMilliseconds: attackSeconds = ATTACK_THIRTY_MS_S; break;
		}
		mState->setAttackCoefficient1(
			Exponentials<double>::exp(-1.0 / (attackSeconds * static_cast<double>(sampleRate))));
		if(mState->getRelease() == SSLBusReleaseTime::Auto) {
			double attack2Seconds = attackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
			mState->setAttackCoefficient2(Exponentials<double>::exp(
				-1.0 / (attack2Seconds * static_cast<double>(sampleRate))));
		}
	}

	auto LevelDetectorSSL<double>::calculateReleaseCoefficients(SSLBusReleaseTime release,
																Hertz sampleRate) noexcept -> void {
		if(release == SSLBusReleaseTime::Auto) {
			double releaseSeconds = 0.0;
			switch(release) {
				case SSLBusReleaseTime::PointOneSeconds:
					releaseSeconds = RELEASE_POINT_ONE_S;
					break;
				case SSLBusReleaseTime::PointThreeSeconds:
					releaseSeconds = RELEASE_POINT_THREE_S;
					break;
				case SSLBusReleaseTime::PointSixSeconds:
					releaseSeconds = RELEASE_POINT_SIX_S;
					break;
				case SSLBusReleaseTime::OnePointTwoSeconds:
					releaseSeconds = RELEASE_ONE_POINT_TWO_S;
					break;
				case SSLBusReleaseTime::Auto: break;
			}
			calculateAttackCoefficients(mState->getAttack(), sampleRate);
			mState->setReleaseCoefficient1(Exponentials<double>::exp(
				-1.0 / (releaseSeconds * static_cast<double>(sampleRate))));
		}
		else {
			mState->setReleaseCoefficient1(Exponentials<double>::exp(
				-1.0 / (AUTO_RELEASE1_S * static_cast<double>(sampleRate))));
			mState->setReleaseCoefficient2(Exponentials<double>::exp(
				-1.0 / (AUTO_RELEASE2_S * static_cast<double>(sampleRate))));
		}
	}
} // namespace apex::dsp
