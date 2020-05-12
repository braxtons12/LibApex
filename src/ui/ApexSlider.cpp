#include "ApexSlider.h"

namespace apex {
	namespace ui {

		ApexSlider::ApexSlider(Slider::SliderStyle style, std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc, ApexFilmStrip filmStrip)
			: Slider(style, Slider::TextEntryBoxPosition::NoTextBox), mFilmStrip(filmStrip)
		{
			mPopupTextBox = std::make_unique<TextEditor>();
			addChildComponent(mPopupTextBox.get());
			mPopupTextBox->addListener(this);
			mPopupTextBox->setSelectAllWhenFocused(true);
			mPopupTextBox->setInputRestrictions(mNumTextBoxCharacters, "+-.0123456789");
			setPaintingIsUnclipped(true);
			setSliderSnapsToMousePosition(false);
			setVelocityBasedMode(true);
			setVelocityModeParameters(0.4, 1, 0.02, false);

			mTextBox.setEditable(true, true, false);
			mTextBox.setText("0", dontSendNotification);
			mTextBox.addListener(this);
			mTextBox.setJustificationType(Justification::centred);
			mTextBox.setColour(Label::textColourId, mTextBoxFontColour);

			onValueChange = [&] {
				mTextBox.setText(getTextFromValue(getValue()), dontSendNotification);
			};

			mUsesFilmStrip = true;
			mProportionToValueFunc = proportionToValueFunc;
			mValueToProportionFunc = valueToProportionFunc;
		}

		ApexSlider::ApexSlider(Slider::SliderStyle style, std::function<float(float)> proportionToValueFunc,
				std::function<float(float)> valueToProportionFunc)
			: Slider(style, Slider::TextEntryBoxPosition::NoTextBox)
		{
			mPopupTextBox = std::make_unique<TextEditor>();
			addChildComponent(mPopupTextBox.get());
			mPopupTextBox->addListener(this);
			mPopupTextBox->setSelectAllWhenFocused(true);
			mPopupTextBox->setInputRestrictions(mNumTextBoxCharacters, "+-.0123456789");
			setPaintingIsUnclipped(true);
			setSliderSnapsToMousePosition(false);
			setVelocityBasedMode(true);
			setVelocityModeParameters(0.4, 1, 0.02, false);

			mTextBox.setEditable(true, true, false);
			mTextBox.setText("0", dontSendNotification);
			mTextBox.addListener(this);
			mTextBox.setJustificationType(Justification::centred);
			mTextBox.setColour(Label::textColourId, mTextBoxFontColour);

			onValueChange = [&] {
				mTextBox.setText(getTextFromValue(getValue()), dontSendNotification);
			};

			mUsesFilmStrip = false;
			mProportionToValueFunc = proportionToValueFunc;
			mValueToProportionFunc = valueToProportionFunc;
		}

		ApexSlider::~ApexSlider() {

		}

		String ApexSlider::getTextFromValue(double value) {
			double val = getValueFromProportion(value);
			String temp = String(val);
			return temp.dropLastCharacters(temp.length() -
					(temp.upToFirstOccurrenceOf(".", true, true).length() + 2));
		}

		double ApexSlider::getValueFromText(const String& text) {
			return getProportionFromValue(text.getDoubleValue());
		}

		void ApexSlider::mouseDown(const MouseEvent& e) {
			mPopupTextBox->setVisible(false);
			if(e.mods.isLeftButtonDown() && e.mods.isCtrlDown())
				setValue(getDoubleClickReturnValue(), sendNotificationAsync);
			else if (isInBounds(e.getMouseDownPosition()))
				Slider::mouseDown(e);
		}

		void ApexSlider::mouseUp(const MouseEvent& e) {
			if(isInBounds(e.getMouseDownPosition()))
				Slider::mouseUp(e);
		}

		void ApexSlider::mouseDoubleClick(const MouseEvent& e) {
			String text = getTextFromValue(getValue());
			String textUntilDecimal = text.upToFirstOccurrenceOf(".", true, true);
			String newText = text.dropLastCharacters(text.length() - (textUntilDecimal.length() + 2));
			mPopupTextBox->setSize(jmax(SliderFloatingTextBoxStartWidth * mXScaleFactor * newText.length() + 0.5f,
						SliderFloatingTextBoxStartWidth * mXScaleFactor * 3), SliderFloatingTextBoxStartHeight * mYScaleFactor + 0.5f);
			size_t left = size_t(e.getPosition().x - mPopupTextBox->getWidth() / 2);
			size_t top = size_t(e.getPosition().y - mPopupTextBox->getHeight() / 2);
			mPopupTextBox->setTopLeftPosition(left, top);
			mPopupTextBox->setText(newText);
			mPopupTextBox->setVisible(true);
			mPopupTextBox->toFront(true);
			mPopupTextBox->grabKeyboardFocus();
			mPopupTextBox->setJustification(Justification::centred);
		}

