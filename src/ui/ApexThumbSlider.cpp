#include "ApexThumbSlider.h"

namespace apex {
	namespace ui {
		ApexThumbSlider::ApexThumbSlider(juce::Slider::SliderStyle style,
				std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc, juce::Image thumbImage)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mThumbImage(thumbImage),
			mUsesThumbImage(true)
		{
			mThumbComponent.setImage(mThumbImage);
			mThumbComponent.setImagePlacement(juce::RectanglePlacement(
						juce::RectanglePlacement::xMid | juce::RectanglePlacement::yTop));
			addAndMakeVisible(mThumbComponent);
			mInitialThumbWidth = mThumbImage.getWidth();
			mInitialThumbHeight = mThumbImage.getHeight();
		}

		ApexThumbSlider::ApexThumbSlider(juce::Slider::SliderStyle style,
				std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc,
				int initialThumbWidth, int initialThumbHeight)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mUsesThumbImage(false),
			mInitialThumbWidth(initialThumbWidth),
			mInitialThumbHeight(initialThumbHeight)
		{

		}

		ApexThumbSlider::~ApexThumbSlider() {

		}

		void ApexThumbSlider::paint(juce::Graphics& g) {
			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

			if(mUsesThumbImage) {
				resizeThumb();
			}
			else if(mLookAndFeel != nullptr) {
				mLookAndFeel->drawLinearApexSliderThumb(g, getX(), getY(),
						getWidth(), getHeight(), static_cast<float>(sliderPos), *this);
			}

		}

		void ApexThumbSlider::resized() {
			if(mUsesThumbImage) {
				resizeThumb();
			}
		}

		void ApexThumbSlider::resizeThumb() {
			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

			int thumbWidth = static_cast<int>(mInitialThumbWidth * mXScaleFactor + 0.5f);
			int thumbHeight = static_cast<int>(mInitialThumbHeight * mYScaleFactor + 0.5f);

			if(isHorizontal()) {
				int thumbX = static_cast<int>(sliderPos * getWidth() - (thumbWidth / 2.0f));
				int thumbY = static_cast<int>(getHeight() * 0.5f - (thumbHeight / 2.0f));
				juce::Rectangle<int> bounds = { thumbX, thumbY, thumbWidth, thumbHeight };
				mThumbComponent.setBounds(bounds);
			}
			else {
				int thumbX = static_cast<int>(getWidth() * 0.5f - (thumbWidth / 2.0f));
				int thumbY = static_cast<int>(sliderPos * getHeight() - (thumbHeight / 2.0f));
				juce::Rectangle<int> bounds = { thumbX, thumbY, thumbWidth, thumbHeight };
				mThumbComponent.setBounds(bounds);
			}
		}

		bool ApexThumbSlider::isInBounds(juce::Point<int> p) {
			int x = p.x;
			int y = p.y;

			x += getX();
			y += getY();

			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

			int thumbWidth = static_cast<int>(mInitialThumbWidth * mXScaleFactor + 0.5f);
			int thumbHeight = static_cast<int>(mInitialThumbHeight * mYScaleFactor + 0.5f);
			int thumbX;
			int thumbY;
			if(isHorizontal()) {
				thumbX = static_cast<int>(sliderPos * getWidth() - (thumbWidth / 2.0f));
				thumbY = static_cast<int>(getHeight() * 0.5f - (thumbHeight / 2.0f));
			}
			else {
				thumbX = static_cast<int>(getWidth() * 0.5f - (thumbWidth / 2.0f));
				thumbY = static_cast<int>(sliderPos * getHeight() - (thumbHeight / 2.0f));
			}
			return (x >= thumbX && x <= thumbX + thumbWidth
					&& y >= thumbY && y <= thumbY + thumbWidth);
		}
	}
}
