/// @brief Contains base implementatin for an RMS Level Detector Component of a Dynamic Range Processor's
/// Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "LevelDetector.h"

namespace apex {
	namespace dsp {

		/// @brief Base RMS Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class LevelDetectorRMS : LevelDetector<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
			};

		/// @brief Base RMS Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetectorRMS<float> : LevelDetector<float> {
				public:
					LevelDetectorRMS() noexcept = default;

					/// @brief Constructs an `RMSLevelDetector` with the given parameters
					///
					/// @param attackMS - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					/// @param type - The detector topology to use
					LevelDetectorRMS(float attackMS, float releaseMS, size_t sampleRate,
							DetectorType type = NonCorrected) noexcept;

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
					/// @param attackMS - The new attack time, in milliseconds
					virtual void setAttackTime(float attackMS) noexcept override;

					/// @brief Returns the current attack time
					///
					/// @return - The attack time, in milliseconds
					virtual float getAttackTime() const noexcept override;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseMS - The new release time, in milliseconds
					virtual void setReleaseTime(float relaseMS) noexcept override;

					/// @brief Returns the current release time
					///
					/// @return - The release time, in milliseconds
					virtual float getReleaseTime() const noexcept override;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Returns the current sample rate
					///
					/// @return - The sample rate, in Hertz
					virtual size_t getSampleRate() const noexcept override;

					LevelDetectorRMS<float>& operator=(LevelDetectorRMS<float>&& detector) noexcept = default;

				protected:
					//y[n-1] squared
					float mYSquared1 = 0.0f;
					float mRMSSeconds = 0.1f;
					float mRMSCoeff = 0.9997732683f;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
			};

		/// @brief Base RMS Level Detector used for the level detection portion of a
		/// Dynamic Range Processor's Sidechain
		template<>
			class LevelDetectorRMS<double> : LevelDetector<double> {
				public:
					LevelDetectorRMS() noexcept = default;

					/// @brief Constructs an `RMSLevelDetector` with the given parameters
					///
					/// @param attackMS - The attack time, in milliseconds
					/// @param releaseMS - The release time, in milliseconds
					/// @param sampleRate - The sample rate, in Hertz
					/// @param type - The detector topology to use
					LevelDetectorRMS(double attackMS, double releaseMS, size_t sampleRate,
							DetectorType type = NonCorrected) noexcept;

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
					/// @param attackMS - The new attack time, in milliseconds
					virtual void setAttackTime(double attackMS) noexcept override;

					/// @brief Returns the current attack time
					///
					/// @return - The attack time, in milliseconds
					virtual double getAttackTime() const noexcept override;

					/// @brief Sets the release time to the given value
					///
					/// @param releaseMS - The new release time, in milliseconds
					virtual void setReleaseTime(double relaseMS) noexcept override;

					/// @brief Returns the current release time
					///
					/// @return - The release time, in milliseconds
					virtual double getReleaseTime() const noexcept override;

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					virtual void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Returns the current sample rate
					///
					/// @return - The sample rate, in Hertz
					virtual size_t getSampleRate() const noexcept override;

					LevelDetectorRMS<double>& operator=(LevelDetectorRMS<double>&& detector) noexcept = default;

				protected:
					//y[n-1] squared
					double mYSquared1 = 0.0;
					double mRMSSeconds = 0.1;
					double mRMSCoeff = 0.9997732683;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetectorRMS)
			};
	}
}
