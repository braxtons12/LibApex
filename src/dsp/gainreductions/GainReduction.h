#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Base class for calculating gain reduction values adjusted to match different topology behaviors.
		/// Base implementation only performs basic slew-rate adjustments.
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class GainReduction {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
			};

		/// @brief Base class for calculating gain reduction values adjusted to match different topology behaviors.
		/// Base implementation only performs basic slew-rate adjustments.
		template<>
			class GainReduction<float> {
				public:
					/// @brief Constructs a default `GainReduction` (sample rate = 44100, slew rate = 0)
					GainReduction();

					/// @brief Constructs a `GainReduction` with the given sample rate and a slew rate of 0
					///
					/// @param sampleRate - The sample rate to use
					GainReduction(size_t sampleRate);

					/// @brief Constructs a `GainReduction` with the given slew rate and a sample rate of 44100
					///
					/// @param riseTimeSeconds - The slew rate to use
					GainReduction(float riseTimeSeconds);

					/// @brief Constructs a `GainReduction` with the given sample rate and slew rate
					///
					/// @param sampleRate - The sample rate to use
					/// @param riseTimeSeconds - The slew rate to use
					GainReduction(size_t sampleRate, float riseTimeSeconds);
					~GainReduction();

					/// @brief Resets this `GainReduction` to an initial state.
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					virtual void reset(float currentGainReduction = 0.0f);

					/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
					///
					/// @param actualGainReduction - The actualGainReduction determined by other adjustment processes in the signal chain
					/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
					///
					/// @return The adjusted gain reduction
					virtual float adjustedGainReduction(float actualGainReduction, float idealGainReduction);

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					virtual void setSampleRate(size_t sampleRate);

					/// @brief Sets the slew rate to use for calculations to the given value
					///
					/// @param seconds - The new slew rate
					virtual void setRiseTimeSeconds(float seconds);

				protected:
					///The sample rate
					size_t mSampleRate = 44100;
					///The current sample in the slew
					size_t mCurrentSample = 0;
					///The number of samples the slew takes to complete
					size_t mNumSamplesToTransitionGain = 0;
					///The current gain reduction value
					float mCurrentGainReduction = 0.0f;
					///The slew rate
					float mRiseTimeSeconds = 0.0f;
			};

		/// @brief Base class for calculating gain reduction values adjusted to match different topology behaviors.
		/// Base implementation only performs basic slew-rate adjustments.
		template<>
			class GainReduction<double> {
				public:
					/// @brief Constructs a default `GainReduction` (sample rate = 44100, slew rate = 0)
					GainReduction();

					/// @brief Constructs a `GainReduction` with the given sample rate and a slew rate of 0
					///
					/// @param sampleRate - The sample rate to use
					GainReduction(size_t sampleRate);

					/// @brief Constructs a `GainReduction` with the given slew rate and a sample rate of 44100
					///
					/// @param riseTimeSeconds - The slew rate to use
					GainReduction(double riseTimeSeconds);

					/// @brief Constructs a `GainReduction` with the given sample rate and slew rate
					///
					/// @param sampleRate - The sample rate to use
					/// @param riseTimeSeconds - The slew rate to use
					GainReduction(size_t sampleRate, double riseTimeSeconds);
					~GainReduction();

					/// @brief Resets this `GainReduction` to an initial state.
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					virtual void reset(double currentGainReduction = 0.0);

					/// @brief Calculates the adjusted gain reduction based on this `GainReduction`'s parameters
					///
					/// @param actualGainReduction - The actualGainReduction determined by other adjustment processes in the signal chain
					/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
					///
					/// @return The adjusted gain reduction
					virtual double adjustedGainReduction(double actualGainReduction, double idealGainReduction);

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					virtual void setSampleRate(size_t sampleRate);

					/// @brief Sets the slew rate to use for calculations to the given value
					///
					/// @param seconds - The new slew rate
					virtual void setRiseTimeSeconds(double seconds);

				protected:
					///The sample rate
					size_t mSampleRate = 44100;
					///The current sample in the slew
					size_t mCurrentSample = 0;
					///The number of samples the slew takes to complete
					size_t mNumSamplesToTransitionGain = 0;
					///The current gain reduction value
					double mCurrentGainReduction = 0.0;
					///The slew rate
					double mRiseTimeSeconds = 0.0;
			};
	}
}
