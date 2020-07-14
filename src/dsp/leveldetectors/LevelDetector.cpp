#include "LevelDetector.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a `LevelDetector` with the given parameters
		///
		/// @param attackMS - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		/// @param type - The detector topology to use
		LevelDetector<float>::LevelDetector(float attackMS, float releaseMS,
				size_t sampleRate, DetectorType type) noexcept
			: mAttackSeconds(attackMS * 0.001f),
			mReleaseSeconds(releaseMS * 0.001f),
			mSampleRate(sampleRate),
			mAttackCoeff(math::expf(-1.0f / (mAttackSeconds * static_cast<float>(mSampleRate)))),
			mReleaseCoeff(math::expf(-1.0f / (mReleaseSeconds * static_cast<float>(mSampleRate)))),
			mType(type)
			{

			}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		float LevelDetector<float>::process(float input) noexcept {
			switch(mType) {
				case NonCorrected: return processNonCorrected(input);
				case Branching: return processBranching(input);
				case Decoupled: return processDecoupled(input);
				case BranchingSmooth: return processBranchingSmooth(input);
				case DecoupledSmooth: return processDecoupledSmooth(input);
			}
		}

		/// @brief Resets this level detector to an initial state
		void LevelDetector<float>::reset() noexcept {
			mYOut1 = 0.0f;
			mYTempStage1 = 0.0f;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackMS - The new attack time, in milliseconds
		void LevelDetector<float>::setAttackTime(float attackMS) noexcept {
			mAttackSeconds = attackMS * 0.001f;
			mAttackCoeff = math::expf(-1.0f / (mAttackSeconds * static_cast<float>(mSampleRate)));
		}

		/// @brief Returns the current attack time
		///
		/// @return - The attack time, in milliseconds
		float LevelDetector<float>::getAttackTime() const noexcept {
			return mAttackSeconds * 1000.0f;
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseMS - The new release time, in milliseconds
		void LevelDetector<float>::setReleaseTime(float releaseMS) noexcept {
			mReleaseSeconds = releaseMS * 0.001f;
			mReleaseCoeff = math::expf(-1.0f / (mReleaseSeconds * static_cast<float>(mSampleRate)));
		}

		/// @brief Returns the current release time
		///
		/// @return - The release time, in milliseconds
		float LevelDetector<float>::getReleaseTime() const noexcept {
			return mReleaseSeconds * 1000.0f;
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetector<float>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mAttackCoeff = math::expf(-1.0f / (mAttackSeconds * static_cast<float>(mSampleRate)));
			mReleaseCoeff = math::expf(-1.0f / (mReleaseSeconds * static_cast<float>(mSampleRate)));
		}

		/// @brief Returns the current sample rate
		///
		/// @return - The sample rate, in Hertz
		size_t LevelDetector<float>::getSampleRate() const noexcept {
			return mSampleRate;
		}

		float LevelDetector<float>::processNonCorrected(float input) noexcept {
			//y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
			float yn = mReleaseCoeff * mYOut1 + (1.0f - mAttackCoeff) *
				math::max(input - mYOut1, 0.0f);
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processBranching(float input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
			//       {
			float yn = (input > mYOut1 ?
					(mAttackCoeff * mYOut1 + (1.0f - mAttackCoeff) * input)
					: (mReleaseCoeff * mYOut1));
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processDecoupled(float input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			float ytempn = math::max(input, mReleaseCoeff * mYTempStage1);
			float yn = mAttackCoeff * mYOut1 + (1.0f - mAttackCoeff) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processBranchingSmooth(float input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
			//       {
			float yn = (input > mYOut1 ?
					(mAttackCoeff * mYOut1 + (1.0f - mAttackCoeff) * input)
					: (mReleaseCoeff * mYOut1 + (1.0f - mReleaseCoeff) * input));
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processDecoupledSmooth(float input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * input)
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			float ytempn = math::max(input, mReleaseCoeff * mYTempStage1
					+ (1.0f - mReleaseCoeff) * input);
			float yn = mAttackCoeff * mYOut1 + (1.0f - mAttackCoeff) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		/// @brief Constructs a `LevelDetector` with the given parameters
		///
		/// @param attackMS - The attack time, in milliseconds
		/// @param releaseMS - The release time, in milliseconds
		/// @param sampleRate - The sample rate, in Hertz
		/// @param type - The detector topology to use
		LevelDetector<double>::LevelDetector(double attackMS, double releaseMS,
				size_t sampleRate, DetectorType type) noexcept
			: mAttackSeconds(attackMS * 0.001),
			mReleaseSeconds(releaseMS * 0.001),
			mSampleRate(sampleRate),
			mAttackCoeff(math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate)))),
			mReleaseCoeff(math::exp(-1.0 / (mReleaseSeconds * static_cast<double>(mSampleRate)))),
			mType(type)
			{

			}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		double LevelDetector<double>::process(double input) noexcept {
			switch(mType) {
				case NonCorrected: return processNonCorrected(input);
				case Branching: return processBranching(input);
				case Decoupled: return processDecoupled(input);
				case BranchingSmooth: return processBranchingSmooth(input);
				case DecoupledSmooth: return processDecoupledSmooth(input);
			}
		}

		/// @brief Resets this level detector to an initial state
		void LevelDetector<double>::reset() noexcept {
			mYOut1 = 0.0;
			mYTempStage1 = 0.0;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackMS - The new attack time, in milliseconds
		void LevelDetector<double>::setAttackTime(double attackMS) noexcept {
			mAttackSeconds = attackMS * 0.001;
			mAttackCoeff = math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate)));
		}

		/// @brief Returns the current attack time
		///
		/// @return - The attack time, in milliseconds
		double LevelDetector<double>::getAttackTime() const noexcept {
			return mAttackSeconds * 1000.0;
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseMS - The new release time, in milliseconds
		void LevelDetector<double>::setReleaseTime(double releaseMS) noexcept {
			mReleaseSeconds = releaseMS * 0.001;
			mReleaseCoeff = math::exp(-1.0 / (mReleaseSeconds * static_cast<double>(mSampleRate)));
		}

		/// @brief Returns the current release time
		///
		/// @return - The release time, in milliseconds
		double LevelDetector<double>::getReleaseTime() const noexcept {
			return mReleaseSeconds * 1000.0;
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		void LevelDetector<double>::setSampleRate(size_t sampleRate) noexcept {
			mSampleRate = sampleRate;
			mAttackCoeff = math::exp(-1.0 / (mAttackSeconds * static_cast<double>(mSampleRate)));
			mReleaseCoeff = math::exp(-1.0 / (mReleaseSeconds * static_cast<double>(mSampleRate)));
		}

		/// @brief Returns the current sample rate
		///
		/// @return - The sample rate, in Hertz
		size_t LevelDetector<double>::getSampleRate() const noexcept {
			return mSampleRate;
		}

		double LevelDetector<double>::processNonCorrected(double input) noexcept {
			//y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
			double yn = mReleaseCoeff * mYOut1 + (1.0 - mAttackCoeff) *
				math::max(input - mYOut1, 0.0);
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processBranching(double input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
			//       {
			double yn = (input > mYOut1 ?
					(mAttackCoeff * mYOut1 + (1.0 - mAttackCoeff) * input)
					: (mReleaseCoeff * mYOut1));
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processDecoupled(double input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			double ytempn = math::max(input, mReleaseCoeff * mYTempStage1);
			double yn = mAttackCoeff * mYOut1 + (1.0 - mAttackCoeff) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processBranchingSmooth(double input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
			//       {
			double yn = (input > mYOut1 ?
					(mAttackCoeff * mYOut1 + (1.0 - mAttackCoeff) * input)
					: (mReleaseCoeff * mYOut1 + (1.0 - mReleaseCoeff) * input));
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processDecoupledSmooth(double input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * x[n])
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			double ytempn = math::max(input, mReleaseCoeff * mYTempStage1
					+ (1.0 - mReleaseCoeff) * input);
			double yn = mAttackCoeff * mYOut1 + (1.0 - mAttackCoeff) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		} //dsp, not sure why auto-format is glitching w/ these
		} //apex, not sure why auto-format is glitching w/ these
