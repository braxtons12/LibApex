#include "Sidechain.h"

namespace apex {
	namespace dsp {
		float Sidechain<float>::process(float input) noexcept {
			float x = 0.0f;
			switch(mComputerTopology) {
				case FeedForward: {
									  switch(mDetectorTopology) {
										  case ReturnToZero:
											  x = processFeedForwardReturnToZero(input);
											  break;
										  case ReturnToThreshold:
											  x = processFeedForwardReturnToThreshold(input);
											  break;
										  case AlternateReturnToThreshold:
											  x = processFeedForwardAlternateReturnToThreshold(input);
											  break;
									  }
								  }
								  break;
				case FeedBack: {
								   switch(mDetectorTopology) {
									   case ReturnToZero:
										   x = processFeedBackReturnToZero(input);
										   break;
									   case ReturnToThreshold:
										   x = processFeedBackReturnToThreshold(input);
										   break;
									   case AlternateReturnToThreshold:
										   x = processFeedBackAlternateReturnToThreshold(input);
										   break;
								   }
							   }
							   break;
			}
			return mGainReductionProcessor.adjustedGainReduction(x);
		}

		void Sidechain<float>::setAttackTime(float attackMS) noexcept {
			mState.setAttack(attackMS * MS_TO_SECS_MULT);
		}

		float Sidechain<float>::getAttackTime() const noexcept {
			return mState.getAttack();
		}

		void Sidechain<float>::setReleaseTime(float releaseMS) noexcept {
			mState.setRelease(releaseMS * MS_TO_SECS_MULT);
		}

		float Sidechain<float>::getReleaseTime() const noexcept {
			return mState.getRelease();
		}

		void Sidechain<float>::setRatio(float ratio) noexcept {
			mState.setRatio(ratio);
		}

		float Sidechain<float>::getRatio() const noexcept {
			return mState.getRatio();
		}

		void Sidechain<float>::setThreshold(float threshold) noexcept {
			mState.setThreshold(threshold);
		}

		float Sidechain<float>::getThreshold() const noexcept {
			return mState.getThreshold();
		}

		void Sidechain<float>::setKneeWidth(float kneeWidth) noexcept {
			mState.setKneeWidth(kneeWidth);
		}

		float Sidechain<float>::getKneeWidth() const noexcept {
			return mState.getKneeWidth();
		}

		void Sidechain<float>::setDynamicsType(DynamicsType type) noexcept {
			mDynamicsType = type;
			if(mDynamicsType == DynamicsType::Compressor) mGainComputer = &mCompressorComputer;
			else mGainComputer = &mExpanderComputer;
		}

		Sidechain<float>::DynamicsType Sidechain<float>::getDynamicsType() const noexcept {
			return mDynamicsType;
		}

		void Sidechain<float>::setSampleRate(size_t sampleRate) noexcept {
			mState.setSampleRate(sampleRate);
		}

		size_t Sidechain<float>::getSampleRate() const noexcept {
			return mState.getSampleRate();
		}

		float Sidechain<float>::getCurrentGainReduction() const noexcept {
			return mGainReductionDB;
		}

		void Sidechain<float>::setLevelDetectorType(LevelDetector<float>::DetectorType type) noexcept {
			mDetectorType = type;
			mLevelDetector = LevelDetector<float>(&mState, mDetectorType);
		}

		LevelDetector<float>::DetectorType Sidechain<float>::getLevelDetectorType() const noexcept {
			return mDetectorType;
		}

		void Sidechain<float>::setComputerTopology(ComputerTopology topology) noexcept {
			mComputerTopology = topology;
		}

		Sidechain<float>::ComputerTopology Sidechain<float>::getComputerTopology() const noexcept {
			return mComputerTopology;
		}

		void Sidechain<float>::setDetectorTopology(DetectorTopology topology) noexcept {
			mDetectorTopology = topology;
		}

		Sidechain<float>::DetectorTopology Sidechain<float>::getDetectorTopology() const noexcept {
			return mDetectorTopology;
		}

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

