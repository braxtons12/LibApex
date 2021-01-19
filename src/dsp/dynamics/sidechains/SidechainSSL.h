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
	template<typename FloatType, std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class SidechainSSL : Sidechain<FloatType> {
	  private:
		using DynamicsState = DynamicsState<FloatType, SSLBusAttackTime, SSLBusReleaseTime>;
		using Sidechain = Sidechain<FloatType>;
		using LevelDetectorSSL = LevelDetectorSSL<FloatType>;
		using GainReductionVCA = GainReductionVCA<FloatType, SSLBusAttackTime, SSLBusReleaseTime>;
		using GainReduction = GainReduction<FloatType, SSLBusAttackTime, SSLBusReleaseTime>;
		using GainComputerCompressor
			= GainComputerCompressor<FloatType, SSLBusAttackTime, SSLBusReleaseTime>;
		using GainComputerExpander
			= GainComputerExpander<FloatType, SSLBusAttackTime, SSLBusReleaseTime>;
		using GainComputer = GainComputer<FloatType, SSLBusAttackTime, SSLBusReleaseTime>;

	  public:
		/// @brief Constructs a `SidechainSSL` with the following defaults:
		/// * attack: 10ms
		/// * release: auto
		/// * ratio: 4.0
		/// * threshold: -12dB
		/// * knee width: 6dB
		/// * sampleRate 44100Hz
		SidechainSSL() noexcept {
			Sidechain::mState = DynamicsState(SSLBusAttackTime::TenMilliseconds,
											  SSLBusReleaseTime::Auto,
											  narrow_cast<FloatType>(4.0),
											  -12.0_dB,
											  6.0_dB,
											  44.1_kHz);
			Sidechain::mLevelDetector = LevelDetectorSSL(&Sidechain::mState);
			Sidechain::mGainReductionProcessor = GainReductionVCA(&Sidechain::mState);
			Sidechain::mCompressorComputer = GainComputerCompressor(&Sidechain::mState);
			Sidechain::mExpanderComputer = GainComputerExpander(&Sidechain::mState);
			Sidechain::mGainComputer = &Sidechain::mCompressorComputer;
		}

		/// @brief Move constructs the given `SidechainSSL`
		///
		/// @param sidechain - The `SidechainSSL` to move
		SidechainSSL(SidechainSSL&& sidechain) noexcept = default;
		~SidechainSSL() noexcept override = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		inline auto process(FloatType input) noexcept -> Decibels override {
			return processFeedBackAlternateReturnToThreshold(input);
		}

		/// @brief Sets the attack to the given value
		///
		/// @param attack- The attack time
		inline auto setAttackTime(SSLBusAttackTime attack) noexcept -> void {
			Sidechain::mState.setAttack(attack);
		}

		/// @brief Returns the attack
		///
		/// @return - The attack time
		[[nodiscard]] inline auto getAttackTime() const noexcept -> SSLBusAttackTime {
			Sidechain::mState.getAttack();
		}

		/// @brief Sets the release to the given value
		///
		/// @param releaseMS - The release time
		inline auto setReleaseTime(SSLBusReleaseTime release) noexcept -> void {
			Sidechain::mState.setRelease(release);
			if(release == SSLBusReleaseTime::Auto) {
				Sidechain::mState.setAutoReleaseEnabled(true);
			}
			else {
				Sidechain::mState.setAutoReleaseEnabled(false);
			}
		}

		/// @brief Returns the release
		///
		/// @return - The release time
		[[nodiscard]] inline auto getReleaseTime() const noexcept -> SSLBusReleaseTime {
			Sidechain::mState.getRelease();
		}

	  private:
		/// @deprecated DO NOT USE, LevelDetectorType is fixed for this `Sidechain`
		[[deprecated("Don't use. LevelDetectorType is fixed for this Sidechain")]] inline auto
		setLevelDetectorType(DetectorType type) noexcept -> void override {
			juce::ignoreUnused(type);
		}

		/// @deprecated DO NOT USE, LevelDetectorType is fixed for this `Sidechain`
		[[deprecated(
			"Don't use. LevelDetectorType is fixed for this Sidechain")]] [[nodiscard]] auto
		getLevelDetectorType() const noexcept -> DetectorType override {
			return DetectorType::BranchingSmooth;
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated("Don't use. ComputerTopology is fixed for this Sidechain")]] inline auto
		setComputerTopology(ComputerTopology topology) noexcept -> void override {
			juce::ignoreUnused(topology);
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated(
			"Don't use. ComputerTopology is fixed for this Sidechain")]] [[nodiscard]] inline auto
		getComputerTopology() const noexcept -> ComputerTopology override {
			return ComputerTopology::FeedBack;
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated("Don't use. ComputerTopology is fixed for this Sidechain")]] inline auto
		setDetectorTopology(DetectorTopology topology) noexcept -> void override {
			juce::ignoreUnused(topology);
		}

		/// @deprecated DO NOT USE, ComputerTopology is fixed for this `Sidechain`
		[[deprecated(
			"Don't use. ComputerTopology is fixed for this Sidechain")]] [[nodiscard]] inline auto
		getDetectorTopology() const noexcept -> DetectorTopology override {
			return DetectorTopology::AlternateReturnToThreshold;
		}

		/// @deprecated DO NOT USE, GainReductionProcessor is fixed for this `Sidechain`
		[[deprecated("Don't use. GainReductionProcessor is fixed for this Sidechain")]] inline auto
		setGainReductionProcessor(GainReduction&& reduction) const noexcept -> void override {
			juce::ignoreUnused(reduction);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainSSL)
	};
} // namespace apex::dsp
