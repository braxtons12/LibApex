#pragma once

#include <array>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "../filters/BiQuadFilter.h"
#include "juce_core/system/juce_PlatformDefs.h"

namespace apex::dsp {
	template<typename FloatType,
			 size_t OverSampleRate = 2,
			 std::enable_if_t<(OverSampleRate % 2) == 0 && std::is_floating_point_v<FloatType>,
							  bool> = true>
	class OverSampler {
	  private:
		using BiQuadFilter = BiQuadFilter<FloatType>;

	  public:
		constexpr OverSampler() noexcept = default;
		constexpr explicit OverSampler(Hertz sampleRate) noexcept : mSampleRate(sampleRate) {
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Creating Oversampler");
#endif
		}
		constexpr OverSampler(OverSampler&& overSampler) noexcept = default;

		[[nodiscard]] constexpr inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		[[nodiscard]] constexpr inline auto getOverSampledSampleRate() const noexcept -> Hertz {
			return mSampleRate * OverSampleRate;
		}

		constexpr inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Updating Oversampler Sample Rate");
#endif
			mSampleRate = sampleRate;
			mPreAntiAliasFilter.setSampleRate(mSampleRate * OverSampleRate);
			mPreAntiAliasFilter.setFrequency(narrow_cast<FloatType>(mSampleRate)
											 / narrow_cast<FloatType>(2.0));
			mPostAntiAliasFilter.setSampleRate(mSampleRate * OverSampleRate);
			mPostAntiAliasFilter.setFrequency(narrow_cast<FloatType>(mSampleRate)
											  / narrow_cast<FloatType>(2.0));
		}

		constexpr inline auto setBufferSize(size_t bufferSize) noexcept -> void {
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Updating Oversampler Buffer Size");
#endif
			mBufferSize = bufferSize;
			mOverSampledSize = OverSampleRate * bufferSize;
		}

		constexpr inline auto reset() noexcept -> void {
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Resetting Oversampler");
#endif

			mPreAntiAliasFilter.reset();
			mPostAntiAliasFilter.reset();
			for(auto& sample : mOverSampled) {
				sample = narrow_cast<FloatType>(0.0);
			}
			for(auto& sample : mDownSampled) {
				sample = narrow_cast<FloatType>(0.0);
			}
		}

		[[nodiscard]] constexpr inline auto
		overSample(Span<FloatType> input) noexcept -> Span<FloatType> {
			jassert(input.size() <= mOverSampled.size() / OverSampleRate);

#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Oversampler Copying Samples from input to oversample buffer");
#endif

			auto inputSize = General<FloatType>::min(input.size(), mBufferSize);
			for(size_t index = 0, overSampledIndex = 0; index < inputSize;
				++index, overSampledIndex += OverSampleRate)
			{
				if(overSampledIndex < mOverSampledSize) {
					mOverSampled.at(overSampledIndex) = input.at(index);
				}
			}

#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Oversampler Processing With Antialias filter");
#endif
			auto span = Span<FloatType>::MakeSpan(mOverSampled.data(), mOverSampled.size())
							.first(mOverSampledSize);
			mPreAntiAliasFilter.process(span, span);
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Finished Oversampling");
#endif
			return span;
		}

		[[nodiscard]] constexpr inline auto
		overSample(Span<const FloatType> input) noexcept -> Span<const FloatType> {
			jassert(input.size() <= mOverSampled.size() / OverSampleRate);

#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Oversampler Copying Samples from input to oversample buffer");
#endif

			auto inputSize = General<FloatType>::min(input.size(), mBufferSize);
			for(size_t index = 0, overSampledIndex = 0; index < inputSize;
				++index, overSampledIndex += OverSampleRate)
			{
				if(overSampledIndex < mOverSampledSize) {
					mOverSampled.at(overSampledIndex) = input.at(index);
				}
			}

#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Oversampler Processing With Antialias filter");
#endif
			auto span = Span<FloatType>::MakeSpan(mOverSampled.data(), mOverSampled.size())
							.first(mOverSampledSize);
			mPreAntiAliasFilter.process(span, span);
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Finished Oversampling");
#endif
			auto output = Span<const FloatType>::MakeSpan(mOverSampled.data(), mOverSampled.size())
							  .first(mOverSampledSize);
			return output;
		}

		constexpr inline auto downSample() noexcept -> Span<FloatType> {
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Downsampling from" + juce::String(OverSampleRate)
							   + "X Oversampling");
#endif
			Span<FloatType> input
				= Span<FloatType>::MakeSpan(mOverSampled.data(), mOverSampled.size())
					  .first(mOverSampledSize);
			Span<FloatType> output
				= Span<FloatType>::MakeSpan(mDownSampled.data(), mDownSampled.size())
					  .first(mBufferSize);
			jassert(output.size() <= (input.size() / OverSampleRate));
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Oversampler Processing With Antialias filter");
#endif
			mPostAntiAliasFilter.process(input, input);
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Oversampler Copying oversample buffer samples to output buffer");
#endif
			for(size_t index = 0, overSampledIndex = 0; index < mBufferSize;
				++index, overSampledIndex += OverSampleRate)
			{
				if(overSampledIndex < mOverSampledSize) {
					output.at(index) = input.at(overSampledIndex);
				}
			}
#ifdef TESTING_OVERSAMPLER
			Logger::LogMessage("Finished Downsampling");
#endif
			return output;
		}

		constexpr inline auto
		operator=(OverSampler&& overSampler) noexcept -> OverSampler& = default;

	  private:
		static const constexpr size_t DOWNSAMPLED_MAX_BUFFER = 16384U;
		Hertz mSampleRate = 44.1_kHz;
		size_t mBufferSize = 1024U;
		size_t mOverSampledSize = mBufferSize * OverSampleRate;
		BiQuadFilter mPreAntiAliasFilter = BiQuadFilter::MakeLowpass(
			narrow_cast<FloatType>(mSampleRate) / narrow_cast<FloatType>(2.0),
			mSampleRate* OverSampleRate);
		BiQuadFilter mPostAntiAliasFilter = BiQuadFilter::MakeLowpass(
			narrow_cast<FloatType>(mSampleRate) / narrow_cast<FloatType>(2.0),
			mSampleRate* OverSampleRate);
		std::array<FloatType, DOWNSAMPLED_MAX_BUFFER* OverSampleRate> mOverSampled
			= std::array<FloatType, DOWNSAMPLED_MAX_BUFFER * OverSampleRate>();
		std::array<FloatType, DOWNSAMPLED_MAX_BUFFER> mDownSampled
			= std::array<FloatType, DOWNSAMPLED_MAX_BUFFER>();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverSampler)
	};
} // namespace apex::dsp
