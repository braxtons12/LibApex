#pragma once

#include <type_traits>
#include <utility>

#include "GainComputer.h"
#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

#ifndef GAIN_COMPUTER_EXPANDER
#define GAIN_COMPUTER_EXPANDER

namespace apex::dsp {
	/// @brief Gain Computer for expanders
	///
	/// @tparam T - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<typename T, typename AttackKind, typename ReleaseKind>
		class GainComputerExpander : public GainComputer<T, AttackKind, ReleaseKind> {
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

				/// @brief Constructs a `GainComputerExpander` with zeroed shared state
				GainComputerExpander() noexcept
					: GainComputer<T, AttackKind, ReleaseKind>()
					{

					}

				/// @brief Constructs a `GainComputerExpander` with the given shared state
				///
				/// @param state - The shared state
				explicit GainComputerExpander(DynamicsState* state) noexcept
					: GainComputer<T, AttackKind, ReleaseKind>(state)
					{

					}

				/// @brief Move constructs the given `GainComputerExpander`
				///
				/// @param computer - The `GainComputerExpander` to move
				GainComputerExpander(GainComputerExpander<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
				~GainComputerExpander() override = default;

				/// @brief Calculates the target gain reduction value
				///
				/// @param input - The input to calculate gain reduction for
				///
				/// @return - The target gain reduction
				[[nodiscard]]
					auto process(T input) noexcept -> T override;

				auto operator=(GainComputerExpander<T, AttackKind, ReleaseKind>&& computer)
					noexcept -> GainComputerExpander<T, AttackKind, ReleaseKind>& = default;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
		};

	template<typename AttackKind, typename ReleaseKind>
		class GainComputerExpander<float, AttackKind, ReleaseKind> :
		public GainComputer<float, AttackKind, ReleaseKind>
		{
			protected:
				using Field = typename apex::dsp::DynamicsState<float, AttackKind, ReleaseKind>::Field;
				using DynamicsState = typename apex::dsp::DynamicsState<float, AttackKind, ReleaseKind>;

			public:
				static_assert((std::is_floating_point<AttackKind>::value &&
							std::is_same<float, AttackKind>::value) ||
						std::is_enum<AttackKind>::value,
						"AttackKind must be the same floating point type as T, or an enum");
				static_assert((std::is_floating_point<ReleaseKind>::value &&
							std::is_same<float, ReleaseKind>::value) ||
						std::is_enum<ReleaseKind>::value,
						"ReleaseKind must be the same floating point type as T, or an enum");

				/// @brief Constructs a `GainComputerExpander` with zeroed shared state
				GainComputerExpander() noexcept
					: GainComputer<float, AttackKind, ReleaseKind>()
					{

					}

				/// @brief Constructs a `GainComputerExpander` with the given shared state
				///
				/// @param state - The shared state
				explicit GainComputerExpander(DynamicsState* state) noexcept
					: GainComputer<float, AttackKind, ReleaseKind>(state)
					{

					}

				/// @brief Move constructs the given `GainComputerExpander`
				///
				/// @param computer - The `GainComputerExpander` to move
				GainComputerExpander(GainComputerExpander<float, AttackKind, ReleaseKind>&& computer) noexcept = default;
				~GainComputerExpander() override = default;

				/// @brief Calculates the target gain reduction value
				///
				/// @param input - The input to calculate gain reduction for
				///
				/// @return - The target gain reduction
				[[nodiscard]]
					auto process(float input) noexcept -> float override {
						float threshold = this->mState->getThreshold();
						float ratio = this->mState->getRatio();
						float kneeWidth = this->mState->getKneeWidth();

						float twoXMinusT = 2.0F * (input - this->mState->getThreshold());
						if(twoXMinusT < -kneeWidth) {
							return threshold + (input - threshold) * ratio;
						}
						else if(twoXMinusT > kneeWidth) {
							return input;
						}
						else {
							return input + (1.0F - ratio)
								* math::pow2f(input - threshold - kneeWidth / 2.0F) / (2.0F * kneeWidth);
						}
					}

				auto operator=(GainComputerExpander<float, AttackKind, ReleaseKind>&& computer)
					noexcept -> GainComputerExpander<float, AttackKind, ReleaseKind>& = default;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
		};

	template<typename AttackKind, typename ReleaseKind>
		class GainComputerExpander<double, AttackKind, ReleaseKind> :
		public GainComputer<double, AttackKind, ReleaseKind>
		{
			protected:
				using Field = typename apex::dsp::DynamicsState<double, AttackKind, ReleaseKind>::Field;
				using DynamicsState = typename apex::dsp::DynamicsState<double, AttackKind, ReleaseKind>;

			public:
				static_assert((std::is_floating_point<AttackKind>::value &&
							std::is_same<double, AttackKind>::value) ||
						std::is_enum<AttackKind>::value,
						"AttackKind must be the same floating point type as T, or an enum");
				static_assert((std::is_floating_point<ReleaseKind>::value &&
							std::is_same<double, ReleaseKind>::value) ||
						std::is_enum<ReleaseKind>::value,
						"ReleaseKind must be the same floating point type as T, or an enum");

				/// @brief Constructs a `GainComputerExpander` with zeroed shared state
				GainComputerExpander() noexcept
					: GainComputer<double, AttackKind, ReleaseKind>()
					{

					}

				/// @brief Constructs a `GainComputerExpander` with the given shared state
				///
				/// @param state - The shared state
				explicit GainComputerExpander(DynamicsState* state) noexcept
					: GainComputer<double, AttackKind, ReleaseKind>(state)
					{

					}

				/// @brief Move constructs the given `GainComputerExpander`
				///
				/// @param computer - The `GainComputerExpander` to move
				GainComputerExpander(GainComputerExpander<double, AttackKind, ReleaseKind>&& computer) noexcept = default;
				~GainComputerExpander() override = default;

				/// @brief Calculates the target gain reduction value
				///
				/// @param input - The input to calculate gain reduction for
				///
				/// @return - The target gain reduction
				[[nodiscard]]
					auto process(double input) noexcept -> double override {
						double threshold = this->mState->getThreshold();
						double ratio = this->mState->getRatio();
						double kneeWidth = this->mState->getKneeWidth();

						double twoXMinusT = 2.0 * (input - this->mState->getThreshold());
						if(twoXMinusT < -kneeWidth) {
							return threshold + (input - threshold) * ratio;
						}
						else if(twoXMinusT > kneeWidth) {
							return input;
						}
						else {
							return input + (1.0 - ratio)
								* math::pow2(input - threshold - kneeWidth / 2.0) / (2.0 * kneeWidth);
						}
					}

				auto operator=(GainComputerExpander<double, AttackKind, ReleaseKind>&& computer)
					noexcept -> GainComputerExpander<double, AttackKind, ReleaseKind>& = default;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
		};
} //namespace apex::dsp

#endif //GAIN_COMPUTER_EXPANDER
