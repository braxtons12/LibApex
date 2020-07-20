/// @brief Contains base implementatin for Level Detector Component of a Dynamic Range Processor's
/// Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

namespace apex {
	namespace dsp {
		/// @brief Base Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class LevelDetector {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
			};

		/// @brief Base Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetector<float> {
				private:
					typedef typename apex::dsp::DynamicsState<float, float, float>::Field Field;
					typedef typename apex::dsp::DynamicsState<float, float, float> DynamicsState;

				public:
					/// @brief The different detector topologies
					enum class DetectorType {
						NonCorrected,
						Branching,
						Decoupled,
						BranchingSmooth,
						DecoupledSmooth
					};

					/// @brief Constructs a default `LevelDetector` (zeroed shared state)
					LevelDetector() noexcept = default;

					/// @brief Constructs a `LevelDetector` of the given type
					/// with the given shared state
					///
					/// @param state - The shared state
					/// @param type - The detector type
					LevelDetector(DynamicsState* state,
							DetectorType type = DetectorType::NonCorrected) noexcept;

					/// @brief Move constructs a `LevelDetector` from the given one
					///
					/// @param detector - The `LevelDetector` to move
					LevelDetector(LevelDetector<float>&& detector) noexcept = default;
					virtual ~LevelDetector() noexcept = default;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					virtual float process(float input) noexcept;

					/// @brief Resets this level detector to an initial state
					virtual void reset() noexcept;

					/// @brief Sets the attack time to the given value
					///
					/// @param attackSeconds - The new attack time, in seconds
					virtual void setAttackTime(float attackSeconds) noexcept;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseSeconds - The new release time, in seconds
					virtual void setReleaseTime(float releaseSeconds) noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept;

					LevelDetector<float>& operator=(LevelDetector<float>&& detector) noexcept = default;

				protected:
					//y[n-1]
					float mYOut1 = 0.0f;
					//used in decoupled calculations to store y_1[n-1]
					float mYTempStage1 = 0.0f;
					DetectorType mType = DetectorType::NonCorrected;

					virtual float processNonCorrected(float input) noexcept;
					virtual float processBranching(float input) noexcept;
					virtual float processDecoupled(float input) noexcept;
					virtual float processBranchingSmooth(float input) noexcept;
					virtual float processDecoupledSmooth(float input) noexcept;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
			};

		/// @brief Base Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetector<double> {
				private:
					typedef typename apex::dsp::DynamicsState<double, double, double>::Field Field;
					typedef typename apex::dsp::DynamicsState<double, double, double> DynamicsState;

				public:
					/// @brief The different detector topologies
					enum class DetectorType {
						NonCorrected,
						Branching,
						Decoupled,
						BranchingSmooth,
						DecoupledSmooth
					};

					/// @brief Constructs a default `LevelDetector` (zeroed shared state)
					LevelDetector() noexcept = default;

					/// @brief Constructs a `LevelDetector` of the given type
					/// with the given shared state
					///
					/// @param state - The shared state
					/// @param type - The detector type
					LevelDetector(DynamicsState* state,
							DetectorType type = DetectorType::NonCorrected) noexcept;

					/// @brief Move constructs a `LevelDetector` from the given one
					///
					/// @param detector - The `LevelDetector` to move
					LevelDetector(LevelDetector<double>&& detector) noexcept = default;
					virtual ~LevelDetector() noexcept = default;

					/// @brief Generates the detected level from the given input
					///
					/// @param input - The input to detect on
					///
					/// @return - The detected level
					virtual double process(double input) noexcept;

					/// @brief Resets this level detector to an initial state
					virtual void reset() noexcept;

					/// @brief Sets the attack time to the given value
					///
					/// @param attackSeconds - The new attack time, in seconds
					virtual void setAttackTime(double attackSeconds) noexcept;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseSeconds - The new release time, in seconds
					virtual void setReleaseTime(double releaseSeconds) noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept;

					LevelDetector<double>& operator=(LevelDetector<double>&& detector) noexcept = default;

				protected:
					//y[n-1]
					double mYOut1 = 0.0;
					//used in decoupled calculations to store y_1[n-1]
					double mYTempStage1 = 0.0;
					DetectorType mType = DetectorType::NonCorrected;

					virtual double processNonCorrected(double input) noexcept;
					virtual double processBranching(double input) noexcept;
					virtual double processDecoupled(double input) noexcept;
					virtual double processBranchingSmooth(double input) noexcept;
					virtual double processDecoupledSmooth(double input) noexcept;

				private:
					DynamicsState DEFAULT_STATE = DynamicsState();
					DynamicsState* mState = &DEFAULT_STATE;
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
			};
	}
}
