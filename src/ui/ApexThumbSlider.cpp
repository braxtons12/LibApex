#include "ApexThumbSlider.h"

namespace apex {
	namespace ui {
		ApexThumbSlider::ApexThumbSlider(Slider::SliderStyle style,
				std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc, Image thumbImage)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mThumbImage(thumbImage),
			mUsesThumbImage(true)
		{
			mThumbComponent.setImage(mThumbImage);
			mThumbComponent.setImagePlacement(RectanglePlacement(
						RectanglePlacement::xMid | RectanglePlacement::yTop));
			addAndMakeVisible(mThumbComponent);
			mInitialThumbWidth = mThumbImage.getWidth();
			mInitialThumbHeight = mThumbImage.getHeight();
		}

		ApexThumbSlider::ApexThumbSlider(Slider::SliderStyle style,
				std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc,
				int initialThumbWidth, int initialThumbHeight)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mUsesThumbImage(false),
			mInitialThumbWidth(initialThumbWidth),
			mInitialThumbHeight(initialThumbHeight)
		{

		}

		ApexThumbSlider::~ApexThumbSlider() {

		}

		void ApexThumbSlider::paint(Graphics& g) {
			float sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

			if(mUsesThumbImage) {
				resizeThumb();
			}
			else if(mLookAndFeel != nullptr) {
				mLookAndFeel->drawLinearApexSliderThumb(g, getX(), getY(),
						getWidth(), getHeight(), sliderPos, *this);
			}

		}

		void ApexThumbSlider::resized() {
			if(mUsesThumbImage) {
				resizeThumb();
			}
		}

		void ApexThumbSlider::resizeThumb() {
			float sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

			int thumbWidth = mInitialThumbWidth * mXScaleFactor + 0.5f;
			int thumbHeight = mInitialThumbHeight * mYScaleFactor + 0.5f;

			if(isHorizontal()) {
				int thumbX = sliderPos * getWidth() - (thumbWidth / 2.0f);
				int thumbY = getHeight() * 0.5f - (thumbHeight / 2.0f);
				Rectangle<int> bounds = { thumbX, thumbY, thumbWidth, thumbHeight };
				mThumbComponent.setBounds(bounds);
			}
			else {
				int thumbX = getWidth() * 0.5f - (thumbWidth / 2.0f);
				int thumbY = sliderPos * getHeight() - (thumbHeight / 2.0f);
				Rectangle<int> bounds = { thumbX, thumbY, thumbWidth, thumbHeight };
				mThumbComponent.setBounds(bounds);
			}
		}

		bool ApexThumbSlider::isInBounds(Point<int> p) {
			int x = p.x;
			int y = p.y;

			x += getX();
			y += getY();

			float sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

			int thumbWidth = mInitialThumbWidth * mXScaleFactor + 0.5f;
			int thumbHeight = mInitialThumbHeight * mYScaleFactor + 0.5f;
			int thumbX;
			int thumbY;
			if(isHorizontal()) {
				thumbX = sliderPos * getWidth() - (thumbWidth / 2.0f);
				thumbY = getHeight() * 0.5f - (thumbHeight / 2.0f);
			}
			else {
				thumbX = getWidth() * 0.5f - (thumbWidth / 2.0f);
				thumbY = sliderPos * getHeight() - (thumbHeight / 2.0f);
			}
			return (x >= thumbX && x <= thumbX + thumbWidth
					&& y >= thumbY && y <= thumbY + thumbWidth);
		}
	}
}
