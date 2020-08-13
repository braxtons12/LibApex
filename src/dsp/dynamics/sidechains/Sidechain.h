#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "../gaincomputers/GainComputer.h"
#include "../gaincomputers/GainComputerCompressor.h"
#include "../gaincomputers/GainComputerExpander.h"
#include "../gainreductions/GainReduction.h"
#include "../leveldetectors/LevelDetector.h"

namespace apex::dsp {
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
		enum class DynamicsType
		{
			Compressor,
			Expander
		};

		/// @brief The macro-level topology to use for the gain computer (the component
		/// that calculates the target gain reduction)
		enum class ComputerTopology
		{
			FeedForward,
			FeedBack
		};

		/// @brief The macro-level topology to use for the level detector (the component
		/// that performs attack and release envelope modulation)
		enum class DetectorTopology
		{
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
		virtual auto process(float input) noexcept -> Decibels;

		/// @brief Sets the attack to the given value
		///
		/// @param attackMS - The attack time, in milliseconds
		virtual auto setAttackTime(float attackMS) noexcept -> void;

		/// @brief Returns the attack
		///
		/// @return - The attack time, in milliseconds
		[[nodiscard]] virtual auto getAttackTime() const noexcept -> float;

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time, in milliseconds
		virtual auto setReleaseTime(float releaseMS) noexcept -> void;

		/// @brief Returns the release
		///
		/// @return - The release time, in milliseconds
		[[nodiscard]] virtual auto getReleaseTime() const noexcept -> float;

		/// @brief Sets the Ratio
		///
		/// @param ratio - The ratio
		virtual auto setRatio(float ratio) noexcept -> void;

		/// @brief Returns the Ratio
		///
		/// @return - The ratio
		[[nodiscard]] virtual auto getRatio() const noexcept -> float;

		/// @brief Sets the Threshold
		///
		/// @param threshold - The threshold, in decibels
		virtual auto setThreshold(Decibels threshold) noexcept -> void;

		/// @brief Returns the Threshold
		///
		/// @return - The threshold, in decibels
		[[nodiscard]] virtual auto getThreshold() const noexcept -> Decibels;

		/// @brief Sets the KneeWidth
		///
		/// @param kneeWidth - The knee width, in decibels
		virtual auto setKneeWidth(Decibels kneeWidth) noexcept -> void;

		/// @brief Returns the KneeWidth
		///
		/// @return - The knee width, in decibels
		[[nodiscard]] virtual auto getKneeWidth() const noexcept -> Decibels;

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		virtual auto setDynamicsType(DynamicsType type) noexcept -> void;

		/// @brief Returns the DynamicsType
		///
		/// @return - The dynamics type
		[[nodiscard]] virtual auto getDynamicsType() const noexcept -> DynamicsType;

		/// @brief Sets the SampleRate
		///
		/// @param sampleRate - The sample rate, in Hertz
		virtual auto setSampleRate(Hertz sampleRate) noexcept -> void;

		/// @brief Returns the SampleRate
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] virtual auto getSampleRate() const noexcept -> Hertz;

		/// @brief Returns the most recently calculated gain reduction value
		///
		/// @return - The most recently calculated gain reduction value (linear)
		[[nodiscard]] virtual auto getCurrentGainReduction() const noexcept -> Decibels;

		/// @brief Sets the type of the `LevelDetector` to the given value
		///
		/// @param type - The type of the `LevelDetector`
		auto setLevelDetectorType(LevelDetector<float>::DetectorType type) noexcept -> void;

		/// @brief Returns the type of the `LevelDetector`
		///
		/// @return - The type of the `LevelDetector`
		[[nodiscard]] auto
		getLevelDetectorType() const noexcept -> LevelDetector<float>::DetectorType;

		/// @brief Sets the macro-level topology of the gain reduction computer
		///
		/// @param topology - The macro-level topology of the gain reduction computer
		virtual auto setComputerTopology(ComputerTopology topology) noexcept -> void;

		/// @brief Returns the macro-level topology of the gain reduction computer
		///
		/// @return - The macro-level topology of the gain reduction computer
		[[nodiscard]] auto getComputerTopology() const noexcept -> ComputerTopology;

		/// @brief Sets the macro-level topology of the level detector
		///
		/// @param topology - The macro-level topology of the level detector
		virtual auto setDetectorTopology(DetectorTopology topology) noexcept -> void;

		/// @brief Returns the macro-level topology of the level detector
		///
		/// @return - The macro-level topology of the level detector
		[[nodiscard]] auto getDetectorTopology() const noexcept -> DetectorTopology;

		/// @brief Replaces the `GainReduction` adjuster with the given one
		///
		/// @param reduction - The new `GainReduction`
		virtual auto
		setGainReductionProcessor(GainReduction<float, float, float>&& reduction) noexcept -> void;

