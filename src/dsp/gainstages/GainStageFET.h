#pragma once

#include <type_traits>
#include <utility>

#include "GainStage.h"
#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {
		template<typename T>
			class GainStageFET : public GainStage<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
				
				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
			};

		template<>
			class GainStageFET<float> : public GainStage<float> {
				public:
					GainStageFET() noexcept = default;
					GainStageFET(GainStageFET<float>&& stage) noexcept = default;
					virtual ~GainStageFET() noexcept override = default;

					float process(float input) noexcept override;

					GainStageFET<float>& operator=(GainStageFET<float>&& stage) noexcept = default;

				private:
					static const constexpr float SATURATION_AMOUNT = 0.5f;
					static const constexpr float SATURATION_SLOPE = 0.7f;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
			};
		
		template<>
			class GainStageFET<double> : public GainStage<double> {
				public:
					GainStageFET() noexcept = default;
					GainStageFET(GainStageFET<double>&& stage) noexcept = default;
					virtual ~GainStageFET() noexcept override = default;

					double process(double input) noexcept override;

					GainStageFET<double>& operator=(GainStageFET<double>&& stage) noexcept = default;

				private:
					static const constexpr double SATURATION_AMOUNT = 0.5;
					static const constexpr double SATURATION_SLOPE = 0.7;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageFET)
			};
	}
}
