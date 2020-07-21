#pragma once

#include <type_traits>
#include <utility>

#include "../DynamicsState.h"
#include "../../../base/StandardIncludes.h"
#include "../leveldetectors/LevelDetector.h"
#include "../gainreductions/GainReduction.h"
#include "../gaincomputers/GainComputer.h"
#include "../gaincomputers/GainComputerCompressor.h"
#include "../gaincomputers/GainComputerExpander.h"

namespace apex {
	namespace dsp {
		/// @brief Base dynamics processor sidechain. This is the signal chain that determines the
		/// gain reduction that the containing dynamics processor should apply to the input signal
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class Sidechain {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
			};

		/// @brief Base dynamics processor sidechain. This is the signal chain that determines the
		/// gain reduction that the containing dynamics processor should apply to the input signal
		template<>
			class Sidechain<float> {
				private:
					using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
					using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

				public:
					/// @brief The type of dynamics processor this is used in
					enum class DynamicsType {
						Compressor,
						Expander
					};

					/// @brief The macro-level topology to use for the gain computer (the component
					/// that calculates the target gain reduction)
					enum class ComputerTopology {
						FeedForward,
						FeedBack
					};

					/// @brief The macro-level topology to use for the level detector (the component
					/// that performs attack and release envelope modulation)
					enum class DetectorTopology {
						ReturnToZero,
						ReturnToThreshold,
						AlternateReturnToThreshold
					};

					/// @brief Constructs a `Sidechain` with the following defaults:
					/// * attack: 10ms
					/// * release: 50ms
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					/// * sampleRate 44100Hz
					Sidechain() noexcept = default;

					/// @brief Move constructs the given `Sidechain`
					///
					/// @param sidechain - The `Sidechain` to move
					Sidechain(Sidechain<float>&& sidechain) noexcept = default;
					virtual ~Sidechain() noexcept = default;

					/// @brief Calculates the target gain reduction to apply to the input value
					///
					/// @param input - The input value to calculate gain reduction for
					///
					/// @return - The target gain reduction
					virtual float process(float input) noexcept;

					/// @brief Sets the attack to the given value
					///
					/// @param attackMS - The attack time, in milliseconds
					void setAttackTime(float attackMS) noexcept;

					/// @brief Returns the attack
					///
					/// @return - The attack time, in milliseconds
					float getAttackTime() const noexcept;

					/// @brief Sets the release to the given value
					///
					/// @param releaseMS - The release time, in milliseconds
					void setReleaseTime(float releaseMS) noexcept;

					/// @brief Returns the release
					///
					/// @return - The release time, in milliseconds
					float getReleaseTime() const noexcept;

					/// @brief Sets the Ratio
					///
					/// @param ratio - The ratio
					virtual void setRatio(float ratio) noexcept;

					/// @brief Returns the Ratio
					///
					/// @return - The ratio
					virtual float getRatio() const noexcept;

					/// @brief Sets the Threshold
					///
					/// @param threshold - The threshold, in decibels
					virtual void setThreshold(float threshold) noexcept;

					/// @brief Returns the Threshold
					///
					/// @return - The threshold, in decibels
					virtual float getThreshold() const noexcept;

					/// @brief Sets the KneeWidth
					///
					/// @param kneeWidth - The knee width, in decibels
					virtual void setKneeWidth(float kneeWidth) noexcept;

					/// @brief Returns the KneeWidth
					///
					/// @return - The knee width, in decibels
					virtual float getKneeWidth() const noexcept;

					/// @brief Sets the DynamicsType
					///
					/// @param type - The dynamics type
					virtual void setDynamicsType(DynamicsType type) noexcept;

					/// @brief Returns the DynamicsType
					///
					/// @return - The dynamics type
					virtual DynamicsType getDynamicsType() const noexcept;

					/// @brief Sets the SampleRate
					///
					/// @param sampleRate - The sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the SampleRate
					///
					/// @return - The sample rate, in Hertz
					virtual size_t getSampleRate() const noexcept;

					/// @brief Returns the most recently calculated gain reduction value
					///
					/// @return - The most recently calculated gain reduction value (linear)
					virtual float getCurrentGainReduction() const noexcept;

					/// @brief Sets the type of the `LevelDetector` to the given value
					///
					/// @param type - The type of the `LevelDetector`
					void setLevelDetectorType(LevelDetector<float>::DetectorType type) noexcept;

					/// @brief Returns the type of the `LevelDetector`
					///
					/// @return - The type of the `LevelDetector`
					LevelDetector<float>::DetectorType getLevelDetectorType() const noexcept;

