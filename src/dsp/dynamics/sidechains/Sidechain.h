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

	/// @brief Base dynamics processor sidechain. This is the signal chain that determines the
	/// gain reduction that the containing dynamics processor should apply to the input signal
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Sidechain {
	  private:
		using DynamicsState = DynamicsState<FloatType, FloatType, FloatType>;
		using LevelDetector = LevelDetector<FloatType>;
		using GainReduction = GainReduction<FloatType>;
		using GainComputer = GainComputer<FloatType>;
		using GainComputerCompressor = GainComputerCompressor<FloatType>;
		using GainComputerExpander = GainComputerExpander<FloatType>;

	  public:
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
		Sidechain(Sidechain&& sidechain) noexcept = default;
		virtual ~Sidechain() noexcept = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		virtual inline auto process(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing Input");
#endif
			Decibels x = 0.0_dB;
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
			return mGainReductionProcessor.adjustedGainReduction(Decibels(x));
		}

		/// @brief Sets the attack to the given value
		///
		/// @param attackMS - The attack time, in milliseconds
		virtual inline auto setAttackTime(FloatType attackMS) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Attack Time");
#endif
			mState.setAttack(attackMS * MS_TO_SECS_MULT);
		}

		/// @brief Returns the attack
		///
		/// @return - The attack time, in milliseconds
		[[nodiscard]] virtual inline auto getAttackTime() const noexcept -> FloatType {
			return mState.getAttack() / MS_TO_SECS_MULT;
		}

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time, in milliseconds
		virtual inline auto setReleaseTime(FloatType releaseMS) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Release Time");
#endif
			mState.setRelease(releaseMS * MS_TO_SECS_MULT);
		}

		/// @brief Returns the release
		///
		/// @return - The release time, in milliseconds
		[[nodiscard]] virtual inline auto getReleaseTime() const noexcept -> FloatType {
			return mState.getRelease() / MS_TO_SECS_MULT;
		}

		/// @brief Sets the Ratio
		///
		/// @param ratio - The ratio
		virtual inline auto setRatio(FloatType ratio) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Ratio");
#endif
			mState.setRatio(ratio);
		}

		/// @brief Returns the Ratio
		///
		/// @return - The ratio
		[[nodiscard]] virtual inline auto getRatio() const noexcept -> FloatType {
			return mState.getRatio();
		}

		/// @brief Sets the Threshold
		///
		/// @param threshold - The threshold, in decibels
		virtual inline auto setThreshold(Decibels threshold) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Threshold");
#endif
			mState.setThreshold(threshold);
		}

		/// @brief Returns the Threshold
		///
		/// @return - The threshold, in decibels
		[[nodiscard]] virtual inline auto getThreshold() const noexcept -> Decibels {
			return mState.getThreshold();
		}

		/// @brief Sets the KneeWidth
		///
		/// @param kneeWidth - The knee width, in decibels
		virtual inline auto setKneeWidth(Decibels kneeWidth) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating KneeWidth");
#endif
			mState.setKneeWidth(kneeWidth);
		}

		/// @brief Returns the KneeWidth
		///
		/// @return - The knee width, in decibels
		[[nodiscard]] virtual inline auto getKneeWidth() const noexcept -> Decibels {
			return mState.getKneeWidth();
		}

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		virtual inline auto setDynamicsType(DynamicsType type) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Dynamics Type");
#endif
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
		[[nodiscard]] virtual inline auto getDynamicsType() const noexcept -> DynamicsType {
			return mDynamicsType;
		}

		/// @brief Sets the SampleRate
		///
		/// @param sampleRate - The sample rate, in Hertz
		virtual inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Sample Rate");
#endif
			mState.setSampleRate(sampleRate);
		}

		/// @brief Returns the SampleRate
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] virtual inline auto getSampleRate() const noexcept -> Hertz {
			return mState.getSampleRate();
		}

		/// @brief Returns the most recently calculated gain reduction value
		///
		/// @return - The most recently calculated gain reduction value (linear)
		[[nodiscard]] virtual inline auto getCurrentGainReduction() const noexcept -> Decibels {
			return mGainReductionDB;
		}

		/// @brief Sets the type of the `LevelDetector` to the given value
		///
		/// @param type - The type of the `LevelDetector`
		virtual inline auto setLevelDetectorType(DetectorType type) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating LevelDetector Type");
#endif
			mDetectorType = type;
			mLevelDetector = LevelDetector(&mState, mDetectorType);
		}

		/// @brief Returns the type of the `LevelDetector`
		///
		/// @return - The type of the `LevelDetector`
		[[nodiscard]] virtual inline auto getLevelDetectorType() const noexcept -> DetectorType {
			return mDetectorType;
		}

		/// @brief Sets the macro-level topology of the gain reduction computer
		///
		/// @param topology - The macro-level topology of the gain reduction computer
		virtual inline auto setComputerTopology(ComputerTopology topology) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating Gain Computer Topology");
#endif
			mComputerTopology = topology;
		}

		/// @brief Returns the macro-level topology of the gain reduction computer
		///
		/// @return - The macro-level topology of the gain reduction computer
		[[nodiscard]] virtual inline auto getComputerTopology() const noexcept -> ComputerTopology {
			return mComputerTopology;
		}

		/// @brief Sets the macro-level topology of the level detector
		///
		/// @param topology - The macro-level topology of the level detector
		virtual inline auto setDetectorTopology(DetectorTopology topology) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating LevelDetector Topology");
