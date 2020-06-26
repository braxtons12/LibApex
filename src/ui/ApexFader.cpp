#include "ApexFader.h"

namespace apex {
	namespace ui {
		ApexFader::ApexFader(Slider::SliderStyle style,
				std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc,
				ApexFilmStrip filmStrip)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc, filmStrip),
			mUsesThumbImage(false), mIsThumbOnly(false)
		{

		}

		ApexFader::ApexFader(Slider::SliderStyle style,
				std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc,
				Image thumbImage)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mThumbImage(thumbImage), mUsesThumbImage(true), mIsThumbOnly(true)
		{
			mThumbComponent.setImage(mThumbImage);
			mThumbComponent.setImagePlacement(RectanglePlacement(
						RectanglePlacement::xMid | RectanglePlacement::yTop));
			addAndMakeVisible(mThumbComponent);
			mInitialThumbWidth = mThumbImage.getWidth();
			mInitialThumbHeight = mThumbImage.getHeight();
		}

		ApexFader::ApexFader(Slider::SliderStyle style,
				std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc,
				bool isThumbOnlySlider,
				int initialThumbWidth, int initialThumbHeight)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mUsesThumbImage(false), mIsThumbOnly(isThumbOnlySlider),
			mInitialThumbWidth(initialThumbWidth), mInitialThumbHeight(initialThumbHeight)
		{

		}

		ApexFader::~ApexFader() {

		}

		String ApexFader::getTextFromValue(double value) {
			double val = getValueFromProportion(value);
			String temp = String(val);
			if (math::fabs(val) <= 0.010) return String("0.0");
			else return temp.dropLastCharacters(temp.length() -
					(temp.upToFirstOccurrenceOf(".", true, true).length() + 2));
		}

		double ApexFader::getValueFromText(const String& text) {
			return getProportionFromValue(text.getDoubleValue());
		}

		void ApexFader::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) {
			double reversed = wheel.isReversed ? -1.0 : 1.0;
			double val = getValueFromProportion(getValue());
			if (e.mods.isShiftDown()) {
				setValue(getProportionFromValue(val + 1.0 * reversed), sendNotificationAsync);
			}
			else {
				setValue(getProportionFromValue(val + 3.0 * reversed), sendNotificationAsync);
			}
		}

		void ApexFader::paint(Graphics& g) {
			float sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

			auto style = getSliderStyle();

			if(mUsesThumbImage) {
				resizeThumb();
			}
			else if(mIsThumbOnly && mLookAndFeel != nullptr) {
				mLookAndFeel->drawLinearApexSliderThumb(g, getX(), getY(),
						getWidth(), getHeight(), sliderPos, *this);
			}
			else if(mLookAndFeel != nullptr) {
				mLookAndFeel->drawLinearApexSlider(g, getX(), getY(), getWidth(), getHeight(),
						sliderPos, style, *this);
			}
		}

		void ApexFader::resized() {
			if(mUsesThumbImage) {
				resizeThumb();
			}
		}

		void ApexFader::resizeThumb() {
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

		bool ApexFader::isInBounds(Point<int> p) {
			int x = p.x;
			int y = p.y;

			x += getX();
			y += getY();

			if(mIsThumbOnly) {
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
			else {
				return (x >= getX() && x <= getX() + getWidth()
						&& y >= getY() && y <= getY() + getHeight());
			}
		}
	}
}