					/// @brief Sets the macro-level topology of the gain reduction computer
					///
					/// @param topology - The macro-level topology of the gain reduction computer
					virtual void setComputerTopology(ComputerTopology topology) noexcept;

					/// @brief Returns the macro-level topology of the gain reduction computer
					///
					/// @return - The macro-level topology of the gain reduction computer
					ComputerTopology getComputerTopology() const noexcept;

					/// @brief Sets the macro-level topology of the level detector
					///
					/// @param topology - The macro-level topology of the level detector
					virtual void setDetectorTopology(DetectorTopology topology) noexcept;

					/// @brief Returns the macro-level topology of the level detector
					///
					/// @return - The macro-level topology of the level detector
					DetectorTopology getDetectorTopology() const noexcept;

					/// @brief Replaces the `GainReduction` adjuster with the given one
					///
					/// @param reduction - The new `GainReduction`
					virtual void setGainReductionProcessor(GainReduction<float, float, float>&& reduction) noexcept;

					Sidechain<float>& operator=(Sidechain<float>&& sidechain) noexcept = default;

				protected:
					static const constexpr float DEFAULT_ATTACK_SECONDS = 0.01f;
					static const constexpr float DEFAULT_RELEASE_SECONDS = 0.05f;
					static const constexpr size_t DEFAULT_SAMPLE_RATE = 44100;
					static const constexpr float DEFAULT_RATIO = 1.1f;
					static const constexpr float DEFAULT_THRESHOLD = -12.0f;
					static const constexpr float DEFAULT_KNEE_WIDTH = 6.0f;
					static const constexpr float MS_TO_SECS_MULT = 0.001f;
					DynamicsState mState = DynamicsState(
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

					virtual float processFeedForwardReturnToZero(float input) noexcept;
					virtual float processFeedForwardReturnToThreshold(float input) noexcept;
					virtual float processFeedForwardAlternateReturnToThreshold(float input) noexcept;

					virtual float processFeedBackReturnToZero(float input) noexcept;
					virtual float processFeedBackReturnToThreshold(float input) noexcept;
					virtual float processFeedBackAlternateReturnToThreshold(float input) noexcept;

				private:
					LevelDetector<float> mLevelDetector =
						LevelDetector<float>(&mState, mDetectorType);
					GainReduction<float, float, float> mGainReductionProcessor =
						GainReduction<float, float, float>(&mState);
					GainComputerExpander<float, float, float> mExpanderComputer =
						GainComputerExpander<float, float, float>(&mState);
					GainComputerCompressor<float, float, float> mCompressorComputer =
						GainComputerCompressor<float, float, float>(&mState);
					GainComputer<float, float, float>* mGainComputer = &mCompressorComputer;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
			};

		/// @brief Base dynamics processor sidechain. This is the signal chain that determines the
		/// gain reduction that the containing dynamics processor should apply to the input signal
		template<>
			class Sidechain<double> {
				private:
					using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
					using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

				public:
					/// @brief The type of dynamics processor this is used in
					enum DynamicsType {
						Compressor = 0,
						Expander
					};

					/// @brief The macro-level topology to use for the gain computer (the component
					/// that calculates the target gain reduction)
					enum ComputerTopology {
						FeedForward = 0,
						FeedBack
					};

					/// @brief The macro-level topology to use for the level detector (the component
					/// that performs attack and release envelope modulation)
					enum DetectorTopology {
						ReturnToZero = 0,
						ReturnToThreshold,
						AlternateReturnToThreshold
					};

					/// @brief Constructs a `Sidechain` with the following defaults:
					/// * attack: 10ms
					/// * release: 50ms
					/// * ratio: 1.1
					/// * threshold: -12dB
					/// * knee width: 6dB
					/// * sampleRate 44100Hz
					Sidechain() noexcept = default;

					/// @brief Move constructs the given `Sidechain`
					///
					/// @param sidechain - The `Sidechain` to move
					Sidechain(Sidechain<double>&& sidechain) noexcept = default;
					virtual ~Sidechain() noexcept = default;

					/// @brief Calculates the target gain reduction to apply to the input value
					///
					/// @param input - The input value to calculate gain reduction for
					///
					/// @return - The target gain reduction
					virtual double process(double input) noexcept;

					/// @brief Sets the attack to the given value
					///
					/// @param attackMS - The attack time, in milliseconds
					void setAttackTime(double attackMS) noexcept;

					/// @brief Returns the attack
					///
					/// @return - The attack time, in milliseconds
					double getAttackTime() const noexcept;

					/// @brief Sets the release to the given value
					///
					/// @param releaseMS - The release time, in milliseconds
					void setReleaseTime(double releaseMS) noexcept;

					/// @brief Returns the release
					///
					/// @return - The release time, in milliseconds
					double getReleaseTime() const noexcept;

