 #pragma once

#include <type_traits>
#include <utility>

#include "GainComputer.h"
#include "../../../base/StandardIncludes.h"
#include "../DynamicsState.h"

#ifndef GAIN_COMPUTER_EXPANDER
#define GAIN_COMPUTER_EXPANDER

namespace apex {
	namespace dsp {
		/// @brief Gain Computer for expanders
		/// 
		/// @tparam T - The floating point type to back operations
		/// @tparam AttackKind - The attack type used by the shared `DynamicsState`
		/// @tparam ReleaseKind - The release type used by the shared `DynamicsState`
		template<typename T, typename AttackKind, typename ReleaseKind>
			class GainComputerExpander : public GainComputer<T, AttackKind, ReleaseKind> {
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
				
				GainComputerExpander() noexcept
					: GainComputer<T, AttackKind, ReleaseKind>()
				{

				};
				
				GainComputerExpander(DynamicsState* state) noexcept
					: GainComputer<T, AttackKind, ReleaseKind>(state)
				{

				}

				GainComputerExpander(GainComputerExpander<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
				virtual ~GainComputerExpander() override;

				T process(T input) noexcept override;

				GainComputerExpander<T, AttackKind, ReleaseKind>& operator=(
					GainComputerExpander<T, AttackKind, ReleaseKind>&& computer) noexcept = default;
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
			};
			
		template<typename AttackKind, typename ReleaseKind>
			class GainComputerExpander<float, AttackKind, ReleaseKind> :
				public GainComputer<float, AttackKind, ReleaseKind>
			{
				protected:
					typedef typename DynamicsState<float, AttackKind, ReleaseKind>::Field Field;
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
				
				GainComputerExpander() noexcept
					: GainComputer<T, AttackKind, ReleaseKind>()
				{

				};
				
				GainComputerExpander(DynamicsState* state) noexcept
					: GainComputer<T, AttackKind, ReleaseKind>(state)
				{

				}

				GainComputerExpander(GainComputerExpander<float, AttackKind, ReleaseKind>&& computer) noexcept = default;
				virtual ~GainComputerExpander() override;

				float process(float input) noexcept override {
					float threshold = mState->getThreshold();
					float ratio = mState->getRatio();
					float kneeWidth = mState->getKneeWidth();

					float twoXMinusT = 2.0f * (input - mState->getThreshold());
					if(twoXMinusT < -kneeWidth) {
						return threshold + (input - threshold) * ratio;
					}
					else if(twoXMinusT > kneeWidth) {
						return input;
					}
					else {
						return input + (1.0f - ratio)
							* math::pow2f(input - threshold - kneeWidth / 2.0f) / (2.0f * kneeWidth);
					}
				}

				GainComputerExpander<float, AttackKind, ReleaseKind>& operator=(
					GainComputerExpander<float, AttackKind, ReleaseKind>&& computer) noexcept = default;
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
			};
			
		template<typename AttackKind, typename ReleaseKind>
			class GainComputerExpander<double, AttackKind, ReleaseKind> :
				public GainComputer<double, AttackKind, ReleaseKind>
			{
				protected:
					typedef typename DynamicsState<double, AttackKind, ReleaseKind>::Field Field;
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
				
				GainComputerExpander() noexcept
					: GainComputer<T, AttackKind, ReleaseKind>()
				{

				};
				
				GainComputerExpander(DynamicsState* state) noexcept
					: GainComputer<T, AttackKind, ReleaseKind>(state)
				{

				}

				GainComputerExpander(GainComputerExpander<double, AttackKind, ReleaseKind>&& computer) noexcept = default;
				virtual ~GainComputerExpander() override;

				double process(double input) noexcept override {
					double threshold = mState->getThreshold();
					double ratio = mState->getRatio();
					double kneeWidth = mState->getKneeWidth();

					double twoXMinusT = 2.0 * (input - mState->getThreshold());
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

				GainComputerExpander<double, AttackKind, ReleaseKind>& operator=(
					GainComputerExpander<double, AttackKind, ReleaseKind>&& computer) noexcept = default;
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComputerExpander)
			};
	}
}

#endif //GAIN_COMPUTER_EXPANDER
