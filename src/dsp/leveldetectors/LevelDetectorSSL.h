#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex {
	namespace dsp {

		/// @brief SSL-style Bus Compressor Level Detector
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class LevelDetectorSSL : LevelDetector<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
			};

		/// @brief SSL-style Bus Compressor Level Detector
		template<>
			class LevelDetectorSSL<float> : LevelDetector<float> {
				public:
					enum AttackType {
						PointOneMilliseconds = 0,
						PointThreeMilliseconds,
						OneMilliseconds,
						ThreeMilliseconds,
						TenMilliseconds,
						ThirtyMilliseconds
					};

					enum ReleaseType {
						PointOneSeconds = 0,
						PointThreeSeconds,
						PointSixSeconds,
						OnePointTwoSeconds,
						Auto
					};

					/// @brief Creates a `LevelDetectorSSL` with the default parameters of:
					/// * attack: 3ms
					/// * release: 600ms
					/// * sample rate: 44100Hz
					LevelDetectorSSL() noexcept;

					/// @brief Creates a `LevelDetectorSSL` with the given parameters
					///
					/// @param attck - The attack type to use
					/// @param release - The release type to use
					/// @param sampleRate - The sample rate, in Hertz
					LevelDetectorSSL(AttackType attck, ReleaseType release, size_t sampleRate) noexcept;

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

					/// @deprecated DO NOT USE, USE `getAttackType`
					[[deprecated("Use `getAttackType` for this specific `LevelDetector`")]]
						float getAttackTime() const noexcept override {
							return 0.0f;
						}

					/// @deprecated DO NOT USE, USE `setReleaseType`
					[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]]
						void setReleaseTime(float releaseMS) noexcept override {
							juce::ignoreUnused(releaseMS);
						}

					/// @deprecated DO NOT USE, USE `getReleaseType`
					[[deprecated("Use `getReleaseType` for this specific `LevelDetector`")]]
						float getReleaseTime() const noexcept override {
							return 0.0f;
						}

					/// @brief Sets the attack type to the given value
					///
					/// @param attack - The new attack type
					void setAttackType(AttackType attack) noexcept;

					/// @brief Returns the current attack type
					///
					/// @return - The current attack type
					AttackType getAttackType() const noexcept;

					/// @brief Sets the release type to the given value
					///
					/// @param release - The new release type
					void setReleaseType(ReleaseType release) noexcept;

					/// @brief Returns the current release type
					///
					/// @return - The current release type
					ReleaseType getReleaseType() const noexcept;

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
					AttackType mAttackType = AttackType::ThreeMilliseconds;
					ReleaseType mReleaseType = ReleaseType::PointSixSeconds;
					float mAutoReleaseAttack2Coeff = 0.9994777743f;
					float mAutoRelease1Coeff = 0.999469962f;
					float mAutoRelease2Coeff = 0.9999955538f;
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

					static const constexpr float INITIAL_ATTACK_MS = 3.0f;
					static const constexpr float INITIAL_RELEASE_MS = 600.0f;
					static const constexpr size_t INITIAL_SAMPLERATE = 44100;

					void updateCoefficients() noexcept;
					void calculateAttacks() noexcept;
					void calculateReleases() noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
			};


		/// @brief SSL-style Bus Compressor Level Detector
		template<>
			class LevelDetectorSSL<double> : LevelDetector<double> {
				public:
					enum AttackType {
						PointOneMilliseconds = 0,
						PointThreeMilliseconds,
						OneMilliseconds,
						ThreeMilliseconds,
						TenMilliseconds,
						ThirtyMilliseconds
					};

					enum ReleaseType {
						PointOneSeconds = 0,
						PointThreeSeconds,
						PointSixSeconds,
						OnePointTwoSeconds,
						Auto
					};

					/// @brief Creates a `LevelDetectorSSL` with the default parameters of:
					/// * attack: 3ms
					/// * release: 600ms
					/// * sample rate: 44100Hz
					LevelDetectorSSL() noexcept;

					/// @brief Creates a `LevelDetectorSSL` with the given parameters
					///
					/// @param attck - The attack type to use
					/// @param release - The release type to use
					/// @param sampleRate - The sample rate, in Hertz
					LevelDetectorSSL(AttackType attck, ReleaseType release, size_t sampleRate) noexcept;

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

					/// @deprecated DO NOT USE, USE `getAttackType`
					[[deprecated("Use `getAttackType` for this specific `LevelDetector`")]]
						double getAttackTime() const noexcept override {
							return 0.0;
						}

					/// @deprecated DO NOT USE, USE `setReleaseType`
					[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]]
						void setReleaseTime(double releaseMS) noexcept override {
							juce::ignoreUnused(releaseMS);
						}

					/// @deprecated DO NOT USE, USE `getReleaseType`
					[[deprecated("Use `getReleaseType` for this specific `LevelDetector`")]]
						double getReleaseTime() const noexcept override {
							return 0.0;
						}

					/// @brief Sets the attack type to the given value
					///
					/// @param attack - The new attack type
					void setAttackType(AttackType attack) noexcept;

					/// @brief Returns the current attack type
					///
					/// @return - The current attack type
					AttackType getAttackType() const noexcept;

					/// @brief Sets the release type to the given value
					///
					/// @param release - The new release type
					void setReleaseType(ReleaseType release) noexcept;

					/// @brief Returns the current release type
					///
					/// @return - The current release type
					ReleaseType getReleaseType() const noexcept;

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
					AttackType mAttackType = AttackType::ThreeMilliseconds;
					ReleaseType mReleaseType = ReleaseType::PointSixSeconds;
					double mAutoReleaseAttack2Coeff = 0.9994777743;
					double mAutoRelease1Coeff = 0.999469962;
					double mAutoRelease2Coeff = 0.9999955538;
					double mY1N1 = 0.0f;
					double mY2N1 = 0.0f;

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

					static const constexpr double INITIAL_ATTACK_MS = 3.0;
					static const constexpr double INITIAL_RELEASE_MS = 600.0;
					static const constexpr size_t INITIAL_SAMPLERATE = 44100;

					void updateCoefficients() noexcept;
					void calculateAttacks() noexcept;
					void calculateReleases() noexcept;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
			};
	}
}
