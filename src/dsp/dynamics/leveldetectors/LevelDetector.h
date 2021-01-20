/// @brief Contains base implementatin for Level Detector Component of a Dynamic Range Processor's
/// Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and
/// Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

//#define TESTING_LEVELDETECTOR

namespace apex::dsp {
	/// @brief The different detector topologies
	enum class DetectorType
	{
		NonCorrected,
		Branching,
		Decoupled,
		BranchingSmooth,
		DecoupledSmooth
	};

	/// @brief Base Level Detector used for the level detection portion of a
	/// Dynamic Range Processor's Sidechain
	///
	/// @tparam FloatType - The floating point type to back operations, either float or double
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class LevelDetector {
	  private:
		using DynamicsState = typename apex::dsp::DynamicsState<FloatType, FloatType, FloatType>;

	  public:
		/// @brief Constructs a default `LevelDetector` (zeroed shared state)
		LevelDetector() noexcept = default;

		/// @brief Constructs a `LevelDetector` of the given type
		/// with the given shared state
		///
		/// @param state - The shared state
		/// @param type - The detector type
		explicit LevelDetector(DynamicsState* state, DetectorType type) noexcept
			: mState(state), mType(type) {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Creating Base Level Detector");
#endif
			mState->template registerCallback<FloatType, DynamicsField::Attack>(
				[this](FloatType attack) { this->setAttackTime(attack); });
			mState->template registerCallback<FloatType, DynamicsField::Release>(
				[this](FloatType release) { this->setReleaseTime(release); });
			mState->template registerCallback<Hertz, DynamicsField::SampleRate>(
				[this](Hertz sampleRate) { this->setSampleRate(sampleRate); });
		}

		/// @brief Move constructs a `LevelDetector` from the given one
		///
		/// @param detector - The `LevelDetector` to move
		LevelDetector(LevelDetector&& detector) noexcept = default;
		virtual ~LevelDetector() noexcept = default;

		/// @brief Generates the detected level from the given input
		///
		/// @param input - The input to detect on
		///
		/// @return - The detected level
		[[nodiscard]] virtual auto process(FloatType input) noexcept -> FloatType {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Processing Input");
#endif
			switch(mType) {
				case DetectorType::NonCorrected: return processNonCorrected(input);
				case DetectorType::Branching: return processBranching(input);
				case DetectorType::Decoupled: return processDecoupled(input);
				case DetectorType::BranchingSmooth: return processBranchingSmooth(input);
				case DetectorType::DecoupledSmooth: return processDecoupledSmooth(input);
			}
			return narrow_cast<FloatType>(0.0);
		}

		/// @brief Resets this level detector to an initial state
		virtual inline auto reset() noexcept -> void {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Resetting");
#endif
			mYOut1 = narrow_cast<FloatType>(0.0);
			mYTempStage1 = narrow_cast<FloatType>(0.0);
		}

		/// @brief Sets the attack time to the given value
		///
		/// @param attackSeconds - The new attack time, in seconds
		virtual inline auto setAttackTime(FloatType attackSeconds) noexcept -> void {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Updating Attack Time");
#endif
			juce::ignoreUnused(attackSeconds);
			mState->setAttackCoefficient1(calculateAttackCoefficient1(mState->getSampleRate()));
		}

		/// @brief Sets the release time to the given value
		///
		/// @param releaseSeconds - The new release time, in seconds
		virtual inline auto setReleaseTime(FloatType releaseSeconds) noexcept -> void {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Updating Release Time");
#endif
			juce::ignoreUnused(releaseSeconds);
			mState->setReleaseCoefficient1(calculateReleaseCoefficient1(mState->getSampleRate()));
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		virtual inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
			mState->setAttackCoefficient1(calculateAttackCoefficient1(sampleRate));
			mState->setReleaseCoefficient1(calculateReleaseCoefficient1(sampleRate));
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage(
				"Base LevelDetector AttackSeconds: " + juce::String(mState->getAttack())
				+ "\nBase LevelDetector ReleaseSeconds: " + juce::String(mState->getRelease())
				+ "\nBase LevelDetector AttackCoeff: "
				+ juce::String(mState->getAttackCoefficient1())
				+ "\nBase LevelDetector ReleaseCoeff: "
				+ juce::String(mState->getReleaseCoefficient1()));
#endif
		}

		virtual inline auto setDetectorType(DetectorType type) noexcept -> void {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Updating Detector Type");
#endif
			mType = type;
		}

		[[nodiscard]] virtual inline auto getDetectorType() const noexcept -> DetectorType {
			return mType;
		}

		auto operator=(LevelDetector&& detector) noexcept -> LevelDetector& = default;

	  protected:
		DynamicsState DEFAULT_STATE = DynamicsState();
		DynamicsState* mState = &DEFAULT_STATE;
		// y[n-1]
		FloatType mYOut1 = narrow_cast<FloatType>(0.0);
		// used in decoupled calculations to store y_1[n-1]
		FloatType mYTempStage1 = narrow_cast<FloatType>(0.0);
		DetectorType mType = DetectorType::NonCorrected;

		[[nodiscard]] virtual auto processNonCorrected(FloatType input) noexcept -> FloatType {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Processing NonCorrected");
#endif
			// y[n] = releaseCoeff * y[n-1] + (1 - attackCoeff) * max(x[n] - y[n-1], 0)
			auto yn = mState->getReleaseCoefficient1() * mYOut1
					  + (narrow_cast<FloatType>(1.0) - mState->getAttackCoefficient1())
							* General<FloatType>::max(input - mYOut1, narrow_cast<FloatType>(0.0));
			mYOut1 = yn;
			return yn;
		}

		[[nodiscard]] virtual auto processBranching(FloatType input) noexcept -> FloatType {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Processing Branching");
#endif
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n], x[n] > y[n-1]
			// y[n] = { releaseCoeff * y[n-1],                           x[n] <= y[n-1]
			//       {
			auto yn
				= (input > mYOut1 ?
						 (mState->getAttackCoefficient1() * mYOut1
						+ (narrow_cast<FloatType>(1.0) - mState->getAttackCoefficient1()) * input) :
						 (mState->getReleaseCoefficient1() * mYOut1));
			mYOut1 = yn;
			return yn;
		}

