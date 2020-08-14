#pragma once

#include <array>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "../filters/BiQuadFilter.h"

namespace apex::dsp {
	template<typename T, T OverSampleRate = static_cast<T>(2.0)>
	class OverSampler {
	  public:
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

		constexpr OverSampler() noexcept = default;
		constexpr explicit OverSampler(Hertz sampleRate) noexcept : mSampleRate(sampleRate) {
		}
		constexpr OverSampler(OverSampler<T, OverSampleRate>&& overSampler) noexcept = default;

		[[nodiscard]] constexpr inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		[[nodiscard]] constexpr inline auto getOverSampledSampleRate() const noexcept -> Hertz {
			return mSampleRate * OverSampleRate;
		}

		constexpr inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
			mSampleRate = sampleRate;
			mPreAntiAliasFilter.setSampleRate(mSampleRate * OverSampleRate);
			mPreAntiAliasFilter.setFrequency(mSampleRate / 2.0);
			mPostAntiAliasFilter.setSampleRate(mSampleRate * OverSampleRate);
			mPostAntiAliasFilter.setFrequency(mSampleRate / 2.0);
			mOverSampled.resize(mSampleRate * OverSampleRate);
			mDownSampled.resize(mSampleRate);
		}

		constexpr inline auto reset() noexcept -> void {
			mPreAntiAliasFilter.reset();
			mPostAntiAliasFilter.reset();
			for(auto& sample : mOverSampled) {
				sample = static_cast<T>(0.0);
			}
			for(auto& sample : mDownSampled) {
				sample = static_cast<T>(0.0);
			}
		}

		[[nodiscard]] constexpr inline auto overSample(Span<T> input) noexcept -> Span<T> {
			mOverSampled.at(0) = input.at(0);
			for(size_t index = 1; index < mOverSampled.size(); ++index) {
				if(index % 2 == 0) {
					mOverSampled.at(index) = input.at(index / 2);
				}
				else {
					mOverSampled.at(index) = static_cast<T>(0.0);
				}
			}

			mPreAntiAliasFilter.process(Span<T>::MakeSpan(mOverSampled));
			return Span<T>::MakeSpan(mOverSampled);
		}

		constexpr inline auto downSample() noexcept -> Span<T> {
			Span<T> input = Span<T>::MakeSpan(mOverSampled);
			Span<T> output = Span<T>::MakeSpan(mDownSampled);
			jassert(output.size() == (input.size() / OverSampleRate));
			mPostAntiAliasFilter.process(input);
			mDownSampled.at(0) = input.at(0);
			for(size_t index = 1; index < input.size(); ++index) {
				if(index % 2 == 0) {
					output.at(index / 2) = input.at(index);
				}
			}
		}

		constexpr inline auto operator=(OverSampler<T, OverSampleRate>&& overSampler) noexcept
			-> OverSampler<T, OverSampleRate>& = default;

	  private:
		Hertz mSampleRate = 44100_Hz;
		BiQuadFilter<T> mPreAntiAliasFilter
			= BiQuadFilter<T>::MakeLowpass(mSampleRate / 2.0, mSampleRate* OverSampleRate);
		BiQuadFilter<T> mPostAntiAliasFilter
			= BiQuadFilter<T>::MakeLowpass(mSampleRate / 2.0, mSampleRate* OverSampleRate);
		std::vector<T> mOverSampled = std::vector<T>(mSampleRate * OverSampleRate);
		std::vector<T> mDownSampled = std::vector<T>(mSampleRate);
	};
} // namespace apex::dsp
