#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "../gaincomputers/GainComputer.h"
#include "../gaincomputers/GainComputerCompressor.h"
#include "../gaincomputers/GainComputerExpander.h"
#include "../gainreductions/GainReductionVCA.h"
#include "../leveldetectors/LevelDetectorSSL.h"
#include "Sidechain.h"
#include "juce_core/system/juce_PlatformDefs.h"

namespace apex::dsp {
	template<typename T>
	class SidechainSSL : Sidechain<T> {
	  private:
		using Field =
			typename apex::dsp::DynamicsState<T, SSLBusAttackTime, SSLBusReleaseTime>::Field;
		using DynamicsState =
			typename apex::dsp::DynamicsState<T, SSLBusAttackTime, SSLBusReleaseTime>;

	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

		/// @brief Constructs a `SidechainSSL` with the following defaults:
		/// * attack: 10ms
		/// * release: auto
		/// * ratio: 4.0
		/// * threshold: -12dB
		/// * knee width: 6dB
		/// * sampleRate 44100Hz
		SidechainSSL() noexcept = default;

		/// @brief Move constructs the given `SidechainSSL`
		///
		/// @param sidechain - The `SidechainSSL` to move
		SidechainSSL(SidechainSSL<T>&& sidechain) noexcept = default;
		~SidechainSSL() noexcept override = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		inline auto process(T input) noexcept -> Decibels override {
			return processFeedBackAlternateReturnToThreshold(input);
		}

		/// @brief Sets the attack to the given value
		///
		/// @param attack- The attack time
		inline auto setAttackTime(SSLBusAttackTime attack) noexcept -> void {
			mState.setAttack(attack);
		}

		/// @brief Returns the attack
		///
		/// @return - The attack time
		[[nodiscard]] inline auto getAttackTime() const noexcept -> SSLBusAttackTime {
			mState.getAttack();
		}

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time
		inline auto setReleaseTime(SSLBusReleaseTime release) noexcept -> void {
			mState.setRelease(release);
			if(release == SSLBusReleaseTime::Auto) {
				mState.setAutoReleaseEnabled(true);
			}
			else {
				mState.setAutoReleaseEnabled(false);
			}
		}

		/// @brief Returns the release
		///
		/// @return - The release time
		[[nodiscard]] inline auto getReleaseTime() const noexcept -> SSLBusReleaseTime {
			mState.getRelease();
		}

		/// @brief Sets the Ratio
		///
		/// @param ratio - The ratio
		inline auto setRatio(T ratio) noexcept -> void override {
			mState.setRatio(ratio);
		}

		/// @brief Returns the Ratio
		///
		/// @return - The ratio
		[[nodiscard]] auto getRatio() const noexcept -> T override {
			mState.getRatio();
		}

		/// @brief Sets the Threshold
		///
		/// @param threshold - The threshold, in decibels
		inline auto setThreshold(Decibels threshold) noexcept -> void override {
			mState.setThreshold(threshold);
		}

		/// @brief Returns the Threshold
		///
		/// @return - The threshold, in decibels
		[[nodiscard]] inline auto getThreshold() const noexcept -> Decibels override {
			return mState.getThreshold();
		}

		/// @brief Sets the KneeWidth
		///
		/// @param kneeWidth - The knee width, in decibels
		inline auto setKneeWidth(Decibels kneeWidth) const noexcept -> void override {
			mState.setKneeWidth(kneeWidth);
		}

		/// @brief Returns the KneeWidth
		///
		/// @return - The knee width, in decibels
		[[nodiscard]] inline auto getKneeWidth() const noexcept -> Decibels override {
			return mState.getKneeWidth();
		}

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		inline auto setDynamicsType(DynamicsState type) noexcept -> void {
			Sidechain<T>::mDynamicsType = type;
			if(Sidechain<T>::mDynamicsType == Sidechain<T>::DynamicsType::Compressor) {
				mGainComputer = &mCompressorComputer;
			}
			else {
				mGainComputer = &mExpanderComputer;
			}
		}

		/// @brief Sets the SampleRate
		///
		/// @param sampleRate - The sample rate, in Hertz
		inline auto setsampleRate(Hertz sampleRate) noexcept -> void override {
			mState.setSampleRate(sampleRate);
		}

