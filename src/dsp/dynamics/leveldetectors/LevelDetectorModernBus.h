#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex::dsp {
	/// @brief Modern-style Bus compressor Level Detector
	///
	/// @tparam FloatType - The floating point type to back operations
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class LevelDetectorModernBus : public LevelDetector<FloatType> {
	  private:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, FloatType, FloatType>;
		using LevelDetector = LevelDetector<FloatType>;

	  public:
		/// @brief Constructs a `LevelDetectorModernBus` with zeroed shared state
		LevelDetectorModernBus() noexcept = default;

		///@brief Constructs a `LevelDetectorModernBus` with the given shared state
		///
		///@param state - The shared state
		explicit LevelDetectorModernBus(DynamicsState* state) noexcept
			: LevelDetector(state, DetectorType::DecoupledSmooth) {
			LevelDetector::mState->setHasAutoRelease(true);
			LevelDetector::mState->template registerCallback<FloatType, DynamicsField::Attack>(
				[this](FloatType attack) { this->setAttackTime(attack); });
			LevelDetector::mState->template registerCallback<FloatType, DynamicsField::Release>(
				[this](FloatType release) { this->setReleaseTime(release); });
			LevelDetector::mState->template registerCallback<Hertz, DynamicsField::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
			LevelDetector::mState->template registerCallback<bool, DynamicsField::AutoRelease>(
				[this](bool enabled) { this->setAutoRelease(enabled); });
		}

		/// @brief Move constructs a `LevelDetectorModernBus` from the given one
		///
		/// @param detector - The `LevelDetectorModernBus` to move
		LevelDetectorModernBus(LevelDetectorModernBus&& detector) noexcept = default;
		~LevelDetectorModernBus() noexcept override = default;

		/// @brief Sets the attack time
		///
		/// @param attackSeconds - The attack time in seconds
		auto setAttackTime(FloatType attackSeconds) noexcept -> void override {
			juce::ignoreUnused(attackSeconds);
			LevelDetector::mState->setAttackCoefficient1(
				calculateAttackCoefficient1(LevelDetector::mState->getSampleRate()));
			if(LevelDetector::mState->getAutoReleaseEnabled()) {
				LevelDetector::mState->setAttackCoefficient2(
					calculateAttackCoefficient2(LevelDetector::mState->getSampleRate()));
			}
		}

		/// @brief Sets the release time
		///
		/// @param releaseSeconds - The release time in seconds
		auto setReleaseTime(FloatType releaseSeconds) noexcept -> void override {
			juce::ignoreUnused(releaseSeconds);
			auto sampleRate = LevelDetector::mState->getSampleRate();

			LevelDetector::mState->setReleaseCoefficient1(calculateReleaseCoefficient1(sampleRate));
			if(LevelDetector::mState->getAutoReleaseEnabled()) {
				LevelDetector::mState->setReleaseCoefficient2(
					calculateReleaseCoefficient2(sampleRate));
			}
		}

		/// @brief Sets whether auto release is enabled
		///
		/// @param autoRelease - Whether auto release should be enabled
		auto setAutoRelease(bool autoRelease) noexcept -> void {
			auto sampleRate = LevelDetector::mState->getSampleRate();

			LevelDetector::mState->setReleaseCoefficient1(calculateReleaseCoefficient1(sampleRate));
			if(autoRelease) {
				LevelDetector::mState->setAttackCoefficient2(
					calculateAttackCoefficient2(sampleRate));
				LevelDetector::mState->setReleaseCoefficient2(
					calculateReleaseCoefficient2(sampleRate));
			}
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		auto setSampleRate(Hertz sampleRate) noexcept -> void override {
			LevelDetector::mState->setAttackCoefficient1(
				LevelDetector::calculateAttackCoefficient1(sampleRate));
			LevelDetector::mState->setReleaseCoefficient1(calculateReleaseCoefficient1(sampleRate));

			if(LevelDetector::mState->getAutoReleaseEnabled()) {
				LevelDetector::mState->setAttackCoefficient2(
					calculateAttackCoefficient2(sampleRate));
				LevelDetector::mState->setReleaseCoefficient2(
					calculateReleaseCoefficient2(sampleRate));
			}
		}

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		[[nodiscard]] auto process(FloatType input) noexcept -> FloatType override {
			if(!LevelDetector::mState->getAutoReleaseEnabled()) {
				return LevelDetector::process(input);
			}
			else {
				auto y1n = mY1N1
						   + LevelDetector::mState->getAttackCoefficient1()
								 * General<FloatType>::max(input - LevelDetector::mYOut1,
														   narrow_cast<FloatType>(0.0))
						   - LevelDetector::mState->getReleaseCoefficient1() * mY1N1;
				auto y2n = mY2N1
						   + LevelDetector::mState->getAttackCoefficient2()
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

		auto
		operator=(LevelDetectorModernBus&& detector) noexcept -> LevelDetectorModernBus& = default;

	  private:
		FloatType mY1N1 = narrow_cast<FloatType>(0.0);
		FloatType mY2N1 = narrow_cast<FloatType>(0.0);
		static const constexpr FloatType AUTO_RELEASE_ATTACK2_MULTIPLIER
			= narrow_cast<FloatType>(14.47);
		static const constexpr FloatType AUTO_RELEASE1_S = narrow_cast<FloatType>(0.04277);
		static const constexpr FloatType AUTO_RELEASE2_S = narrow_cast<FloatType>(2.0);

	  protected:
		[[nodiscard]] inline auto
		calculateAttackCoefficient2(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (LevelDetector::mState->getAttack() * AUTO_RELEASE_ATTACK2_MULTIPLIER
				   * narrow_cast<FloatType>(sampleRate)));
		}

		[[nodiscard]] inline auto
		calculateReleaseCoefficient1(Hertz sampleRate) noexcept -> FloatType override {
			if(LevelDetector::mState->getAutoReleaseEnabled()) {
				return Exponentials<FloatType>::exp(
					narrow_cast<FloatType>(-1.0)
					/ (AUTO_RELEASE1_S * narrow_cast<FloatType>(sampleRate)));
			}
			else {
				return LevelDetector::calculateReleaseCoefficient1(sampleRate);
			}
		}

		[[nodiscard]] virtual inline auto
		calculateReleaseCoefficient2(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (AUTO_RELEASE2_S * narrow_cast<FloatType>(sampleRate)));
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorModernBus)
	};
} // namespace apex::dsp
