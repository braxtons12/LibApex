#include "ApexSlider.h"

namespace apex {
	namespace ui {

		ApexSlider::ApexSlider(juce::Slider::SliderStyle style,
							   std::function<double(double)> proportionToValueFunc,
							   std::function<double(double)> valueToProportionFunc,
							   ApexFilmStrip filmStrip)
			: juce::Slider(style, juce::Slider::TextEntryBoxPosition::NoTextBox),
			  mFilmStrip(filmStrip) {
			mPopupTextBox = std::make_unique<juce::TextEditor>();
			addChildComponent(mPopupTextBox.get());
			mPopupTextBox->addListener(this);
			mPopupTextBox->setSelectAllWhenFocused(true);
			mPopupTextBox->setInputRestrictions(mNumTextBoxCharacters, "+-.0123456789");
			setPaintingIsUnclipped(true);
			setSliderSnapsToMousePosition(false);
			setVelocityBasedMode(true);
			setVelocityModeParameters(0.4, 1, 0.02, false);

			mTextBox.setEditable(true, true, false);
			mTextBox.setText("0", juce::dontSendNotification);
			mTextBox.addListener(this);
			mTextBox.setJustificationType(juce::Justification::centred);
			mTextBox.setColour(juce::Label::textColourId, mTextBoxFontColour);

			onValueChange = [&] {
				mTextBox.setText(getTextFromValue(getValue()), juce::dontSendNotification);
			};

			mUsesFilmStrip = true;
			mProportionToValueFunc = proportionToValueFunc;
			mValueToProportionFunc = valueToProportionFunc;
		}

		ApexSlider::ApexSlider(juce::Slider::SliderStyle style,
							   std::function<double(double)> proportionToValueFunc,
							   std::function<double(double)> valueToProportionFunc)
			: juce::Slider(style, juce::Slider::TextEntryBoxPosition::NoTextBox) {
			mPopupTextBox = std::make_unique<juce::TextEditor>();
			addChildComponent(mPopupTextBox.get());
			mPopupTextBox->addListener(this);
			mPopupTextBox->setSelectAllWhenFocused(true);
			mPopupTextBox->setInputRestrictions(mNumTextBoxCharacters, "+-.0123456789");
			// setPaintingIsUnclipped(true);
			setSliderSnapsToMousePosition(false);
			setVelocityBasedMode(true);
			setVelocityModeParameters(0.4, 1, 0.02, false);

			mTextBox.setEditable(true, true, false);
			mTextBox.setText("0", juce::dontSendNotification);
			mTextBox.addListener(this);
			mTextBox.setJustificationType(juce::Justification::centred);
			mTextBox.setColour(juce::Label::textColourId, mTextBoxFontColour);

			onValueChange = [&] {
				mTextBox.setText(getTextFromValue(getValue()), juce::dontSendNotification);
			};

			mUsesFilmStrip = false;
			mProportionToValueFunc = proportionToValueFunc;
			mValueToProportionFunc = valueToProportionFunc;
		}

		ApexSlider::~ApexSlider() {
		}

		juce::String ApexSlider::getTextFromValue(double value) {
			double val = getValueFromProportion(value);
			juce::String temp = juce::String(val);
			return temp.dropLastCharacters(
				temp.length() - (temp.upToFirstOccurrenceOf(".", true, true).length() + 2));
		}

		double ApexSlider::getValueFromText(const juce::String& text) {
			return getProportionFromValue(text.getDoubleValue());
		}

		void ApexSlider::mouseDown(const juce::MouseEvent& e) {
			mPopupTextBox->setVisible(false);
			if(e.mods.isLeftButtonDown() && e.mods.isCtrlDown())
				setValue(getDoubleClickReturnValue(), juce::sendNotificationAsync);
			else if(isInBounds(e.getMouseDownPosition()))
				juce::Slider::mouseDown(e);
		}

		void ApexSlider::mouseUp(const juce::MouseEvent& e) {
			if(isInBounds(e.getMouseDownPosition()))
				juce::Slider::mouseUp(e);
		}

		void ApexSlider::mouseDoubleClick(const juce::MouseEvent& e) {
			juce::String text = getTextFromValue(getValue());
			juce::String textUntilDecimal = text.upToFirstOccurrenceOf(".", true, true);
			juce::String newText
				= text.dropLastCharacters(text.length() - (textUntilDecimal.length() + 2));
			mPopupTextBox->setSize(
				static_cast<int>(juce::jmax(
					SliderFloatingTextBoxStartWidth * mXScaleFactor * newText.length() + 0.5f,
					SliderFloatingTextBoxStartWidth * mXScaleFactor * 3)),
				static_cast<int>(SliderFloatingTextBoxStartHeight * mYScaleFactor + 0.5f));
			size_t left = size_t(e.getPosition().x - mPopupTextBox->getWidth() / 2);
			size_t top = size_t(e.getPosition().y - mPopupTextBox->getHeight() / 2);
			mPopupTextBox->setTopLeftPosition(static_cast<int>(left), static_cast<int>(top));
			mPopupTextBox->setText(newText);
			mPopupTextBox->setVisible(true);
			mPopupTextBox->toFront(true);
			mPopupTextBox->grabKeyboardFocus();
			mPopupTextBox->setJustification(juce::Justification::centred);
		}

