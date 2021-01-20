#pragma once

#include <array>
#include <memory>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "../filters/BiQuadFilter.h"
#include "../gainstages/GainStage.h"
#include "../meters/RMSMeter.h"
#include "Gain.h"
#include "Processor.h"

namespace apex::dsp {

	enum class SidechainPreEmphasisFilterMode
	{
		Disabled,
		Soft,
		Hard
	};

	template<typename FloatType, std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class BaseCompressor : Processor<FloatType> {
	  private:
		using RMSMeter = RMSMeter<FloatType>;
		using BiQuadFilter = BiQuadFilter<FloatType>;
		using GainStage = GainStage<FloatType>;
		using Processor = Processor<FloatType>;

	  public:
		BaseCompressor() noexcept {
			auto one = narrow_cast<FloatType>(1.0);
			for(auto& filt : mSidechainFilter) {
				filt = BiQuadFilter::MakeHighpass(mSidechainHPFCutoffFreq, one, 44.1_kHz);
			}
			for(auto& filt : mSoftLowShelfFilter) {
				filt = BiQuadFilter::MakeLowShelf(240_Hz, one, -8_dB, 44.1_kHz);
			}
			for(auto& filt : mSoftHighShelfFilter) {
				filt = BiQuadFilter::MakeHighShelf(2.4_kHz, one, 8_dB, 44.1_kHz);
			}
			for(auto& filt : mHardLowShelfFilter) {
				filt = BiQuadFilter::MakeLowShelf(700_Hz, one, -8_dB, 44.1_kHz);
			}
			for(auto& filt : mHardHighShelfFilter) {
				filt = BiQuadFilter::MakeHighShelf(700_Hz, one, 8_dB, 44.1_kHz);
			}
		}
		BaseCompressor(BaseCompressor&& compressor) noexcept = default;
		~BaseCompressor() noexcept override = default;

		[[nodiscard]] virtual auto
		processMonoSidechained(FloatType input, FloatType sidechain) noexcept -> FloatType = 0;

		virtual auto processMonoSidechained(Span<FloatType> input,
											Span<FloatType> sidechain,
											Span<FloatType> output) noexcept -> void
			= 0;

		virtual auto processMonoSidechained(Span<FloatType> input,
											Span<const FloatType> sidechain,
											Span<FloatType> output) noexcept -> void
			= 0;

		virtual auto processMonoSidechained(Span<const FloatType> input,
											Span<const FloatType> sidechain,
											Span<FloatType> output) noexcept -> void
			= 0;

		[[nodiscard]] virtual auto processStereoSidechained(FloatType inputLeft,
															FloatType inputRight,
															FloatType sidechainLeft,
															FloatType sidechainRight) noexcept
			-> std::tuple<FloatType, FloatType> = 0;

		virtual auto processStereoSidechained(Span<FloatType> inputLeft,
											  Span<FloatType> inputRight,
											  Span<FloatType> sidechainLeft,
											  Span<FloatType> sidechainRight,
											  Span<FloatType> outputLeft,
											  Span<FloatType> outputRight) noexcept -> void
			= 0;

		virtual auto processStereoSidechained(Span<FloatType> inputLeft,
											  Span<FloatType> inputRight,
											  Span<const FloatType> sidechainLeft,
											  Span<const FloatType> sidechainRight,
											  Span<FloatType> outputLeft,
											  Span<FloatType> outputRight) noexcept -> void
			= 0;

		virtual auto processStereoSidechained(Span<const FloatType> inputLeft,
											  Span<const FloatType> inputRight,
											  Span<const FloatType> sidechainLeft,
											  Span<const FloatType> sidechainRight,
											  Span<FloatType> outputLeft,
											  Span<FloatType> outputRight) noexcept -> void
			= 0;

		inline auto reset() noexcept -> void override {
			mInputMeter.reset();
			mOutputMeter.reset();
			for(auto& filt : mSidechainFilter) {
				filt.reset();
			}
			for(auto& filt : mSoftLowShelfFilter) {
				filt.reset();
			}
			for(auto& filt : mSoftHighShelfFilter) {
				filt.reset();
			}
			for(auto& filt : mHardLowShelfFilter) {
				filt.reset();
			}
			for(auto& filt : mHardHighShelfFilter) {
				filt.reset();
			}
		}

		[[nodiscard]] inline auto getCurrentGainReduction() const noexcept -> Decibels {
			return mCurrentGainReduction;
		}

		virtual inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
			mSampleRate = sampleRate;
			mInputMeter.setSampleRate(sampleRate);
			mOutputMeter.setSampleRate(sampleRate);
			for(auto& filt : mSidechainFilter) {
				filt.setSampleRate(sampleRate);
			}
			for(auto& filt : mSoftLowShelfFilter) {
				filt.setSampleRate(sampleRate);
			}
			for(auto& filt : mSoftHighShelfFilter) {
				filt.setSampleRate(sampleRate);
			}
			for(auto& filt : mHardLowShelfFilter) {
				filt.setSampleRate(sampleRate);
			}
			for(auto& filt : mHardHighShelfFilter) {
				filt.setSampleRate(sampleRate);
			}
		}

