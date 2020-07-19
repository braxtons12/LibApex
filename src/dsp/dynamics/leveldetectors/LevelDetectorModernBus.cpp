#include "LevelDetectorModernBus.h"

namespace apex {
	namespace dsp {
		/// @brief Modern-style Bus compressor Level Detector
		///
		/// @tparam T - The floating point type to back operations
		LevelDetectorModernBus<float>::LevelDetectorModernBus() noexcept
			: LevelDetector<float>()
			{

			}

		LevelDetectorModernBus<float>::LevelDetectorModernBus(DynamicsState* state) noexcept
			: LevelDetector<float>(state, DetectorType::DecoupledSmooth)
			{
				mState->registerCallback<float, DynamicsState::Field::Attack>([this](float attack) {
					this->setAttackTime(attack);
					});
				mState->registerCallback<float, DynamicsState::Field::Release>([this](float release) {
					this->setReleaseTime(release);
					});
				mState->registerCallback<size_t, DynamicsState::Field::SampleRate>([this](size_t sampleRate) {
					this->setSampleRate(sampleRate);
					});
				mState->registerCallback<bool, DynamicsState::Field::AutoRelease>([this](bool enabled) {
					this->setAutoRelease(enabled);
					});
			}

		/// @brief Sets whether auto release is enabled
		///
		/// @param autoRelease - Whether auto release should be enabled
		void LevelDetectorModernBus<float>::setAutoRelease(bool autoRelease) noexcept {
			mState->setAttackCoefficient2(
				math::expf(-1.0f / (mState->getAttack() * AUTO_RELEASE_ATTACK2_MULTIPLIER *
					static_cast<float>(mState->getSampleRate())))
			);
			mState->setReleaseCoefficient1(
				math::expf(-1.0f / (AUTO_RELEASE1_S * static_cast<float>(mState->getSampleRate())))
			);
			mState->setReleaseCoefficient2(
				math::expf(-1.0f / (AUTO_RELEASE2_S * static_cast<float>(mState->getSampleRate())))
			);
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackSeconds - The new attack time, in seconds
		void LevelDetectorModernBus<float>::setAttackTime(float attackSeconds) noexcept {
			mState->setAttackCoefficient1(
				math::expf(-1.0f / (attackSeconds * static_cast<float>(mState->getSampleRate())))
			);
			if(mState->getAutoReleaseEnabled()) {
				mState->setAttackCoefficient2(
					math::expf(-1.0f / (attackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER *
						static_cast<float>(mState->getSampleRate())))
				);
			}
		}

		/// @brief Sets the release time to the given value 
		/// 
		/// @param releaseSeconds - The new release time, in seconds
		void LevelDetectorModernBus<float>::setReleaseTime(float releaseSeconds) noexcept {
			if(mState->getAutoReleaseEnabled()) {
				mState->setReleaseCoefficient1(
					math::expf(-1.0f / (AUTO_RELEASE1_S * static_cast<float>(mState->getSampleRate())))
				);
				mState->setReleaseCoefficient2(
					math::expf(-1.0f / (AUTO_RELEASE2_S * static_cast<float>(mState->getSampleRate())))
				);
			}
			else {
				mState->setReleaseCoefficient1(
					math::expf(-1.0f / (releaseSeconds * static_cast<float>(mState->getSampleRate())))
				);
			}
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetectorModernBus<float>::setSampleRate(size_t sampleRate) noexcept {
			mState->setAttackCoefficient1(
				math::expf(-1.0f / (mState->getAttack() * static_cast<float>(mState->getSampleRate())))
			);
			if(mState->getAutoReleaseEnabled()) {
				mState->setAttackCoefficient2(
					math::expf(-1.0f / (mState->getAttack() * AUTO_RELEASE_ATTACK2_MULTIPLIER *
						static_cast<float>(sampleRate)))
				);
				mState->setReleaseCoefficient1(
					math::expf(-1.0f / (AUTO_RELEASE1_S * static_cast<float>(sampleRate)))
				);
				mState->setReleaseCoefficient2(
					math::expf(-1.0f / (AUTO_RELEASE2_S * static_cast<float>(sampleRate)))
				);
			}
			else {
				mState->setReleaseCoefficient1(
					math::expf(-1.0f / (mState->getRelease() * static_cast<float>(sampleRate)))
				);
			}
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		float LevelDetectorModernBus<float>::process(float input) noexcept {
			if(!mState->getAutoReleaseEnabled()) return LevelDetector<float>::process(input);
			else {
				float y1n = mY1N1 + mState->getAttackCoefficient1() * math::max(input - mYOut1, 0.0f)
					- mState->getReleaseCoefficient1() * mY1N1;
				float y2n = mY2N1 + mState->getAttackCoefficient2() * math::max(input - mYOut1, 0.0f)
					- mState->getReleaseCoefficient2() * mY2N1;
				float yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				mYOut1 = yn;
				return yn;
			}
		}

		/// @brief Modern-style Bus compressor Level Detector
		///
		/// @tparam T - The floating point type to back operations
		LevelDetectorModernBus<double>::LevelDetectorModernBus() noexcept
			: LevelDetector<double>()
			{

			}

		LevelDetectorModernBus<double>::LevelDetectorModernBus(DynamicsState* state) noexcept
			: LevelDetector<double>(state, DetectorType::DecoupledSmooth)
			{
				mState->registerCallback<double, DynamicsState::Field::Attack>([this](double attack) {
					this->setAttackTime(attack);
					});
				mState->registerCallback<double, DynamicsState::Field::Release>([this](double release) {
					this->setReleaseTime(release);
					});
				mState->registerCallback<size_t, DynamicsState::Field::SampleRate>([this](size_t sampleRate) {
					this->setSampleRate(sampleRate);
					});
				mState->registerCallback<bool, DynamicsState::Field::AutoRelease>([this](bool enabled) {
					this->setAutoRelease(enabled);
					});
			}

		/// @brief Sets whether auto release is enabled
		///
		/// @param autoRelease - Whether auto release should be enabled
		void LevelDetectorModernBus<double>::setAutoRelease(bool autoRelease) noexcept {
			mState->setAttackCoefficient2(
				math::exp(-1.0 / (mState->getAttack() * AUTO_RELEASE_ATTACK2_MULTIPLIER *
					static_cast<double>(mState->getSampleRate())))
			);
			mState->setReleaseCoefficient1(
				math::exp(-1.0 / (AUTO_RELEASE1_S * static_cast<double>(mState->getSampleRate())))
			);
			mState->setReleaseCoefficient2(
				math::exp(-1.0 / (AUTO_RELEASE2_S * static_cast<double>(mState->getSampleRate())))
			);
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackSeconds - The new attack time, in seconds
		void LevelDetectorModernBus<double>::setAttackTime(double attackSeconds) noexcept {
			mState->setAttackCoefficient1(
				math::exp(-1.0 / (attackSeconds * static_cast<double>(mState->getSampleRate())))
			);
			if(mState->getAutoReleaseEnabled()) {
				mState->setAttackCoefficient2(
					math::exp(-1.0 / (attackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER *
						static_cast<double>(mState->getSampleRate())))
				);
			}
		}

		/// @brief Sets the release time to the given value 
		/// 
		/// @param releaseSeconds - The new release time, in seconds
		void LevelDetectorModernBus<double>::setReleaseTime(double releaseSeconds) noexcept {
			if(mState->getAutoReleaseEnabled()) {
				mState->setReleaseCoefficient1(
					math::exp(-1.0 / (AUTO_RELEASE1_S * static_cast<double>(mState->getSampleRate())))
				);
				mState->setReleaseCoefficient2(
					math::exp(-1.0 / (AUTO_RELEASE2_S * static_cast<double>(mState->getSampleRate())))
				);
			}
			else {
				mState->setReleaseCoefficient1(
					math::exp(-1.0 / (releaseSeconds * static_cast<double>(mState->getSampleRate())))
				);
			}
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetectorModernBus<double>::setSampleRate(size_t sampleRate) noexcept {
			mState->setAttackCoefficient1(
				math::exp(-1.0 / (mState->getAttack() * static_cast<double>(mState->getSampleRate())))
			);
			if(mState->getAutoReleaseEnabled()) {
				mState->setAttackCoefficient2(
					math::exp(-1.0 / (mState->getAttack() * AUTO_RELEASE_ATTACK2_MULTIPLIER *
						static_cast<double>(sampleRate)))
				);
				mState->setReleaseCoefficient1(
					math::exp(-1.0 / (AUTO_RELEASE1_S * static_cast<double>(sampleRate)))
				);
				mState->setReleaseCoefficient2(
					math::exp(-1.0 / (AUTO_RELEASE2_S * static_cast<double>(sampleRate)))
				);
			}
			else {
				mState->setReleaseCoefficient1(
					math::exp(-1.0 / (mState->getRelease() * static_cast<double>(sampleRate)))
				);
			}
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		double LevelDetectorModernBus<double>::process(double input) noexcept {
			if(!mState->getAutoReleaseEnabled()) return LevelDetector<double>::process(input);
			else {
				double y1n = mY1N1 + mState->getAttackCoefficient1() * math::max(input - mYOut1, 0.0)
					- mState->getReleaseCoefficient1() * mY1N1;
				double y2n = mY2N1 + mState->getAttackCoefficient2() * math::max(input - mYOut1, 0.0)
					- mState->getReleaseCoefficient2() * mY2N1;
				double yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				mYOut1 = yn;
				return yn;
			}
		}
	}
}
