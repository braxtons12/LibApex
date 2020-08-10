#include "Sidechain.h"

namespace apex::dsp {
	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto Sidechain<float>::process(float input) noexcept -> float {
		float x = 0.0F;
		switch(mComputerTopology) {
			case ComputerTopology::FeedForward:
				{
					switch(mDetectorTopology) {
						case DetectorTopology::ReturnToZero:
							x = processFeedForwardReturnToZero(input);
							break;
						case DetectorTopology::ReturnToThreshold:
							x = processFeedForwardReturnToThreshold(input);
							break;
						case DetectorTopology::AlternateReturnToThreshold:
							x = processFeedForwardAlternateReturnToThreshold(input);
							break;
					}
				}
				break;
			case ComputerTopology::FeedBack:
				{
					switch(mDetectorTopology) {
						case DetectorTopology::ReturnToZero:
							x = processFeedBackReturnToZero(input);
							break;
						case DetectorTopology::ReturnToThreshold:
							x = processFeedBackReturnToThreshold(input);
							break;
						case DetectorTopology::AlternateReturnToThreshold:
							x = processFeedBackAlternateReturnToThreshold(input);
							break;
					}
				}
				break;
		}
		return mGainReductionProcessor.adjustedGainReduction(x);
	}

	/// @brief Sets the attack to the given value
	///
	/// @param attackMS - The attack time, in milliseconds
	auto Sidechain<float>::setAttackTime(float attackMS) noexcept -> void {
		mState.setAttack(attackMS * MS_TO_SECS_MULT);
	}

