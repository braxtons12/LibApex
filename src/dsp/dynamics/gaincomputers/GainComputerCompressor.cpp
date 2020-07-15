#include "GainComputerCompressor.h"

namespace apex {
	namespace dsp {

		///@brief Construct a `GainComputerCompressor` with the given parameters
		///
		///@param ratio - The ratio
		///@param threshold - The threshold, in Decibels
		///@param kneeWidth - The knee width, in Decibels
		GainComputerCompressor<float>::GainComputerCompressor(float ratio, float threshold, 
			float kneeWidth) noexcept
		{
			mRatio = ratio;
			mThreshold = threshold;
			mKneeWidth = kneeWidth;
		}

		///@brief Calculates the target compressed output value 
		///
		///@param input - The input to calculate compression for
		///
		///@return - The target output
		float GainComputerCompressor<float>::process(float input) noexcept {
			float twoXMinusT = 2.0f * (input - mThreshold);
			if(twoXMinusT < -mKneeWidth) {
				return input;
			}
			else if(twoXMinusT > mKneeWidth) {
				return mThreshold + (input - mThreshold) / mRatio;
			}
			else {
				return input + ( (1.0f / mRatio) - 1.0f)
					* math::pow2f(input - mThreshold + mKneeWidth / 2.0f) / (2.0f * mKneeWidth);
			}
		}

		///@brief Construct a `GainComputerCompressor` with the given parameters
		///
		///@param ratio - The ratio
		///@param threshold - The threshold, in Decibels
		///@param kneeWidth - The knee width, in Decibels
		GainComputerCompressor<double>::GainComputerCompressor(double ratio, double threshold, 
			double kneeWidth) noexcept
		{
			mRatio = ratio;
			mThreshold = threshold;
			mKneeWidth = kneeWidth;
		}

		///@brief Calculates the target compressed output value 
		///
		///@param input - The input to calculate compression for
		///
		///@return - The target output
		double GainComputerCompressor<double>::process(double input) noexcept {
			double twoXMinusT = 2.0 * (input - mThreshold);
			if(twoXMinusT < -mKneeWidth) {
				return input;
			}
			else if(twoXMinusT > mKneeWidth) {
				return mThreshold + (input - mThreshold) / mRatio;
			}
			else {
				return input + ( (1.0 / mRatio) - 1.0)
					* math::pow2(input - mThreshold + mKneeWidth / 2.0) / (2.0 * mKneeWidth);
			}
		}
	}
}