#endif
			mDetectorTopology = topology;
		}

		/// @brief Returns the macro-level topology of the level detector
		///
		/// @return - The macro-level topology of the level detector
		[[nodiscard]] virtual inline auto getDetectorTopology() const noexcept -> DetectorTopology {
			return mDetectorTopology;
		}

		/// @brief Replaces the `GainReduction` adjuster with the given one
		///
		/// @param reduction - The new `GainReduction`
		virtual inline auto setGainReductionProcessor(GainReduction&& reduction) noexcept -> void {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Updating GainReduction Processor");
#endif
			mGainReductionProcessor = std::move(reduction);
		}

		auto operator=(Sidechain&& sidechain) noexcept -> Sidechain& = default;

		static const constexpr FloatType DEFAULT_ATTACK_SECONDS = narrow_cast<FloatType>(0.01);
		static const constexpr FloatType DEFAULT_RELEASE_SECONDS = narrow_cast<FloatType>(0.05);
		static const constexpr Hertz DEFAULT_SAMPLE_RATE = 44.1_kHz;
		static const constexpr FloatType DEFAULT_RATIO = narrow_cast<FloatType>(1.1);
		static const constexpr Decibels DEFAULT_THRESHOLD = -12.0_dB;
		static const constexpr Decibels DEFAULT_KNEE_WIDTH = 6.0_dB;

	  protected:
		static const constexpr FloatType MS_TO_SECS_MULT = narrow_cast<FloatType>(0.001);
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
		DetectorType mDetectorType = DetectorType::Decoupled;
		LevelDetector mLevelDetector = LevelDetector(&mState, mDetectorType);
		GainReduction mGainReductionProcessor = GainReduction(&mState);
		GainComputerExpander mExpanderComputer = GainComputerExpander(&mState);
		GainComputerCompressor mCompressorComputer = GainComputerCompressor(&mState);
		GainComputer* mGainComputer = &mCompressorComputer;

		virtual inline auto processFeedForwardReturnToZero(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing FeedForwardReturnToZero");
#endif
			auto rectified = General<FloatType>::abs(input);
			Decibels detectedDB = Decibels::fromLinear(mLevelDetector.process(rectified));
			Decibels outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB = outputDB - detectedDB;
			mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
			return mGainReductionDB;
		}

		virtual inline auto
		processFeedForwardReturnToThreshold(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing FeedForwardReturnToThreshold");
#endif
			auto rectified = General<FloatType>::abs(input);
			auto thresholdLinear = narrow_cast<FloatType>(mState.getThreshold().getLinear());
			Decibels detectedDB = Decibels::fromLinear(
				mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
			Decibels outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB = outputDB - detectedDB;
			mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
			return mGainReductionDB;
		}

		virtual inline auto
		processFeedForwardAlternateReturnToThreshold(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing FeedForwardAlternateReturnToThreshold");
#endif
			auto rectified = General<FloatType>::abs(input);
			Decibels rectifiedDB = Decibels::fromLinear(rectified);
			Decibels gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
			mGainReductionDB = mLevelDetector.process(narrow_cast<FloatType>(gainReduction));
			mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
			return mGainReductionDB;
		}

		virtual inline auto processFeedBackReturnToZero(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing FeedBackReturnToZero");
#endif
			auto rectified = General<FloatType>::abs(input)
							 * narrow_cast<FloatType>(mGainReductionDB.getLinear());
			auto detectedDB = math::Decibels::linearToDecibels(mLevelDetector.process(rectified));
			Decibels outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB += outputDB - detectedDB;
			mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
			return mGainReductionDB;
		}

		virtual inline auto processFeedBackReturnToThreshold(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing FeedBackReturnToThreshold");
#endif
			auto rectified = General<FloatType>::abs(input)
							 * narrow_cast<FloatType>(mGainReductionDB.getLinear());
			auto thresholdLinear = narrow_cast<FloatType>(mState.getThreshold().getLinear());
			Decibels detectedDB = Decibels::fromLinear(
				mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
			Decibels outputDB = mGainComputer->process(detectedDB);
			mGainReductionDB += outputDB - detectedDB;
			mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
			return mGainReductionDB;
		}

		virtual inline auto
		processFeedBackAlternateReturnToThreshold(FloatType input) noexcept -> Decibels {
#ifdef TESTING_SIDECHAIN
			Logger::LogMessage("Base Sidechain Processing FeedBackAlternateReturnToThreshold");
#endif
			auto rectified = General<FloatType>::abs(input)
							 * narrow_cast<FloatType>(mGainReductionDB.getLinear());
			Decibels rectifiedDB = Decibels::fromLinear(rectified);
			Decibels gainReduction
				= mGainReductionDB + mGainComputer->process(rectifiedDB) - rectifiedDB;
			mGainReductionDB = mLevelDetector.process(narrow_cast<FloatType>(gainReduction));
			mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
			return mGainReductionDB;
		}

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain)
	};
} // namespace apex::dsp
