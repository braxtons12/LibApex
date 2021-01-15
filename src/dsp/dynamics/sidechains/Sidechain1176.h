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
	/// @brief 1176-style dynamics processor sidechain
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class Sidechain1176 final : public Sidechain<T> {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidechain1176)
	};

	/// @brief 1176-style dynamics processor sidechain
	template<>
	class Sidechain1176<float> final : public Sidechain<float> {
	  private:
		using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

	  public:
		/// @brief The possible ratio values
		enum class Ratio
		{
			FourToOne,
			EightToOne,
			TwelveToOne,
			TwentyToOne,
			AllButtonsIn
		};

		/// @brief Constructs a `Sidechain1176` with the following defaults:
		/// * attack: 390uS
		/// * release: 525mS
		/// * ratio: 4
		/// * sampleRate: 44100Hz
		///
		/// Threshold and knee width are ratio-dependent semi-fixed values
		Sidechain1176() noexcept;

		Sidechain1176(Sidechain1176<float>&& sidechain) noexcept = default;
		~Sidechain1176() noexcept final = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		auto process(float input) noexcept -> Decibels final;

		/// @brief Sets the attack to the given value
		/// Valid values are in [20uS (20 microseconds), 800uS (800 microseconds)]
		///
		/// @param attackMS - The attack time in milliseconds
		auto setAttackTime(float attackMS) noexcept -> void final;

		/// @brief Returns the attack
		///
		/// @return - The attack time in milliseconds
		[[nodiscard]] auto getAttackTime() const noexcept -> float final;

		/// @brief Sets the release to the given value
		/// Valid values are in [50mS (50 milliseconds), 1.1s (1.1 seconds)]
		///
		/// @param releaseMS - The release time in milliseconds
		auto setReleaseTime(float releaseMS) noexcept -> void final;

		/// @brief Returns the release
		///
		/// @return - The release time in milliseconds
		[[nodiscard]] auto getReleaseTime() const noexcept -> float final;

		/// @brief Sets the ratio to the given value
		///
		/// @param ratio - The ratio
		auto setRatio(Ratio ratio) noexcept -> void;

		[[nodiscard]] auto getEnumRatio() const noexcept -> Ratio {
			return mRatio;
		}

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		auto setDynamicsType(DynamicsType type) noexcept -> void final;

		auto
		operator=(Sidechain1176<float>&& sidechain) noexcept -> Sidechain1176<float>& = default;

		static const constexpr float DEFAULT_ATTACK_SECONDS = 0.00039F;
		static const constexpr float MIN_ATTACK_SECONDS = 0.00002F;
		static const constexpr float MAX_ATTACK_SECONDS = 0.0008F;

		static const constexpr float DEFAULT_RELEASE_SECONDS = 0.525F;
		static const constexpr float MIN_RELEASE_SECONDS = 0.05F;
		static const constexpr float MAX_RELEASE_SECONDS = 1.1F;

	  private:
		static const constexpr float DEFAULT_RATIO = 4.0F;
		static const constexpr float RATIO_4_TO_1 = 4.0F;
		static const constexpr float RATIO_8_TO_1 = 8.0F;
		static const constexpr float RATIO_12_TO_1 = 12.0F;
		static const constexpr float RATIO_20_TO_1 = 20.0F;
		static const constexpr float RATIO_ALL_BUTTONS = 30.0F;

		static const constexpr float THRESHOLD_RATIO_4_TO_1 = -13.0F;
		static const constexpr float THRESHOLD_RATIO_8_TO_1 = -11.0F;
		static const constexpr float THRESHOLD_RATIO_12_TO_1 = -10.0F;
		static const constexpr float THRESHOLD_RATIO_20_TO_1 = -8.0F;
		static const constexpr float THRESHOLD_RATIO_ALL_BUTTONS = -12.0F;

		static const constexpr float KNEE_RATIO_4_TO_1 = 8.0F;
		static const constexpr float KNEE_RATIO_8_TO_1 = 4.0F;
		static const constexpr float KNEE_RATIO_12_TO_1 = 2.0F;
		static const constexpr float KNEE_RATIO_20_TO_1 = 1.0F;
		static const constexpr float KNEE_RATIO_ALL_BUTTONS = 0.5F;

		Ratio mRatio = Ratio::FourToOne;
		LevelDetector1176<float> mLevelDetector = LevelDetector1176<float>(&mState);
		GainReductionFET<float, float, float> mGainReductionProcessor
			= GainReductionFET<float, float, float>(&mState);
		GainComputerExpander<float, float, float> mExpanderComputer
			= GainComputerExpander<float, float, float>(&mState);
		GainComputerCompressor<float, float, float> mCompressorComputer
			= GainComputerCompressor<float, float, float>(&mState);
		GainComputer<float, float, float>* mGainComputer = &mCompressorComputer;

		auto processFeedForwardReturnToZero(float input) noexcept -> Decibels final;

		/// @deprecated DO NOT USE, ratio is fixed for this `Sidechain`
		[[deprecated("Don't use, ratio is fixed for this `Sidechain`")]] auto
		setRatio(float ratio) noexcept -> void final {
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

	/// @brief 1176-style dynamics processor sidechain
	template<>
	class Sidechain1176<double> final : public Sidechain<double> {
	  private:
		using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
		using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

	  public:
		/// @brief The possible ratio values
		enum class Ratio
		{
			FourToOne,
			EightToOne,
			TwelveToOne,
			TwentyToOne,
			AllButtonsIn
		};

		/// @brief Constructs a `Sidechain1176` with the following defaults:
		/// * attack: 390uS
		/// * release: 525mS
		/// * ratio: 4
		/// * sampleRate: 44100Hz
		///
		/// Threshold and knee width are ratio-dependent semi-fixed values
		Sidechain1176() noexcept;

		Sidechain1176(Sidechain1176<double>&& sidechain) noexcept = default;
		~Sidechain1176() noexcept final = default;

		/// @brief Calculates the target gain reduction to apply to the input value
		///
		/// @param input - The input value to calculate gain reduction for
		///
		/// @return - The target gain reduction
		auto process(double input) noexcept -> Decibels final;

		/// @brief Sets the attack to the given value
		/// Valid values are in [20uS (20 microseconds), 800uS (800 microseconds)]
		///
		/// @param attackMS - The attack time in milliseconds
		auto setAttackTime(double attackMS) noexcept -> void final;

		/// @brief Returns the attack
		///
		/// @return - The attack time in milliseconds
		[[nodiscard]] auto getAttackTime() const noexcept -> double final;

		/// @brief Sets the release to the given value
		/// Valid values are in [50mS (50 milliseconds), 1.1s (1.1 seconds)]
		///
		/// @param releaseMS - The release time in milliseconds
		auto setReleaseTime(double releaseMS) noexcept -> void final;

		/// @brief Returns the release
		///
		/// @return - The release time in milliseconds
		[[nodiscard]] auto getReleaseTime() const noexcept -> double final;

		/// @brief Sets the ratio to the given value
		///
		/// @param ratio - The ratio
		auto setRatio(Ratio ratio) noexcept -> void;

		[[nodiscard]] auto getEnumRatio() const noexcept -> Ratio {
			return mRatio;
		}

		/// @brief Sets the DynamicsType
		///
		/// @param type - The dynamics type
		auto setDynamicsType(DynamicsType type) noexcept -> void final;

		auto
		operator=(Sidechain1176<double>&& sidechain) noexcept -> Sidechain1176<double>& = default;

		static const constexpr double DEFAULT_ATTACK_SECONDS = 0.00039;
		static const constexpr double MIN_ATTACK_SECONDS = 0.00002;
		static const constexpr double MAX_ATTACK_SECONDS = 0.0008;

		static const constexpr double DEFAULT_RELEASE_SECONDS = 0.525;
		static const constexpr double MIN_RELEASE_SECONDS = 0.05;
		static const constexpr double MAX_RELEASE_SECONDS = 1.1;

	  private:
		static const constexpr double DEFAULT_RATIO = 4.0;
		static const constexpr double RATIO_4_TO_1 = 4.0;
		static const constexpr double RATIO_8_TO_1 = 8.0;
		static const constexpr double RATIO_12_TO_1 = 12.0;
		static const constexpr double RATIO_20_TO_1 = 20.0;
		static const constexpr double RATIO_ALL_BUTTONS = 30.0;

		static const constexpr double THRESHOLD_RATIO_4_TO_1 = -13.0;
		static const constexpr double THRESHOLD_RATIO_8_TO_1 = -11.0;
		static const constexpr double THRESHOLD_RATIO_12_TO_1 = -10.0;
		static const constexpr double THRESHOLD_RATIO_20_TO_1 = -8.0;
		static const constexpr double THRESHOLD_RATIO_ALL_BUTTONS = -12.0;

		static const constexpr double KNEE_RATIO_4_TO_1 = 8.0;
		static const constexpr double KNEE_RATIO_8_TO_1 = 4.0;
		static const constexpr double KNEE_RATIO_12_TO_1 = 2.0;
		static const constexpr double KNEE_RATIO_20_TO_1 = 1.0;
		static const constexpr double KNEE_RATIO_ALL_BUTTONS = 0.5;

		Ratio mRatio = Ratio::FourToOne;
		LevelDetector1176<double> mLevelDetector = LevelDetector1176<double>(&mState);
		GainReductionFET<double, double, double> mGainReductionProcessor
			= GainReductionFET<double, double, double>(&mState);
		GainComputerExpander<double, double, double> mExpanderComputer
			= GainComputerExpander<double, double, double>(&mState);
		GainComputerCompressor<double, double, double> mCompressorComputer
			= GainComputerCompressor<double, double, double>(&mState);
		GainComputer<double, double, double>* mGainComputer = &mCompressorComputer;

		auto processFeedForwardReturnToZero(double input) noexcept -> Decibels final;

		/// @deprecated DO NOT USE, ratio is fixed for this `Sidechain`
		[[deprecated("Don't use, ratio is fixed for this `Sidechain`")]] auto
		setRatio(double ratio) noexcept -> void final {
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
