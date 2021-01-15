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
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class LevelDetectorSSL : public LevelDetector<FloatType> {
	  private:
		using DynamicsState =
			typename apex::dsp::DynamicsState<float, SSLBusAttackTime, SSLBusReleaseTime>;
		using LevelDetector = LevelDetector<FloatType>;

	  public:
		/// @brief Constructs a `LevelDetectorSSL` with zereod shared state
		LevelDetectorSSL() noexcept = default;

		/// @brief Constructs a `LevelDetectorSSL` with the given shared state
		///
		/// @param state - The shared state
		explicit LevelDetectorSSL(DynamicsState* state) noexcept : LevelDetector(state) {
			LevelDetector::mState->setHasAutoRelease(true);
			LevelDetector::mState
				->template registerCallback<SSLBusAttackTime, DynamicsField::Attack>(
					[this](SSLBusAttackTime attack) { this->setAttackTime(attack); });
			LevelDetector::mState
				->template registerCallback<SSLBusReleaseTime, DynamicsField::Release>(
					[this](SSLBusReleaseTime release) { this->setReleaseTime(release); });
			LevelDetector::mState->template registerCallback<Hertz, DynamicsField::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
			LevelDetector::mState->template registerCallback<bool, DynamicsField::AutoRelease>(
				[this](bool enabled) {
					if(enabled) {
						this->setReleaseTime(SSLBusReleaseTime::Auto);
					}
					else {
						this->setReleaseTime(this->LevelDetector::mState->getRelease());
					}
				});
		}

		/// @brief Move constructs a `LevelDetectorSSL` from the given one
		///
		/// @param detector - The `LevelDetectorSSL` to move
		LevelDetectorSSL(LevelDetectorSSL&& detector) noexcept = default;

		~LevelDetectorSSL() noexcept override = default;

		/// @brief Sets the attack time to the given value
		///
		/// @param attack - The new attack time
		auto setAttackTime(SSLBusAttackTime attack) noexcept -> void {
			calculateAttackCoefficients(attack, LevelDetector::mState->getSampleRate());
		}

		/// @brief Sets the release time to the given value
		///
		/// @param release - The new release time
		auto setReleaseTime(SSLBusReleaseTime release) noexcept -> void {
			calculateReleaseCoefficients(release, LevelDetector::mState->getSampleRate());
			if(release == SSLBusReleaseTime::Auto) {
				calculateAttackCoefficients(LevelDetector::mState->getAttack(),
											LevelDetector::mState->getSampleRate());
			}
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		auto setSampleRate(Hertz sampleRate) noexcept -> void override {
			calculateAttackCoefficients(LevelDetector::mState->getAttack(), sampleRate);
			calculateReleaseCoefficients(LevelDetector::mState->getRelease(), sampleRate);
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		[[nodiscard]] auto process(FloatType input) noexcept -> FloatType override {
			if(LevelDetector::mState->getRelease() != SSLBusReleaseTime::Auto) {
				return LevelDetector::process(input);
			}
			else {
				auto y1n = mY1N1
						   + LevelDetector::mState->getAttackCoefficient1()
								 * General<FloatType>::max(input - LevelDetector::mYOut1,
														   narrow_cast<FloatType>(0.0))
						   - LevelDetector::mState->getReleaseCoefficient1() * mY1N1;
				auto y2n = mY2N1 * LevelDetector::mState->getAttackCoefficient2()
							   * General<FloatType>::max(input - LevelDetector::mYOut1,
														 narrow_cast<FloatType>(0.0))
						   - LevelDetector::mState->getReleaseCoefficient2() * mY2N1;
				auto yn = y1n + y2n;
				mY1N1 = y1n;
				mY2N1 = y2n;
				LevelDetector::mYOut1 = yn;
				return yn;
			}
		}

		auto operator=(LevelDetectorSSL&& detector) noexcept -> LevelDetectorSSL& = default;

	  private:
		FloatType mY1N1 = narrow_cast<FloatType>(0.0);
		FloatType mY2N1 = narrow_cast<FloatType>(0.0);

		static const constexpr FloatType ATTACK_POINT_ONE_MS_S = narrow_cast<FloatType>(0.0001);
		static const constexpr FloatType ATTACK_POINT_THREE_MS_S = narrow_cast<FloatType>(0.0003);
		static const constexpr FloatType ATTACK_ONE_MS_S = narrow_cast<FloatType>(0.001);
		static const constexpr FloatType ATTACK_THREE_MS_S = narrow_cast<FloatType>(0.003);
		static const constexpr FloatType ATTACK_TEN_MS_S = narrow_cast<FloatType>(0.01);
		static const constexpr FloatType ATTACK_THIRTY_MS_S = narrow_cast<FloatType>(0.03);

		static const constexpr SSLBusAttackTime DEFAULT_ATTACK = SSLBusAttackTime::TenMilliseconds;

		static const constexpr FloatType RELEASE_POINT_ONE_S = narrow_cast<FloatType>(0.1);
		static const constexpr FloatType RELEASE_POINT_THREE_S = narrow_cast<FloatType>(0.3);
		static const constexpr FloatType RELEASE_POINT_SIX_S = narrow_cast<FloatType>(0.6);
		static const constexpr FloatType RELEASE_ONE_POINT_TWO_S = narrow_cast<FloatType>(1.2);

		static const constexpr SSLBusReleaseTime DEFAULT_RELEASE
			= SSLBusReleaseTime::PointThreeSeconds;

		static const constexpr FloatType AUTO_RELEASE_ATTACK2_MULTIPLIER
			= narrow_cast<FloatType>(14.47);
		static const constexpr FloatType AUTO_RELEASE1_S = narrow_cast<FloatType>(0.04277);
		static const constexpr FloatType AUTO_RELEASE2_S = narrow_cast<FloatType>(5.1);

		auto
		calculateAttackCoefficients(SSLBusAttackTime attack, Hertz sampleRate) noexcept -> void {
			auto attackSeconds = narrow_cast<FloatType>(0.0);
			switch(attack) {
				case SSLBusAttackTime::PointOneMilliseconds:
					attackSeconds = ATTACK_POINT_ONE_MS_S;
					break;
				case SSLBusAttackTime::PointThreeMilliseconds:
					attackSeconds = ATTACK_POINT_THREE_MS_S;
					break;
				case SSLBusAttackTime::OneMilliseconds: attackSeconds = ATTACK_ONE_MS_S; break;
				case SSLBusAttackTime::ThreeMilliseconds: attackSeconds = ATTACK_THREE_MS_S; break;
				case SSLBusAttackTime::TenMilliseconds: attackSeconds = ATTACK_TEN_MS_S; break;
				case SSLBusAttackTime::ThirtyMilliseconds:
					attackSeconds = ATTACK_THIRTY_MS_S;
					break;
			}
			LevelDetector::mState->setAttackCoefficient1(Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (attackSeconds * narrow_cast<FloatType>(sampleRate))));
			if(LevelDetector::mState->getRelease() == SSLBusReleaseTime::Auto) {
				auto attack2Seconds = attackSeconds * AUTO_RELEASE_ATTACK2_MULTIPLIER;
				LevelDetector::mState->setAttackCoefficient2(Exponentials<FloatType>::exp(
					narrow_cast<FloatType>(-1.0)
					/ (attack2Seconds * narrow_cast<FloatType>(sampleRate))));
			}
		}

		auto
		calculateReleaseCoefficients(SSLBusReleaseTime release, Hertz sampleRate) noexcept -> void {
			if(release == SSLBusReleaseTime::Auto) {
				auto releaseSeconds = narrow_cast<FloatType>(0.0);
				switch(release) {
					case SSLBusReleaseTime::PointOneSeconds:
						releaseSeconds = RELEASE_POINT_ONE_S;
						break;
					case SSLBusReleaseTime::PointThreeSeconds:
						releaseSeconds = RELEASE_POINT_THREE_S;
						break;
					case SSLBusReleaseTime::PointSixSeconds:
						releaseSeconds = RELEASE_POINT_SIX_S;
						break;
					case SSLBusReleaseTime::OnePointTwoSeconds:
						releaseSeconds = RELEASE_ONE_POINT_TWO_S;
						break;
					case SSLBusReleaseTime::Auto: break;
				}
				calculateAttackCoefficients(LevelDetector::mState->getAttack(), sampleRate);
				LevelDetector::mState->setReleaseCoefficient1(Exponentials<FloatType>::exp(
					narrow_cast<FloatType>(-1.0)
					/ (releaseSeconds * narrow_cast<FloatType>(sampleRate))));
			}
			else {
				LevelDetector::mState->setReleaseCoefficient1(Exponentials<FloatType>::exp(
					narrow_cast<FloatType>(-1.0)
					/ (AUTO_RELEASE1_S * narrow_cast<FloatType>(sampleRate))));
				LevelDetector::mState->setReleaseCoefficient2(Exponentials<FloatType>::exp(
					narrow_cast<FloatType>(-1.0)
					/ (AUTO_RELEASE2_S * narrow_cast<FloatType>(sampleRate))));
			}
		}

		/// @deprecated DO NOT USE, USE `setAttackTime(SSLBusAttackTime)`
		[[deprecated("Use `setAttackType` for this specific `LevelDetector`")]] auto
		setAttackTime(FloatType attackMS) noexcept -> void override {
			juce::ignoreUnused(attackMS);
		}

		/// @deprecated DO NOT USE, USE `setReleaseTime(SSLBusReleaseTime)`
		[[deprecated("Use `setReleaseType` for this specific `LevelDetector`")]] auto
		setReleaseTime(FloatType releaseMS) noexcept -> void override {
			juce::ignoreUnused(releaseMS);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorSSL)
	};
} // namespace apex::dsp
