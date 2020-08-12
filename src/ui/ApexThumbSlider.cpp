#include "ApexThumbSlider.h"

namespace apex::ui {
	/// @brief Constructs an `ApexThumbSlider` with the given style and thumb image
	///
	/// @param style - The style of the slider
	/// @param proportionToValueFunc - The function to convert the proportion of this slider to
	/// the corresponding user/dsp facing value
	/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
	/// corresponding proportion of this slider
	/// @param thumbImage - The image asset to use to draw the slider thumb
	ApexThumbSlider::ApexThumbSlider(juce::Slider::SliderStyle style,
									 std::function<double(double)> proportionToValueFunc,
									 std::function<double(double)> valueToProportionFunc,
									 juce::Image thumbImage) noexcept
		: ApexSlider(style, std::move(proportionToValueFunc), std::move(valueToProportionFunc)),
		  mThumbImage(std::move(thumbImage)), mUsesThumbImage(true) {
		mThumbComponent.setImage(mThumbImage);
		mThumbComponent.setImagePlacement(juce::RectanglePlacement(
			juce::RectanglePlacement::xMid | juce::RectanglePlacement::yTop));
		addAndMakeVisible(mThumbComponent);
		mInitialThumbWidth = mThumbImage.getWidth();
		mInitialThumbHeight = mThumbImage.getHeight();
	}

	/// @brief Constructs an `ApexThumbSlider` with the given style and initial width and height
	///
	/// @param style - The style of the slider
	/// @param proportionToValueFunc - The function to convert the proportion of this slider to
	/// the corresponding user/dsp facing value
	/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
	/// corresponding proportion of this slider
	/// @param initialThumbWidth - The initial width of the thumb (usually the maximum size)
	/// @param initialThumbHeight - The initial height of the thumb (usually the maximum size)
	ApexThumbSlider::ApexThumbSlider(juce::Slider::SliderStyle style,
									 std::function<double(double)> proportionToValueFunc,
									 std::function<double(double)> valueToProportionFunc,
									 int initialThumbWidth,
									 int initialThumbHeight) noexcept
		: ApexSlider(style, std::move(proportionToValueFunc), std::move(valueToProportionFunc)),
		  mInitialThumbWidth(initialThumbWidth), mInitialThumbHeight(initialThumbHeight) {
	}

	/// @brief Draws this slider to the screen
	///
	/// @param g - The graphics context to use for drawing
	void ApexThumbSlider::paint(juce::Graphics& g) noexcept {
		double sliderPos = getProportionFromValue(getValue());
		jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

		if(mUsesThumbImage) {
			resizeThumb();
		}
		else if(mLookAndFeel != nullptr) {
			mLookAndFeel->drawLinearApexSliderThumb(g,
													getX(),
													getY(),
													getWidth(),
													getHeight(),
													static_cast<float>(sliderPos),
													*this);
		}
	}

	/// @brief Resizes the thumb to fit the new bounds and value of the slider
	void ApexThumbSlider::resizeThumb() noexcept {
		double sliderPos = getProportionFromValue(getValue());
		jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

		int thumbWidth = math::round(gsl::narrow_cast<float>(mInitialThumbWidth) * mXScaleFactor);
		int thumbHeight = math::round(gsl::narrow_cast<float>(mInitialThumbHeight) * mYScaleFactor);

		if(isHorizontal()) {
			int thumbX = static_cast<int>(sliderPos * getWidth() - (thumbWidth / 2.0));
			int thumbY = static_cast<int>(getHeight() * 0.5 - (thumbHeight / 2.0));
			juce::Rectangle<int> bounds = {thumbX, thumbY, thumbWidth, thumbHeight};
			mThumbComponent.setBounds(bounds);
		}
		else {
			int thumbX = static_cast<int>(getWidth() * 0.5 - (thumbWidth / 2.0));
			int thumbY = static_cast<int>(sliderPos * getHeight() - (thumbHeight / 2.0));
			juce::Rectangle<int> bounds = {thumbX, thumbY, thumbWidth, thumbHeight};
			mThumbComponent.setBounds(bounds);
		}
	}

	/// @brief Determines if the given point is within the control-responsive bounds of this slider
	///
	/// @param p - The point in question
	/// @return - Whether the point is within the controllable bounds
	auto ApexThumbSlider::isInBounds(juce::Point<int> p) const noexcept -> bool {
		int x = p.x;
		int y = p.y;

		x += getX();
		y += getY();

		double sliderPos = getProportionFromValue(getValue());
		jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

		int thumbWidth = math::round(gsl::narrow_cast<float>(mInitialThumbWidth) * mXScaleFactor);
		int thumbHeight = math::round(gsl::narrow_cast<float>(mInitialThumbHeight) * mYScaleFactor);
		int thumbX = 0;
		int thumbY = 0;
		if(isHorizontal()) {
			thumbX = static_cast<int>(sliderPos * getWidth() - (thumbWidth / 2.0));
			thumbY = static_cast<int>(getHeight() * 0.5 - (thumbHeight / 2.0));
		}
		else {
			thumbX = static_cast<int>(getWidth() * 0.5 - (thumbWidth / 2.0));
			thumbY = static_cast<int>(sliderPos * getHeight() - (thumbHeight / 2.0));
		}
		return (x >= thumbX && x <= thumbX + thumbWidth && y >= thumbY && y <= thumbY + thumbWidth);
	}
} // namespace apex::ui
