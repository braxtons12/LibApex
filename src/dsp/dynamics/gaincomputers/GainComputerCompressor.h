#pragma once

#include <type_traits>
#include <utility>

#include "GainComputer.h"
#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

#ifndef GAIN_COMPUTER_COMPRESSOR
#define GAIN_COMPUTER_COMPRESSOR

namespace apex {
	namespace dsp {
		/// @brief Gain Computer for compressors
		///
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainComputerCompressor : public GainComputer<T, AttackKind, ReleaseKind> {
				protected:
					using Field = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>::Field;
					using DynamicsState = typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind>;

				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
					static_assert((std::is_floating_point<AttackKind>::value &&
								std::is_same<T, AttackKind>::value) ||
							std::is_enum<AttackKind>::value,
							"AttackKind must be the same floating point type as T, or an enum");
					static_assert((std::is_floating_point<ReleaseKind>::value &&
								std::is_same<T, ReleaseKind>::value) ||
							std::is_enum<ReleaseKind>::value,
							"ReleaseKind must be the same floating point type as T, or an enum");

					/// @brief Constructs a `GainComputerCompressor` with zeroed shared state
					GainComputerCompressor() noexcept
						: GainComputer<T, AttackKind, ReleaseKind>()
						{

						}

					/// @brief Constructs a `GainComputerCompressor` with the given shared state
					///
					/// @param state - The shared state
					GainComputerCompressor(DynamicsState* state) noexcept
						: GainComputer<T, AttackKind, ReleaseKind>(state)
						{

						}

					/// @brief Move constructs the given `GainComputerCompressor`
					///
					/// @param computer - The `GainComputerCompressor` to move
					GainComputerCompressor(GainComputerCompressor<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
					virtual ~GainComputerCompressor() override = default;

					/// @brief Calculates the target gain reduction value
					///
					/// @param input - The input to calculate gain reduction for
					///
					/// @return - The target gain reduction
					T process(T input) noexcept override;

					GainComputerCompressor<T, AttackKind, ReleaseKind>& operator=(
							GainComputerCompressor<T, AttackKind, ReleaseKind>&& computer) noexcept = default;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
			};

		template<typename AttackKind, typename ReleaseKind>
			class GainComputerCompressor<float, AttackKind, ReleaseKind> :
			public GainComputer<float, AttackKind, ReleaseKind>
			{
				protected:
					typedef typename apex::dsp::DynamicsState<float, AttackKind, ReleaseKind>::Field Field;
					typedef typename apex::dsp::DynamicsState<float, AttackKind, ReleaseKind> DynamicsState;

				public:
					static_assert((std::is_floating_point<AttackKind>::value &&
								std::is_same<float, AttackKind>::value) ||
							std::is_enum<AttackKind>::value,
							"AttackKind must be the same floating point type as T, or an enum");
					static_assert((std::is_floating_point<ReleaseKind>::value &&
								std::is_same<float, ReleaseKind>::value) ||
							std::is_enum<ReleaseKind>::value,
							"ReleaseKind must be the same floating point type as T, or an enum");

					/// @brief Constructs a `GainComputerCompressor` with zeroed shared state
					GainComputerCompressor() noexcept
						: GainComputer<float, AttackKind, ReleaseKind>()
						{

						}

					/// @brief Constructs a `GainComputerCompressor` with the given shared state
					///
					/// @param state - The shared state
					GainComputerCompressor(DynamicsState* state) noexcept
						: GainComputer<float, AttackKind, ReleaseKind>(state)
						{

						}

					/// @brief Move constructs the given `GainComputerCompressor`
					///
					/// @param computer - The `GainComputerCompressor` to move
					GainComputerCompressor(GainComputerCompressor<float, AttackKind, ReleaseKind>&& computer) noexcept = default;
					virtual ~GainComputerCompressor() override = default;

					/// @brief Calculates the target gain reduction value
					///
					/// @param input - The input to calculate gain reduction for
					///
					/// @return - The target gain reduction
					float process(float input) noexcept override {
						float threshold = this->mState->getThreshold();
						float ratio = this->mState->getRatio();
						float kneeWidth = this->mState->getKneeWidth();

						float twoXMinusT = 2.0f * (input - this->mState->getThreshold());
						if(twoXMinusT < -kneeWidth) {
							return input;
						}
						else if(twoXMinusT > kneeWidth) {
							return threshold + (input - threshold) / ratio;
						}
						else {
							return input + ( (1.0f / ratio) - 1.0f)
								* math::pow2f(input - threshold + kneeWidth / 2.0f) / (2.0f * kneeWidth);
						}
					}

					GainComputerCompressor<float, AttackKind, ReleaseKind>& operator=(
							GainComputerCompressor<float, AttackKind, ReleaseKind>&& computer) noexcept = default;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
			};

		template<typename AttackKind, typename ReleaseKind>
			class GainComputerCompressor<double, AttackKind, ReleaseKind> :
			public GainComputer<double, AttackKind, ReleaseKind>
			{
				protected:
					typedef typename apex::dsp::DynamicsState<double, AttackKind, ReleaseKind>::Field Field;
					typedef typename apex::dsp::DynamicsState<double, AttackKind, ReleaseKind> DynamicsState;

				public:
					static_assert((std::is_floating_point<AttackKind>::value &&
								std::is_same<double, AttackKind>::value) ||
							std::is_enum<AttackKind>::value,
							"AttackKind must be the same floating point type as T, or an enum");
					static_assert((std::is_floating_point<ReleaseKind>::value &&
								std::is_same<double, ReleaseKind>::value) ||
							std::is_enum<ReleaseKind>::value,
							"ReleaseKind must be the same floating point type as T, or an enum");

					/// @brief Constructs a `GainComputerCompressor` with zeroed shared state
					GainComputerCompressor() noexcept
						: GainComputer<double, AttackKind, ReleaseKind>()
						{

						}

					/// @brief Constructs a `GainComputerCompressor` with the given shared state
					///
					/// @param state - The shared state
					GainComputerCompressor(DynamicsState* state) noexcept
						: GainComputer<double, AttackKind, ReleaseKind>(state)
						{

						}

					/// @brief Move constructs the given `GainComputerCompressor`
					///
					/// @param computer - The `GainComputerCompressor` to move
					GainComputerCompressor(GainComputerCompressor<double, AttackKind, ReleaseKind>&& computer) noexcept = default;
					virtual ~GainComputerCompressor() override = default;

					/// @brief Calculates the target gain reduction value
					///
					/// @param input - The input to calculate gain reduction for
					///
					/// @return - The target gain reduction
					double process(double input) noexcept override {
						double threshold = this->mState->getThreshold();
						double ratio = this->mState->getRatio();
						double kneeWidth = this->mState->getKneeWidth();

						double twoXMinusT = 2.0 * (input - this->mState->getThreshold());
						if(twoXMinusT < -kneeWidth) {
							return input;
						}
						else if(twoXMinusT > kneeWidth) {
							return threshold + (input - threshold) / ratio;
						}
						else {
							return input + ( (1.0 / ratio) - 1.0)
								* math::pow2(input - threshold + kneeWidth / 2.0) / (2.0 * kneeWidth);
						}
					}

					GainComputerCompressor<double, AttackKind, ReleaseKind>& operator=(
							GainComputerCompressor<double, AttackKind, ReleaseKind>&& computer) noexcept = default;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerCompressor)
			};
	}
}

#endif //GAIN_COMPUTER_COMPRESSOR
