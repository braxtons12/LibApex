#include "LevelDetectorRMS.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs an `RMSLevelDetector` with the given parameters
		///
		/// @param attackMS - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		/// @param type - The detector topology to use
		LevelDetectorRMS<float>::LevelDetectorRMS(float attackMS, float releaseMS,
				size_t sampleRate, DetectorType type) noexcept
			: LevelDetector<float>(attackMS, releaseMS, sampleRate, type),
			mRMSSeconds(releaseMS * 2.0f),
			mRMSCoeff(math::expf(-1.0f / (mRMSSeconds * static_cast<float>(sampleRate))))
			{

			}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		float LevelDetectorRMS<float>::process(float input) noexcept {
			float xn = LevelDetector<float>::process(input);
			float y2n = mRMSCoeff * mYSquared1 + (1.0f - mRMSCoeff) * (xn * xn);
			float yn = math::sqrtf(y2n);
			mYSquared1 = y2n;
			return yn;
		}

		/// @brief Resets this level detector to an initial state
		void LevelDetectorRMS<float>::reset() noexcept {
			LevelDetector<float>::reset();
			mYSquared1 = 0.0f;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackMS - The new attack time, in milliseconds
		void LevelDetectorRMS<float>::setAttackTime(float attackMS) noexcept {
			LevelDetector<float>::setAttackTime(attackMS);
		}

		/// @brief Returns the current attack time
		///
		/// @return - The attack time, in milliseconds
		float LevelDetectorRMS<float>::getAttackTime() const noexcept {
			return LevelDetector<float>::getAttackTime();
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseMS - The new release time, in milliseconds
		void LevelDetectorRMS<float>::setReleaseTime(float releaseMS) noexcept {
			LevelDetector<float>::setReleaseTime(releaseMS);
			mRMSSeconds = mReleaseSeconds * 2.0f;
			mRMSCoeff = math::expf(-1.0f / (mRMSSeconds * static_cast<float>(mSampleRate)));
		}

		/// @brief Returns the current release time
		///
		/// @return - The release time, in milliseconds
		float LevelDetectorRMS<float>::getReleaseTime() const noexcept {
			return LevelDetector<float>::getReleaseTime();
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetectorRMS<float>::setSampleRate(size_t sampleRate) noexcept {
			LevelDetector<float>::setSampleRate(sampleRate);
			mRMSCoeff = math::expf(-1.0f / (mRMSSeconds * static_cast<float>(mSampleRate)));
		}

		/// @brief Returns the current sample rate
		///
		/// @return - The sample rate, in Hertz
		size_t LevelDetectorRMS<float>::getSampleRate() const noexcept {
			return LevelDetector<float>::getSampleRate();
		}

		/// @brief Constructs an `RMSLevelDetector` with the given parameters
		///
		/// @param attackMS - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		/// @param type - The detector topology to use
		LevelDetectorRMS<double>::LevelDetectorRMS(double attackMS, double releaseMS,
				size_t sampleRate, DetectorType type) noexcept
			: LevelDetector<double>(attackMS, releaseMS, sampleRate, type),
			mRMSSeconds(releaseMS * 2.0),
			mRMSCoeff(math::exp(-1.0 / (mRMSSeconds * static_cast<double>(sampleRate))))
			{

			}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		double LevelDetectorRMS<double>::process(double input) noexcept {
			double xn = LevelDetector<double>::process(input);
			double y2n = mRMSCoeff * mYSquared1 + (1.0 - mRMSCoeff) * (xn * xn);
			double yn = math::sqrt(y2n);
			mYSquared1 = y2n;
			return yn;
		}

		/// @brief Resets this level detector to an initial state
		void LevelDetectorRMS<double>::reset() noexcept {
			LevelDetector<double>::reset();
			mYSquared1 = 0.0f;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackMS - The new attack time, in milliseconds
		void LevelDetectorRMS<double>::setAttackTime(double attackMS) noexcept {
			LevelDetector<double>::setAttackTime(attackMS);
		}

		/// @brief Returns the current attack time
		///
		/// @return - The attack time, in milliseconds
		double LevelDetectorRMS<double>::getAttackTime() const noexcept {
			return LevelDetector<double>::getAttackTime();
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseMS - The new release time, in milliseconds
		void LevelDetectorRMS<double>::setReleaseTime(double releaseMS) noexcept {
			LevelDetector<double>::setReleaseTime(releaseMS);
			mRMSSeconds = mReleaseSeconds * 2.0;
			mRMSCoeff = math::exp(-1.0 / (mRMSSeconds * static_cast<double>(mSampleRate)));
		}

		/// @brief Returns the current release time
		///
		/// @return - The release time, in milliseconds
		double LevelDetectorRMS<double>::getReleaseTime() const noexcept {
			return LevelDetector<double>::getReleaseTime();
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetectorRMS<double>::setSampleRate(size_t sampleRate) noexcept {
			LevelDetector<double>::setSampleRate(sampleRate);
			mRMSCoeff = math::exp(-1.0 / (mRMSSeconds * static_cast<double>(mSampleRate)));
		}

		/// @brief Returns the current sample rate
		///
		/// @return - The sample rate, in Hertz
		size_t LevelDetectorRMS<double>::getSampleRate() const noexcept {
			return LevelDetector<double>::getSampleRate();
		}
	}
}