		double Sidechain<double>::process(double input) noexcept {
			double x = 0.0;
			switch(mComputerTopology) {
				case FeedForward: {
									  switch(mDetectorTopology) {
										  case ReturnToZero:
											  x = processFeedForwardReturnToZero(input);
											  break;
										  case ReturnToThreshold:
											  x = processFeedForwardReturnToThreshold(input);
											  break;
										  case AlternateReturnToThreshold:
											  x = processFeedForwardAlternateReturnToThreshold(input);
											  break;
									  }
								  }
								  break;
				case FeedBack: {
								   switch(mDetectorTopology) {
									   case ReturnToZero:
										   x = processFeedBackReturnToZero(input);
										   break;
									   case ReturnToThreshold:
										   x =  processFeedBackReturnToThreshold(input);
										   break;
									   case AlternateReturnToThreshold:
										   x = processFeedBackAlternateReturnToThreshold(input);
										   break;
								   }
							   }
							   break;
			}
			return mGainReductionProcessor.adjustedGainReduction(x);
		}

		void Sidechain<double>::setAttackTime(double attackMS) noexcept {
			mState.setAttack(attackMS * MS_TO_SECS_MULT);
		}

		double Sidechain<double>::getAttackTime() const noexcept {
			return mState.getAttack();
		}

		void Sidechain<double>::setReleaseTime(double releaseMS) noexcept {
			mState.setRelease(releaseMS * MS_TO_SECS_MULT);
		}

		double Sidechain<double>::getReleaseTime() const noexcept {
			return mState.getRelease();
		}

		void Sidechain<double>::setRatio(double ratio) noexcept {
			mState.setRatio(ratio);
		}

		double Sidechain<double>::getRatio() const noexcept {
			return mState.getRatio();
		}

		void Sidechain<double>::setThreshold(double threshold) noexcept {
			mState.setThreshold(threshold);
		}

		double Sidechain<double>::getThreshold() const noexcept {
			return mState.getThreshold();
		}

		void Sidechain<double>::setKneeWidth(double kneeWidth) noexcept {
			mState.setKneeWidth(kneeWidth);
		}

		double Sidechain<double>::getKneeWidth() const noexcept {
			return mState.getKneeWidth();
		}

		void Sidechain<double>::setDynamicsType(DynamicsType type) noexcept {
			mDynamicsType = type;
			if(mDynamicsType == DynamicsType::Compressor) mGainComputer = &mCompressorComputer;
			else mGainComputer = &mExpanderComputer;
		}

		Sidechain<double>::DynamicsType Sidechain<double>::getDynamicsType() const noexcept {
			return mDynamicsType;
		}

		void Sidechain<double>::setSampleRate(size_t sampleRate) noexcept {
			mState.setSampleRate(sampleRate);
		}

		size_t Sidechain<double>::getSampleRate() const noexcept {
			return mState.getSampleRate();
		}

		double Sidechain<double>::getCurrentGainReduction() const noexcept {
			return mGainReductionDB;
		}

		void Sidechain<double>::setLevelDetectorType(LevelDetector<double>::DetectorType type) noexcept {
			mDetectorType = type;
			mLevelDetector = LevelDetector<double>(&mState, mDetectorType);
		}

		LevelDetector<double>::DetectorType Sidechain<double>::getLevelDetectorType() const noexcept {
			return mDetectorType;
		}

		void Sidechain<double>::setComputerTopology(ComputerTopology topology) noexcept {
			mComputerTopology = topology;
		}

		Sidechain<double>::ComputerTopology Sidechain<double>::getComputerTopology() const noexcept {
			return mComputerTopology;
		}

		void Sidechain<double>::setDetectorTopology(DetectorTopology topology) noexcept {
			mDetectorTopology = topology;
		}

		Sidechain<double>::DetectorTopology Sidechain<double>::getDetectorTopology() const noexcept {
			return mDetectorTopology;
		}

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
