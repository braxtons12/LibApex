#include "GainComputerExpander.h"

namespace apex {
	namespace dsp {

		///@brief Construct a `GainComputerExpander` with the given parameters
		///
		///@param ratio - The ratio
		///@param threshold - The threshold, in Decibels
		///@param kneeWidth - The knee width, in Decibels
		GainComputerExpander<float>::GainComputerExpander(float ratio, float threshold, 
			float kneeWidth) noexcept
		{
			mRatio = ratio;
			mThreshold = threshold;
			mKneeWidth = kneeWidth;
		}

		///@brief Calculates the target expanded output value 
		///
		///@param input - The input to calculate expansion for
		///
		///@return - The target output
		float GainComputerExpander<float>::process(float input) noexcept {
			float twoXMinusT = 2.0f * (input - mThreshold);
			if(twoXMinusT < -mKneeWidth) {
				return mThreshold + (input - mThreshold) * mRatio;
			}
			else if(twoXMinusT > mKneeWidth) {
				return input;
			}
			else {
				return input + (1.0f - mRatio)
					* math::pow2f(input - mThreshold - mKneeWidth / 2.0f) / (2.0f * mKneeWidth);
			}
		}

		///@brief Construct a `GainComputerExpander` with the given parameters
		///
		///@param ratio - The ratio
		///@param threshold - The threshold, in Decibels
		///@param kneeWidth - The knee width, in Decibels
		GainComputerExpander<double>::GainComputerExpander(double ratio, double threshold, 
			double kneeWidth) noexcept
		{
			mRatio = ratio;
			mThreshold = threshold;
			mKneeWidth = kneeWidth;
		}

		///@brief Calculates the target expanded output value 
		///
		///@param input - The input to calculate expansion for
		///
		///@return - The target output
		double GainComputerExpander<double>::process(double input) noexcept {
			double twoXMinusT = 2.0 * (input - mThreshold);
			if(twoXMinusT < -mKneeWidth) {
				return mThreshold + (input - mThreshold) * mRatio;
			}
			else if(twoXMinusT > mKneeWidth) {
				return input;
			}
			else {
				return input + (1.0 - mRatio)
					* math::pow2(input - mThreshold - mKneeWidth / 2.0) / (2.0 * mKneeWidth);
			}
		}
	}
}