		void ApexSlider::mouseDrag(const juce::MouseEvent& e) {
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
					juce::Slider::mouseDrag(e);
				}
			}
			if(e.mods.isShiftDown() || mNormalModeIsVelocityMode)
				e.source.enableUnboundedMouseMovement(false, true);
		}

		void ApexSlider::mouseWheelMove(const juce::MouseEvent& e,
										const juce::MouseWheelDetails& wheel) {
			double reversed = wheel.isReversed ? -1.0 : 1.0;
			double val = getValue();
			if(e.mods.isShiftDown())
				setValue(val + wheel.deltaY * (reversed / 100.0f), juce::sendNotificationAsync);
			else
				setValue(val + wheel.deltaY * (reversed / 25.0f), juce::sendNotificationAsync);
		}

		void ApexSlider::globalFocusChanged(juce::Component* focusedComponent) {
			ignoreUnused(focusedComponent);
			mPopupTextBox->setVisible(false);
		}

		void ApexSlider::textEditorReturnKeyPressed(juce::TextEditor& editor) {
			double value = getValueFromText(editor.getText());
			isValueValid(value) ? setValue(value) : setValue(getValue());
			mPopupTextBox->setVisible(false);
		}

		void ApexSlider::textEditorEscapeKeyPressed(juce::TextEditor& editor) {
			juce::ignoreUnused(editor);
			mPopupTextBox->setVisible(false);
		}

		void ApexSlider::textEditorFocusLost(juce::TextEditor& editor) {
			juce::ignoreUnused(editor);
			if(mPopupTextBox->isVisible() && !mPopupTextBox->hasKeyboardFocus(true))
				mPopupTextBox->setVisible(false);
		}

		void ApexSlider::labelTextChanged(juce::Label* label) {
			if(label) {
				double val = getValueFromText(label->getText());
				isValueValid(val) ?
					  setValue(val, juce::sendNotificationAsync) :
					  label->setText(getTextFromValue(getValue()), juce::dontSendNotification);
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

		void ApexSlider::setPopupTextBoxFont(juce::Font font) {
			mPopupTextBoxFont = font;
			mPopupTextBox->applyFontToAllText(mPopupTextBoxFont, true);
		}

		void ApexSlider::setTextBoxFont(juce::Font font) {
			mTextBoxFont = font;
			mTextBox.setFont(mTextBoxFont);
		}

		void ApexSlider::setTextBoxBounds(juce::Rectangle<int> bounds) {
			mTextBox.setBounds(bounds);
		}

		juce::Label* ApexSlider::getTextBox() {
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

		void ApexSlider::paint(juce::Graphics& g) {
			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

			auto style = getSliderStyle();
			if(style != IncDecButtons && mLookAndFeel != nullptr) {
				if(isRotary()) {
					auto rotaryParams = getRotaryParameters();
					mLookAndFeel->drawRotaryApexSlider(g,
													   0,
													   0,
													   getWidth(),
													   getHeight(),
													   static_cast<float>(sliderPos),
													   rotaryParams.startAngleRadians,
													   rotaryParams.endAngleRadians,
													   *this);
				}
				else {
					mLookAndFeel->drawLinearApexSlider(g,
													   0,
													   0,
													   getWidth(),
													   getHeight(),
													   static_cast<float>(sliderPos),
													   style,
													   *this);
				}
			}
		}

		bool ApexSlider::isInBounds(juce::Point<int> p) {
			if(isRotary() && mLookAndFeel != nullptr) {
				juce::Rectangle<int> bounds
					= mLookAndFeel->getActualRotaryBounds(getX(), getY(), getWidth(), getHeight());

				int x = p.x;
				int y = p.y;

				x += getX();
				y += getY();
				juce::Point<int> actual(x, y);

				return bounds.contains(actual);
			}
			else {
				int x = p.x;
				int y = p.y;

				x += getX();
				y += getY();

				return (x >= getX() && x <= getX() + getWidth() && y >= getY()
						&& y <= getY() + getHeight());
			}
		}
	} // namespace ui
} // namespace apex
