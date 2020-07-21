#include "Sidechain.h"

namespace apex {
	namespace dsp {
		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		float Sidechain<float>::process(float input) noexcept {
			float x = 0.0f;
			switch(mComputerTopology) {
				case ComputerTopology::FeedForward: {
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
				case ComputerTopology::FeedBack: {
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
		void Sidechain<float>::setAttackTime(float attackMS) noexcept {
			mState.setAttack(attackMS * MS_TO_SECS_MULT);
		}

		/// @brief Returns the attack
		///
		/// @return - The attack time, in milliseconds
		float Sidechain<float>::getAttackTime() const noexcept {
			return mState.getAttack() / MS_TO_SECS_MULT;
		}

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time, in milliseconds
		void Sidechain<float>::setReleaseTime(float releaseMS) noexcept {
			mState.setRelease(releaseMS * MS_TO_SECS_MULT);
		}

		/// @brief Returns the release
		///
		/// @return - The release time, in milliseconds
		float Sidechain<float>::getReleaseTime() const noexcept {
			return mState.getRelease() / MS_TO_SECS_MULT;
		}

		/// @brief Sets the Ratio
		///
		/// @param ratio - The ratio
		void Sidechain<float>::setRatio(float ratio) noexcept {
			mState.setRatio(ratio);
		}

		/// @brief Returns the Ratio
		///
		/// @return - The ratio
		float Sidechain<float>::getRatio() const noexcept {
			return mState.getRatio();
		}

		/// @brief Sets the Threshold
		///
		/// @param threshold - The threshold, in decibels
		void Sidechain<float>::setThreshold(float threshold) noexcept {
			mState.setThreshold(threshold);
		}

		/// @brief Returns the Threshold
		///
		/// @return - The threshold, in decibels
		float Sidechain<float>::getThreshold() const noexcept {
			return mState.getThreshold();
		}

		/// @brief Sets the KneeWidth
		///
		/// @param kneeWidth - The knee width, in decibels
		void Sidechain<float>::setKneeWidth(float kneeWidth) noexcept {
			mState.setKneeWidth(kneeWidth);
		}

		/// @brief Returns the KneeWidth
		///
		/// @return - The knee width, in decibels
		float Sidechain<float>::getKneeWidth() const noexcept {
			return mState.getKneeWidth();
		}

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		void Sidechain<float>::setDynamicsType(DynamicsType type) noexcept {
			mDynamicsType = type;
			if(mDynamicsType == DynamicsType::Compressor) mGainComputer = &mCompressorComputer;
			else mGainComputer = &mExpanderComputer;
		}

		/// @brief Returns the DynamicsType
		///
		/// @return - The dynamics type
		Sidechain<float>::DynamicsType Sidechain<float>::getDynamicsType() const noexcept {
			return mDynamicsType;
		}

		/// @brief Sets the SampleRate
		///
		/// @param sampleRate - The sample rate, in Hertz
		void Sidechain<float>::setSampleRate(size_t sampleRate) noexcept {
			mState.setSampleRate(sampleRate);
		}

		/// @brief Returns the SampleRate
		///
		/// @return - The sample rate, in Hertz
		size_t Sidechain<float>::getSampleRate() const noexcept {
			return mState.getSampleRate();
		}

		/// @brief Returns the most recently calculated gain reduction value
		///
		/// @return - The most recently calculated gain reduction value (linear)
		float Sidechain<float>::getCurrentGainReduction() const noexcept {
			return mGainReductionDB;
		}

		/// @brief Sets the type of the `LevelDetector` to the given value
		///
		/// @param type - The type of the `LevelDetector`
		void Sidechain<float>::setLevelDetectorType(LevelDetector<float>::DetectorType type) noexcept {
			mDetectorType = type;
			mLevelDetector = LevelDetector<float>(&mState, mDetectorType);
		}

		/// @brief Returns the type of the `LevelDetector`
		///
		/// @return - The type of the `LevelDetector`
		LevelDetector<float>::DetectorType Sidechain<float>::getLevelDetectorType() const noexcept {
			return mDetectorType;
		}

		/// @brief Sets the macro-level topology of the gain reduction computer
		///
		/// @param topology - The macro-level topology of the gain reduction computer
		void Sidechain<float>::setComputerTopology(ComputerTopology topology) noexcept {
			mComputerTopology = topology;
		}

		/// @brief Returns the macro-level topology of the gain reduction computer
		///
		/// @return - The macro-level topology of the gain reduction computer
		Sidechain<float>::ComputerTopology Sidechain<float>::getComputerTopology() const noexcept {
			return mComputerTopology;
		}

		/// @brief Sets the macro-level topology of the level detector
		///
		/// @param topology - The macro-level topology of the level detector
		void Sidechain<float>::setDetectorTopology(DetectorTopology topology) noexcept {
			mDetectorTopology = topology;
		}

		/// @brief Returns the macro-level topology of the level detector
		///
		/// @return - The macro-level topology of the level detector
		Sidechain<float>::DetectorTopology Sidechain<float>::getDetectorTopology() const noexcept {
			return mDetectorTopology;
		}

		/// @brief Replaces the `GainReduction` adjuster with the given one
		///
		/// @param reduction - The new `GainReduction`
		void Sidechain<float>::setGainReductionProcessor(
				GainReduction<float, float, float>&& reduction) noexcept
		{
			mGainReductionProcessor = std::move(reduction);
		}

		float Sidechain<float>::processFeedForwardReturnToZero(float input) noexcept {
			float rectified = math::fabsf(input);
			float detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
			float outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB = outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		float Sidechain<float>::processFeedForwardReturnToThreshold(float input) noexcept {
			float rectified = math::fabsf(input);
			float thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
			float detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(
						rectified - thresholdLinear) + thresholdLinear);
			float outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB = outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		float Sidechain<float>::processFeedForwardAlternateReturnToThreshold(float input) noexcept {
			float rectified = math::fabsf(input);
			float rectifiedDB = math::Decibels::linearToDecibels(rectified);
			float gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
			mGainReductionDB = mLevelDetector.process(gainReduction);
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		float Sidechain<float>::processFeedBackReturnToZero(float input) noexcept {
			float rectified = math::fabsf(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
			float detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
			float outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB += outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		float Sidechain<float>::processFeedBackReturnToThreshold(float input) noexcept {
			float rectified = math::fabsf(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
			float thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
			float detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(
						rectified - thresholdLinear) + thresholdLinear);
			float outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB += outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		float Sidechain<float>::processFeedBackAlternateReturnToThreshold(float input) noexcept {
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
		double Sidechain<double>::process(double input) noexcept {
			double x = 0.0;
			switch(mComputerTopology) {
				case ComputerTopology::FeedForward: {
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
				case ComputerTopology::FeedBack: {
													 switch(mDetectorTopology) {
														 case DetectorTopology::ReturnToZero:
															 x = processFeedBackReturnToZero(input);
															 break;
														 case DetectorTopology::ReturnToThreshold:
															 x =  processFeedBackReturnToThreshold(input);
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
		void Sidechain<double>::setAttackTime(double attackMS) noexcept {
			mState.setAttack(attackMS * MS_TO_SECS_MULT);
		}

		/// @brief Returns the attack
		///
		/// @return - The attack time, in milliseconds
		double Sidechain<double>::getAttackTime() const noexcept {
			return mState.getAttack() / MS_TO_SECS_MULT;
		}

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time, in milliseconds
		void Sidechain<double>::setReleaseTime(double releaseMS) noexcept {
			mState.setRelease(releaseMS * MS_TO_SECS_MULT);
		}

		/// @brief Returns the release
		///
		/// @return - The release time, in milliseconds
		double Sidechain<double>::getReleaseTime() const noexcept {
			return mState.getRelease() / MS_TO_SECS_MULT;
		}

		/// @brief Sets the Ratio
		///
		/// @param ratio - The ratio
		void Sidechain<double>::setRatio(double ratio) noexcept {
			mState.setRatio(ratio);
		}

		/// @brief Returns the Ratio
		///
		/// @return - The ratio
		double Sidechain<double>::getRatio() const noexcept {
			return mState.getRatio();
		}

		/// @brief Sets the Threshold
		///
		/// @param threshold - The threshold, in decibels
		void Sidechain<double>::setThreshold(double threshold) noexcept {
			mState.setThreshold(threshold);
		}

		/// @brief Returns the Threshold
		///
		/// @return - The threshold, in decibels
		double Sidechain<double>::getThreshold() const noexcept {
			return mState.getThreshold();
		}

		/// @brief Sets the KneeWidth
		///
		/// @param kneeWidth - The knee width, in decibels
		void Sidechain<double>::setKneeWidth(double kneeWidth) noexcept {
			mState.setKneeWidth(kneeWidth);
		}

		/// @brief Returns the KneeWidth
		///
		/// @return - The knee width, in decibels
		double Sidechain<double>::getKneeWidth() const noexcept {
			return mState.getKneeWidth();
		}

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		void Sidechain<double>::setDynamicsType(DynamicsType type) noexcept {
			mDynamicsType = type;
			if(mDynamicsType == DynamicsType::Compressor) mGainComputer = &mCompressorComputer;
			else mGainComputer = &mExpanderComputer;
		}

		/// @brief Returns the DynamicsType
		///
		/// @return - The dynamics type
		Sidechain<double>::DynamicsType Sidechain<double>::getDynamicsType() const noexcept {
			return mDynamicsType;
		}

		/// @brief Sets the SampleRate
		///
		/// @param sampleRate - The sample rate, in Hertz
		void Sidechain<double>::setSampleRate(size_t sampleRate) noexcept {
			mState.setSampleRate(sampleRate);
		}

		/// @brief Returns the SampleRate
		///
		/// @return - The sample rate, in Hertz
		size_t Sidechain<double>::getSampleRate() const noexcept {
			return mState.getSampleRate();
		}

		/// @brief Returns the most recently calculated gain reduction value
		///
		/// @return - The most recently calculated gain reduction value (linear)
		double Sidechain<double>::getCurrentGainReduction() const noexcept {
			return mGainReductionDB;
		}

		/// @brief Sets the type of the `LevelDetector` to the given value
		///
		/// @param type - The type of the `LevelDetector`
		void Sidechain<double>::setLevelDetectorType(LevelDetector<double>::DetectorType type) noexcept {
			mDetectorType = type;
			mLevelDetector = LevelDetector<double>(&mState, mDetectorType);
		}

		/// @brief Returns the type of the `LevelDetector`
		///
		/// @return - The type of the `LevelDetector`
		LevelDetector<double>::DetectorType Sidechain<double>::getLevelDetectorType() const noexcept {
			return mDetectorType;
		}

		/// @brief Sets the macro-level topology of the gain reduction computer
		///
		/// @param topology - The macro-level topology of the gain reduction computer
		void Sidechain<double>::setComputerTopology(ComputerTopology topology) noexcept {
			mComputerTopology = topology;
		}

		/// @brief Returns the macro-level topology of the gain reduction computer
		///
		/// @return - The macro-level topology of the gain reduction computer
		Sidechain<double>::ComputerTopology Sidechain<double>::getComputerTopology() const noexcept {
			return mComputerTopology;
		}

		/// @brief Sets the macro-level topology of the level detector
		///
		/// @param topology - The macro-level topology of the level detector
		void Sidechain<double>::setDetectorTopology(DetectorTopology topology) noexcept {
			mDetectorTopology = topology;
		}

		/// @brief Returns the macro-level topology of the level detector
		///
		/// @return - The macro-level topology of the level detector
		Sidechain<double>::DetectorTopology Sidechain<double>::getDetectorTopology() const noexcept {
			return mDetectorTopology;
		}

		/// @brief Replaces the `GainReduction` adjuster with the given one
		///
		/// @param reduction - The new `GainReduction`
		void Sidechain<double>::setGainReductionProcessor(
				GainReduction<double, double, double>&& reduction) noexcept
		{
			mGainReductionProcessor = std::move(reduction);
		}

		double Sidechain<double>::processFeedForwardReturnToZero(double input) noexcept {
			double rectified = math::fabs(input);
			double detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
			double outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB = outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		double Sidechain<double>::processFeedForwardReturnToThreshold(double input) noexcept {
			double rectified = math::fabs(input);
			double thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
			double detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(
						rectified - thresholdLinear) + thresholdLinear);
			double outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB = outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		double Sidechain<double>::processFeedForwardAlternateReturnToThreshold(double input) noexcept {
			double rectified = math::fabs(input);
			double rectifiedDB = math::Decibels::linearToDecibels(rectified);
			double gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
			mGainReductionDB = mLevelDetector.process(gainReduction);
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		double Sidechain<double>::processFeedBackReturnToZero(double input) noexcept {
			double rectified = math::fabs(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
			double detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
			double outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB += outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		double Sidechain<double>::processFeedBackReturnToThreshold(double input) noexcept {
			double rectified = math::fabs(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
			double thresholdLinear = math::Decibels::decibelsToLinear(mState.getThreshold());
			double detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(
						rectified - thresholdLinear) + thresholdLinear);
			double outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB += outputDB - detectedDB;
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}

		double Sidechain<double>::processFeedBackAlternateReturnToThreshold(double input) noexcept {
			double rectified = math::fabs(input) * math::Decibels::decibelsToLinear(mGainReductionDB);
			double rectifiedDB = math::Decibels::linearToDecibels(rectified);
			double gainReduction = mGainReductionDB + mGainComputer->process(rectifiedDB) - rectifiedDB;
			mGainReductionDB = mLevelDetector.process(gainReduction);
			return math::Decibels::decibelsToLinear(mGainReductionDB);
		}
	}
}
