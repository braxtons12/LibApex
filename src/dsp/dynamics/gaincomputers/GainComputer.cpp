#include "GainComputer.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a `GainComputer` with the given values
		///
		/// @param ratio - The ratio
		/// @param threshold - The threshold, in Decibels
		/// @param kneeWidth - The knee width, in Decibels
		GainComputer<float>::GainComputer(float ratio, float threshold, float kneeWidth) noexcept
			: mRatio(ratio),
			mThreshold(threshold),
			mKneeWidth(kneeWidth)
			{

			}

		/// @brief Sets the ratio to the given value
		///
		/// @param ratio - The new ratio
		void GainComputer<float>::setRatio(float ratio) noexcept {
			mRatio = ratio;
		}

		/// @brief Returns the current ratio
		///
		/// @return - The current ratio
		float GainComputer<float>::getRatio() const noexcept {
			return mRatio;
		}

		/// @brief Sets the threshold to the given value
		///
		/// @param threshold - The new threshold, in Decibels
		void GainComputer<float>::setThreshold(float threshold) noexcept {
			mThreshold = threshold;
		}

		/// @brief Returns the current threshold
		///
		/// @return - The current threshold, in Decibels
		float GainComputer<float>::getThreshold() const noexcept {
			return mThreshold;
		}

		/// @brief Sets the knee width to the given value
		///
		/// @param kneeWidth - The new knee width, in Decibels
		void GainComputer<float>::setKneeWidth(float kneeWidth) noexcept {
			mKneeWidth = kneeWidth;
		}

		/// @brief Returns the current knee width
		///
		/// @return - The current knee width, in Decibels
		float GainComputer<float>::getKneeWidth() const noexcept {
			return mKneeWidth;
		}

		/// @brief Constructs a `GainComputer` with the given values
		///
		/// @param ratio - The ratio
		/// @param threshold - The threshold, in Decibels
		/// @param kneeWidth - The knee width, in Decibels
		GainComputer<double>::GainComputer(double ratio, double threshold, double kneeWidth) noexcept
			: mRatio(ratio),
			mThreshold(threshold),
			mKneeWidth(kneeWidth)
			{

			}

		/// @brief Sets the ratio to the given value
		///
		/// @param ratio - The new ratio
		void GainComputer<double>::setRatio(double ratio) noexcept {
			mRatio = ratio;
		}

		/// @brief Returns the current ratio
		///
		/// @return - The current ratio
		double GainComputer<double>::getRatio() const noexcept {
			return mRatio;
		}

		/// @brief Sets the threshold to the given value
		///
		/// @param threshold - The new threshold, in Decibels
		void GainComputer<double>::setThreshold(double threshold) noexcept {
			mThreshold = threshold;
		}

		/// @brief Returns the current threshold
		///
		/// @return - The current threshold, in Decibels
		double GainComputer<double>::getThreshold() const noexcept {
			return mThreshold;
		}

		/// @brief Sets the knee width to the given value
		///
		/// @param kneeWidth - The new knee width, in Decibels
		void GainComputer<double>::setKneeWidth(double kneeWidth) noexcept {
			mKneeWidth = kneeWidth;
		}

		/// @brief Returns the current knee width
		///
		/// @return - The current knee width, in Decibels
		double GainComputer<double>::getKneeWidth() const noexcept {
			return mKneeWidth;
		}
	}
}
