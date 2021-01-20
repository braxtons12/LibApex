#pragma once

#include <array>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "../dynamics/sidechains/Sidechain1176.h"
#include "../gainstages/GainStageFET.h"
#include "BaseCompressor.h"

namespace apex::dsp {
	template<typename FloatType, std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Compressor1176 final : public BaseCompressor<FloatType> {
	  private:
		using BaseCompressor = BaseCompressor<FloatType>;
		using Sidechain1176 = Sidechain1176<FloatType>;
		using GainStageFET = GainStageFET<FloatType>;
		using Processor = Processor<FloatType>;

	  public:
		Compressor1176() noexcept {
			BaseCompressor::mInputStage = std::make_unique<GainStageFET>();

			BaseCompressor::mOutputStage = std::make_unique<GainStageFET>();
		}
		Compressor1176(Compressor1176&& compressor) noexcept = default;
		~Compressor1176() noexcept final = default;

		[[nodiscard]] inline auto processMono(FloatType input) noexcept -> FloatType final {
			BaseCompressor::mInputMeter.update(input);
			auto procced = BaseCompressor::mInputStage->process(input);
			auto sidechain = procced;
			if(BaseCompressor::mSidechainHPFEnabled) {
				sidechain = BaseCompressor::mSidechainFilter.at(Processor::MONO).process(sidechain);
			}
			if(BaseCompressor::mPreEmphasisMode == SidechainPreEmphasisFilterMode::Soft) {
				sidechain
					= BaseCompressor::mSoftLowShelfFilter.at(Processor::MONO).process(sidechain);
				sidechain
					= BaseCompressor::mSoftHighShelfFilter.at(Processor::MONO).process(sidechain);
			}
			else if(BaseCompressor::mPreEmphasisMode == SidechainPreEmphasisFilterMode::Hard) {
				sidechain
					= BaseCompressor::mHardLowShelfFilter.at(Processor::MONO).process(sidechain);
				sidechain
					= BaseCompressor::mHardHighShelfFilter.at(Processor::MONO).process(sidechain);
			}
			BaseCompressor::mCompressionGain.at(Processor::MONO)
				= mSidechains.at(Processor::MONO).process(sidechain);
			procced *= narrow_cast<FloatType>(
				Decibels::toLinear(BaseCompressor::mCompressionGain.at(Processor::MONO)
								   * BaseCompressor::mCompressionProportion));
			BaseCompressor::mCurrentGainReduction
				= BaseCompressor::mCompressionGain.at(Processor::MONO)
				  * BaseCompressor::mCompressionProportion;
			procced = BaseCompressor::mOutputStage->process(input);
			BaseCompressor::mOutputMeter.update(procced);
			if(BaseCompressor::mAutoMakeupEnabled) {
				BaseCompressor::mMakeupGain = BaseCompressor::mOutputMeter.getLevelDB()
											  - BaseCompressor::mInputMeter.getLevelDB();
				procced *= narrow_cast<FloatType>(BaseCompressor::mMakeupGain.getLinear());
			}
			return procced * BaseCompressor::mMixProportion
				   + (narrow_cast<FloatType>(1.0) - BaseCompressor::mMixProportion) * input;
		}

		inline auto
		processMono(Span<FloatType> input, Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = processMono(input.at(i));
			}
		}

