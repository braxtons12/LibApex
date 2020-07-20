#pragma once

#include <type_traits>
#include <utility>

#include "../../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {
		/// @brief  Base Gain Computer behaviors for use in a dynamic range processor's `Sidechain`
		/// 
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainComputer {
				protected:
					typedef typename DynamicsState<T, AttackKind, ReleaseKind>::Field Field;
					typedef typename apex::dsp::DynamicsState<T, AttackKind, ReleaseKind> DynamicsState;
				
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

				GainComputer() noexcept = default;

				GainComputer(DynamicsState* state) noexcept
					: mState(state)
				{
;
				}

				GainComputer(GainComputer<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
				virtual ~GainComputer() noexcept = default;

				virtual T process(T input) noexcept = 0;

				GainComputer<T, AttackKind, ReleaseKind>& operator=(
					GainComputer<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputer);
			};
	}
}