		auto operator=(Sidechain<float>&& sidechain) noexcept -> Sidechain<float>& = default;

		static const constexpr float DEFAULT_ATTACK_SECONDS = 0.01F;
		static const constexpr float DEFAULT_RELEASE_SECONDS = 0.05F;
		static const constexpr Hertz DEFAULT_SAMPLE_RATE = 44100_Hz;
		static const constexpr float DEFAULT_RATIO = 1.1F;
		static const constexpr Decibels DEFAULT_THRESHOLD = -12.0_dB;
		static const constexpr Decibels DEFAULT_KNEE_WIDTH = 6.0_dB;

	  protected:
		static const constexpr float MS_TO_SECS_MULT = 0.001F;
		DynamicsState mState = DynamicsState(DEFAULT_ATTACK_SECONDS,
											 DEFAULT_RELEASE_SECONDS,
											 DEFAULT_RATIO,
											 DEFAULT_THRESHOLD,
											 DEFAULT_KNEE_WIDTH,
											 DEFAULT_SAMPLE_RATE);
		Decibels mGainReductionDB = 0.0_dB;

		DynamicsType mDynamicsType = DynamicsType::Compressor;
		ComputerTopology mComputerTopology = ComputerTopology::FeedBack;
		DetectorTopology mDetectorTopology = DetectorTopology::AlternateReturnToThreshold;
		LevelDetector<float>::DetectorType mDetectorType
			= LevelDetector<float>::DetectorType::Decoupled;

		virtual auto processFeedForwardReturnToZero(float input) noexcept -> Decibels;
		virtual auto processFeedForwardReturnToThreshold(float input) noexcept -> Decibels;
		virtual auto processFeedForwardAlternateReturnToThreshold(float input) noexcept -> Decibels;

		virtual auto processFeedBackReturnToZero(float input) noexcept -> Decibels;
		virtual auto processFeedBackReturnToThreshold(float input) noexcept -> Decibels;
		virtual auto processFeedBackAlternateReturnToThreshold(float input) noexcept -> Decibels;

	  private:
		LevelDetector<float> mLevelDetector = LevelDetector<float>(&mState, mDetectorType);
		GainReduction<float, float, float> mGainReductionProcessor
			= GainReduction<float, float, float>(&mState);
		GainComputerExpander<float, float, float> mExpanderComputer
			= GainComputerExpander<float, float, float>(&mState);
		GainComputerCompressor<float, float, float> mCompressorComputer
			= GainComputerCompressor<float, float, float>(&mState);
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
		enum DynamicsType
		{
			Compressor = 0,
			Expander
		};

		/// @brief The macro-level topology to use for the gain computer (the component
		/// that calculates the target gain reduction)
		enum ComputerTopology
		{
			FeedForward = 0,
			FeedBack
		};

		/// @brief The macro-level topology to use for the level detector (the component
		/// that performs attack and release envelope modulation)
		enum DetectorTopology
		{
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
		virtual auto process(double input) noexcept -> Decibels;

		/// @brief Sets the attack to the given value
		///
		/// @param attackMS - The attack time, in milliseconds
		virtual auto setAttackTime(double attackMS) noexcept -> void;

		/// @brief Returns the attack
		///
		/// @return - The attack time, in milliseconds
		[[nodiscard]] virtual auto getAttackTime() const noexcept -> double;

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time, in milliseconds
		virtual auto setReleaseTime(double releaseMS) noexcept -> void;

		/// @brief Returns the release
		///
		/// @return - The release time, in milliseconds
		[[nodiscard]] virtual auto getReleaseTime() const noexcept -> double;

		/// @brief Sets the Ratio
		///
		/// @param ratio - The ratio
		virtual auto setRatio(double ratio) noexcept -> void;

		/// @brief Returns the Ratio
		///
		/// @return - The ratio
		[[nodiscard]] virtual auto getRatio() const noexcept -> double;

		/// @brief Sets the Threshold
		///
		/// @param threshold - The threshold, in decibels
		virtual auto setThreshold(Decibels threshold) noexcept -> void;

		/// @brief Returns the Threshold
		///
		/// @return - The threshold, in decibels
		[[nodiscard]] virtual auto getThreshold() const noexcept -> Decibels;

		/// @brief Sets the KneeWidth
		///
		/// @param kneeWidth - The knee width, in decibels
		virtual auto setKneeWidth(Decibels kneeWidth) noexcept -> void;

		/// @brief Returns the KneeWidth
		///
		/// @return - The knee width, in decibels
		[[nodiscard]] virtual auto getKneeWidth() const noexcept -> Decibels;

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		virtual auto setDynamicsType(DynamicsType type) noexcept -> void;

