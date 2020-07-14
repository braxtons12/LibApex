/// @brief Contains base implementatin for Level Detector Component of a Dynamic Range Processor's
/// Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

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
				public:
					/// @brief The different detector topologies
					enum DetectorType {
						NonCorrected = 0,
						Branching,
						Decoupled,
						BranchingSmooth,
						DecoupledSmooth
					};

					LevelDetector() noexcept = default;

					/// @brief Constructs a `LevelDetector` with the given parameters
					///
					/// @param attackMS - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					/// @param type - The detector topology to use
					LevelDetector(float attackMS, float releaseMS, size_t sampleRate,
							DetectorType type = NonCorrected) noexcept;

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
					/// @param attackMS - The new attack time, in milliseconds
					virtual void setAttackTime(float attackMS) noexcept;

					/// @brief Returns the current attack time
					///
					/// @return - The attack time, in milliseconds
					virtual float getAttackTime() const noexcept;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseMS - The new release time, in milliseconds
					virtual void setReleaseTime(float releaseMS) noexcept;

					/// @brief Returns the current release time
					///
					/// @return - The release time, in milliseconds
					virtual float getReleaseTime() const noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the current sample rate
					///
					/// @return - The sample rate, in Hertz
					virtual size_t getSampleRate() const noexcept;

					LevelDetector<float>& operator=(LevelDetector<float>&& detector) noexcept = default;

				protected:
					float mAttackSeconds = 0.01f;
					float mReleaseSeconds = 0.05f;
					size_t mSampleRate = 44100;
					float mAttackCoeff = 0.9977349953f;
					float mReleaseCoeff = 0.9995465881f;
					//y[n-1]
					float mYOut1 = 0.0f;
					//used in decoupled calculations to store y_1[n-1]
					float mYTempStage1 = 0.0f;
					DetectorType mType = NonCorrected;

					virtual float processNonCorrected(float input) noexcept;
					virtual float processBranching(float input) noexcept;
					virtual float processDecoupled(float input) noexcept;
					virtual float processBranchingSmooth(float input) noexcept;
					virtual float processDecoupledSmooth(float input) noexcept;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
			};

		/// @brief Base Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetector<double> {
				public:
					/// @brief The different detector topologies
					enum DetectorType {
						NonCorrected = 0,
						Branching,
						Decoupled,
						BranchingSmooth,
						DecoupledSmooth
					};

					LevelDetector() noexcept = default;

					/// @brief Constructs a `LevelDetector` with the given parameters
					///
					/// @param attackMS - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					/// @param type - The detector topology to use
					LevelDetector(double attackMS, double releaseMS, size_t sampleRate,
							DetectorType = NonCorrected) noexcept;

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
					/// @param attackMS - The new attack time, in milliseconds
					virtual void setAttackTime(double attackMS) noexcept;

					/// @brief Returns the current attack time
					///
					/// @return - The attack time, in milliseconds
					virtual double getAttackTime() const noexcept;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseMS - The new release time, in milliseconds
					virtual void setReleaseTime(double releaseMS) noexcept;

					/// @brief Returns the current release time
					///
					/// @return - The release time, in milliseconds
					virtual double getReleaseTime() const noexcept;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept;

					/// @brief Returns the current sample rate
					///
					/// @return - The sample rate, in Hertz
					virtual size_t getSampleRate() const noexcept;

					LevelDetector<double>& operator=(LevelDetector<double>&& detector) noexcept = default;

				protected:
					double mAttackSeconds = 0.01;
					double mReleaseSeconds = 0.05;
					size_t mSampleRate = 44100;
					double mAttackCoeff = 0.9977349953;
					double mReleaseCoeff = 0.9995465881;
					//y[n-1]
					double mYOut1 = 0.0;
					//used in decoupled calculations to store y_1[n-1]
					double mYTempStage1 = 0.0;
					DetectorType mType = NonCorrected;

					virtual double processNonCorrected(double input) noexcept;
					virtual double processBranching(double input) noexcept;
					virtual double processDecoupled(double input) noexcept;
					virtual double processBranchingSmooth(double input) noexcept;
					virtual double processDecoupledSmooth(double input) noexcept;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
			};
	}
}
