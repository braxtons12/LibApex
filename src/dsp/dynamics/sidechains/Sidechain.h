#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../leveldetectors/LevelDetector.h"
#include "../gainreductions/GainReduction.h"
#include "../gaincomputers/GainComputer.h"
#include "../gaincomputers/GainComputerCompressor.h"
#include "../gaincomputers/GainComputerExpander.h"

namespace apex {
	namespace dsp {
		template<typename T>
			class Sidechain {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
			};

		template<>
			class Sidechain<float> {
				public:
					enum DynamicsType {
						Compressor = 0,
						Expander
					};

					enum ComputerTopology {
						FeedForward = 0,
						FeedBack
					};

					enum DetectorTopology {
						ReturnToZero = 0,
						ReturnToThreshold,
						AlternateReturnToThreshold
					};

					Sidechain() noexcept = default;
					Sidechain(Sidechain<float>&& sidechain) noexcept = default;
					virtual ~Sidechain() noexcept = default;

					virtual float process(float input) noexcept;

					virtual void setAttackTime(float attackMS) noexcept;
					virtual float getAttackTime() const noexcept;
					virtual void setReleaseTime(float releaseMS) noexcept;
					virtual float getReleaseTime() const noexcept;
					virtual void setRatio(float ratio) noexcept;
					virtual float getRatio() const noexcept;
					virtual void setThreshold(float threshold) noexcept;
					virtual float getThreshold() const noexcept;
					virtual void setKneeWidth(float kneeWidth) noexcept;
					virtual float getKneeWidth() const noexcept;
					virtual void setDynamicsType(DynamicsType type) noexcept;
					virtual DynamicsType getDynamicsType() const noexcept;
					virtual void setSampleRate(size_t sampleRate) noexcept;
					virtual size_t getSampleRate() const noexcept;
					virtual float getCurrentGainReduction() const noexcept;

					void setLevelDetectorType(LevelDetector<float>::DetectorType type) noexcept;
					LevelDetector<float>::DetectorType getLevelDetectorType() const noexcept;

					void setComputerTopology(ComputerTopology topology) noexcept;
					ComputerTopology getComputerTopology() const noexcept;

					void setDetectorTopology(DetectorTopology topology) noexcept;
					DetectorTopology getDetectorTopology() const noexcept;

					void setGainReductionProcessor(GainReduction<float>&& reduction) noexcept;

				protected:
					float mAttackMS = 10.0f;
					float mReleaseMS = 50.0f;
					size_t mSampleRate = 44100;
					float mRatio = 1.1f;
					float mThreshold = -12.0f;
					float mKneeWidth = 6.0f;
					float mGainReductionDB = 0.0f;
					DynamicsType mDynamicsType = DynamicsType::Compressor;
					ComputerTopology mComputerTopology = ComputerTopology::FeedBack;
					DetectorTopology mDetectorTopology =
						DetectorTopology::AlternateReturnToThreshold;
					LevelDetector<float>::DetectorType mDetectorType =
						LevelDetector<float>::DetectorType::Decoupled;
					LevelDetector<float> mLevelDetector =
						LevelDetector<float>(mAttackMS, mReleaseMS, mSampleRate, mDetectorType);
					GainReduction<float> mGainReductionProcessor = GainReduction<float>();
					GainComputerExpander<float> mExpanderComputer = GainComputerExpander<float>();
					GainComputerCompressor<float> mCompressorComputer =
						GainComputerCompressor<float>();
					GainComputer<float>* mGainComputer = &mCompressorComputer;

					float processFeedForwardReturnToZero(float input) noexcept;
					float processFeedForwardReturnToThreshold(float input) noexcept;
					float processFeedForwardAlternateReturnToThreshold(float input) noexcept;

					float processFeedBackReturnToZero(float input) noexcept;
					float processFeedBackReturnToThreshold(float input) noexcept;
					float processFeedBackAlternateReturnToThreshold(float input) noexcept;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
			};

		template<>
			class Sidechain<double> {
				public:
					enum DynamicsType {
						Compressor = 0,
						Expander
					};

					enum ComputerTopology {
						FeedForward = 0,
						FeedBack
					};

					enum DetectorTopology {
						ReturnToZero = 0,
						ReturnToThreshold,
						AlternateReturnToThreshold
					};

					Sidechain() noexcept = default;
					Sidechain(Sidechain<double>&& sidechain) noexcept = default;
					virtual ~Sidechain() noexcept = default;

					virtual double process(double input) noexcept;

					virtual void setAttackTime(double attackMS) noexcept;
					virtual double getAttackTime() const noexcept;
					virtual void setReleaseTime(double releaseMS) noexcept;
					virtual double getReleaseTime() const noexcept;
					virtual void setRatio(double ratio) noexcept;
					virtual double getRatio() const noexcept;
					virtual void setThreshold(double threshold) noexcept;
					virtual double getThreshold() const noexcept;
					virtual void setKneeWidth(double kneeWidth) noexcept;
					virtual double getKneeWidth() const noexcept;
					virtual void setDynamicsType(DynamicsType type) noexcept;
					virtual DynamicsType getDynamicsType() const noexcept;
					virtual void setSampleRate(size_t sampleRate) noexcept;
					virtual size_t getSampleRate() const noexcept;
					virtual double getCurrentGainReduction() const noexcept;

					void setLevelDetectorType(LevelDetector<double>::DetectorType type) noexcept;
					LevelDetector<double>::DetectorType getLevelDetectorType() const noexcept;

					void setComputerTopology(ComputerTopology topology) noexcept;
					ComputerTopology getComputerTopology() const noexcept;

					void setDetectorTopology(DetectorTopology topology) noexcept;
					DetectorTopology getDetectorTopology() const noexcept;

					void setGainReductionProcessor(GainReduction<double>&& reduction) noexcept;

				protected:
					double mAttackMS = 10.0;
					double mReleaseMS = 50.0;
					size_t mSampleRate = 44100;
					double mRatio = 1.1;
					double mThreshold = -12.0;
					double mKneeWidth = 6.0;
					double mGainReductionDB = 0.0;
					DynamicsType mDynamicsType = DynamicsType::Compressor;
					ComputerTopology mComputerTopology = ComputerTopology::FeedBack;
					DetectorTopology mDetectorTopology =
						DetectorTopology::AlternateReturnToThreshold;
					LevelDetector<double>::DetectorType mDetectorType =
						LevelDetector<double>::DetectorType::Decoupled;
					LevelDetector<double> mLevelDetector =
						LevelDetector<double>(mAttackMS, mReleaseMS, mSampleRate, mDetectorType);
					GainReduction<double> mGainReductionProcessor = GainReduction<double>();
					GainComputerExpander<double> mExpanderComputer = GainComputerExpander<double>();
					GainComputerCompressor<double> mCompressorComputer =
						GainComputerCompressor<double>();
					GainComputer<double>* mGainComputer = &mCompressorComputer;

					double processFeedForwardReturnToZero(double input) noexcept;
					double processFeedForwardReturnToThreshold(double input) noexcept;
					double processFeedForwardAlternateReturnToThreshold(double input) noexcept;

					double processFeedBackReturnToZero(double input) noexcept;
					double processFeedBackReturnToThreshold(double input) noexcept;
					double processFeedBackAlternateReturnToThreshold(double input) noexcept;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
			};
	}
}