		/// @brief Returns the DynamicsType
		///
		/// @return - The dynamics type
		[[nodiscard]] virtual auto getDynamicsType() const noexcept -> DynamicsType;

		/// @brief Sets the SampleRate
		///
		/// @param sampleRate - The sample rate, in Hertz
		virtual auto setSampleRate(Hertz sampleRate) noexcept -> void;

		/// @brief Returns the SampleRate
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] virtual auto getSampleRate() const noexcept -> Hertz;

		/// @brief Returns the most recently calculated gain reduction value
		///
		/// @return - The most recently calculated gain reduction value (linear)
		[[nodiscard]] virtual auto getCurrentGainReduction() const noexcept -> Decibels;

		/// @brief Sets the type of the `LevelDetector` to the given value
		///
		/// @param type - The type of the `LevelDetector`
		auto setLevelDetectorType(LevelDetector<double>::DetectorType type) noexcept -> void;

		/// @brief Returns the type of the `LevelDetector`
		///
		/// @return - The type of the `LevelDetector`
		[[nodiscard]] auto
		getLevelDetectorType() const noexcept -> LevelDetector<double>::DetectorType;

		/// @brief Sets the macro-level topology of the gain reduction computer
		///
		/// @param topology - The macro-level topology of the gain reduction computer
		virtual auto setComputerTopology(ComputerTopology topology) noexcept -> void;

		/// @brief Returns the macro-level topology of the gain reduction computer
		///
		/// @return - The macro-level topology of the gain reduction computer
		[[nodiscard]] auto getComputerTopology() const noexcept -> ComputerTopology;

		/// @brief Sets the macro-level topology of the level detector
		///
		/// @param topology - The macro-level topology of the level detector
		virtual auto setDetectorTopology(DetectorTopology topology) noexcept -> void;

		/// @brief Returns the macro-level topology of the level detector
		///
		/// @return - The macro-level topology of the level detector
		[[nodiscard]] auto getDetectorTopology() const noexcept -> DetectorTopology;

		/// @brief Replaces the `GainReduction` adjuster with the given one
		///
		/// @param reduction - The new `GainReduction`
		virtual auto
		setGainReductionProcessor(GainReduction<double, double, double>&& reduction) noexcept
			-> void;

		auto operator=(Sidechain<double>&& sidechain) noexcept -> Sidechain<double>& = default;

		static const constexpr double DEFAULT_ATTACK_SECONDS = 0.01;
		static const constexpr double DEFAULT_RELEASE_SECONDS = 0.05;
		static const constexpr Hertz DEFAULT_SAMPLE_RATE = 44100_Hz;
		static const constexpr double DEFAULT_RATIO = 1.1;
		static const constexpr Decibels DEFAULT_THRESHOLD = -12.0_dB;
		static const constexpr Decibels DEFAULT_KNEE_WIDTH = 6.0_dB;

	  protected:
		static const constexpr double MS_TO_SECS_MULT = 0.001;
		DynamicsState mState = DynamicsState(DEFAULT_ATTACK_SECONDS,
											 DEFAULT_RELEASE_SECONDS,
											 DEFAULT_RATIO,
											 DEFAULT_THRESHOLD,
											 DEFAULT_KNEE_WIDTH,
											 DEFAULT_SAMPLE_RATE);
		Decibels mGainReductionDB = 0.0_dB;

		DynamicsType mDynamicsType = DynamicsType::Compressor;
		ComputerTopology mComputerTopology = ComputerTopology::FeedBack;
		DetectorTopology mDetectorTopology = DetectorTopology::AlternateReturnToThreshold;
		LevelDetector<double>::DetectorType mDetectorType
			= LevelDetector<double>::DetectorType::Decoupled;

		virtual auto processFeedForwardReturnToZero(double input) noexcept -> Decibels;
		virtual auto processFeedForwardReturnToThreshold(double input) noexcept -> Decibels;
		virtual auto
		processFeedForwardAlternateReturnToThreshold(double input) noexcept -> Decibels;

		virtual auto processFeedBackReturnToZero(double input) noexcept -> Decibels;
		virtual auto processFeedBackReturnToThreshold(double input) noexcept -> Decibels;
		virtual auto processFeedBackAlternateReturnToThreshold(double input) noexcept -> Decibels;

	  private:
		LevelDetector<double> mLevelDetector = LevelDetector<double>(&mState, mDetectorType);
		GainReduction<double, double, double> mGainReductionProcessor
			= GainReduction<double, double, double>(&mState);
		GainComputerExpander<double, double, double> mExpanderComputer
			= GainComputerExpander<double, double, double>(&mState);
		GainComputerCompressor<double, double, double> mCompressorComputer
			= GainComputerCompressor<double, double, double>(&mState);
		GainComputer<double, double, double>* mGainComputer = &mCompressorComputer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
	};
} // namespace apex::dsp
