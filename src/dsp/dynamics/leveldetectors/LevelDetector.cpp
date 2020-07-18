#include "LevelDetector.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a `LevelDetector` of the given type
		/// with the given shared state
		///
		/// @param state - The shared state
		/// @param type - The detector type
		LevelDetector<float>::LevelDetector(DynamicsState* state,
				DetectorType type) noexcept
			: mType(type),
			mState(state)
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

			}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		float LevelDetector<float>::process(float input) noexcept {
			switch(mType) {
				case DetectorType::NonCorrected: return processNonCorrected(input);
				case DetectorType::Branching: return processBranching(input);
				case DetectorType::Decoupled: return processDecoupled(input);
				case DetectorType::BranchingSmooth: return processBranchingSmooth(input);
				case DetectorType::DecoupledSmooth: return processDecoupledSmooth(input);
			}
			return 0.0f;
		}

		/// @brief Resets this level detector to an initial state
		inline void LevelDetector<float>::reset() noexcept {
			mYOut1 = 0.0f;
			mYTempStage1 = 0.0f;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackSeconds - The new attack time, in seconds
		inline void LevelDetector<float>::setAttackTime(float attackSeconds) noexcept {
			mState->setAttackCoefficient1(math::expf(-1.0f /
						(attackSeconds * static_cast<float>(mState->getSampleRate()))));
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseSeconds - The new release time, in seconds
		inline void LevelDetector<float>::setReleaseTime(float releaseSeconds) noexcept {
			mState->setReleaseCoefficient1(math::expf(-1.0f /
						(releaseSeconds * static_cast<float>(mState->getSampleRate()))));
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline void LevelDetector<float>::setSampleRate(size_t sampleRate) noexcept {
			mState->setAttackCoefficient1(math::expf(-1.0f /
						(mState->getAttack() * static_cast<float>(sampleRate))));
			mState->setReleaseCoefficient1(math::expf(-1.0f /
						(mState->getRelease() * static_cast<float>(sampleRate))));
		}

		float LevelDetector<float>::processNonCorrected(float input) noexcept {
			//y[n] = releaseCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
			float yn = mState->getReleaseCoefficient1() * mYOut1 +
				(1.0f - mState->getAttackCoefficient1()) *
				math::max(input - mYOut1, 0.0f);
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processBranching(float input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
			//       {
			float yn = (input > mYOut1 ?
					(mState->getAttackCoefficient1() * mYOut1 +
					 (1.0f - mState->getAttackCoefficient1()) * input)
					: (mState->getReleaseCoefficient1() * mYOut1));
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processDecoupled(float input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			float ytempn = math::max(input, mState->getReleaseCoefficient1() * mYTempStage1);
			float yn = mState->getAttackCoefficient1() * mYOut1 +
				(1.0f - mState->getAttackCoefficient1()) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processBranchingSmooth(float input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
			//       {
			float yn = (input > mYOut1 ?
					(mState->getAttackCoefficient1() * mYOut1 +
					 (1.0f - mState->getAttackCoefficient1()) * input)
					: (mState->getReleaseCoefficient1() * mYOut1 +
						(1.0f - mState->getReleaseCoefficient1()) * input));
			mYOut1 = yn;
			return yn;
		}

		float LevelDetector<float>::processDecoupledSmooth(float input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * input)
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			float ytempn = math::max(input, mState->getReleaseCoefficient1() * mYTempStage1
					+ (1.0f - mState->getReleaseCoefficient1()) * input);
			float yn = mState->getAttackCoefficient1() * mYOut1 +
				(1.0f - mState->getAttackCoefficient1()) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		/// @brief Constructs a `LevelDetector` of the given type
		/// with the given shared state
		///
		/// @param state - The shared state
		/// @param type - The detector type
		LevelDetector<double>::LevelDetector(DynamicsState* state,
				DetectorType type) noexcept
			: mType(type),
			mState(state)
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
			}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		double LevelDetector<double>::process(double input) noexcept {
			switch(mType) {
				case DetectorType::NonCorrected: return processNonCorrected(input);
				case DetectorType::Branching: return processBranching(input);
				case DetectorType::Decoupled: return processDecoupled(input);
				case DetectorType::BranchingSmooth: return processBranchingSmooth(input);
				case DetectorType::DecoupledSmooth: return processDecoupledSmooth(input);
			}
			return 0.0f;
		}

		/// @brief Resets this level detector to an initial state
		inline void LevelDetector<double>::reset() noexcept {
			mYOut1 = 0.0f;
			mYTempStage1 = 0.0f;
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackSeconds - The new attack time, in seconds
		inline void LevelDetector<double>::setAttackTime(double attackSeconds) noexcept {
			mState->setAttackCoefficient1(math::exp(-1.0 /
						(attackSeconds * static_cast<double>(mState->getSampleRate()))));
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseSeconds - The new release time, in seconds
		inline void LevelDetector<double>::setReleaseTime(double releaseSeconds) noexcept {
			mState->setReleaseCoefficient1(math::exp(-1.0 /
						(releaseSeconds * static_cast<double>(mState->getSampleRate()))));
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline void LevelDetector<double>::setSampleRate(size_t sampleRate) noexcept {
			mState->setAttackCoefficient1(math::exp(-1.0 /
						(mState->getAttack() * static_cast<double>(sampleRate))));
			mState->setReleaseCoefficient1(math::exp(-1.0 /
						(mState->getRelease() * static_cast<double>(sampleRate))));
		}

		double LevelDetector<double>::processNonCorrected(double input) noexcept {
			//y[n] = releaseCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
			double yn = mState->getReleaseCoefficient1() * mYOut1 +
				(1.0 - mState->getAttackCoefficient1()) *
				math::max(input - mYOut1, 0.0);
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processBranching(double input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
			//       {
			double yn = (input > mYOut1 ?
					(mState->getAttackCoefficient1() * mYOut1 +
					 (1.0 - mState->getAttackCoefficient1()) * input)
					: (mState->getReleaseCoefficient1() * mYOut1));
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processDecoupled(double input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			double ytempn = math::max(input, mState->getReleaseCoefficient1() * mYTempStage1);
			double yn = mState->getAttackCoefficient1() * mYOut1 +
				(1.0 - mState->getAttackCoefficient1()) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processBranchingSmooth(double input) noexcept {
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
			//y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
			//       {
			double yn = (input > mYOut1 ?
					(mState->getAttackCoefficient1() * mYOut1 +
					 (1.0 - mState->getAttackCoefficient1()) * input)
					: (mState->getReleaseCoefficient1() * mYOut1 +
						(1.0 - mState->getReleaseCoefficient1()) * input));
			mYOut1 = yn;
			return yn;
		}

		double LevelDetector<double>::processDecoupledSmooth(double input) noexcept {
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * input)
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			double ytempn = math::max(input, mState->getReleaseCoefficient1() * mYTempStage1
					+ (1.0 - mState->getReleaseCoefficient1()) * input);
			double yn = mState->getAttackCoefficient1() * mYOut1 +
				(1.0 - mState->getAttackCoefficient1()) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}
		} //dsp, not sure why auto-format is glitching w/ these
		} //apex, not sure why auto-format is glitching w/ these
