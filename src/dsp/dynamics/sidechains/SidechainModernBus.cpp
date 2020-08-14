#include "SidechainModernBus.h"

namespace apex::dsp {
	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto SidechainModernBus<float>::process(float input) noexcept -> Decibels {
		return processFeedForwardAlternateReturnToThreshold(input);
	}

	/// @brief Sets the DynamicsType
	///
	/// @param type - The dynamics type
	void SidechainModernBus<float>::setDynamicsType(DynamicsType type) noexcept {
		mDynamicsType = type;
		if(mDynamicsType == DynamicsType::Compressor) {
			mGainComputer = &mCompressorComputer;
		}
		else {
			mGainComputer = &mExpanderComputer;
		}
	}

	auto
	SidechainModernBus<float>::processFeedForwardAlternateReturnToThreshold(float input) noexcept
		-> Decibels {
		float rectified = math::fabsf(input);
		Decibels rectifiedDB = Decibels::fromLinear(rectified);
		Decibels gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(static_cast<float>(gainReduction));
		mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
		return mGainReductionDB;
	}

	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto SidechainModernBus<double>::process(double input) noexcept -> Decibels {
		return processFeedForwardAlternateReturnToThreshold(input);
	}

	/// @brief Sets the DynamicsType
	///
	/// @param type - The dynamics type
	void SidechainModernBus<double>::setDynamicsType(DynamicsType type) noexcept {
		mDynamicsType = type;
		if(mDynamicsType == DynamicsType::Compressor) {
			mGainComputer = &mCompressorComputer;
		}
		else {
			mGainComputer = &mExpanderComputer;
		}
	}

	auto
	SidechainModernBus<double>::processFeedForwardAlternateReturnToThreshold(double input) noexcept
		-> Decibels {
		double rectified = math::fabs(input);
		Decibels rectifiedDB = Decibels::fromLinear(rectified);
		Decibels gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(static_cast<double>(gainReduction));
		mGainReductionDB = mGainReductionProcessor.adjustedGainReduction(mGainReductionDB);
		return mGainReductionDB;
	}
} // namespace apex::dsp