		[[nodiscard]] inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		virtual auto setRatioProportional(FloatType ratioProportional) noexcept -> void = 0;
		[[nodiscard]] virtual auto getRatio() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto getMaxRatio() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto getMinRatio() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto isRatioControlEnabled() const noexcept -> bool = 0;

		virtual auto setThreshold(Decibels threshold) noexcept -> void = 0;
		[[nodiscard]] virtual auto getThreshold() const noexcept -> Option<Decibels> = 0;
		[[nodiscard]] virtual auto getMaxThreshold() const noexcept -> Option<Decibels> = 0;
		[[nodiscard]] virtual auto getMinThreshold() const noexcept -> Option<Decibels> = 0;
		[[nodiscard]] virtual auto isThresholdControlEnabled() const noexcept -> bool = 0;

		virtual auto setKneeWidth(Decibels threshold) noexcept -> void = 0;
		[[nodiscard]] virtual auto getKneeWidth() const noexcept -> Option<Decibels> = 0;
		[[nodiscard]] virtual auto getMaxKneeWidth() const noexcept -> Option<Decibels> = 0;
		[[nodiscard]] virtual auto getMinKneeWidth() const noexcept -> Option<Decibels> = 0;
		[[nodiscard]] virtual auto isKneeControlEnabled() const noexcept -> bool = 0;

		virtual auto setAttackProportional(FloatType attackProportional) noexcept -> void = 0;
		[[nodiscard]] virtual auto getAttackSeconds() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto getMaxAttackSeconds() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto getMinAttackSeconds() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto isAttackControlEnabled() const noexcept -> bool = 0;

		virtual auto setReleaseProportional(FloatType attackProportional) noexcept -> void = 0;
		[[nodiscard]] virtual auto getReleaseSeconds() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto getMaxReleaseSeconds() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto getMinReleaseSeconds() const noexcept -> Option<FloatType> = 0;
		[[nodiscard]] virtual auto isReleaseControlEnabled() const noexcept -> bool = 0;

		virtual inline auto setStereoLinkProportion(FloatType proportion) noexcept -> void {
			jassert(proportion >= narrow_cast<FloatType>(0.0));
			mStereoLinkProportion = proportion * narrow_cast<FloatType>(0.5);
		}

		[[nodiscard]] inline auto getStereoLinkProportion() const noexcept -> FloatType {
			return mStereoLinkProportion;
		}

		inline auto setMixProportion(FloatType proportion) noexcept -> void {
			jassert(proportion >= narrow_cast<FloatType>(0.0));
			mMixProportion = proportion;
		}

		[[nodiscard]] inline auto getMixProportion() const noexcept -> FloatType {
			return mMixProportion;
		}

		inline auto setCompressionProportion(FloatType proportion) noexcept -> void {
			jassert(proportion >= narrow_cast<FloatType>(0.0));
			mCompressionProportion(proportion);
		}