		[[nodiscard]] virtual auto processDecoupled(FloatType input) noexcept -> FloatType {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Processing Decoupled");
#endif
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1])
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]
			auto ytempn
				= General<FloatType>::max(input, mState->getReleaseCoefficient1() * mYTempStage1);
			auto yn = mState->getAttackCoefficient1() * mYOut1
					  + (narrow_cast<FloatType>(1.0) - mState->getAttackCoefficient1()) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		[[nodiscard]] virtual auto processBranchingSmooth(FloatType input) noexcept -> FloatType {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Processing Branching Smooth");
#endif
			//       { attackCoeff * y[n-1] + (1 - attackCoeff) * x[n],   x[n] > y[n-1]
			// y[n] = { releaseCoeff * y[n-1] + (1 - releaseCoeff) * x[n], x[n] <= y[n-1]
			//       {

			auto one = narrow_cast<FloatType>(1.0);
			auto yn = (input > mYOut1 ? (mState->getAttackCoefficient1() * mYOut1
										 + (one - mState->getAttackCoefficient1()) * input) :
										  (mState->getReleaseCoefficient1() * mYOut1
										 + (one - mState->getReleaseCoefficient1()) * input));
			mYOut1 = yn;
			return yn;
		}

		[[nodiscard]] virtual auto processDecoupledSmooth(FloatType input) noexcept -> FloatType {
#ifdef TESTING_LEVELDETECTOR
			Logger::LogMessage("Base Level Detector Processing Decoupled Smooth");
#endif
			// y_1[n] = max(x[n], releaseCoeff * y_1[n-1] + (1 - releaseCoeff) * input)
			// y[n] = attackCoeff * y[n-1] + (1 - attackCoeff) * y_1[n]

			auto one = narrow_cast<FloatType>(1.0);
			auto ytempn
				= General<FloatType>::max(input,
										  mState->getReleaseCoefficient1() * mYTempStage1
											  + (one - mState->getReleaseCoefficient1()) * input);
			auto yn = mState->getAttackCoefficient1() * mYOut1
					  + (one - mState->getAttackCoefficient1()) * ytempn;
			mYTempStage1 = ytempn;
			mYOut1 = yn;
			return yn;
		}

		[[nodiscard]] virtual inline auto
		calculateAttackCoefficient1(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mState->getAttack() * narrow_cast<FloatType>(sampleRate)));
		}

		[[nodiscard]] virtual inline auto
		calculateReleaseCoefficient1(Hertz sampleRate) noexcept -> FloatType {
			return Exponentials<FloatType>::exp(
				narrow_cast<FloatType>(-1.0)
				/ (mState->getRelease() * narrow_cast<FloatType>(sampleRate)));
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
	};
} // namespace apex::dsp
