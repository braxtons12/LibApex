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

namespace apex::dsp {
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
				explicit LevelDetectorRMS(DynamicsState* state,
						DetectorType type = DetectorType::NonCorrected) noexcept;

				/// @brief Move contructs an `RMSLevelDetector` from the given one
				///
				/// @param detector - The `RMSLevelDetector` to move
				LevelDetectorRMS(LevelDetectorRMS<float>&& detector) noexcept = default;
				~LevelDetectorRMS() noexcept override = default;

				/// @brief Generates the detected level from the given input
				///
				/// @param input - The input to detect on
				///
				/// @return - The detected level
				[[nodiscard]]
					auto process(float input) noexcept -> float override;

				/// @brief Resets this level detector to an initial state
				auto reset() noexcept -> void override;

				/// @brief Sets the attack time to the given value
				///
				/// @param attackSeconds - The new attack time, in seconds
				auto setAttackTime(float attackSeconds) noexcept -> void override;

				/// @brief Sets the release time to the given value
				///
				/// @param releaseSeconds - The new release time, in seconds
				auto setReleaseTime(float releaseSeconds) noexcept -> void override;

				/// @brief Sets the sample rate to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				auto setSampleRate(size_t sampleRate) noexcept -> void override;

				auto operator=(LevelDetectorRMS<float>&& detector)
					noexcept -> LevelDetectorRMS<float>& = default;

			protected:
				//y[n-1] squared
				float mYSquared1 = 0.0F;
				float mRMSSeconds = 0.1F;
				float mRMSCoeff = 0.9997732683F;

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
				explicit LevelDetectorRMS(DynamicsState* state,
						DetectorType type = DetectorType::NonCorrected) noexcept;

				/// @brief Move contructs an `RMSLevelDetector` from the given one
				///
				/// @param detector - The `RMSLevelDetector` to move
				LevelDetectorRMS(LevelDetectorRMS<double>&& detector) noexcept = default;
				~LevelDetectorRMS() noexcept override = default;

				/// @brief Generates the detected level from the given input
				///
				/// @param input - The input to detect on
				///
				/// @return - The detected level
				[[nodiscard]]
					auto process(double input) noexcept -> double override;

				/// @brief Resets this level detector to an initial state
				auto reset() noexcept -> void override;

				/// @brief Sets the attack time to the given value
				///
				/// @param attackSeconds - The new attack time, in seconds
				auto setAttackTime(double attackSeconds) noexcept -> void override;

				/// @brief Sets the release time to the given value
				///
				/// @param releaseSeconds - The new release time, in seconds
				auto setReleaseTime(double releaseSeconds) noexcept -> void override;

				/// @brief Sets the sample rate to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				auto setSampleRate(size_t sampleRate) noexcept -> void override;

				auto operator=(LevelDetectorRMS<double>&& detector)
					noexcept -> LevelDetectorRMS<double>& = default;

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
} //namespace apex::dsp
