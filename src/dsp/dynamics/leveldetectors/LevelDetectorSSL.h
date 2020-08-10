#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex::dsp {
	/// @brief Possible attack times for an SSL-style bus compressor
	enum class SSLBusAttackTime
	{
		PointOneMilliseconds,
		PointThreeMilliseconds,
		OneMilliseconds,
		ThreeMilliseconds,
		TenMilliseconds,
		ThirtyMilliseconds
	};

	/// @brief Possible release times for an SSL-Style bus compressor
	enum class SSLBusReleaseTime
	{
		PointOneSeconds,
		PointThreeSeconds,
		PointSixSeconds,
		OnePointTwoSeconds,
		Auto
	};

	/// @brief SSL-style Bus Compressor Level Detector
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class LevelDetectorSSL : public LevelDetector<T> {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
	};

	/// @brief SSL-style Bus Compressor Level Detector
	template<>
	class LevelDetectorSSL<float> : public LevelDetector<float> {
	  private:
		using Field =
			typename apex::dsp::DynamicsState<float, SSLBusAttackTime, SSLBusReleaseTime>::Field;
		using DynamicsState =
			typename apex::dsp::DynamicsState<float, SSLBusAttackTime, SSLBusReleaseTime>;

	  public:
		/// @brief Constructs a `LevelDetectorSSL` with zereod shared state
		LevelDetectorSSL() noexcept = default;

		/// @brief Constructs a `LevelDetectorSSL` with the given shared state
		///
		/// @param state - The shared state
		explicit LevelDetectorSSL(DynamicsState* state) noexcept;

		/// @brief Move constructs a `LevelDetectorSSL` from the given one
		///
		/// @param detector - The `LevelDetectorSSL` to move
		LevelDetectorSSL(LevelDetectorSSL<float>&& detector) noexcept = default;

		~LevelDetectorSSL() noexcept override = default;

		/// @brief Sets the attack time to the given value
		///
		/// @param attack - The new attack time
		auto setAttackTime(SSLBusAttackTime attack) noexcept -> void;

		/// @brief Sets the release time to the given value
		///
		/// @param release - The new release time
		auto setReleaseTime(SSLBusReleaseTime release) noexcept -> void;

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		auto setSampleRate(size_t sampleRate) noexcept -> void override;

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		[[nodiscard]] auto process(float input) noexcept -> float override;

		auto operator=(LevelDetectorSSL<float>&& detector) noexcept
			-> LevelDetectorSSL<float>& = default;

	  private:
		DynamicsState DEFAULT_STATE = DynamicsState();
		DynamicsState* mState = &DEFAULT_STATE;
		float mY1N1 = 0.0F;
		float mY2N1 = 0.0F;

		static const constexpr float ATTACK_POINT_ONE_MS_S = 0.0001F;
		static const constexpr float ATTACK_POINT_THREE_MS_S = 0.0003F;
		static const constexpr float ATTACK_ONE_MS_S = 0.001F;
		static const constexpr float ATTACK_THREE_MS_S = 0.003F;
		static const constexpr float ATTACK_TEN_MS_S = 0.01F;
		static const constexpr float ATTACK_THIRTY_MS_S = 0.03F;

		static const constexpr SSLBusAttackTime DEFAULT_ATTACK = SSLBusAttackTime::TenMilliseconds;

		static const constexpr float RELEASE_POINT_ONE_S = 0.1F;
		static const constexpr float RELEASE_POINT_THREE_S = 0.3F;
		static const constexpr float RELEASE_POINT_SIX_S = 0.6F;
		static const constexpr float RELEASE_ONE_POINT_TWO_S = 1.2F;

		static const constexpr SSLBusReleaseTime DEFAULT_RELEASE
			= SSLBusReleaseTime::PointThreeSeconds;

		static const constexpr float AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47F;
		static const constexpr float AUTO_RELEASE1_S = 0.04277F;
		static const constexpr float AUTO_RELEASE2_S = 5.1F;

		auto
		calculateAttackCoefficients(SSLBusAttackTime attack, size_t sampleRate) noexcept -> void;
		auto
		calculateReleaseCoefficients(SSLBusReleaseTime release, size_t sampleRate) noexcept -> void;

		/// @deprecated DO NOT USE, USE `setAttackTime(SSLBusAttackTime)`
		[[deprecated("Use `setAttackType` for this specific `LevelDetector`")]] auto
		setAttackTime(float attackMS) noexcept -> void override {
			juce::ignoreUnused(attackMS);
		}

		/// @deprecated DO NOT USE, USE `setReleaseTime(SSLBusReleaseTime)`
		[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]] auto
		setReleaseTime(float releaseMS) noexcept -> void override {
			juce::ignoreUnused(releaseMS);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
	};

	/// @brief SSL-style Bus Compressor Level Detector
	template<>
	class LevelDetectorSSL<double> : public LevelDetector<double> {
	  private:
		using Field =
			typename apex::dsp::DynamicsState<double, SSLBusAttackTime, SSLBusReleaseTime>::Field;
		using DynamicsState =
			typename apex::dsp::DynamicsState<double, SSLBusAttackTime, SSLBusReleaseTime>;

	  public:
		/// @brief Constructs a `LevelDetectorSSL` with zereod shared state
		LevelDetectorSSL() noexcept = default;

		/// @brief Constructs a `LevelDetectorSSL` with the given shared state
		///
		/// @param state - The shared state
		explicit LevelDetectorSSL(DynamicsState* state) noexcept;

		/// @brief Move constructs a `LevelDetectorSSL` from the given one
		///
		/// @param detector - The `LevelDetectorSSL` to move
		LevelDetectorSSL(LevelDetectorSSL<double>&& detector) noexcept = default;

		~LevelDetectorSSL() noexcept override = default;

		/// @brief Sets the attack time to the given value
		///
		/// @param attack - The new attack time
		auto setAttackTime(SSLBusAttackTime attack) noexcept -> void;

		/// @brief Sets the release time to the given value
		///
		/// @param release - The new release time
		auto setReleaseTime(SSLBusReleaseTime release) noexcept -> void;

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		auto setSampleRate(size_t sampleRate) noexcept -> void override;

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		[[nodiscard]] auto process(double input) noexcept -> double override;

		auto operator=(LevelDetectorSSL<double>&& detector) noexcept
			-> LevelDetectorSSL<double>& = default;

	  private:
		DynamicsState DEFAULT_STATE = DynamicsState();
		DynamicsState* mState = &DEFAULT_STATE;
		double mY1N1 = 0.0;
		double mY2N1 = 0.0;

		static const constexpr double ATTACK_POINT_ONE_MS_S = 0.0001;
		static const constexpr double ATTACK_POINT_THREE_MS_S = 0.0003;
		static const constexpr double ATTACK_ONE_MS_S = 0.001;
		static const constexpr double ATTACK_THREE_MS_S = 0.003;
		static const constexpr double ATTACK_TEN_MS_S = 0.01;
		static const constexpr double ATTACK_THIRTY_MS_S = 0.03;

		static const constexpr SSLBusAttackTime DEFAULT_ATTACK = SSLBusAttackTime::TenMilliseconds;

		static const constexpr double RELEASE_POINT_ONE_S = 0.1;
		static const constexpr double RELEASE_POINT_THREE_S = 0.3;
		static const constexpr double RELEASE_POINT_SIX_S = 0.6;
		static const constexpr double RELEASE_ONE_POINT_TWO_S = 1.2;

		static const constexpr SSLBusReleaseTime DEFAULT_RELEASE
			= SSLBusReleaseTime::PointThreeSeconds;

		static const constexpr double AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47;
		static const constexpr double AUTO_RELEASE1_S = 0.04277;
		static const constexpr double AUTO_RELEASE2_S = 5.1;

		auto
		calculateAttackCoefficients(SSLBusAttackTime attack, size_t sampleRate) noexcept -> void;
		auto
		calculateReleaseCoefficients(SSLBusReleaseTime release, size_t sampleRate) noexcept -> void;

		/// @deprecated DO NOT USE, USE `setAttackTime(SSLBusAttackTime)`
		[[deprecated("Use `setAttackType` for this specific `LevelDetector`")]] auto
		setAttackTime(double attackMS) noexcept -> void override {
			juce::ignoreUnused(attackMS);
		}

		/// @deprecated DO NOT USE, USE `setReleaseTime(SSLBusReleaseTime)`
		[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]] auto
		setReleaseTime(double releaseMS) noexcept -> void override {
			juce::ignoreUnused(releaseMS);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
	};
} // namespace apex::dsp
