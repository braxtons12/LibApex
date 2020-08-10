#include "SidechainModernBus.h"

namespace apex::dsp {
	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto SidechainModernBus<float>::process(float input) noexcept -> float {
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
		-> float {
		float rectified = math::fabsf(input);
		float rectifiedDB = math::Decibels::linearToDecibels(rectified);
		float gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(gainReduction);
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}

	/// @brief Calculates the target gain reduction to apply to the input value
	///
	/// @param input - The input value to calculate gain reduction for
	///
	/// @return - The target gain reduction
	auto SidechainModernBus<double>::process(double input) noexcept -> double {
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
		-> double {
		double rectified = math::fabs(input);
		double rectifiedDB = math::Decibels::linearToDecibels(rectified);
		double gainReduction = mGainComputer->process(rectifiedDB) - rectifiedDB;
		mGainReductionDB = mLevelDetector.process(gainReduction);
		return math::Decibels::decibelsToLinear(mGainReductionDB);
	}
} // namespace apex::dsp