		[[nodiscard]] inline auto getCompressionProportion() const noexcept -> FloatType {
			return mCompressionProportion;
		}

		virtual inline auto setSidechainHPFCutoffFrequency(Hertz frequency) noexcept -> void {
			mSidechainHPFCutoffFreq = frequency;
		}

		[[nodiscard]] inline auto getSidechainHPFCutoffFrequency() const noexcept -> Hertz {
			return mSidechainHPFCutoffFreq;
		}

		inline auto enableSidechainHPF() noexcept -> void {
			mSidechainHPFEnabled = true;
		}

		inline auto disableSidechainHPF() noexcept -> void {
			mSidechainHPFEnabled = false;
		}

		[[nodiscard]] inline auto isSidechainHPFEnabled() const noexcept -> bool {
			return mSidechainHPFEnabled;
		}

		inline auto
		setSidechainPreEmphasisMode(SidechainPreEmphasisFilterMode mode) noexcept -> void {
			mPreEmphasisMode = mode;
		}

		inline auto enableAutoMakeupGain() noexcept -> void {
			mAutoMakeupEnabled = true;
		}

		inline auto disableAutoMakeupGain() noexcept -> void {
			mAutoMakeupEnabled = false;
		}

		[[nodiscard]] inline auto isAutoMakeupGainEnabled() noexcept -> bool {
			return mAutoMakeupEnabled;
		}

		virtual inline auto setInputStage(std::unique_ptr<GainStage> stage) noexcept -> void {
			mInputStage = std::move(stage);
		}

		virtual inline auto setOutputStage(std::unique_ptr<GainStage> stage) noexcept -> void {
			mOutputStage = std::move(stage);
		}

		auto operator=(BaseCompressor&& compressor) noexcept -> BaseCompressor& = default;

	  protected:
		Hertz mSampleRate = 44.1_kHz;
		Hertz mSidechainHPFCutoffFreq = 100_Hz;
		Decibels mCurrentGainReduction = 0.0_dB;
		Decibels mMakeupGain = 0.0_dB;
		FloatType mStereoLinkProportion = narrow_cast<FloatType>(0.5);
		FloatType mMixProportion = narrow_cast<FloatType>(1.0);
		FloatType mCompressionProportion = narrow_cast<FloatType>(1.0);
		bool mAutoMakeupEnabled = false;
		bool mSidechainHPFEnabled = false;
		SidechainPreEmphasisFilterMode mPreEmphasisMode = SidechainPreEmphasisFilterMode::Disabled;
		RMSMeter mInputMeter = RMSMeter(mSampleRate);
		RMSMeter mOutputMeter = RMSMeter(mSampleRate);
		std::unique_ptr<GainStage> mInputStage = std::make_unique<GainStage>();
		std::unique_ptr<GainStage> mOutputStage = std::make_unique<GainStage>();
		std::array<Decibels, Processor::MAX_CHANNELS> mCompressionGain
			= std::array<Decibels, Processor::MAX_CHANNELS>();
		std::array<BiQuadFilter, Processor::MAX_CHANNELS> mSidechainFilter
			= std::array<BiQuadFilter, Processor::MAX_CHANNELS>();
		std::array<BiQuadFilter, Processor::MAX_CHANNELS> mSoftLowShelfFilter
			= std::array<BiQuadFilter, Processor::MAX_CHANNELS>();
		std::array<BiQuadFilter, Processor::MAX_CHANNELS> mSoftHighShelfFilter
			= std::array<BiQuadFilter, Processor::MAX_CHANNELS>();
		std::array<BiQuadFilter, Processor::MAX_CHANNELS> mHardLowShelfFilter
			= std::array<BiQuadFilter, Processor::MAX_CHANNELS>();
		std::array<BiQuadFilter, Processor::MAX_CHANNELS> mHardHighShelfFilter
			= std::array<BiQuadFilter, Processor::MAX_CHANNELS>();

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCompressor)
	};
} // namespace apex::dsp
