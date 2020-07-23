#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

#ifndef GAIN_COMPUTER
#define GAIN_COMPUTER

namespace apex::dsp {
	/// @brief  Base Gain Computer behaviors for use in a dynamic range processor's `Sidechain`
	///
	/// @tparam T - The floating point type to back operations
	/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
	/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
	template<typename T, typename AttackKind, typename ReleaseKind>
		class GainComputer {
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

				/// @brief Constructs a `GainComputer` with zeroed shared state
				GainComputer() noexcept = default;

				/// @brief Constructs a `GainComputer` with the given shared state
				///
				/// @param state - The shared state
				explicit GainComputer(DynamicsState* state) noexcept
					: mState(state)
					{

					}

				/// @brief Move constructs the given `GainComputer`
				///
				/// @param computer - The `GainComputer` to move
				GainComputer(GainComputer<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
				virtual ~GainComputer() noexcept = default;

				/// @brief Calculates the target gain reduction value
				///
				/// @param input - The input to calculate gain reduction for
				///
				/// @return - The target gain reduction
				[[nodiscard]]
					virtual auto process(T input) noexcept -> T = 0;

				auto operator=(GainComputer<T, AttackKind, ReleaseKind>&& computer)
					noexcept -> GainComputer<T, AttackKind, ReleaseKind>& = default;

			protected:
				DynamicsState DEFAULT_STATE = DynamicsState();
				DynamicsState* mState = &DEFAULT_STATE;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputer)
		};
} //namespace apex::dsp

#endif //GAIN_COMPUTER
