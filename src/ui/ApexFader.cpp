#include "ApexFader.h"

namespace apex {
	namespace ui {
		ApexFader::ApexFader(juce::Slider::SliderStyle style,
				std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc,
				ApexFilmStrip filmStrip)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc, filmStrip),
			mUsesThumbImage(false), mIsThumbOnly(false)
		{

		}

		ApexFader::ApexFader(juce::Slider::SliderStyle style,
				std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc,
				juce::Image thumbImage)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mThumbImage(thumbImage), mUsesThumbImage(true), mIsThumbOnly(true)
		{
			mThumbComponent.setImage(mThumbImage);
			mThumbComponent.setImagePlacement(juce::RectanglePlacement(
						juce::RectanglePlacement::xMid | juce::RectanglePlacement::yTop));
			addAndMakeVisible(mThumbComponent);
			mInitialThumbWidth = mThumbImage.getWidth();
			mInitialThumbHeight = mThumbImage.getHeight();
		}

		ApexFader::ApexFader(juce::Slider::SliderStyle style,
				std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc,
				bool isThumbOnlySlider,
				int initialThumbWidth, int initialThumbHeight)
			: ApexSlider(style, proportionToValueFunc, valueToProportionFunc),
			mUsesThumbImage(false), mIsThumbOnly(isThumbOnlySlider),
			mInitialThumbWidth(initialThumbWidth), mInitialThumbHeight(initialThumbHeight)
		{

		}

		ApexFader::~ApexFader() {

		}

		juce::String ApexFader::getTextFromValue(double value) {
			double val = getValueFromProportion(value);
			juce::String temp = juce::String(val);
			if (math::fabs(val) <= 0.010) return juce::String("0.0");
			else return temp.dropLastCharacters(temp.length() -
					(temp.upToFirstOccurrenceOf(".", true, true).length() + 2));
		}

		double ApexFader::getValueFromText(const juce::String& text) {
			return getProportionFromValue(text.getDoubleValue());
		}

		void ApexFader::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) {
			double reversed = wheel.isReversed ? -1.0 : 1.0;
			double val = getValueFromProportion(getValue());
			if (e.mods.isShiftDown()) {
				setValue(getProportionFromValue(val + 1.0 * reversed), juce::sendNotificationAsync);
			}
			else {
				setValue(getProportionFromValue(val + 3.0 * reversed), juce::sendNotificationAsync);
			}
		}

		void ApexFader::paint(juce::Graphics& g) {
			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

			auto style = getSliderStyle();

			if(mUsesThumbImage) {
				resizeThumb();
			}
			else if(mIsThumbOnly && mLookAndFeel != nullptr) {
				mLookAndFeel->drawLinearApexSliderThumb(g, getX(), getY(),
						getWidth(), getHeight(), static_cast<float>(sliderPos), *this);
			}
			else if(mLookAndFeel != nullptr) {
				mLookAndFeel->drawLinearApexSlider(g, getX(), getY(), getWidth(), getHeight(),
						static_cast<float>(sliderPos), style, *this);
			}
		}

		void ApexFader::resized() {
			if(mUsesThumbImage) {
				resizeThumb();
			}
		}

		void ApexFader::resizeThumb() {
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

		bool ApexFader::isInBounds(juce::Point<int> p) {
			int x = p.x;
			int y = p.y;

			x += getX();
			y += getY();

			if(mIsThumbOnly) {
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
			else {
				return (x >= getX() && x <= getX() + getWidth()
						&& y >= getY() && y <= getY() + getHeight());
			}
		}
	}
}