		/// @brief Returns the SampleRate
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] inline auto getSampleRate() const noexcept -> Hertz override {
			return mState.getSampleRate();
		}

	  private:
		DynamicsState mState = DynamicsState(SSLBusAttackTime::TenMilliseconds,
											 SSLBusReleaseTime::Auto,
											 static_cast<T>(4.0),
											 -12.0_dB,
											 6.0_dB,
											 44100_Hz);
		LevelDetectorSSL<T> mLevelDetector = LevelDetectorSSL<T>(&mState);
		GainReductionVCA<T, SSLBusAttackTime, SSLBusReleaseTime> mGainReductionProcessor
			= GainReductionVCA<T, SSLBusAttackTime, SSLBusReleaseTime>(&mState);
		GainComputerCompressor<float, SSLBusAttackTime, SSLBusReleaseTime> mCompressorComputer
			= GainComputerCompressor<float, SSLBusAttackTime, SSLBusReleaseTime>(&mState);
		GainComputerExpander<float, SSLBusAttackTime, SSLBusReleaseTime> mExpanderComputer
			= GainComputerExpander<float, SSLBusAttackTime, SSLBusReleaseTime>(&mState);
		GainComputer<float, SSLBusAttackTime, SSLBusReleaseTime>* mGainComputer
			= &mCompressorComputer;

		inline auto processFeedBackAlternateReturnToThreshold(T input) -> Decibels override {
			T rectified = gsl::narrow_cast<T>(
				math::fabs(input)
				* gsl::narrow_cast<T>(Sidechain<T>::mGainReductionDB.getLinear()));
			auto thresholdLinear = gsl::narrow_cast<T>(mState.getThreshold().getLinear());
			Decibels detectedDB = Decibels::fromLinear(
				mLevelDetector.process(rectified - thresholdLinear) + thresholdLinear);
			Decibels outputDB = mGainComputer->process(detectedDB);
			Sidechain<T>::mGainReductionDB += outputDB - detectedDB;
			Sidechain<T>::mGainReductionDB
				= mGainReductionProcessor.adjustedGainReduction(Sidechain<T>::mGainReductionDB);
			return Sidechain<T>::mGainReductionDB;
		}

		/// @deprecated DO NOT USE, LevelDetectorType is fixed for this `Sidechain`
		[[deprecated("Don't use. LevelDetectorType is fixed for this Sidechain")]] inline auto
		setLevelDetectorType(typename LevelDetector<T>::DetectorType type) noexcept
			-> void override {
			juce::ignoreUnused(type);
		}

		/// @deprecated DO NOT USE, LevelDetectorType is fixed for this `Sidechain`
		[[deprecated(
			"Don't use. LevelDetectorType is fixed for this Sidechain")]] [[nodiscard]] auto
		getLevelDetectorType() const noexcept -> typename LevelDetector<T>::DetectorType override {
			return LevelDetector<T>::DetectorType::BranchingSmooth;
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated("Don't use. ComputerTopology is fixed for this Sidechain")]] inline auto
		setComputerTopology(typename Sidechain<T>::ComputerTopology topology) noexcept
			-> void override {
			juce::ignoreUnused(topology);
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated("Don't use. ComputerTopology is fixed for this Sidechain")]] inline auto
		getComputerTopology() const noexcept -> typename Sidechain<T>::ComputerTopology override {
			return Sidechain<T>::ComputerTopology::FeedBack;
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated("Don't use. ComputerTopology is fixed for this Sidechain")]] inline auto
		setDetectorTopology(typename Sidechain<T>::DetectorTopology topology) noexcept
			-> void override {
			juce::ignoreUnused(topology);
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated("Don't use. ComputerTopology is fixed for this Sidechain")]] inline auto
		getDetectorTopology() const noexcept -> typename Sidechain<T>::DetectorTopology override {
			return Sidechain<T>::DetectorTopology::AlternateReturnToThreshold;
		}

		/// @deprecated DO NOT USE, GainReductionProcessor is fixed for this `Sidechain`
		[[deprecated("Don't use. GainReductionProcessor is fixed for this Sidechain")]] inline auto
		setGainReductionProcessor(GainReduction<T, SSLBusAttackTime, SSLBusReleaseTime>&& reduction)
			const noexcept -> void override {
			juce::ignoreUnused(reduction);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainSSL)
	};
} // namespace apex::dsp
