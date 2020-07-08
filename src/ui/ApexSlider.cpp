#include "ApexSlider.h"

namespace apex {
	namespace ui {

		ApexSlider::ApexSlider(Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc, ApexFilmStrip filmStrip)
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

		ApexSlider::ApexSlider(Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
				std::function<double(double)> valueToProportionFunc)
			: Slider(style, Slider::TextEntryBoxPosition::NoTextBox)
		{
			mPopupTextBox = std::make_unique<TextEditor>();
			addChildComponent(mPopupTextBox.get());
			mPopupTextBox->addListener(this);
			mPopupTextBox->setSelectAllWhenFocused(true);
			mPopupTextBox->setInputRestrictions(mNumTextBoxCharacters, "+-.0123456789");
			//setPaintingIsUnclipped(true);
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
			mPopupTextBox->setSize(
				static_cast<int>(
					jmax(SliderFloatingTextBoxStartWidth * mXScaleFactor * newText.length() + 0.5f,
						SliderFloatingTextBoxStartWidth * mXScaleFactor * 3)
						), 
				static_cast<int>(SliderFloatingTextBoxStartHeight * mYScaleFactor + 0.5f)
						);
			size_t left = size_t(e.getPosition().x - mPopupTextBox->getWidth() / 2);
			size_t top = size_t(e.getPosition().y - mPopupTextBox->getHeight() / 2);
			mPopupTextBox->setTopLeftPosition(static_cast<int>(left), static_cast<int>(top));
			mPopupTextBox->setText(newText);
			mPopupTextBox->setVisible(true);
			mPopupTextBox->toFront(true);
			mPopupTextBox->grabKeyboardFocus();
			mPopupTextBox->setJustification(Justification::centred);
		}

		void ApexSlider::mouseDrag(const MouseEvent& e) {
			if(e.mods.isShiftDown()) {
				setVelocityBasedMode(true);
				setVelocityModeParameters(mFineSensitivity, 1, mFineInitialVelocity, false);
			}
			else {
				setVelocityBasedMode(mNormalModeIsVelocityMode);
				setVelocityModeParameters(mNormalSensitivity, 1, mNormalInitialVelocity, false);
			}

			if(e.mods.isLeftButtonDown()) {
				if(isInBounds(e.getMouseDownPosition())) {
					Slider::mouseDrag(e);
				}
			}
			if(e.mods.isShiftDown() || mNormalModeIsVelocityMode)
				e.source.enableUnboundedMouseMovement(false, true);
		}

		void ApexSlider::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) {
			double reversed = wheel.isReversed ? -1.0 : 1.0;
			double val = getValue();
			if(e.mods.isShiftDown())
				setValue(val + wheel.deltaY * (reversed / 100.0f), sendNotificationAsync);
			else
				setValue(val + wheel.deltaY * (reversed / 25.0f), sendNotificationAsync);
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

		float ApexSlider::getXScaleFactor() const {
			return mXScaleFactor;
		}

		void ApexSlider::setYScaleFactor(float scaleFactor) {
			mYScaleFactor = scaleFactor;
		}

		float ApexSlider::getYScaleFactor() const {
			return mYScaleFactor;
		}

		bool ApexSlider::isValueValid(double value) const {
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

		void ApexSlider::setNormalSensitivity(double sensitivity, double initialVelocity) {
			mNormalSensitivity = sensitivity;
			mNormalInitialVelocity = initialVelocity;
		}

		void ApexSlider::setFineSensitivity(double sensitivity, double initialVelocity) {
			mFineSensitivity = sensitivity;
			mFineInitialVelocity = initialVelocity;
		}

		void ApexSlider::setNormalModeIsVelocityBased(bool set) {
			mNormalModeIsVelocityMode = set;
		}

		void ApexSlider::hideEditor() {
			mPopupTextBox->setVisible(false);
		}

		Option<ApexFilmStrip> ApexSlider::getFilmStrip() {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip) :
				Option<ApexFilmStrip>::None();
		}

		double ApexSlider::getValueFromProportion(double prop) const {
			return mProportionToValueFunc(prop);
		}

		double ApexSlider::getProportionFromValue(double value) const {
			return mValueToProportionFunc(value);
		}

		void ApexSlider::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) {
			mLookAndFeel = lookNFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

		void ApexSlider::paint(Graphics& g) {
			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

			auto style = getSliderStyle();
			if (style != IncDecButtons && mLookAndFeel != nullptr) {
				if(isRotary()) {
					auto rotaryParams = getRotaryParameters();
					mLookAndFeel->drawRotaryApexSlider(g, 0, 0, getWidth(), getHeight(),
							static_cast<float>(sliderPos), rotaryParams.startAngleRadians,
							rotaryParams.endAngleRadians, *this);
				}
				else {
					mLookAndFeel->drawLinearApexSlider(g, 0, 0, getWidth(), getHeight(),
							static_cast<float>(sliderPos), style, *this);
				}
			}
		}

		bool ApexSlider::isInBounds(Point<int> p) {
			if(isRotary() && mLookAndFeel != nullptr) {
				Rectangle<int> bounds = mLookAndFeel->getActualRotaryBounds(getX(), getY(),
						getWidth(), getHeight());

				int x = p.x;
				int y = p.y;

				x += getX();
				y += getY();
				Point<int> actual(x, y);

				return bounds.contains(actual);
			}
			else {
				int x = p.x;
				int y = p.y;

				x += getX();
				y += getY();

				return (x >= getX() && x <= getX() + getWidth()
						&& y >= getY() && y <= getY() + getHeight());
			}
		}
	}
}
