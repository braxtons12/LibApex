#include "ApexMeter.h"

namespace apex::ui {
	/// @brief Constructs an `ApexMeter` with the given parameters
	///
	/// @param proportionOfHeightToValueFunc - The function to convert the proportion of height
	/// to the user/dsp facing value
	/// @param valueToProportionOfHeightFunc - The function to convert the user/dsp facing value
	/// to the proportion of height
	/// @param clipLevelProportional - The proportion of the height for which "clipping"
	/// indication should start
	ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						 std::function<double(double)> valueToProportionOfHeightFunc,
						 double clipLevelProportional) noexcept
		: mProportionOfHeightToValueFunc(std::move(proportionOfHeightToValueFunc)),
		  mValueToProportionOfHeightFunc(std::move(valueToProportionOfHeightFunc)),
		  mClipLevelProportion(clipLevelProportional) {
	}

	/// @brief Constructs an `ApexMeter` with the given parameters
	///
	/// @param proportionOfHeightToValueFunc - The function to convert the proportion of height
	/// to the user/dsp facing value
	/// @param valueToProportionOfHeightFunc - The function to convert the user/dsp facing value
	/// to the proportion of height
	/// @param clipLevelProportional - The proportion of the height for which "clipping"
	/// indication should start
	/// @param numSteps - The number of equally spaced level "ticks" on the meter
	ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						 std::function<double(double)> valueToProportionOfHeightFunc,
						 double clipLevelProportional,
						 size_t numSteps) noexcept
		: mProportionOfHeightToValueFunc(std::move(proportionOfHeightToValueFunc)),
		  mValueToProportionOfHeightFunc(std::move(valueToProportionOfHeightFunc)),
		  mNumSteps(numSteps), mClipLevelProportion(clipLevelProportional) {
	}

	/// @brief Constructs an `ApexMeter` that uses a film strip image asset to draw itself
	///
	/// @param proportionOfFilmStripToValueFunc - The function to convert the proportion of the
	/// number of film strip frames to the user/dsp facing value
	/// @param valueToProportionOfFilmStripFunc - The function to convert the user/dsp facing
	/// value to the proportion of the number of film strip frames
	/// @param filmStrip - The film strip asset used to visually represent this
	ApexMeter::ApexMeter(std::function<double(double)> proportionOfFilmStripToValueFunc,
						 std::function<double(double)> valueToProportionOfFilmStripFunc,
						 ApexFilmStrip filmStrip) noexcept
		: mProportionOfHeightToValueFunc(std::move(proportionOfFilmStripToValueFunc)),
		  mValueToProportionOfHeightFunc(std::move(valueToProportionOfFilmStripFunc)),
		  mFilmStrip(std::move(filmStrip)), mUsesFilmStrip(true) {
	}

	/// @brief Constructs an `ApexMeter` that uses an image of the meter at its max level to
	/// draw itself
	///
	/// @param proportionOfHeightToValueFunc - The function to convert the proportion of the
	/// meter height to the corresponding user/dsp facing value
	/// @param valueToProportionOfHeightFunc - The function to convert the user/dsp facing value
	/// to the corresponding proportion of meter height
	/// @param maxedMeterImage - The image of the meter at its maximum level
	ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						 std::function<double(double)> valueToProportionOfHeightFunc,
						 juce::Image maxedMeterImage) noexcept
		: mProportionOfHeightToValueFunc(std::move(proportionOfHeightToValueFunc)),
		  mValueToProportionOfHeightFunc(std::move(valueToProportionOfHeightFunc)),
		  mMaxedMeterImage(std::move(maxedMeterImage)), mUsesMaxedImage(true) {
	}

	/// @brief Draws this meter to the screen
	///
	/// @param g - The graphics context to use to draw this
	auto ApexMeter::paint(juce::Graphics& g) noexcept -> void {
		if(mLookAndFeel != nullptr) {
			mLookAndFeel->drawApexMeter(g,
										static_cast<float>(mLevel),
										static_cast<float>(mClipLevelProportion),
										mNumSteps,
										*this);
		}
	}
} // namespace apex::ui
