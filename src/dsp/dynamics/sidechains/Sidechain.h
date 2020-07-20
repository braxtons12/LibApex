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
				private:
					typedef typename apex::dsp::DynamicsState<float, float, float>::Field Field;
					typedef typename apex::dsp::DynamicsState<float, float, float> State;

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

					void setGainReductionProcessor(GainReduction<float, float, float>&& reduction) noexcept;

				protected:
					static const constexpr float DEFAULT_ATTACK_SECONDS = 0.01f;
					static const constexpr float DEFAULT_RELEASE_SECONDS = 0.05f;
					static const constexpr size_t DEFAULT_SAMPLE_RATE = 44100;
					static const constexpr float DEFAULT_RATIO = 1.1f;
					static const constexpr float DEFAULT_THRESHOLD = -12.0f;
					static const constexpr float DEFAULT_KNEE_WIDTH = 6.0f;
					static const constexpr float MS_TO_SECS_MULT = 0.001f;
					State mState = State(
						DEFAULT_ATTACK_SECONDS,
						DEFAULT_RELEASE_SECONDS,
						DEFAULT_RATIO,
						DEFAULT_THRESHOLD,
						DEFAULT_KNEE_WIDTH,
						DEFAULT_SAMPLE_RATE
					);
					float mGainReductionDB = 0.0f;

					DynamicsType mDynamicsType = DynamicsType::Compressor;
					ComputerTopology mComputerTopology = ComputerTopology::FeedBack;
					DetectorTopology mDetectorTopology =
						DetectorTopology::AlternateReturnToThreshold;
					LevelDetector<float>::DetectorType mDetectorType =
						LevelDetector<float>::DetectorType::Decoupled;
					LevelDetector<float> mLevelDetector =
						LevelDetector<float>(&mState, mDetectorType);
					GainReduction<float, float, float> mGainReductionProcessor = 
						GainReduction<float, float, float>(&mState);
					GainComputerExpander<float, float, float> mExpanderComputer =
						GainComputerExpander<float, float, float>(&mState);
					GainComputerCompressor<float, float, float> mCompressorComputer =
						GainComputerCompressor<float, float, float>(&mState);
					GainComputer<float, float, float>* mGainComputer = &mCompressorComputer;

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
				private:
					typedef typename apex::dsp::DynamicsState<double, double, double>::Field Field;
					typedef typename apex::dsp::DynamicsState<double, double, double> State;

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

					void setGainReductionProcessor(GainReduction<double, double, double>&& reduction) noexcept;

				protected:
					static const constexpr double DEFAULT_ATTACK_SECONDS = 0.01;
					static const constexpr double DEFAULT_RELEASE_SECONDS = 0.05;
					static const constexpr size_t DEFAULT_SAMPLE_RATE = 44100;
					static const constexpr double DEFAULT_RATIO = 1.1;
					static const constexpr double DEFAULT_THRESHOLD = -12.0;
					static const constexpr double DEFAULT_KNEE_WIDTH = 6.0;
					static const constexpr double MS_TO_SECS_MULT = 0.001;
					State mState = State(
						DEFAULT_ATTACK_SECONDS,
						DEFAULT_RELEASE_SECONDS,
						DEFAULT_RATIO,
						DEFAULT_THRESHOLD,
						DEFAULT_KNEE_WIDTH,
						DEFAULT_SAMPLE_RATE
					);
					double mGainReductionDB = 0.0;

					DynamicsType mDynamicsType = DynamicsType::Compressor;
					ComputerTopology mComputerTopology = ComputerTopology::FeedBack;
					DetectorTopology mDetectorTopology =
						DetectorTopology::AlternateReturnToThreshold;
					LevelDetector<double>::DetectorType mDetectorType =
						LevelDetector<double>::DetectorType::Decoupled;
					LevelDetector<double> mLevelDetector =
						LevelDetector<double>(&mState, mDetectorType);
					GainReduction<double, double, double> mGainReductionProcessor = 
						GainReduction<double, double, double>(&mState);
					GainComputerExpander<double, double, double> mExpanderComputer =
						GainComputerExpander<double, double, double>(&mState);
					GainComputerCompressor<double, double, double> mCompressorComputer =
						GainComputerCompressor<double, double, double>(&mState);
					GainComputer<double, double, double>* mGainComputer = &mCompressorComputer;

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
