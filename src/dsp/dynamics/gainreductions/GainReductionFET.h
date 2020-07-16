#pragma once

#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Class for calculating gain reduction values adjusted to roughly model FET topology behavior
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class GainReductionFET : public GainReduction<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionFET)
			};

		/// @brief Class for calculating gain reduction values adjusted to roughly model FET topology behavior
		template<>
			class GainReductionFET<float> : public GainReduction<float> {
				public:
					/// @brief Constructs a default `GainReductionFET` (sample rate = 44100, slew rate = 0.2ms)
					GainReductionFET() noexcept;

					/// @brief Constructs a `GainReductionFET` with the given sample rate and a slew rate of 0.2ms
					///
					/// @param sampleRate - The sample rate to use
					GainReductionFET(size_t sampleRate) noexcept;

					/// @brief Constructs a `GainReductionFET` with the given slew rate and a sample rate of 44100
					///
					/// @param riseTimeSeconds - The slew rate to use
					GainReductionFET(float riseTimeSeconds) noexcept;

					/// @brief Constructs a `GainReductionFET` with the given sample rate and slew rate
					///
					/// @param sampleRate - The sample rate to use
					/// @param riseTimeSeconds - The slew rate to use
					GainReductionFET(size_t sampleRate, float riseTimeSeconds) noexcept;
					virtual ~GainReductionFET() noexcept override;

					/// @brief Resets this `GainReductionFET` to an initial state
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					void reset(float currentGainReduction = 0.0f) noexcept override;

					/// @brief Calculates the adjusted gain reduction based on this `GainReductionFET`'s parameters
					///
					/// @param gainReduction - The gain reduction determined by the gain computer
					///
					/// @return  - The adjusted gain reduction
					float adjustedGainReduction(float gainReduction) noexcept override;

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Sets the slew rate to use for calculations to the given value
					///
					/// @param seconds - The new slew rate
					void setRiseTimeSeconds(float seconds) noexcept override;

				private:
					///The "amount" for the `softSaturation` WaveShaper
					static const constexpr float mWAVE_SHAPER_AMOUNT = -0.2f;
					///The "slope" for the `softSaturation` WaveShaper
					static const constexpr float mWAVE_SHAPER_SLOPE = 0.25f;
					///The "amount" for the slew rate WaveShaper
					static const constexpr float mSLEW_RATE_AMOUNT = 0.4f;
					///The "slope" for the slew rate WaveShaper
					static const constexpr float mSLEW_RATE_SLOPE = 0.4f;
					///The offset for the slew rate WaveShaper
					static const constexpr float mSLEW_RATE_OFFSET = 0.1f;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionFET)
			};

		/// @brief Class for calculating gain reduction values adjusted to roughly model FET topology behavior
		template<>
			class GainReductionFET<double> : public GainReduction<double> {
				public:
					/// @brief Constructs a default `GainReductionFET` (sample rate = 44100, slew rate = 0.2ms)
					GainReductionFET() noexcept;

					/// @brief Constructs a `GainReductionFET` with the given sample rate and a slew rate of 0.2ms
					///
					/// @param sampleRate - The sample rate to use
					GainReductionFET(size_t sampleRate) noexcept;

					/// @brief Constructs a `GainReductionFET` with the given slew rate and a sample rate of 44100
					///
					/// @param riseTimeSeconds - The slew rate to use
					GainReductionFET(double riseTimeSeconds) noexcept;

					/// @brief Constructs a `GainReductionFET` with the given sample rate and slew rate
					///
					/// @param sampleRate - The sample rate to use
					/// @param riseTimeSeconds - The slew rate to use
					GainReductionFET(size_t sampleRate, double riseTimeSeconds) noexcept;
					virtual ~GainReductionFET() noexcept override;

					/// @brief Resets this `GainReductionFET` to an initial state
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					void reset(double currentGainReduction = 0.0) noexcept override;

					/// @brief Calculates the adjusted gain reduction based on this `GainReductionFET`'s parameters
					///
					/// @param gainReduction - The gain reduction determined by the gain computer
					///
					/// @return  - The adjusted gain reduction
					double adjustedGainReduction(double gainReduction) noexcept override;

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) noexcept override;

					/// @brief Sets the slew rate to use for calculations to the given value
					///
					/// @param seconds - The new slew rate
					void setRiseTimeSeconds(double seconds) noexcept override;

				private:
					///The "amount" for the `softSaturation` WaveShaper
					static const constexpr double mWAVE_SHAPER_AMOUNT = -0.2;
					///The "slope" for the `softSaturation` WaveShaper
					static const constexpr double mWAVE_SHAPER_SLOPE = 0.25;
					///The "amount" for the slew rate WaveShaper
					static const constexpr double mSLEW_RATE_AMOUNT = 0.4;
					///The "slope" for the slew rate WaveShaper
					static const constexpr double mSLEW_RATE_SLOPE = 0.4;
					///The offset for the slew rate WaveShaper
					static const constexpr double mSLEW_RATE_OFFSET = 0.1;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionFET)
			};
	}
}
