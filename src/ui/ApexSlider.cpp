#include "ApexSlider.h"

namespace apex::ui {
	/// @brief Creates an `ApexSlider` of the given style
	///
	/// @param style - The style of the slider
	/// @param proportionToValueFunc - The function to convert the proportion of the slider to
	/// the corresponding user/dsp facing value
	/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
	/// corresponding proportion of the slider
	ApexSlider::ApexSlider(juce::Slider::SliderStyle style,
						   std::function<double(double)> proportionToValueFunc,
						   std::function<double(double)> valueToProportionFunc) noexcept
		: juce::Slider(style, juce::Slider::TextEntryBoxPosition::NoTextBox),
		  mProportionToValueFunc(std::move(proportionToValueFunc)),
		  mValueToProportionFunc(std::move(valueToProportionFunc)) {

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
	}

	/// @brief Creates an `ApexSlider` of the given style, that uses the given film strip image
	/// asset to draw itself
	///
	/// @param style - The style of the slider
	/// @param proportionToValueFunc - The function to convert the proportion of the slider to
	/// the corresponding user/dsp facing value
	/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
	/// corresponding proportion of the slider
	/// @param strip - The film strip to use to visually represent this
	ApexSlider::ApexSlider(juce::Slider::SliderStyle style,
						   std::function<double(double)> proportionToValueFunc,
						   std::function<double(double)> valueToProportionFunc,
						   ApexFilmStrip filmStrip) noexcept
		: juce::Slider(style, juce::Slider::TextEntryBoxPosition::NoTextBox),
		  mFilmStrip(std::move(filmStrip)), mUsesFilmStrip(true),
		  mProportionToValueFunc(std::move(proportionToValueFunc)),
		  mValueToProportionFunc(std::move(valueToProportionFunc)) {

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
	}

	/// @brief Mouse button double clicked event handler
	///
	/// @param e - The mouse event to handle
	auto ApexSlider::mouseDoubleClick(const juce::MouseEvent& e) noexcept -> void {
		juce::String text = getTextFromValue(getValue());
		juce::String textUntilDecimal = text.upToFirstOccurrenceOf(".", true, true);
		juce::String newText
			= text.dropLastCharacters(text.length() - (textUntilDecimal.length() + 2));
		mPopupTextBox->setSize(
			static_cast<int>(
				juce::jmax(General<>::roundU(SliderFloatingTextBoxStartWidth * mXScaleFactor
											 * gsl::narrow_cast<float>(newText.length())),
						   General<>::roundU(SliderFloatingTextBoxStartWidth * mXScaleFactor * 3))),
			static_cast<int>(General<>::roundU(SliderFloatingTextBoxStartHeight * mYScaleFactor)));
		size_t left = size_t(e.getPosition().x - mPopupTextBox->getWidth() / 2);
		size_t top = size_t(e.getPosition().y - mPopupTextBox->getHeight() / 2);
		mPopupTextBox->setTopLeftPosition(static_cast<int>(left), static_cast<int>(top));
		mPopupTextBox->setText(newText);
		mPopupTextBox->setVisible(true);
		mPopupTextBox->toFront(true);
		mPopupTextBox->grabKeyboardFocus();
		mPopupTextBox->setJustification(juce::Justification::centred);
	}

	/// @brief Mouse button pressed and dragged event handler
	///
	/// @param e - The mouse event to handle
	auto ApexSlider::mouseDrag(const juce::MouseEvent& e) noexcept -> void {
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
		if(e.mods.isShiftDown() || mNormalModeIsVelocityMode) {
			e.source.enableUnboundedMouseMovement(false, true);
		}
	}

	/// @brief Mouse wheel moved event handler
	///
	/// @param e - The mouse event to handle
	/// @param wheel - The corresponding details of the mouse wheel
	auto ApexSlider::mouseWheelMove(const juce::MouseEvent& e,
									const juce::MouseWheelDetails& wheel) noexcept -> void {
		double reversed = wheel.isReversed ? -1.0 : 1.0;
		double val = getValue();
		if(e.mods.isShiftDown()) {
			setValue(val + wheel.deltaY * (reversed / 100.0F), juce::sendNotificationAsync);
		}
		else {
			setValue(val + wheel.deltaY * (reversed / 25.0F), juce::sendNotificationAsync);
		}
	}

	/// @brief Draws this slider to the screen
	///
	/// @param g - The graphics context to use for drawing
	auto ApexSlider::paint(juce::Graphics& g) noexcept -> void {
		double sliderPos = getValue();
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

	auto ApexSlider::isInBounds(juce::Point<int> p) const noexcept -> bool {
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
} // namespace apex::ui