					/// @brief Sets the Ratio
					///
					/// @param ratio - The ratio
					virtual void setRatio(double ratio) noexcept;

					/// @brief Returns the Ratio
					///
					/// @return - The ratio
					virtual double getRatio() const noexcept;

					/// @brief Sets the Threshold
					///
					/// @param threshold - The threshold, in decibels
					virtual void setThreshold(double threshold) noexcept;

					/// @brief Returns the Threshold
					///
					/// @return - The threshold, in decibels
					virtual double getThreshold() const noexcept;

					/// @brief Sets the KneeWidth
					///
					/// @param kneeWidth - The knee width, in decibels
					virtual void setKneeWidth(double kneeWidth) noexcept;

					/// @brief Returns the KneeWidth
					///
					/// @return - The knee width, in decibels
					virtual double getKneeWidth() const noexcept;

					/// @brief Sets the DynamicsType
					///
					/// @param type - The dynamics type
					virtual void setDynamicsType(DynamicsType type) noexcept;

					/// @brief Returns the DynamicsType
					///
					/// @return - The dynamics type
					virtual DynamicsType getDynamicsType() const noexcept;

					/// @brief Sets the SampleRate
					///
					/// @param sampleRate - The sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the SampleRate
					///
					/// @return - The sample rate, in Hertz
					virtual size_t getSampleRate() const noexcept;

					/// @brief Returns the most recently calculated gain reduction value
					///
					/// @return - The most recently calculated gain reduction value (linear)
					virtual double getCurrentGainReduction() const noexcept;

					/// @brief Sets the type of the `LevelDetector` to the given value
					///
					/// @param type - The type of the `LevelDetector`
					void setLevelDetectorType(LevelDetector<double>::DetectorType type) noexcept;

					/// @brief Returns the type of the `LevelDetector`
					///
					/// @return - The type of the `LevelDetector`
					LevelDetector<double>::DetectorType getLevelDetectorType() const noexcept;

					/// @brief Sets the macro-level topology of the gain reduction computer
					///
					/// @param topology - The macro-level topology of the gain reduction computer
					virtual void setComputerTopology(ComputerTopology topology) noexcept;

					/// @brief Returns the macro-level topology of the gain reduction computer
					///
					/// @return - The macro-level topology of the gain reduction computer
					ComputerTopology getComputerTopology() const noexcept;

					/// @brief Sets the macro-level topology of the level detector
					///
					/// @param topology - The macro-level topology of the level detector
					virtual void setDetectorTopology(DetectorTopology topology) noexcept;

					/// @brief Returns the macro-level topology of the level detector
					///
					/// @return - The macro-level topology of the level detector
					DetectorTopology getDetectorTopology() const noexcept;

					/// @brief Replaces the `GainReduction` adjuster with the given one
					///
					/// @param reduction - The new `GainReduction`
					virtual void setGainReductionProcessor(GainReduction<double, double, double>&& reduction) noexcept;

					Sidechain<double>& operator=(Sidechain<double>&& sidechain) noexcept = default;

				protected:
					static const constexpr double DEFAULT_ATTACK_SECONDS = 0.01;
					static const constexpr double DEFAULT_RELEASE_SECONDS = 0.05;
					static const constexpr size_t DEFAULT_SAMPLE_RATE = 44100;
					static const constexpr double DEFAULT_RATIO = 1.1;
					static const constexpr double DEFAULT_THRESHOLD = -12.0;
					static const constexpr double DEFAULT_KNEE_WIDTH = 6.0;
					static const constexpr double MS_TO_SECS_MULT = 0.001;
					DynamicsState mState = DynamicsState(
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

					virtual double processFeedForwardReturnToZero(double input) noexcept;
					virtual double processFeedForwardReturnToThreshold(double input) noexcept;
					virtual double processFeedForwardAlternateReturnToThreshold(double input) noexcept;

					virtual double processFeedBackReturnToZero(double input) noexcept;
					virtual double processFeedBackReturnToThreshold(double input) noexcept;
					virtual double processFeedBackAlternateReturnToThreshold(double input) noexcept;

				private:
					LevelDetector<double> mLevelDetector =
						LevelDetector<double>(&mState, mDetectorType);
					GainReduction<double, double, double> mGainReductionProcessor =
						GainReduction<double, double, double>(&mState);
					GainComputerExpander<double, double, double> mExpanderComputer =
						GainComputerExpander<double, double, double>(&mState);
					GainComputerCompressor<double, double, double> mCompressorComputer =
						GainComputerCompressor<double, double, double>(&mState);
					GainComputer<double, double, double>* mGainComputer = &mCompressorComputer;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
			};
	}
}