	/// @brief Returns the attack
	///
	/// @return - The attack time, in milliseconds
	auto Sidechain<float>::getAttackTime() const noexcept -> float {
		return mState.getAttack() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the release to the given value
	///
	/// @param releaseMS - The release time, in milliseconds
	auto Sidechain<float>::setReleaseTime(float releaseMS) noexcept -> void {
		mState.setRelease(releaseMS * MS_TO_SECS_MULT);
	}

	/// @brief Returns the release
	///
	/// @return - The release time, in milliseconds
	auto Sidechain<float>::getReleaseTime() const noexcept -> float {
		return mState.getRelease() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the Ratio
	///
	/// @param ratio - The ratio
	auto Sidechain<float>::setRatio(float ratio) noexcept -> void {
		mState.setRatio(ratio);
	}

	/// @brief Returns the Ratio
	///
	/// @return - The ratio
	auto Sidechain<float>::getRatio() const noexcept -> float {
		return mState.getRatio();
	}

	/// @brief Sets the Threshold
	///
	/// @param threshold - The threshold, in decibels
	auto Sidechain<float>::setThreshold(float threshold) noexcept -> void {
		mState.setThreshold(threshold);
	}

	/// @brief Returns the Threshold
	///
	/// @return - The threshold, in decibels
	auto Sidechain<float>::getThreshold() const noexcept -> float {
		return mState.getThreshold();
	}

	/// @brief Sets the KneeWidth
	///
	/// @param kneeWidth - The knee width, in decibels
	auto Sidechain<float>::setKneeWidth(float kneeWidth) noexcept -> void {
		mState.setKneeWidth(kneeWidth);
	}

	/// @brief Returns the KneeWidth
	///
	/// @return - The knee width, in decibels
	auto Sidechain<float>::getKneeWidth() const noexcept -> float {
		return mState.getKneeWidth();
	}

	/// @brief Sets the DynamicsType
	///
	/// @param type - The dynamics type
	auto Sidechain<float>::setDynamicsType(DynamicsType type) noexcept -> void {
		mDynamicsType = type;
		if(mDynamicsType == DynamicsType::Compressor) {
			mGainComputer = &mCompressorComputer;
		}
		else {
			mGainComputer = &mExpanderComputer;
		}
	}

	/// @brief Returns the DynamicsType
	///
	/// @return - The dynamics type
	auto Sidechain<float>::getDynamicsType() const noexcept -> Sidechain<float>::DynamicsType {
		return mDynamicsType;
	}

	/// @brief Sets the SampleRate
	///
	/// @param sampleRate - The sample rate, in Hertz
	auto Sidechain<float>::setSampleRate(size_t sampleRate) noexcept -> void {
		mState.setSampleRate(sampleRate);
	}

	/// @brief Returns the SampleRate
	///
	/// @return - The sample rate, in Hertz
	auto Sidechain<float>::getSampleRate() const noexcept -> size_t {
		return mState.getSampleRate();
	}

	/// @brief Returns the most recently calculated gain reduction value
	///
	/// @return - The most recently calculated gain reduction value (linear)
	auto Sidechain<float>::getCurrentGainReduction() const noexcept -> float {
		return mGainReductionDB;
	}

	/// @brief Sets the type of the `LevelDetector` to the given value
	///
	/// @param type - The type of the `LevelDetector`
	auto Sidechain<float>::setLevelDetectorType(LevelDetector<float>::DetectorType type) noexcept
		-> void {
		mDetectorType = type;
		mLevelDetector = LevelDetector<float>(&mState, mDetectorType);
	}

	/// @brief Returns the type of the `LevelDetector`
	///
	/// @return - The type of the `LevelDetector`
	auto
	Sidechain<float>::getLevelDetectorType() const noexcept -> LevelDetector<float>::DetectorType {
		return mDetectorType;
	}

	/// @brief Sets the macro-level topology of the gain reduction computer
	///
	/// @param topology - The macro-level topology of the gain reduction computer
	auto Sidechain<float>::setComputerTopology(ComputerTopology topology) noexcept -> void {
		mComputerTopology = topology;
	}

	/// @brief Returns the macro-level topology of the gain reduction computer
	///
	/// @return - The macro-level topology of the gain reduction computer
	auto
	Sidechain<float>::getComputerTopology() const noexcept -> Sidechain<float>::ComputerTopology {
		return mComputerTopology;
	}

	/// @brief Sets the macro-level topology of the level detector
	///
	/// @param topology - The macro-level topology of the level detector
	auto Sidechain<float>::setDetectorTopology(DetectorTopology topology) noexcept -> void {
		mDetectorTopology = topology;
	}

	/// @brief Returns the macro-level topology of the level detector
	///
	/// @return - The macro-level topology of the level detector
	auto
	Sidechain<float>::getDetectorTopology() const noexcept -> Sidechain<float>::DetectorTopology {
		return mDetectorTopology;
	}

	/// @brief Replaces the `GainReduction` adjuster with the given one
	///
	/// @param reduction - The new `GainReduction`
	auto Sidechain<float>::setGainReductionProcessor(
		GainReduction<float, float, float>&& reduction) noexcept -> void {
		mGainReductionProcessor = std::move(reduction);
	}

	auto Sidechain<float>::processFeedForwardReturnToZero(float input) noexcept -> float {
		float rectified = math::fabsf(input);
		float detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
		float outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB = outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<float>::processFeedForwardReturnToThreshold(float input) noexcept -> float {
		float rectified = math::fabsf(input);
		float thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
		float detectedDB = math::Decibels::linearToDecibels(
			mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
		float outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB = outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto
	Sidechain<float>::processFeedForwardAlternateReturnToThreshold(float input) noexcept -> float {
		float rectified = math::fabsf(input);
		float rectifiedDB = math::Decibels::linearToDecibels(rectified);
		float gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(gainReduction);
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<float>::processFeedBackReturnToZero(float input) noexcept -> float {
		float rectified = math::fabsf(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
		float detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
		float outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB += outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<float>::processFeedBackReturnToThreshold(float input) noexcept -> float {
		float rectified = math::fabsf(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
		float thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
		float detectedDB = math::Decibels::linearToDecibels(
			mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
		float outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB += outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto
	Sidechain<float>::processFeedBackAlternateReturnToThreshold(float input) noexcept -> float {
		float rectified = math::fabsf(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
		float rectifiedDB = math::Decibels::linearToDecibels(rectified);
		float gainReduction = mGainReductionDB + mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(gainReduction);
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto Sidechain<double>::process(double input) noexcept -> double {
		double x = 0.0;
		switch(mComputerTopology) {
			case ComputerTopology::FeedForward:
				{
					switch(mDetectorTopology) {
						case DetectorTopology::ReturnToZero:
							x = processFeedForwardReturnToZero(input);
							break;
						case DetectorTopology::ReturnToThreshold:
							x = processFeedForwardReturnToThreshold(input);
							break;
						case DetectorTopology::AlternateReturnToThreshold:
							x = processFeedForwardAlternateReturnToThreshold(input);
							break;
					}
				}
				break;
			case ComputerTopology::FeedBack:
				{
					switch(mDetectorTopology) {
						case DetectorTopology::ReturnToZero:
							x = processFeedBackReturnToZero(input);
							break;
						case DetectorTopology::ReturnToThreshold:
							x = processFeedBackReturnToThreshold(input);
							break;
						case DetectorTopology::AlternateReturnToThreshold:
							x = processFeedBackAlternateReturnToThreshold(input);
							break;
					}
				}
				break;
		}
		return mGainReductionProcessor.adjustedGainReduction(x);
	}

	/// @brief Sets the attack to the given value
	///
	/// @param attackMS - The attack time, in milliseconds
	auto Sidechain<double>::setAttackTime(double attackMS) noexcept -> void {
		mState.setAttack(attackMS * MS_TO_SECS_MULT);
	}

	/// @brief Returns the attack
	///
	/// @return - The attack time, in milliseconds
	auto Sidechain<double>::getAttackTime() const noexcept -> double {
		return mState.getAttack() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the release to the given value
	///
	/// @param releaseMS - The release time, in milliseconds
	auto Sidechain<double>::setReleaseTime(double releaseMS) noexcept -> void {
		mState.setRelease(releaseMS * MS_TO_SECS_MULT);
	}

	/// @brief Returns the release
	///
	/// @return - The release time, in milliseconds
	auto Sidechain<double>::getReleaseTime() const noexcept -> double {
		return mState.getRelease() / MS_TO_SECS_MULT;
	}

	/// @brief Sets the Ratio
	///
	/// @param ratio - The ratio
	auto Sidechain<double>::setRatio(double ratio) noexcept -> void {
		mState.setRatio(ratio);
	}

	/// @brief Returns the Ratio
	///
	/// @return - The ratio
	auto Sidechain<double>::getRatio() const noexcept -> double {
		return mState.getRatio();
	}

	/// @brief Sets the Threshold
	///
	/// @param threshold - The threshold, in decibels
	auto Sidechain<double>::setThreshold(double threshold) noexcept -> void {
		mState.setThreshold(threshold);
	}

	/// @brief Returns the Threshold
	///
	/// @return - The threshold, in decibels
	auto Sidechain<double>::getThreshold() const noexcept -> double {
		return mState.getThreshold();
	}

	/// @brief Sets the KneeWidth
	///
	/// @param kneeWidth - The knee width, in decibels
	auto Sidechain<double>::setKneeWidth(double kneeWidth) noexcept -> void {
		mState.setKneeWidth(kneeWidth);
	}

	/// @brief Returns the KneeWidth
	///
	/// @return - The knee width, in decibels
	auto Sidechain<double>::getKneeWidth() const noexcept -> double {
		return mState.getKneeWidth();
	}

	/// @brief Sets the DynamicsType
	///
	/// @param type - The dynamics type
	auto Sidechain<double>::setDynamicsType(DynamicsType type) noexcept -> void {
		mDynamicsType = type;
		if(mDynamicsType == DynamicsType::Compressor) {
			mGainComputer = &mCompressorComputer;
		}
		else {
			mGainComputer = &mExpanderComputer;
		}
	}

	/// @brief Returns the DynamicsType
	///
	/// @return - The dynamics type
	auto Sidechain<double>::getDynamicsType() const noexcept -> Sidechain<double>::DynamicsType {
		return mDynamicsType;
	}

	/// @brief Sets the SampleRate
	///
	/// @param sampleRate - The sample rate, in Hertz
	auto Sidechain<double>::setSampleRate(size_t sampleRate) noexcept -> void {
		mState.setSampleRate(sampleRate);
	}

	/// @brief Returns the SampleRate
	///
	/// @return - The sample rate, in Hertz
	auto Sidechain<double>::getSampleRate() const noexcept -> size_t {
		return mState.getSampleRate();
	}

	/// @brief Returns the most recently calculated gain reduction value
	///
	/// @return - The most recently calculated gain reduction value (linear)
	auto Sidechain<double>::getCurrentGainReduction() const noexcept -> double {
		return mGainReductionDB;
	}

	/// @brief Sets the type of the `LevelDetector` to the given value
	///
	/// @param type - The type of the `LevelDetector`
	auto Sidechain<double>::setLevelDetectorType(LevelDetector<double>::DetectorType type) noexcept
		-> void {
		mDetectorType = type;
		mLevelDetector = LevelDetector<double>(&mState, mDetectorType);
	}

	/// @brief Returns the type of the `LevelDetector`
	///
	/// @return - The type of the `LevelDetector`
	auto Sidechain<double>::getLevelDetectorType() const noexcept
		-> LevelDetector<double>::DetectorType {
		return mDetectorType;
	}

	/// @brief Sets the macro-level topology of the gain reduction computer
	///
	/// @param topology - The macro-level topology of the gain reduction computer
	auto Sidechain<double>::setComputerTopology(ComputerTopology topology) noexcept -> void {
		mComputerTopology = topology;
	}

	/// @brief Returns the macro-level topology of the gain reduction computer
	///
	/// @return - The macro-level topology of the gain reduction computer
	auto
	Sidechain<double>::getComputerTopology() const noexcept -> Sidechain<double>::ComputerTopology {
		return mComputerTopology;
	}

	/// @brief Sets the macro-level topology of the level detector
	///
	/// @param topology - The macro-level topology of the level detector
	auto Sidechain<double>::setDetectorTopology(DetectorTopology topology) noexcept -> void {
		mDetectorTopology = topology;
	}

	/// @brief Returns the macro-level topology of the level detector
	///
	/// @return - The macro-level topology of the level detector
	auto
	Sidechain<double>::getDetectorTopology() const noexcept -> Sidechain<double>::DetectorTopology {
		return mDetectorTopology;
	}

	/// @brief Replaces the `GainReduction` adjuster with the given one
	///
	/// @param reduction - The new `GainReduction`
	auto Sidechain<double>::setGainReductionProcessor(
		GainReduction<double, double, double>&& reduction) noexcept -> void {
		mGainReductionProcessor = std::move(reduction);
	}

	auto Sidechain<double>::processFeedForwardReturnToZero(double input) noexcept -> double {
		double rectified = math::fabs(input);
		double detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
		double outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB = outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<double>::processFeedForwardReturnToThreshold(double input) noexcept -> double {
		double rectified = math::fabs(input);
		double thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
		double detectedDB = math::Decibels::linearToDecibels(
			mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
		double outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB = outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<double>::processFeedForwardAlternateReturnToThreshold(double input) noexcept
		-> double {
		double rectified = math::fabs(input);
		double rectifiedDB = math::Decibels::linearToDecibels(rectified);
		double gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(gainReduction);
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<double>::processFeedBackReturnToZero(double input) noexcept -> double {
		double rectified = math::fabs(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
		double detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
		double outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB += outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto Sidechain<double>::processFeedBackReturnToThreshold(double input) noexcept -> double {
		double rectified = math::fabs(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
		double thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
		double detectedDB = math::Decibels::linearToDecibels(
			mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
		double outputDB = mGainComputer->process(detectedDB);
		mGainReductionDB += outputDB - detectedDB;
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	auto
	Sidechain<double>::processFeedBackAlternateReturnToThreshold(double input) noexcept -> double {
		double rectified = math::fabs(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
		double rectifiedDB = math::Decibels::linearToDecibels(rectified);
		double gainReduction = mGainReductionDB + mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(gainReduction);
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}
} // namespace apex::dsp
