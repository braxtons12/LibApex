#pragma once

#include <vector>
#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Class for calculating gain reduction values adjusted to roughly model Optical topology behavior
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class GainReductionOptical : public GainReduction<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionOptical)
			};

		/// @brief Class for calculating gain reduction values adjusted to roughly model Optical topology behavior
		template<>
			class GainReductionOptical<float> : public GainReduction<float> {
				public:
					/// @brief Constructs a default `GainReductionOptical` (sampleRate = 44100)
					GainReductionOptical() noexcept;

					/// @brief Constructs a `GainReductionOptical` with he given sample rate
					///
					/// @param sampleRate
					GainReductionOptical(size_t sampleRate) noexcept;
					virtual ~GainReductionOptical() noexcept override;

					/// @brief Resets this `GainReductionOptical` to an initial state
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					void reset(float currentGainReduction = 0.0f) noexcept override;

					/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s parameters
					///
					/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
					/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
					///
					/// @return - The adjusted gain reduction
					float adjustedGainReduction(float actualGainReduction, float idealGainReduction) noexcept override;

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) noexcept override;

				private:
					///The number of steps in decibels to store coefficients for
					static const constexpr size_t mNUM_DB_STEPS = 48;
					///The number of coefficients making up each decibel step
					static const constexpr size_t mNUM_COEFFICIENTS_PER_STEP = 2;
					///The total number of coefficients
					static const constexpr size_t mNUM_COEFFICIENTS = mNUM_DB_STEPS * mNUM_COEFFICIENTS_PER_STEP;
					///The "amount" to use for the `softSaturation` wave shaper
					static const constexpr float mWAVE_SHAPER_AMOUNT = 0.2f;
					///The "slope" to use for the `softSaturation` wave shaper
					static const constexpr float mWAVE_SHAPER_SLOPE = 0.2f;
					///The attack response coefficients
					std::vector<float> mAttackCoefficients = std::vector<float>(mNUM_COEFFICIENTS);
					///The release response coefficients
					std::vector<float> mReleaseCoefficients = std::vector<float>(mNUM_COEFFICIENTS);

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionOptical)
			};

		/// @brief Class for calculating gain reduction values adjusted to roughly model Optical topology behavior
		template<>
			class GainReductionOptical<double> : public GainReduction<double> {
				public:
					/// @brief Constructs a default `GainReductionOptical` (sampleRate = 44100)
					GainReductionOptical() noexcept;

					/// @brief Constructs a `GainReductionOptical` with he given sample rate
					///
					/// @param sampleRate
					GainReductionOptical(size_t sampleRate) noexcept;
					virtual ~GainReductionOptical() noexcept override;

					/// @brief Resets this `GainReductionOptical` to an initial state
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					void reset(double currentGainReduction = 0.0) noexcept override;

					/// @brief Calculats the adjusted gain reduction based on this `GainReductionOptical`'s parameters
					///
					/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
					/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
					///
					/// @return - The adjusted gain reduction
					double adjustedGainReduction(double actualGainReduction, double idealGainReduction) noexcept override;

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) noexcept override;

				private:
					///The number of steps in decibels to store coefficients for
					static const constexpr size_t mNUM_DB_STEPS = 48;
					///The number of coefficients making up each decibel step
					static const constexpr size_t mNUM_COEFFICIENTS_PER_STEP = 2;
					///The total number of coefficients
					static const constexpr size_t mNUM_COEFFICIENTS = mNUM_DB_STEPS * mNUM_COEFFICIENTS_PER_STEP;
					///The "amount" to use for the `softSaturation` wave shaper
					static const constexpr double mWAVE_SHAPER_AMOUNT = 0.2;
					///The "slope" to use for the `softSaturation` wave shaper
					static const constexpr double mWAVE_SHAPER_SLOPE = 0.2;
					///The attack response coefficients
					std::vector<double> mAttackCoefficients = std::vector<double>(mNUM_COEFFICIENTS);
					///The release response coefficients
					std::vector<double> mReleaseCoefficients = std::vector<double>(mNUM_COEFFICIENTS);

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainReductionOptical)
			};
	}
}
