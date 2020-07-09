#pragma once

#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Class for calculating gain reduction values adjusted to roughly model VCA topology behavior
		///
		/// @tparam T - The floating point type to back operations, either float or double
		template<typename T>
			class GainReductionVCA : public GainReduction<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
			};

		/// @brief Class for calculating gain reduction values adjusted to roughly model VCA topology behavior
		template<>
			class GainReductionVCA<float> : public GainReduction<float> {
				public:
					/// @brief Constructs a default `GainReductionVCA` (sample rate = 44100)
					GainReductionVCA();

					/// @brief Constructs a `GainReductionVCA` with the given sample rate
					///
					/// @param sampleRate - The sample rate to use
					GainReductionVCA(size_t sampleRate);
					virtual ~GainReductionVCA() override;

					/// @brief Resets this `GainReductionVCA` to an initial state
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					void reset(float currentGainReduction = 0.0f) override;

					/// @brief Calculates the adjusted gain reduction based on this `GainReductionVCA`'s parameters
					///
					/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
					/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
					///
					/// @return - The adjusted gain reduction
					float adjustedGainReduction(float actualGainReduction, float idealGainReduction) override;

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) override;

				private:
					///The "amount" for the `softSaturation` wave shaper
					static const constexpr float mWAVE_SHAPER_AMOUNT = 0.2f;
					///The "slope" for the `softSaturation` wave shaper
					static const constexpr float mWAVE_SHAPER_SLOPE = 0.4f;
			};

		/// @brief Class for calculating gain reduction values adjusted to roughly model VCA topology behavior
		template<>
			class GainReductionVCA<double> : public GainReduction<double> {
				public:
					/// @brief Constructs a default `GainReductionVCA` (sample rate = 44100)
					GainReductionVCA();

					/// @brief Constructs a `GainReductionVCA` with the given sample rate
					///
					/// @param sampleRate - The sample rate to use
					GainReductionVCA(size_t sampleRate);
					virtual ~GainReductionVCA() override;

					/// @brief Resets this `GainReductionVCA` to an initial state
					///
					/// @param currentGainReduction - The gain reduction to use as the initial value
					void reset(double currentGainReduction = 0.0) override;

					/// @brief Calculates the adjusted gain reduction based on this `GainReductionVCA`'s parameters
					///
					/// @param actualGainReduction - The actual gain reduction determined by other adjustment processes in the signal chain
					/// @param idealGainReduction - The ideal gain reduction determined from pure gain reduction calculations only
					///
					/// @return - The adjusted gain reduction
					double adjustedGainReduction(double actualGainReduction, double idealGainReduction) override;

					/// @brief Sets the sample rate to use for calculations to the given value
					///
					/// @param sampleRate - The new sample rate to use
					void setSampleRate(size_t sampleRate) override;

				private:
					///The "amount" for the `softSaturation` wave shaper
					static const constexpr double mWAVE_SHAPER_AMOUNT = 0.2;
					///The "slope" for the `softSaturation` wave shaper
					static const constexpr double mWAVE_SHAPER_SLOPE = 0.4;
			};
	}
}
