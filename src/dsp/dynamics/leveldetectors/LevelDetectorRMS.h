/// @brief Contains base implementatin for an RMS Level Detector Component of a Dynamic Range Processor's
/// Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"
#include "../DynamicsState.h"

namespace apex {
	namespace dsp {

		/// @brief Base RMS Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class LevelDetectorRMS : public LevelDetector<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
			};

		/// @brief Base RMS Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetectorRMS<float> : public LevelDetector<float> {
				private:
					using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
					using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

				public:
					LevelDetectorRMS() noexcept = default;

					/// @brief Constructs a `LevelDetector` of the given type
					/// with the given shared state
					///
					/// @param state - The shared state
					/// @param type - The detector type
					LevelDetectorRMS(DynamicsState* state,
							DetectorType type = DetectorType::NonCorrected) noexcept;

					/// @brief Move contructs an `RMSLevelDetector` from the given one
					///
					/// @param detector - The `RMSLevelDetector` to move
					LevelDetectorRMS(LevelDetectorRMS<float>&& detector) noexcept = default;
					virtual ~LevelDetectorRMS() noexcept override = default;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					virtual float process(float input) noexcept override;

					/// @brief Resets this level detector to an initial state
					virtual void reset() noexcept override;

					/// @brief Sets the attack time to the given value
					///
					/// @param attackSeconds - The new attack time, in seconds
					virtual void setAttackTime(float attackSeconds) noexcept override;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseSeconds - The new release time, in seconds
					virtual void setReleaseTime(float releaseSeconds) noexcept override;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept override;

					LevelDetectorRMS<float>& operator=(LevelDetectorRMS<float>&& detector) noexcept = default;

				protected:
					//y[n-1] squared
					float mYSquared1 = 0.0f;
					float mRMSSeconds = 0.1f;
					float mRMSCoeff = 0.9997732683f;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
			};

		/// @brief Base RMS Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetectorRMS<double> : public LevelDetector<double> {
				private:
					using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
					using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

				public:
					LevelDetectorRMS() noexcept = default;

					/// @brief Constructs a `LevelDetector` of the given type
					/// with the given shared state
					///
					/// @param state - The shared state
					/// @param type - The detector type
					LevelDetectorRMS(DynamicsState* state,
							DetectorType type = DetectorType::NonCorrected) noexcept;

					/// @brief Move contructs an `RMSLevelDetector` from the given one
					///
					/// @param detector - The `RMSLevelDetector` to move
					LevelDetectorRMS(LevelDetectorRMS<double>&& detector) noexcept = default;
					virtual ~LevelDetectorRMS() noexcept override = default;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					virtual double process(double input) noexcept override;

					/// @brief Resets this level detector to an initial state
					virtual void reset() noexcept override;

					/// @brief Sets the attack time to the given value
					///
					/// @param attackSeconds - The new attack time, in seconds
					virtual void setAttackTime(double attackSeconds) noexcept override;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseSeconds - The new release time, in seconds
					virtual void setReleaseTime(double releaseSeconds) noexcept override;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept override;

					LevelDetectorRMS<double>& operator=(LevelDetectorRMS<double>&& detector) noexcept = default;

				protected:
					//y[n-1] squared
					double mYSquared1 = 0.0;
					double mRMSSeconds = 0.1;
					double mRMSCoeff = 0.9997732683;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
			};
	}
}
