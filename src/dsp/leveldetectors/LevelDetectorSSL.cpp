#include "LevelDetectorSSL.h"

namespace apex {
	namespace dsp {

		/// @brief Creates a `LevelDetectorSSL` with the default parameters of:
		/// * attack: 3ms
		/// * release: 600ms
		/// * sample rate: 44100Hz
		LevelDetectorSSL<float>::LevelDetectorSSL() noexcept
			: LevelDetector<float>(INITIAL_ATTACK_MS, INITIAL_RELEASE_MS,
					INITIAL_SAMPLERATE, DetectorType::Branching)
			{

			}

		/// @brief Creates a `LevelDetectorSSL` with the given parameters
		///
		/// @param attck - The attack type to use
		/// @param release - The release type to use
		/// @param sampleRate - The sample rate, in Hertz
		LevelDetectorSSL<float>::LevelDetectorSSL(AttackType attack, ReleaseType release,
				size_t sampleRate) noexcept
		{
			mAttackType = attack;
			mReleaseType = release;
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Sets the attack type to the given value
		///
		/// @param attack - The new attack type
		void LevelDetectorSSL<float>::setAttackType(AttackType attack) noexcept {
			mAttackType = attack;
			calculateAttacks();
		}

		/// @brief Returns the current attack type
		///
		/// @return - The current attack type
		LevelDetectorSSL<float>::AttackType LevelDetectorSSL<float>::getAttackType() const noexcept {
			return mAttackType;
		}

		/// @brief Sets the release type to the given value
		///
		/// @param release - The new release type
		void LevelDetectorSSL<float>::setReleaseType(ReleaseType release) noexcept {
			mReleaseType = release;
			calculateReleases();
		}

		/// @brief Returns the current release type
		///
		/// @return - The current release type
		LevelDetectorSSL<float>::ReleaseType LevelDetectorSSL<float>::getReleaseType() const noexcept {
			return mReleaseType;
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetectorSSL<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		float LevelDetectorSSL<float>::process(float input) noexcept {
			if(mReleaseType != ReleaseType::Auto) return LevelDetector<float>::process(input);
			else {
				float y1n = mY1N1 + mAttackCoeff * math::max(input - mYOut1, 0.0f)
					- mAutoRelease1Coeff * mY1N1;
				float y2n = mY2N1 * mAutoReleaseAttack2Coeff * math::max(input - mYOut1, 0.0f)
					- mAutoRelease2Coeff * mY2N1;
				float yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				mYOut1 = yn;
				return yn;
			}
		}

		void LevelDetectorSSL<float>::updateCoefficients() noexcept {
			calculateAttacks();
			calculateReleases();
		}

		void LevelDetectorSSL<float>::calculateAttacks() noexcept {
			switch(mAttackType) {
				case AttackType::PointOneMilliseconds:
					mAttackSeconds = ATTACK_POINT_ONE_MS_S;
					break;
				case AttackType::PointThreeMilliseconds:
					mAttackSeconds = ATTACK_POINT_THREE_MS_S;
					break;
				case AttackType::OneMilliseconds:
					mAttackSeconds = ATTACK_ONE_MS_S;
					break;
				case AttackType::ThreeMilliseconds:
					mAttackSeconds = ATTACK_THREE_MS_S;
					break;
				case AttackType::TenMilliseconds:
					mAttackSeconds = ATTACK_TEN_MS_S;
					break;
				case AttackType::ThirtyMilliseconds:
					mAttackSeconds = ATTACK_THIRTY_MS_S;
					break;
			}
			mAttackCoeff = math::expf(-1.0f / (mAttackSeconds * static_cast<float>(mSampleRate)));
			if(mReleaseType == ReleaseType::Auto) {
				float attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				mAutoReleaseAttack2Coeff = math::expf(-1.0f /
						(attack2Seconds * static_cast<float>(mSampleRate)));
			}

		}

		void LevelDetectorSSL<float>::calculateReleases() noexcept {
			switch(mReleaseType) {
				case ReleaseType::PointOneSeconds:
					mReleaseSeconds = RELEASE_POINT_ONE_S;
					break;
				case ReleaseType::PointThreeSeconds:
					mReleaseSeconds = RELEASE_POINT_THREE_S;
					break;
				case ReleaseType::PointSixSeconds:
					mReleaseSeconds = RELEASE_POINT_SIX_S;
					break;
				case ReleaseType::OnePointTwoSeconds:
					mReleaseSeconds = RELEASE_ONE_POINT_TWO_S;
					break;
				case ReleaseType::Auto: break;
			}
			mReleaseCoeff = math::expf(-1.0f / (mReleaseSeconds * static_cast<float>(mSampleRate)));
			if(mReleaseType == ReleaseType::Auto) {
				float attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				mAutoReleaseAttack2Coeff = math::expf(-1.0f / (attack2Seconds * static_cast<float>(mSampleRate)));
				mAutoRelease1Coeff = math::expf(-1.0f / (AUTO_RELEASE1_S * static_cast<float>(mSampleRate)));
				mAutoRelease2Coeff = math::expf(-1.0f / (AUTO_RELEASE2_S * static_cast<float>(mSampleRate)));
			}
		}

		/// @brief Creates a `LevelDetectorSSL` with the default parameters of:
		/// * attack: 3ms
		/// * release: 600ms
		/// * sample rate: 44100Hz
		LevelDetectorSSL<double>::LevelDetectorSSL() noexcept
			: LevelDetector<double>(INITIAL_ATTACK_MS, INITIAL_RELEASE_MS,
					INITIAL_SAMPLERATE, DetectorType::Branching)
			{

			}

		/// @brief Creates a `LevelDetectorSSL` with the given parameters
		///
		/// @param attck - The attack type to use
		/// @param release - The release type to use
		/// @param sampleRate - The sample rate, in Hertz
		LevelDetectorSSL<double>::LevelDetectorSSL(AttackType attack, ReleaseType release,
				size_t sampleRate) noexcept
		{
			mAttackType = attack;
			mReleaseType = release;
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Sets the attack type to the given value
		///
		/// @param attack - The new attack type
		void LevelDetectorSSL<double>::setAttackType(AttackType attack) noexcept {
			mAttackType = attack;
			calculateAttacks();
		}

		/// @brief Returns the current attack type
		///
		/// @return - The current attack type
		LevelDetectorSSL<double>::AttackType LevelDetectorSSL<double>::getAttackType() const noexcept {
			return mAttackType;
		}

		/// @brief Sets the release type to the given value
		///
		/// @param release - The new release type
		void LevelDetectorSSL<double>::setReleaseType(ReleaseType release) noexcept {
			mReleaseType = release;
			calculateReleases();
		}

		/// @brief Returns the current release type
		///
		/// @return - The current release type
		LevelDetectorSSL<double>::ReleaseType LevelDetectorSSL<double>::getReleaseType() const noexcept {
			return mReleaseType;
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetectorSSL<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			updateCoefficients();
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		double LevelDetectorSSL<double>::process(double input) noexcept {
			if(mReleaseType != ReleaseType::Auto) return LevelDetector<double>::process(input);
			else {
				double y1n = mY1N1 + mAttackCoeff * math::max(input - mYOut1, 0.0)
					- mAutoRelease1Coeff * mY1N1;
				double y2n = mY2N1 * mAutoReleaseAttack2Coeff * math::max(input - mYOut1, 0.0)
					- mAutoRelease2Coeff * mY2N1;
				double yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				mYOut1 = yn;
				return yn;
			}
		}

		void LevelDetectorSSL<double>::updateCoefficients() noexcept {
			calculateAttacks();
			calculateReleases();
		}

		void LevelDetectorSSL<double>::calculateAttacks() noexcept {
			switch(mAttackType) {
				case AttackType::PointOneMilliseconds:
					mAttackSeconds = ATTACK_POINT_ONE_MS_S;
					break;
				case AttackType::PointThreeMilliseconds:
					mAttackSeconds = ATTACK_POINT_THREE_MS_S;
					break;
				case AttackType::OneMilliseconds:
					mAttackSeconds = ATTACK_ONE_MS_S;
					break;
				case AttackType::ThreeMilliseconds:
					mAttackSeconds = ATTACK_THREE_MS_S;
					break;
				case AttackType::TenMilliseconds:
					mAttackSeconds = ATTACK_TEN_MS_S;
					break;
				case AttackType::ThirtyMilliseconds:
					mAttackSeconds = ATTACK_THIRTY_MS_S;
					break;
			}
			mAttackCoeff = math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate)));
			if(mReleaseType == ReleaseType::Auto) {
				double attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				mAutoReleaseAttack2Coeff = math::exp(-1.0 /
						(attack2Seconds * static_cast<double>(mSampleRate)));
			}

		}

		void LevelDetectorSSL<double>::calculateReleases() noexcept {
			switch(mReleaseType) {
				case ReleaseType::PointOneSeconds:
					mReleaseSeconds = RELEASE_POINT_ONE_S;
					break;
				case ReleaseType::PointThreeSeconds:
					mReleaseSeconds = RELEASE_POINT_THREE_S;
					break;
				case ReleaseType::PointSixSeconds:
					mReleaseSeconds = RELEASE_POINT_SIX_S;
					break;
				case ReleaseType::OnePointTwoSeconds:
					mReleaseSeconds = RELEASE_ONE_POINT_TWO_S;
					break;
				case ReleaseType::Auto: break;
			}
			mReleaseCoeff = math::exp(-1.0 / (mReleaseSeconds * static_cast<double>(mSampleRate)));
			if(mReleaseType == ReleaseType::Auto) {
				double attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				mAutoReleaseAttack2Coeff = math::exp(-1.0 /
						(attack2Seconds * static_cast<double>(mSampleRate)));
				mAutoRelease1Coeff = math::exp(-1.0 /
						(AUTO_RELEASE1_S * static_cast<double>(mSampleRate)));
				mAutoRelease2Coeff = math::exp(-1.0 /
						(AUTO_RELEASE2_S * static_cast<double>(mSampleRate)));
			}
		}
	}
}
