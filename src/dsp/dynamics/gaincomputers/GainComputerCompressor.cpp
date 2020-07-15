#include "GainComputerCompressor.h"

namespace apex {
	namespace dsp {

		///@brief Constructs a `GainComputerCompressor` with the following defaults:
		/// * ratio: 1.1
		/// * threshold: -12dB
		/// * knee width: 6dB
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

		///@brief Constructs a `GainComputerCompressor` with the following defaults:
		/// * ratio: 1.1
		/// * threshold: -12dB
		/// * knee width: 6dB
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
