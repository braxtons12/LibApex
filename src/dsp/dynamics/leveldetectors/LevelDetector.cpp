#include "LevelDetector.h"

namespace apex::dsp {
	/// @brief Constructs a `LevelDetector` of the given type
	/// with the given shared state
	///
	/// @param state - The shared state
	/// @param type - The detector type
	LevelDetector<float>::LevelDetector(DynamicsState* state, DetectorType type) noexcept
		: mType(type), mState(state) {
		mState->registerCallback<float, DynamicsState::Field::Attack>(
			[this](float attack) { this->setAttackTime(attack); });
		mState->registerCallback<float, DynamicsState::Field::Release>(
			[this](float release) { this->setReleaseTime(release); });
		mState->registerCallback<size_t, DynamicsState::Field::SampleRate>(
			[this](size_t sampleRate) { this->setSampleRate(sampleRate); });
	}

	/// @brief Generates the detected level from the given input
	///
	/// @param input - The input to detect on
	///
	/// @return - The detected level
	auto LevelDetector<float>::process(float input) noexcept -> float {
		switch(mType) {
			case DetectorType::NonCorrected: return processNonCorrected(input);
			case DetectorType::Branching: return processBranching(input);
			case DetectorType::Decoupled: return processDecoupled(input);
			case DetectorType::BranchingSmooth: return processBranchingSmooth(input);
			case DetectorType::DecoupledSmooth: return processDecoupledSmooth(input);
		}
		return 0.0F;
	}

	auto LevelDetector<float>::processNonCorrected(float input) noexcept -> float {
		// y[n] = releaseCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
		float yn = mState->getReleaseCoefficient1() * mYOut1
				   + (1.0F - mState->getAttackCoefficient1()) * math::max(input - mYOut1, 0.0F);
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<float>::processBranching(float input) noexcept -> float {
		//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
		// y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
		//       {
		float yn = (input > mYOut1 ? (mState->getAttackCoefficient1() * mYOut1
									  + (1.0F - mState->getAttackCoefficient1()) * input) :
									   (mState->getReleaseCoefficient1() * mYOut1));
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<float>::processDecoupled(float input) noexcept -> float {
		// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
		// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
		float ytempn = math::max(input, mState->getReleaseCoefficient1() * mYTempStage1);
		float yn = mState->getAttackCoefficient1() * mYOut1
				   + (1.0F - mState->getAttackCoefficient1()) * ytempn;
		mYTempStage1 = ytempn;
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<float>::processBranchingSmooth(float input) noexcept -> float {
		//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
		// y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
		//       {
		float yn = (input > mYOut1 ? (mState->getAttackCoefficient1() * mYOut1
									  + (1.0F - mState->getAttackCoefficient1()) * input) :
									   (mState->getReleaseCoefficient1() * mYOut1
									  + (1.0F - mState->getReleaseCoefficient1()) * input));
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<float>::processDecoupledSmooth(float input) noexcept -> float {
		// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * input)
		// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
		float ytempn = math::max(input,
								 mState->getReleaseCoefficient1() * mYTempStage1
									 + (1.0F - mState->getReleaseCoefficient1()) * input);
		float yn = mState->getAttackCoefficient1() * mYOut1
				   + (1.0F - mState->getAttackCoefficient1()) * ytempn;
		mYTempStage1 = ytempn;
		mYOut1 = yn;
		return yn;
	}

	/// @brief Constructs a `LevelDetector` of the given type
	/// with the given shared state
	///
	/// @param state - The shared state
	/// @param type - The detector type
	LevelDetector<double>::LevelDetector(DynamicsState* state, DetectorType type) noexcept
		: mType(type), mState(state) {
		mState->registerCallback<double, DynamicsState::Field::Attack>(
			[this](double attack) { this->setAttackTime(attack); });
		mState->registerCallback<double, DynamicsState::Field::Release>(
			[this](double release) { this->setReleaseTime(release); });
		mState->registerCallback<size_t, DynamicsState::Field::SampleRate>(
			[this](size_t sampleRate) { this->setSampleRate(sampleRate); });
	}

	/// @brief Generates the detected level from the given input
	///
	/// @param input - The input to detect on
	///
	/// @return - The detected level
	auto LevelDetector<double>::process(double input) noexcept -> double {
		switch(mType) {
			case DetectorType::NonCorrected: return processNonCorrected(input);
			case DetectorType::Branching: return processBranching(input);
			case DetectorType::Decoupled: return processDecoupled(input);
			case DetectorType::BranchingSmooth: return processBranchingSmooth(input);
			case DetectorType::DecoupledSmooth: return processDecoupledSmooth(input);
		}
		return 0.0F;
	}

	auto LevelDetector<double>::processNonCorrected(double input) noexcept -> double {
		// y[n] = releaseCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
		double yn = mState->getReleaseCoefficient1() * mYOut1
					+ (1.0 - mState->getAttackCoefficient1()) * math::max(input - mYOut1, 0.0);
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<double>::processBranching(double input) noexcept -> double {
		//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
		// y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
		//       {
		double yn = (input > mYOut1 ? (mState->getAttackCoefficient1() * mYOut1
									   + (1.0 - mState->getAttackCoefficient1()) * input) :
										(mState->getReleaseCoefficient1() * mYOut1));
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<double>::processDecoupled(double input) noexcept -> double {
		// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
		// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
		double ytempn = math::max(input, mState->getReleaseCoefficient1() * mYTempStage1);
		double yn = mState->getAttackCoefficient1() * mYOut1
					+ (1.0 - mState->getAttackCoefficient1()) * ytempn;
		mYTempStage1 = ytempn;
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<double>::processBranchingSmooth(double input) noexcept -> double {
		//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
		// y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
		//       {
		double yn = (input > mYOut1 ? (mState->getAttackCoefficient1() * mYOut1
									   + (1.0 - mState->getAttackCoefficient1()) * input) :
										(mState->getReleaseCoefficient1() * mYOut1
									   + (1.0 - mState->getReleaseCoefficient1()) * input));
		mYOut1 = yn;
		return yn;
	}

	auto LevelDetector<double>::processDecoupledSmooth(double input) noexcept -> double {
		// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * input)
		// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
		double ytempn = math::max(input,
								  mState->getReleaseCoefficient1() * mYTempStage1
									  + (1.0 - mState->getReleaseCoefficient1()) * input);
		double yn = mState->getAttackCoefficient1() * mYOut1
					+ (1.0 - mState->getAttackCoefficient1()) * ytempn;
		mYTempStage1 = ytempn;
		mYOut1 = yn;
		return yn;
	}
} // namespace apex::dsp
