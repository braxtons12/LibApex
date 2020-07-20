#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex {
	namespace dsp {

		enum class SSLBusAttackTime {
			PointOneMilliseconds,
			PointThreeMilliseconds,
			OneMilliseconds,
			ThreeMilliseconds,
			TenMilliseconds,
			ThirtyMilliseconds
		};

		enum class SSLBusReleaseTime {
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
					typedef typename apex::dsp::DynamicsState<float, SSLBusAttackTime, SSLBusReleaseTime>::Field Field;
					typedef typename apex::dsp::DynamicsState<float, SSLBusAttackTime, SSLBusReleaseTime> DynamicsState;

				public:
					/// @brief Constructs a `LevelDetectorSSL` with zereod shared state
					LevelDetectorSSL() noexcept;

					/// @brief Constructs a `LevelDetectorSSL` with the given shared state
					/// 
					/// @param state - The shared state
					LevelDetectorSSL(DynamicsState* state) noexcept;

					/// @brief Move constructs a `LevelDetectorSSL` from the given one
					///
					/// @param detector - The `LevelDetectorSSL` to move
					LevelDetectorSSL(LevelDetectorSSL<float>&& detector) noexcept = default;

					~LevelDetectorSSL() noexcept override = default;

					/// @deprecated DO NOT USE, USE `setAttackType`
					[[deprecated("Use `setAttackType` for this specific `LevelDetector`")]]
						void setAttackTime(float attackMS) noexcept override {
							juce::ignoreUnused(attackMS);
						}

					/// @deprecated DO NOT USE, USE `setReleaseType`
					[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]]
						void setReleaseTime(float releaseMS) noexcept override {
							juce::ignoreUnused(releaseMS);
						}

					/// @brief Sets the attack time to the given value
					///
					/// @param attack - The new attack time
					void setAttackTime(SSLBusAttackTime attack) noexcept;

					/// @brief Sets the release time to the given value
					///
					/// @param release - The new release time
					void setReleaseTime(SSLBusReleaseTime release) noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					float process(float input) noexcept override;

					LevelDetectorSSL<float>& operator=(
							LevelDetectorSSL<float>&& detector) noexcept = default;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					float mY1N1 = 0.0f;
					float mY2N1 = 0.0f;

					static const constexpr float ATTACK_POINT_ONE_MS_S = 0.0001f;
					static const constexpr float ATTACK_POINT_THREE_MS_S = 0.0003f;
					static const constexpr float ATTACK_ONE_MS_S = 0.001f;
					static const constexpr float ATTACK_THREE_MS_S = 0.003f;
					static const constexpr float ATTACK_TEN_MS_S = 0.01f;
					static const constexpr float ATTACK_THIRTY_MS_S = 0.03f;

					static const constexpr float RELEASE_POINT_ONE_S = 0.1f;
					static const constexpr float RELEASE_POINT_THREE_S = 0.3f;
					static const constexpr float RELEASE_POINT_SIX_S = 0.6f;
					static const constexpr float RELEASE_ONE_POINT_TWO_S = 1.2f;

					static const constexpr float AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47f;
					static const constexpr float AUTO_RELEASE1_S = 0.04277f;
					static const constexpr float AUTO_RELEASE2_S = 5.1f;

					void calculateAttackCoefficients(SSLBusAttackTime attack, size_t sampleRate) noexcept;
					void calculateReleaseCoefficients(SSLBusReleaseTime release, size_t sampleRate) noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
			};

		/// @brief SSL-style Bus Compressor Level Detector
		template<>
			class LevelDetectorSSL<double> : public LevelDetector<double> {
				private:
					typedef typename apex::dsp::DynamicsState<double, SSLBusAttackTime, SSLBusReleaseTime>::Field Field;
					typedef typename apex::dsp::DynamicsState<double, SSLBusAttackTime, SSLBusReleaseTime> DynamicsState;

				public:
					/// @brief Constructs a `LevelDetectorSSL` with zereod shared state
					LevelDetectorSSL() noexcept;

					/// @brief Constructs a `LevelDetectorSSL` with the given shared state
					/// 
					/// @param state - The shared state
					LevelDetectorSSL(DynamicsState* state) noexcept;

					/// @brief Move constructs a `LevelDetectorSSL` from the given one
					///
					/// @param detector - The `LevelDetectorSSL` to move
					LevelDetectorSSL(LevelDetectorSSL<double>&& detector) noexcept = default;

					~LevelDetectorSSL() noexcept override = default;

					/// @deprecated DO NOT USE, USE `setAttackType`
					[[deprecated("Use `setAttackType` for this specific `LevelDetector`")]]
						void setAttackTime(double attackMS) noexcept override {
							juce::ignoreUnused(attackMS);
						}

					/// @deprecated DO NOT USE, USE `setReleaseType`
					[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]]
						void setReleaseTime(double releaseMS) noexcept override {
							juce::ignoreUnused(releaseMS);
						}

					/// @brief Sets the attack time to the given value
					///
					/// @param attack - The new attack time
					void setAttackTime(SSLBusAttackTime attack) noexcept;

					/// @brief Sets the release time to the given value
					///
					/// @param release - The new release time
					void setReleaseTime(SSLBusReleaseTime release) noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					double process(double input) noexcept override;

					LevelDetectorSSL<double>& operator=(
							LevelDetectorSSL<double>&& detector) noexcept = default;

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

					static const constexpr double RELEASE_POINT_ONE_S = 0.1;
					static const constexpr double RELEASE_POINT_THREE_S = 0.3;
					static const constexpr double RELEASE_POINT_SIX_S = 0.6;
					static const constexpr double RELEASE_ONE_POINT_TWO_S = 1.2;

					static const constexpr double AUTO_RELEASE_ATTACK2_MULTIPLIER = 14.47;
					static const constexpr double AUTO_RELEASE1_S = 0.04277;
					static const constexpr double AUTO_RELEASE2_S = 5.1;

					void calculateAttackCoefficients(SSLBusAttackTime attack, size_t sampleRate) noexcept;
					void calculateReleaseCoefficients(SSLBusReleaseTime release, size_t sampleRate) noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
			};
	}
}
