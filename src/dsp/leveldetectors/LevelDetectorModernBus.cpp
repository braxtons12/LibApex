#include "LevelDetectorModernBus.h"

namespace apex {
	namespace dsp {
		/// @brief Constructs a `LevelDetectorModernBus` with the following defaults:
		/// * attack: 30mS
		/// * release: 75mS
		/// * sampleRate: 44100Hz
		/// * auto release: disabled
		LevelDetectorModernBus<float>::LevelDetectorModernBus() noexcept
			: LevelDetector<float>(INITIAL_ATTACK_MS, INITIAL_RELEASE_MS,
					INITIAL_SAMPLERATE, DetectorType::DecoupledSmooth)
			{

			}

		/// @brief Constructs a `LevelDetectorModernBus` with the given parameters
		///
		/// @param attackMS - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		/// @param autoRelease - whether auto release is enabled
		LevelDetectorModernBus<float>::LevelDetectorModernBus(float attackMS, float releaseMS,
				size_t sampleRate, bool autoRelease) noexcept
			: LevelDetector<float>(attackMS, releaseMS, sampleRate, DetectorType::DecoupledSmooth),
			mAutoRelease(autoRelease)
			{
				float attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				mAutoReleaseAttack2Coeff = math::expf(-1.0f / (attack2Seconds * static_cast<float>(mSampleRate)));
				mAutoRelease1Coeff = math::expf(-1.0f / (AUTO_RELEASE1_S * static_cast<float>(mSampleRate)));
				mAutoRelease2Coeff = math::expf(-1.0f / (AUTO_RELEASE2_S * static_cast<float>(mSampleRate)));
			}

		/// @brief Sets whether auto release is enabled
		///
		/// @param autoRelease - Whether auto release should be enabled
		void LevelDetectorModernBus<float>::setAutoRelease(bool autoRelease) noexcept {
			mAutoRelease = autoRelease;
		}

		/// @brief Returns whether auto release is enabled
		///
		/// @return - If auto release is enabled
		bool LevelDetectorModernBus<float>::getAutoRelease() const noexcept {
			return mAutoRelease;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackMS - The new attack time, in milliseconds
		void LevelDetectorModernBus<float>::setAttackTime(float attackMS) noexcept {
			LevelDetector<float>::setAttackTime(attackMS);
			float attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
			mAutoReleaseAttack2Coeff = math::expf(-1.0f / (attack2Seconds * static_cast<float>(mSampleRate)));
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in milliseconds
		void LevelDetectorModernBus<float>::setSampleRate(size_t sampleRate) noexcept {
			LevelDetector<float>::setSampleRate(sampleRate);
			float attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
			mAutoReleaseAttack2Coeff = math::expf(-1.0f / (attack2Seconds * static_cast<float>(mSampleRate)));
			mAutoRelease1Coeff = math::expf(-1.0f / (AUTO_RELEASE1_S * static_cast<float>(mSampleRate)));
			mAutoRelease2Coeff = math::expf(-1.0f / (AUTO_RELEASE2_S * static_cast<float>(mSampleRate)));
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		float LevelDetectorModernBus<float>::process(float input) noexcept {
			if(!mAutoRelease) return LevelDetector<float>::process(input);
			else {
				float y1n = mY1N1 + mAttackCoeff * math::max(input - mYOut1, 0.0f)
					- mAutoRelease1Coeff * mY1N1;
				float y2n = mY2N1 + mAutoReleaseAttack2Coeff * math::max(input - mYOut1, 0.0f)
					- mAutoRelease2Coeff * mY2N1;
				float yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				mYOut1 = yn;
				return yn;
			}
		}

		/// @brief Constructs a `LevelDetectorModernBus` with the following defaults:
		/// * attack: 30mS
		/// * release: 75mS
		/// * sampleRate: 44100Hz
		/// * auto release: disabled
		LevelDetectorModernBus<double>::LevelDetectorModernBus() noexcept
			: LevelDetector<double>(INITIAL_ATTACK_MS, INITIAL_RELEASE_MS,
					INITIAL_SAMPLERATE, DetectorType::DecoupledSmooth)
			{

			}

		/// @brief Constructs a `LevelDetectorModernBus` with the given parameters
		///
		/// @param attackMS - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		/// @param autoRelease - whether auto release is enabled
		LevelDetectorModernBus<double>::LevelDetectorModernBus(double attackMS, double releaseMS,
				size_t sampleRate, bool autoRelease) noexcept
			: LevelDetector<double>(attackMS, releaseMS, sampleRate, DetectorType::DecoupledSmooth),
			mAutoRelease(autoRelease)
			{
				double attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				mAutoReleaseAttack2Coeff = math::exp(-1.0 / (attack2Seconds * static_cast<double>(mSampleRate)));
				mAutoRelease1Coeff = math::exp(-1.0 / (AUTO_RELEASE1_S * static_cast<double>(mSampleRate)));
				mAutoRelease2Coeff = math::exp(-1.0 / (AUTO_RELEASE2_S * static_cast<double>(mSampleRate)));
			}

		/// @brief Sets whether auto release is enabled
		///
		/// @param autoRelease - Whether auto release should be enabled
		void LevelDetectorModernBus<double>::setAutoRelease(bool autoRelease) noexcept {
			mAutoRelease = autoRelease;
		}

		/// @brief Returns whether auto release is enabled
		///
		/// @return - If auto release is enabled
		bool LevelDetectorModernBus<double>::getAutoRelease() const noexcept {
			return mAutoRelease;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackMS - The new attack time, in milliseconds
		void LevelDetectorModernBus<double>::setAttackTime(double attackMS) noexcept {
			LevelDetector<double>::setAttackTime(attackMS);
			double attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
			mAutoReleaseAttack2Coeff = math::exp(-1.0 / (attack2Seconds * static_cast<double>(mSampleRate)));
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in milliseconds
		void LevelDetectorModernBus<double>::setSampleRate(size_t sampleRate) noexcept {
			LevelDetector<double>::setSampleRate(sampleRate);
			double attack2Seconds = mAttackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
			mAutoReleaseAttack2Coeff = math::exp(-1.0 / (attack2Seconds * static_cast<double>(mSampleRate)));
			mAutoRelease1Coeff = math::exp(-1.0 / (AUTO_RELEASE1_S * static_cast<double>(mSampleRate)));
			mAutoRelease2Coeff = math::exp(-1.0 / (AUTO_RELEASE2_S * static_cast<double>(mSampleRate)));
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		double LevelDetectorModernBus<double>::process(double input) noexcept {
			if(!mAutoRelease) return LevelDetector<double>::process(input);
			else {
				double y1n = mY1N1 + mAttackCoeff * math::max(input - mYOut1, 0.0)
					- mAutoRelease1Coeff * mY1N1;
				double y2n = mY2N1 + mAutoReleaseAttack2Coeff * math::max(input - mYOut1, 0.0)
					- mAutoRelease2Coeff * mY2N1;
				double yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				mYOut1 = yn;
				return yn;
			}
		}
	}
}
