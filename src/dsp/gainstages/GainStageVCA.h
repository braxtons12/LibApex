#pragma once

#include <type_traits>
#include <utility>

#include "GainStage.h"
#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {
		template<typename T>
			class GainStageVCA : public GainStage<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageVCA)
			};

		template<>
			class GainStageVCA<float> : public GainStage<float> {
				public:
					GainStageVCA() noexcept = default;
					GainStageVCA(GainStageVCA<float>&& stage) noexcept = default;
					virtual ~GainStageVCA() noexcept override = default;

					float process(float input) noexcept override;

					GainStageVCA<float>& operator=(GainStageVCA<float>&& stage) noexcept = default;

				private:
					static const constexpr float SATURATION_AMOUNT = 0.5f;
					static const constexpr float SATURATION_SLOPE = 0.3f;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageVCA)
			};

		template<>
			class GainStageVCA<double> : public GainStage<double> {
				public:
					GainStageVCA() noexcept = default;
					GainStageVCA(GainStageVCA<double>&& stage) noexcept = default;
					virtual ~GainStageVCA() noexcept override = default;

					double process(double input) noexcept override;

					GainStageVCA<double>& operator=(GainStageVCA<double>&& stage) noexcept = default;
				
				private:
					static const constexpr double SATURATION_AMOUNT = 0.5;
					static const constexpr double SATURATION_SLOPE = 0.3;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainStageVCA)
			};
	}
}