		void ApexSlider::mouseDrag(const MouseEvent& e) {
			if(e.mods.isShiftDown())
				setVelocityModeParameters(mFineSensitivity, 1, 0.02, false);
			else
				setVelocityModeParameters(mNormalSensitivity, 1, 0.05, false);

			if(e.mods.isLeftButtonDown()) {
				if(isInBounds(e.getMouseDownPosition())) {
					Slider::mouseDrag(e);
				}
			}
			e.source.enableUnboundedMouseMovement(false, true);
		}

		void ApexSlider::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) {
			double reversed = wheel.deltaY / math::fabs(wheel.deltaY);
			double val = getValue();
			if(e.mods.isShiftDown())
				setValue(val + wheel.deltaY * (reversed / 25.0f), sendNotificationAsync);
			else
				setValue(val + wheel.deltaY * (reversed / 6.25f), sendNotificationAsync);
		}

		void ApexSlider::globalFocusChanged(Component* focusedComponent) {
			ignoreUnused(focusedComponent);
			mPopupTextBox->setVisible(false);
		}

		void ApexSlider::textEditorReturnKeyPressed(TextEditor& editor) {
			double value = getValueFromText(editor.getText());
			isValueValid(value) ? setValue(value) : setValue(getValue());
			mPopupTextBox->setVisible(false);
		}

		void ApexSlider::textEditorEscapeKeyPressed(TextEditor& editor) {
			ignoreUnused(editor);
			mPopupTextBox->setVisible(false);
		}

		void ApexSlider::textEditorFocusLost(TextEditor& editor) {
			ignoreUnused(editor);
			if(mPopupTextBox->isVisible() && ! mPopupTextBox->hasKeyboardFocus(true))
				mPopupTextBox->setVisible(false);
		}

		void ApexSlider::labelTextChanged(Label* label) {
			if(label) {
				double val = getValueFromText(label->getText());
				isValueValid(val) ? setValue(val, sendNotificationAsync) : label->setText(getTextFromValue(getValue()), dontSendNotification);
			}
		}

		void ApexSlider::setXScaleFactor(float scaleFactor) {
			mXScaleFactor = scaleFactor;
		}

		float ApexSlider::getXScaleFactor() {
			return mXScaleFactor;
		}

		void ApexSlider::setYScaleFactor(float scaleFactor) {
			mYScaleFactor = scaleFactor;
		}

		float ApexSlider::getYScaleFactor() {
			return mYScaleFactor;
		}

		bool ApexSlider::isValueValid(double value) {
			return value <= 1.00 && value >= 0.00;
		}

		void ApexSlider::setPopupTextBoxFont(Font font) {
			mPopupTextBoxFont = font;
			mPopupTextBox->applyFontToAllText(mPopupTextBoxFont, true);
		}

		void ApexSlider::setTextBoxFont(Font font) {
			mTextBoxFont = font;
			mTextBox.setFont(mTextBoxFont);
		}

		void ApexSlider::setTextBoxBounds(Rectangle<int> bounds) {
			mTextBox.setBounds(bounds);
		}

		Label* ApexSlider::getTextBox() {
			return &mTextBox;
		}

		void ApexSlider::setNormalSensitivity(double set) {
			mNormalSensitivity = set;
		}

		void ApexSlider::setFineSensitivity(double set) {
			mFineSensitivity = set;
		}

		void ApexSlider::hideEditor() {
			mPopupTextBox->setVisible(false);
		}

		Option<ApexFilmStrip> ApexSlider::getFilmStrip() {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip) :
				Option<ApexFilmStrip>::None();
		}

		float ApexSlider::getValueFromProportion(float prop) {
			return mProportionToValueFunc(prop);
		}

		float ApexSlider::getProportionFromValue(float value) {
			return mValueToProportionFunc(value);
		}

		void ApexSlider::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel) {
			mLookAndFeel = lookAndFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

		void ApexSlider::paint(Graphics& g) {
			float sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00f && sliderPos <= 1.00f);

			auto style = getSliderStyle();
			if (style != IncDecButtons) {
				if(isRotary()) {
					auto rotaryParams = getRotaryParameters();
					mLookAndFeel->drawRotaryApexSlider(g, getX(), getY(), getWidth(), getHeight(),
							sliderPos, rotaryParams.startAngleRadians,
							rotaryParams.endAngleRadians, *this);
				}
				else {
					mLookAndFeel->drawLinearApexSlider(g, getX(), getY(), getWidth(), getHeight(),
							sliderPos, style, *this);
				}
			}
		}

		bool ApexSlider::isInBounds(Point<int> p) {
			int x = p.x;
			int y = p.y;

			x += getX();
			y += getY();

			return (x >= getX() && x <= getX() + getWidth()
					&& y >= getY() && y <= getY() + getHeight());
		}
	}
}
