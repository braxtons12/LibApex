/// @brief Contains base implementatin for Level Detector Component of a Dynamic Range Processor's
/// Sidechain (aka the signal path that calculates the gain reduction/increase)
///
/// @see Giannoulis, MassBerg, & Reiss's "Digital Dynamic Range Compressor Design - A Tutorial and Analysis"
#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

namespace apex::dsp {
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
				using Field = typename apex::dsp::DynamicsState<float, float, float>::Field;
				using DynamicsState = typename apex::dsp::DynamicsState<float, float, float>;

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
				explicit LevelDetector(DynamicsState* state,
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
				virtual auto process(float input) noexcept -> float;

				/// @brief Resets this level detector to an initial state
				virtual auto reset() noexcept -> void;

				/// @brief Sets the attack time to the given value
				///
				/// @param attackSeconds - The new attack time, in seconds
				virtual auto setAttackTime(float attackSeconds) noexcept -> void;

				/// @brief Sets the release time to the given value
				///
				/// @param releaseSeconds - The new release time, in seconds
				virtual auto setReleaseTime(float releaseSeconds) noexcept -> void;

				/// @brief Sets the sample rate to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				virtual auto setSampleRate(size_t sampleRate) noexcept -> void;

				auto operator=(LevelDetector<float>&& detector)
					noexcept -> LevelDetector<float>& = default;

			protected:
				//y[n-1]
				float mYOut1 = 0.0F;
				//used in decoupled calculations to store y_1[n-1]
				float mYTempStage1 = 0.0F;
				DetectorType mType = DetectorType::NonCorrected;

				virtual auto processNonCorrected(float input) noexcept -> float;
				virtual auto processBranching(float input) noexcept -> float;
				virtual auto processDecoupled(float input) noexcept -> float;
				virtual auto processBranchingSmooth(float input) noexcept -> float;
				virtual auto processDecoupledSmooth(float input) noexcept -> float;

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
				using Field = typename apex::dsp::DynamicsState<double, double, double>::Field;
				using DynamicsState = typename apex::dsp::DynamicsState<double, double, double>;

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
				explicit LevelDetector(DynamicsState* state,
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
				virtual auto process(double input) noexcept -> double;

				/// @brief Resets this level detector to an initial state
				virtual auto reset() noexcept -> void;

				/// @brief Sets the attack time to the given value
				///
				/// @param attackSeconds - The new attack time, in seconds
				virtual auto setAttackTime(double attackSeconds) noexcept -> void;

				/// @brief Sets the release time to the given value
				///
				/// @param releaseSeconds - The new release time, in seconds
				virtual auto setReleaseTime(double releaseSeconds) noexcept -> void;

				/// @brief Sets the sample rate to the given value
				///
				/// @param sampleRate - The new sample rate, in Hertz
				virtual auto setSampleRate(size_t sampleRate) noexcept -> void;

				auto operator=(LevelDetector<double>&& detector)
					noexcept -> LevelDetector<double>& = default;

			protected:
				//y[n-1]
				double mYOut1 = 0.0;
				//used in decoupled calculations to store y_1[n-1]
				double mYTempStage1 = 0.0;
				DetectorType mType = DetectorType::NonCorrected;

				virtual auto processNonCorrected(double input) noexcept -> double;
				virtual auto processBranching(double input) noexcept -> double;
				virtual auto processDecoupled(double input) noexcept -> double;
				virtual auto processBranchingSmooth(double input) noexcept -> double;
				virtual auto processDecoupledSmooth(double input) noexcept -> double;

			private:
				DynamicsState DEFAULT_STATE = DynamicsState();
				DynamicsState* mState = &DEFAULT_STATE;
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelDetector)
		};
} //namespace apex::dsp
