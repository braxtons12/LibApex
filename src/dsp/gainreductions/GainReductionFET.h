#pragma once

#include <type_traits>
#include <utility>

#include "GainReduction.h"
#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {
		template<typename T>
			class GainReductionFET : public GainReduction<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
			};

		template<>
			class GainReductionFET<float> : public GainReduction<float> {
				public:
					GainReductionFET();
					GainReductionFET(size_t sampleRate);
					~GainReductionFET();

					void reset(float currentGainReduction) override;
					float adjustedGainReduction(float actualGainReduction, float idealGainReduction) override;
				private:
					float mWaveShaperAmount = 0.4f;
					float mWaveShaperSlope = 0.4f;
			};

		template<>
			class GainReductionFET<double> : public GainReduction<double> {
				public:
					GainReductionFET();
					GainReductionFET(size_t sampleRate);
					~GainReductionFET();

					void reset(double currentGainReduction) override;
					double adjustedGainReduction(double actualGainReduction, double idealGainReduction) override;
				private:
					double mWaveShaperAmount = 0.4;
					double mWaveShaperSlope = 0.4;
			};
	}
}
