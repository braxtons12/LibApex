#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"
#include "../gaincomputers/GainComputer.h"
#include "../gaincomputers/GainComputerCompressor.h"
#include "../gaincomputers/GainComputerExpander.h"
#include "../gainreductions/GainReductionFET.h"
#include "../leveldetectors/LevelDetector1176.h"
#include "Sidechain.h"

namespace apex::dsp {
	/// @brief The possible ratio values
	enum class Ratio1176
	{
		FourToOne,
		EightToOne,
		TwelveToOne,
		TwentyToOne,
		AllButtonsIn
	};

	/// @brief 1176-style dynamics processor sidechain
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Sidechain1176 final : public Sidechain<FloatType, FloatType, FloatType> {
	  private:
		using DynamicsState = DynamicsState<FloatType, FloatType, FloatType>;
		using LevelDetector1176 = LevelDetector1176<FloatType>;
		using GainReductionFET = GainReductionFET<FloatType, FloatType, FloatType>;
		using Sidechain = Sidechain<FloatType, FloatType, FloatType>;

	  public:
		/// @brief Constructs a `Sidechain1176` with the following defaults:
		/// * attack: 390uS
		/// * release: 525mS
		/// * ratio: 4
		/// * sampleRate: 44100Hz
		///
		/// Threshold and knee width are ratio-dependent semi-fixed values
		Sidechain1176() noexcept {
			Sidechain::mState.setAttack(DEFAULT_ATTACK_SECONDS);
			Sidechain::mState.setRelease(DEFAULT_RELEASE_SECONDS);
			Sidechain::mState.setRatio(DEFAULT_RATIO);
			Sidechain::mState.setThreshold(THRESHOLD_RATIO_4_TO_1);
			Sidechain::mState.setKneeWidth(KNEE_RATIO_4_TO_1);

			Sidechain::mLevelDetector = LevelDetector1176(&(Sidechain::mState));
			Sidechain::mGainReductionProcessor = GainReductionFET(&(Sidechain::mState));
		}

		Sidechain1176(Sidechain1176&& sidechain) noexcept = default;
		~Sidechain1176() noexcept final = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		auto process(FloatType input) noexcept -> Decibels final {
			return Sidechain::processFeedForwardReturnToZero(input);
		}

		/// @brief Sets the attack to the given value
		/// Valid values are in [20uS (20 microseconds), 800uS (800 microseconds)]
		///
		/// @param attackMS - The attack time in milliseconds
		auto setAttackTime(FloatType attackMS) noexcept -> void final {
			auto attackSeconds = attackMS * Sidechain::MS_TO_SECS_MULT;
			jassert(attackSeconds > MIN_ATTACK_SECONDS && attackSeconds < MAX_ATTACK_SECONDS);

			Sidechain::mState.setAttack(attackSeconds);
		}

		/// @brief Returns the attack
		///
		/// @return - The attack time in milliseconds
		[[nodiscard]] auto getAttackTime() const noexcept -> FloatType final {
			return Sidechain::mState.getAttack() / Sidechain::MS_TO_SECS_MULT;
		}

		/// @brief Sets the release to the given value
		/// Valid values are in [50mS (50 milliseconds), 1.1s (1.1 seconds)]
		///
		/// @param releaseMS - The release time in milliseconds
		auto setReleaseTime(FloatType releaseMS) noexcept -> void final {
			auto releaseSeconds = releaseMS * Sidechain::MS_TO_SECS_MULT;
			jassert(releaseSeconds > MIN_RELEASE_SECONDS && releaseSeconds < MAX_RELEASE_SECONDS);

			Sidechain::mState.setRelease(releaseSeconds);
		}

		/// @brief Returns the release
		///
		/// @return - The release time in milliseconds
		[[nodiscard]] auto getReleaseTime() const noexcept -> FloatType final {
			return Sidechain::mState.getRelease() / Sidechain::MS_TO_SECS_MULT;
		}

		/// @brief Sets the ratio to the given value
		///
		/// @param ratio - The ratio
		auto setRatio(Ratio1176 ratio) noexcept -> void {
			mRatio1176 = ratio;
			switch(mRatio1176) {
				case Ratio1176::FourToOne:
					{
						Sidechain::mState.setRatio(RATIO_4_TO_1);
						Sidechain::mState.setThreshold(THRESHOLD_RATIO_4_TO_1);
						Sidechain::mState.setKneeWidth(KNEE_RATIO_4_TO_1);
					}
					break;
				case Ratio1176::EightToOne:
					{
						Sidechain::mState.setRatio(RATIO_8_TO_1);
						Sidechain::mState.setThreshold(THRESHOLD_RATIO_8_TO_1);
						Sidechain::mState.setKneeWidth(KNEE_RATIO_8_TO_1);
					}
					break;
				case Ratio1176::TwelveToOne:
					{
						Sidechain::mState.setRatio(RATIO_12_TO_1);
						Sidechain::mState.setThreshold(THRESHOLD_RATIO_12_TO_1);
						Sidechain::mState.setKneeWidth(KNEE_RATIO_12_TO_1);
					}
					break;
				case Ratio1176::TwentyToOne:
					{
						Sidechain::mState.setRatio(RATIO_20_TO_1);
						Sidechain::mState.setThreshold(THRESHOLD_RATIO_20_TO_1);
						Sidechain::mState.setKneeWidth(KNEE_RATIO_20_TO_1);
					}
					break;
				case Ratio1176::AllButtonsIn:
					{
						Sidechain::mState.setRatio(RATIO_ALL_BUTTONS);
						Sidechain::mState.setThreshold(THRESHOLD_RATIO_ALL_BUTTONS);
						Sidechain::mState.setKneeWidth(KNEE_RATIO_ALL_BUTTONS);
					}
					break;
			}
		}

		[[nodiscard]] auto getEnumRatio() const noexcept -> Ratio1176 {
			return mRatio1176;
		}

		auto operator=(Sidechain1176&& sidechain) noexcept -> Sidechain1176& = default;

		static const constexpr FloatType DEFAULT_ATTACK_SECONDS = narrow_cast<FloatType>(0.00039);
		static const constexpr FloatType MIN_ATTACK_SECONDS = narrow_cast<FloatType>(0.00002);
		static const constexpr FloatType MAX_ATTACK_SECONDS = narrow_cast<FloatType>(0.0008);

		static const constexpr FloatType DEFAULT_RELEASE_SECONDS = narrow_cast<FloatType>(0.525);
		static const constexpr FloatType MIN_RELEASE_SECONDS = narrow_cast<FloatType>(0.05);
		static const constexpr FloatType MAX_RELEASE_SECONDS = narrow_cast<FloatType>(1.1);

	  private:
		static const constexpr FloatType DEFAULT_RATIO = narrow_cast<FloatType>(4.0);
		static const constexpr FloatType RATIO_4_TO_1 = narrow_cast<FloatType>(4.0);
		static const constexpr FloatType RATIO_8_TO_1 = narrow_cast<FloatType>(8.0);
		static const constexpr FloatType RATIO_12_TO_1 = narrow_cast<FloatType>(12.0);
		static const constexpr FloatType RATIO_20_TO_1 = narrow_cast<FloatType>(20.0);
		static const constexpr FloatType RATIO_ALL_BUTTONS = narrow_cast<FloatType>(30.0);

		static const constexpr FloatType THRESHOLD_RATIO_4_TO_1 = narrow_cast<FloatType>(-13.0);
		static const constexpr FloatType THRESHOLD_RATIO_8_TO_1 = narrow_cast<FloatType>(-11.0);
		static const constexpr FloatType THRESHOLD_RATIO_12_TO_1 = narrow_cast<FloatType>(-10.0);
		static const constexpr FloatType THRESHOLD_RATIO_20_TO_1 = narrow_cast<FloatType>(-8.0);
		static const constexpr FloatType THRESHOLD_RATIO_ALL_BUTTONS
			= narrow_cast<FloatType>(-12.0);

		static const constexpr FloatType KNEE_RATIO_4_TO_1 = narrow_cast<FloatType>(8.0);
		static const constexpr FloatType KNEE_RATIO_8_TO_1 = narrow_cast<FloatType>(4.0);
		static const constexpr FloatType KNEE_RATIO_12_TO_1 = narrow_cast<FloatType>(2.0);
		static const constexpr FloatType KNEE_RATIO_20_TO_1 = narrow_cast<FloatType>(1.0);
		static const constexpr FloatType KNEE_RATIO_ALL_BUTTONS = narrow_cast<FloatType>(0.5);

		Ratio1176 mRatio1176 = Ratio1176::FourToOne;

		/// @deprecated DO NOT USE, ratio is fixed for this `Sidechain`
		[[deprecated("Don't use, ratio is fixed for this `Sidechain`")]] auto
		setRatio(FloatType ratio) noexcept -> void final {
			juce::ignoreUnused(ratio);
		}

		/// @deprecated DO NOT USE, threshold is fixed for this `Sidechain`
		[[deprecated("Don't use, threshold is fixed for this `Sidechain`")]] auto
		setThreshold(Decibels threshold) noexcept -> void final {
			juce::ignoreUnused(threshold);
		}

		/// @deprecated DO NOT USE, knee width is fixed for this `Sidechain`
		[[deprecated("Don't use, knee width is fixed for this `Sidechain`")]] auto
		setKneeWidth(Decibels kneeWidth) noexcept -> void final {
			juce::ignoreUnused(kneeWidth);
		}

		/// @deprecated DO NOT USE, computer topology is fixed for this `Sidechain`
		[[deprecated("Don't use, computer topology is fixed for this `Sidechain`")]] auto
		setComputerTopology(ComputerTopology topology) noexcept -> void final {
			juce::ignoreUnused(topology);
		}

		/// @deprected DO NOT USE, detector topology is fixed for this `Sidechain`
		[[deprecated("Don't use, detector topology is fixed for this `Sidechain`")]] auto
		setDetectorTopology(DetectorTopology topology) noexcept -> void final {
			juce::ignoreUnused(topology);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain1176)
	};
} // namespace apex::dsp