		inline auto
		processMono(Span<const FloatType> input, Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == output.size());
			auto size = input.size();
			for(auto i = 0U; i < size; ++i) {
				output.at(i) = processMono(input.at(i));
			}
		}

		[[nodiscard]] inline auto
		processMonoSidechained(FloatType input, FloatType sidechain) noexcept -> FloatType final {
			BaseCompressor::mInputMeter.update(input);
			auto procced = BaseCompressor::mInputStage->process(input);
			sidechain = BaseCompressor::mInputStage->process(input);
			if(BaseCompressor::mSidechainHPFEnabled) {
				sidechain = BaseCompressor::mSidechainFilter.at(Processor::MONO).process(sidechain);
			}
			if(BaseCompressor::mPreEmphasisMode == SidechainPreEmphasisFilterMode::Soft) {
				sidechain
					= BaseCompressor::mSoftLowShelfFilter.at(Processor::MONO).process(sidechain);
				sidechain
					= BaseCompressor::mSoftHighShelfFilter.at(Processor::MONO).process(sidechain);
			}
			else if(BaseCompressor::mPreEmphasisMode == SidechainPreEmphasisFilterMode::Hard) {
				sidechain
					= BaseCompressor::mHardLowShelfFilter.at(Processor::MONO).process(sidechain);
				sidechain
					= BaseCompressor::mHardHighShelfFilter.at(Processor::MONO).process(sidechain);
			}
			BaseCompressor::mCompressionGain.at(Processor::MONO)
				= mSidechains.at(Processor::MONO).process(sidechain);
			procced *= narrow_cast<FloatType>(
				Decibels::toLinear(BaseCompressor::mCompressionGain.at(Processor::MONO)
								   * BaseCompressor::mCompressionProportion));
			BaseCompressor::mCurrentGainReduction
				= BaseCompressor::mCompressionGain.at(Processor::MONO)
				  * BaseCompressor::mCompressionProportion;
			procced = BaseCompressor::mOutputStage->process(input);
			BaseCompressor::mOutputMeter.update(procced);
			if(BaseCompressor::mAutoMakeupEnabled) {
				BaseCompressor::mMakeupGain = BaseCompressor::mOutputMeter.getLevelDB()
											  - BaseCompressor::mInputMeter.getLevelDB();
				procced *= narrow_cast<FloatType>(BaseCompressor::mMakeupGain.getLinear());
			}
			return procced * BaseCompressor::mMixProportion
				   + (narrow_cast<FloatType>(1.0) - BaseCompressor::mMixProportion) * input;
		}

		inline auto processMonoSidechained(Span<FloatType> input,
										   Span<FloatType> sidechain,
										   Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == sidechain.size() == output.size());
			auto size = input.size();
			for(auto index = 0U; index < size; ++size) {
				output.at(index) = processMonoSidechained(input.at(index), sidechain.at(index));
			}
		}

		inline auto processMonoSidechained(Span<FloatType> input,
										   Span<const FloatType> sidechain,
										   Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == sidechain.size() == output.size());
			auto size = input.size();
			for(auto index = 0U; index < size; ++size) {
				output.at(index) = processMonoSidechained(input.at(index), sidechain.at(index));
			}
		}

		inline auto processMonoSidechained(Span<const FloatType> input,
										   Span<const FloatType> sidechain,
										   Span<FloatType> output) noexcept -> void final {
			jassert(input.size() == sidechain.size() == output.size());
			auto size = input.size();
			for(auto index = 0U; index < size; ++size) {
				output.at(index) = processMonoSidechained(input.at(index), sidechain.at(index));
			}
		}

		[[nodiscard]] inline auto processStereoSidechained(FloatType inputLeft,
														   FloatType inputRight,
														   FloatType sidechainLeft,
														   FloatType sidechainRight) noexcept
			-> std::tuple<FloatType, FloatType> final {
			BaseCompressor::mInputMeter.update(narrow_cast<FloatType>(0.5)
											   * (inputLeft + inputRight));
			auto proccedLeft = BaseCompressor::mInputStage->process(inputLeft);
			auto proccedRight = BaseCompressor::mInputStage->process(inputRight);
			auto sideLeft = BaseCompressor::mInputStage->process(sidechainLeft);
			auto sideRight = BaseCompressor::mInputStage->process(sidechainRight);
#ifdef TESTING_COMPRESSOR_1176
			Logger::LogMessage("Compressor1176: InputStagedLeft:" + juce::String(proccedLeft)
							   + " SidechainStagedLeft" + juce::String(sideLeft));
#endif
			if(BaseCompressor::mSidechainHPFEnabled) {
				sideLeft = BaseCompressor::mSidechainFilter.at(Processor::LEFT).process(sideLeft);
				sideRight
					= BaseCompressor::mSidechainFilter.at(Processor::RIGHT).process(sideRight);
			}
			if(BaseCompressor::mPreEmphasisMode == SidechainPreEmphasisFilterMode::Soft) {
				sideLeft
					= BaseCompressor::mSoftLowShelfFilter.at(Processor::LEFT).process(sideLeft);
				sideLeft
					= BaseCompressor::mSoftHighShelfFilter.at(Processor::LEFT).process(sideLeft);
				sideRight
					= BaseCompressor::mSoftLowShelfFilter.at(Processor::RIGHT).process(sideRight);
				sideRight
					= BaseCompressor::mSoftHighShelfFilter.at(Processor::RIGHT).process(sideRight);
			}
			else if(BaseCompressor::mPreEmphasisMode == SidechainPreEmphasisFilterMode::Hard) {
				sideLeft
					= BaseCompressor::mHardLowShelfFilter.at(Processor::LEFT).process(sideLeft);
				sideLeft
					= BaseCompressor::mHardHighShelfFilter.at(Processor::LEFT).process(sideLeft);
				sideRight
					= BaseCompressor::mHardLowShelfFilter.at(Processor::RIGHT).process(sideRight);
				sideRight
					= BaseCompressor::mHardHighShelfFilter.at(Processor::RIGHT).process(sideRight);
			}
#ifdef TESTING_COMPRESSOR_1176
			Logger::LogMessage("Compressor1176: SidechainFilteredLeft:" + juce::String(sideLeft));
#endif
			BaseCompressor::mCompressionGain.at(Processor::LEFT)
				= mSidechains.at(Processor::LEFT).process(sideLeft);
			BaseCompressor::mCompressionGain.at(Processor::RIGHT)
				= mSidechains.at(Processor::RIGHT).process(sideRight);
#ifdef TESTING_COMPRESSOR_1176
			Logger::LogMessage(
				"Compressor1176: RawGainReductionLeft:"
				+ juce::String(static_cast<double>(BaseCompressor::mCompressionGain.at(0))));
#endif
			Decibels leftGain = BaseCompressor::mCompressionGain.at(Processor::LEFT)
								* BaseCompressor::mCompressionProportion;
			Decibels rightGain = BaseCompressor::mCompressionGain.at(Processor::RIGHT)
								 * BaseCompressor::mCompressionProportion;
			BaseCompressor::mCurrentGainReduction
				= narrow_cast<FloatType>(0.5) * BaseCompressor::mCompressionProportion
				  * (BaseCompressor::mCompressionGain.at(Processor::LEFT)
					 + BaseCompressor::mCompressionGain.at(Processor::RIGHT));
			Decibels leftGainLinked
				= BaseCompressor::mStereoLinkProportion * rightGain
				  + (narrow_cast<FloatType>(1.0) - BaseCompressor::mStereoLinkProportion)
						* leftGain;
			Decibels rightGainLinked
				= BaseCompressor::mStereoLinkProportion * leftGain
				  + (narrow_cast<FloatType>(1.0) - BaseCompressor::mStereoLinkProportion)
						* rightGain;
#ifdef TESTING_COMPRESSOR_1176
			Logger::LogMessage("Compressor1176: BlendedAndLinkedGainReductionLeft:"
							   + juce::String(static_cast<double>(leftGainLinked)));
#endif
			BaseCompressor::mCurrentGainReduction
				= narrow_cast<FloatType>(0.5) * (leftGainLinked + rightGainLinked);
			proccedLeft *= narrow_cast<FloatType>(leftGainLinked.getLinear());
			proccedRight *= narrow_cast<FloatType>(rightGainLinked.getLinear());
#ifdef TESTING_COMPRESSOR_1176
			Logger::LogMessage("Compressor1176: CompressedLeft:" + juce::String(proccedLeft));
#endif
			proccedLeft = BaseCompressor::mOutputStage->process(proccedLeft);
			proccedRight = BaseCompressor::mOutputStage->process(proccedRight);
#ifdef TESTING_COMPRESSOR_1176
			Logger::LogMessage("Compressor1176: OutputStagedLeft:" + juce::String(proccedLeft));
#endif
			BaseCompressor::mOutputMeter.update(narrow_cast<FloatType>(0.5)
												* (proccedLeft + proccedRight));
			if(BaseCompressor::mAutoMakeupEnabled) {
				BaseCompressor::mMakeupGain = BaseCompressor::mOutputMeter.getLevelDB()
											  - BaseCompressor::mInputMeter.getLevelDB();
				proccedLeft *= narrow_cast<FloatType>(BaseCompressor::mMakeupGain.getLinear());
				proccedRight *= narrow_cast<FloatType>(BaseCompressor::mMakeupGain.getLinear());
			}
			proccedLeft
				= proccedLeft * BaseCompressor::mMixProportion
				  + (narrow_cast<FloatType>(1.0) - BaseCompressor::mMixProportion) * inputLeft;
			proccedRight
				= proccedRight * BaseCompressor::mMixProportion
				  + (narrow_cast<FloatType>(1.0) - BaseCompressor::mMixProportion) * inputRight;
			return {proccedLeft, proccedRight};
		}

		inline auto processStereoSidechained(Span<FloatType> inputLeft,
											 Span<FloatType> inputRight,
											 Span<FloatType> sidechainLeft,
											 Span<FloatType> sidechainRight,
											 Span<FloatType> outputLeft,
											 Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == sidechainLeft.size()
					== sidechainRight.size() == outputLeft.size() == outputRight.size());
			auto numSamples = inputLeft.size();
			for(auto index = 0U; index < numSamples; ++index) {
				auto [outLeft, outRight] = processStereoSidechained(inputLeft.at(index),
																	inputRight.at(index),
																	sidechainLeft.at(index),
																	sidechainRight.at(index));
#ifdef TESTING_COMPRESSOR_1176
				Logger::LogMessage("Compressing Input");
				Logger::LogMessage("Input Left:" + juce::String(inputLeft.at(index))
								   + " Input Right:" + juce::String(inputRight.at(index)));
				Logger::LogMessage("SidechainLeft:" + juce::String(sidechainLeft.at(index))
								   + " SidechainRight:" + juce::String(sidechainRight.at(index)));
				Logger::LogMessage("OutputLeft:" + juce::String(outLeft)
								   + " OutputRight:" + juce::String(outRight));
#endif
				outputLeft.at(index) = outLeft;
				outputRight.at(index) = outRight;
			}
		}

		inline auto processStereoSidechained(Span<const FloatType> inputLeft,
											 Span<const FloatType> inputRight,
											 Span<const FloatType> sidechainLeft,
											 Span<const FloatType> sidechainRight,
											 Span<FloatType> outputLeft,
											 Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == sidechainLeft.size()
					== sidechainRight.size() == outputLeft.size() == outputRight.size());
			auto numSamples = inputLeft.size();
			for(auto index = 0U; index < numSamples; ++index) {
				auto [outLeft, outRight] = processStereoSidechained(inputLeft.at(index),
																	inputRight.at(index),
																	sidechainLeft.at(index),
																	sidechainRight.at(index));
#ifdef TESTING_COMPRESSOR_1176
				Logger::LogMessage("Compressing Input");
				Logger::LogMessage("Input Left:" + juce::String(inputLeft.at(index))
								   + " Input Right:" + juce::String(inputRight.at(index)));
				Logger::LogMessage("SidechainLeft:" + juce::String(sidechainLeft.at(index))
								   + " SidechainRight:" + juce::String(sidechainRight.at(index)));
				Logger::LogMessage("OutputLeft:" + juce::String(outLeft)
								   + " OutputRight:" + juce::String(outRight));
#endif
				outputLeft.at(index) = outLeft;
				outputRight.at(index) = outRight;
			}
		}

		inline auto processStereoSidechained(Span<FloatType> inputLeft,
											 Span<FloatType> inputRight,
											 Span<const FloatType> sidechainLeft,
											 Span<const FloatType> sidechainRight,
											 Span<FloatType> outputLeft,
											 Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == sidechainLeft.size()
					== sidechainRight.size() == outputLeft.size() == outputRight.size());
			auto numSamples = inputLeft.size();
			for(auto index = 0U; index < numSamples; ++index) {
				auto [outLeft, outRight] = processStereoSidechained(inputLeft.at(index),
																	inputRight.at(index),
																	sidechainLeft.at(index),
																	sidechainRight.at(index));
#ifdef TESTING_COMPRESSOR_1176
				Logger::LogMessage("Compressing Input");
				Logger::LogMessage("Input Left:" + juce::String(inputLeft.at(index))
								   + " Input Right:" + juce::String(inputRight.at(index)));
				Logger::LogMessage("SidechainLeft:" + juce::String(sidechainLeft.at(index))
								   + " SidechainRight:" + juce::String(sidechainRight.at(index)));
				Logger::LogMessage("OutputLeft:" + juce::String(outLeft)
								   + " OutputRight:" + juce::String(outRight));
#endif
				outputLeft.at(index) = outLeft;
				outputRight.at(index) = outRight;
			}
		}

		[[nodiscard]] inline auto processStereo(FloatType inputLeft, FloatType inputRight) noexcept
			-> std::tuple<FloatType, FloatType> final {
			return processStereoSidechained(inputLeft, inputRight, inputLeft, inputRight);
		}

		inline auto processStereo(Span<FloatType> inputLeft,
								  Span<FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [outLeft, outRight] = processStereoSidechained(inputLeft.at(i),
																	inputRight.at(i),
																	inputLeft.at(i),
																	inputRight.at(i));
				outputLeft.at(i) = outLeft;
				outputRight.at(i) = outRight;
			}
		}

		inline auto processStereo(Span<const FloatType> inputLeft,
								  Span<const FloatType> inputRight,
								  Span<FloatType> outputLeft,
								  Span<FloatType> outputRight) noexcept -> void final {
			jassert(inputLeft.size() == inputRight.size() == outputLeft.size()
					== outputRight.size());
			auto size = inputLeft.size();
			for(auto i = 0U; i < size; ++i) {
				auto [outLeft, outRight] = processStereoSidechained(inputLeft.at(i),
																	inputRight.at(i),
																	inputLeft.at(i),
																	inputRight.at(i));
				outputLeft.at(i) = outLeft;
				outputRight.at(i) = outRight;
			}
		}

		inline auto reset() noexcept -> void final {
			BaseCompressor::reset();
		}

		inline auto setSampleRate(Hertz sampleRate) noexcept -> void final {
			BaseCompressor::setSampleRate(sampleRate);
			for(auto& sidechain : mSidechains) {
				sidechain.setSampleRate(sampleRate);
			}
		}

		inline auto setRatioProportional(FloatType ratioProportional) noexcept -> void final {
			jassert(ratioProportional >= narrow_cast<FloatType>(0.0)
					&& ratioProportional <= narrow_cast<FloatType>(1.0));
			auto ratio = static_cast<Ratio1176>(ratioProportional * MAX_RATIO_INDEX);
			for(auto& sidechain : mSidechains) {
				sidechain.setRatio(ratio);
			}
		}

		[[nodiscard]] inline auto getRatio() const noexcept -> Option<FloatType> final {
			auto ratio = mSidechains.at(0).getEnumRatio();
			switch(ratio) {
				case Ratio1176::FourToOne: return Option<FloatType>::Some(4.0);
				case Ratio1176::EightToOne: return Option<FloatType>::Some(8.0);
				case Ratio1176::TwelveToOne: return Option<FloatType>::Some(12.0);
				case Ratio1176::TwentyToOne: return Option<FloatType>::Some(20.0);
				case Ratio1176::AllButtonsIn: return Option<FloatType>::Some(24.0);
			}
		}

		[[nodiscard]] inline auto getMaxRatio() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(MAX_RATIO);
		}

		[[nodiscard]] inline auto getMinRatio() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(MIN_RATIO);
		}

		[[nodiscard]] inline auto isRatioControlEnabled() const noexcept -> bool final {
			return true;
		}

		[[deprecated("Threshold control is disabled for this compressor")]] inline auto
		setThreshold(Decibels threshold) noexcept -> void final {
			juce::ignoreUnused(threshold);
		}

		[[deprecated(
			"Threshold control is disabled for this compressor")]] [[nodiscard]] inline auto
		getThreshold() const noexcept -> Option<Decibels> final {
			return Option<Decibels>::None();
		}

		[[deprecated(
			"Threshold control is disabled for this compressor")]] [[nodiscard]] inline auto
		getMaxThreshold() const noexcept -> Option<Decibels> final {
			return Option<Decibels>::None();
		}

		[[deprecated(
			"Threshold control is disabled for this compressor")]] [[nodiscard]] inline auto
		getMinThreshold() const noexcept -> Option<Decibels> final {
			return Option<Decibels>::None();
		}

		[[nodiscard]] inline auto isThresholdControlEnabled() const noexcept -> bool final {
			return false;
		}

		[[deprecated("Knee control is disabled for this compressor")]] inline auto
		setKneeWidth(Decibels kneeWidth) noexcept -> void final {
			juce::ignoreUnused(kneeWidth);
		}

		[[deprecated("Knee control is disabled for this compressor")]] [[nodiscard]] inline auto
		getKneeWidth() const noexcept -> Option<Decibels> final {
			return Option<Decibels>::None();
		}

		[[deprecated("Knee control is disabled for this compressor")]] [[nodiscard]] inline auto
		getMaxKneeWidth() const noexcept -> Option<Decibels> final {
			return Option<Decibels>::None();
		}

		[[deprecated("Knee control is disabled for this compressor")]] [[nodiscard]] inline auto
		getMinKneeWidth() const noexcept -> Option<Decibels> final {
			return Option<Decibels>::None();
		}

		[[nodiscard]] inline auto isKneeControlEnabled() const noexcept -> bool final {
			return false;
		}

		inline auto setAttackProportional(FloatType attackProportional) noexcept -> void final {
			jassert(attackProportional >= narrow_cast<FloatType>(0.0)
					&& attackProportional <= narrow_cast<FloatType>(1.0));
			auto attack = attackProportional * (MAX_ATTACK - MIN_ATTACK) + MIN_ATTACK;
			for(auto& sidechain : mSidechains) {
				sidechain.setAttackTime(attack * narrow_cast<FloatType>(1000.0));
			}
		}

		[[nodiscard]] inline auto getAttackSeconds() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(mSidechains.at(0).getAttackTime()
										   / narrow_cast<FloatType>(1000.0));
		}

		[[nodiscard]] inline auto getMaxAttackSeconds() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(MAX_ATTACK);
		}

		[[nodiscard]] inline auto getMinAttackSeconds() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(MIN_ATTACK);
		}

		[[nodiscard]] inline auto isAttackControlEnabled() const noexcept -> bool final {
			return true;
		}

		inline auto setReleaseProportional(FloatType releaseProportional) noexcept -> void final {
			jassert(releaseProportional >= narrow_cast<FloatType>(0.0)
					&& releaseProportional <= narrow_cast<FloatType>(1.0));
			auto release = releaseProportional * (MAX_RELEASE - MIN_RELEASE) + MIN_RELEASE;
			for(auto& sidechain : mSidechains) {
				sidechain.setAttackTime(release * narrow_cast<FloatType>(1000.0));
			}
		}

		[[nodiscard]] inline auto getReleaseSeconds() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(mSidechains.at(0).getReleaseTime()
										   / narrow_cast<FloatType>(1000.0));
		}

		[[nodiscard]] inline auto getMaxReleaseSeconds() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(MAX_ATTACK);
		}

		[[nodiscard]] inline auto getMinReleaseSeconds() const noexcept -> Option<FloatType> final {
			return Option<FloatType>::Some(MIN_ATTACK);
		}

		[[nodiscard]] inline auto isReleaseControlEnabled() const noexcept -> bool final {
			return true;
		}

	  private:
		static const constexpr size_t MAX_RATIO_INDEX
			= static_cast<size_t>(Ratio1176::AllButtonsIn);
		static const constexpr FloatType MAX_RATIO = narrow_cast<FloatType>(24.0);
		static const constexpr FloatType MIN_RATIO = narrow_cast<FloatType>(4.0);
		static const constexpr FloatType MAX_ATTACK = Sidechain1176::MAX_ATTACK_SECONDS;
		static const constexpr FloatType MIN_ATTACK = Sidechain1176::MIN_ATTACK_SECONDS;
		static const constexpr FloatType MAX_RELEASE = Sidechain1176::MAX_RELEASE_SECONDS;
		static const constexpr FloatType MIN_RELEASE = Sidechain1176::MIN_RELEASE_SECONDS;
		std::array<Sidechain1176, Processor::MAX_CHANNELS> mSidechains
			= std::array<Sidechain1176, Processor::MAX_CHANNELS>();
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor1176)
	};
} // namespace apex::dsp
