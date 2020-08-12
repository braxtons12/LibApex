#pragma once

#include <functional>
#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex::ui {
	/// @brief Apex Audio implementation of a generic `juce::Slider`
	class ApexSlider : public juce::Slider,
					   public juce::TextEditor::Listener,
					   public juce::FocusChangeListener,
					   public juce::Label::Listener {
	  public:
		static const constexpr size_t SliderFloatingTextBoxStartWidth = 25;
		static const constexpr size_t SliderFloatingTextBoxStartHeight = 30;

		/// @brief Creates an `ApexSlider` of the given style
		///
		/// @param style - The style of the slider
		/// @param proportionToValueFunc - The function to convert the proportion of the slider to
		/// the corresponding user/dsp facing value
		/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
		/// corresponding proportion of the slider
		ApexSlider(juce::Slider::SliderStyle style,
				   std::function<double(double)> proportionToValueFunc,
				   std::function<double(double)> valueToProportionFunc) noexcept;

		/// @brief Creates an `ApexSlider` of the given style, that uses the given film strip image
		/// asset to draw itself
		///
		/// @param style - The style of the slider
		/// @param proportionToValueFunc - The function to convert the proportion of the slider to
		/// the corresponding user/dsp facing value
		/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
		/// corresponding proportion of the slider
		/// @param strip - The film strip to use to visually represent this
		ApexSlider(juce::Slider::SliderStyle style,
				   std::function<double(double)> proportionToValueFunc,
				   std::function<double(double)> valueToProportionFunc,
				   ApexFilmStrip strip) noexcept;

		~ApexSlider() noexcept override = default;

		/// @brief Returns the user/dsp facing value, as a string, corresponding to the given
		/// proportion
		///
		/// @param value the proportion to get the user/dsp facing value for
		///
		/// @return - The corresponding user/dsp facing value, as a string
		inline auto getTextFromValue(double value) noexcept -> juce::String override {
			double val = getValueFromProportion(value);
			juce::String temp = juce::String(val);
			return temp.dropLastCharacters(
				temp.length() - (temp.upToFirstOccurrenceOf(".", true, true).length() + 2));
		}

		/// @brief Returns the proportion corresponding to the given string representing the
		/// user/dsp facing value
		///
		/// @param text - The user/dsp facing value, as a string
		///
		/// @return - The corresponding proportion of this slider
		inline auto getValueFromText(const juce::String& text) noexcept -> double override {
			return getProportionFromValue(text.getDoubleValue());
		}

		/// @brief Mouse button pressed event handler
		///
		/// @param e - The mouse event to handle
		inline auto mouseDown(const juce::MouseEvent& e) noexcept -> void override {
			mPopupTextBox->setVisible(false);
			if(e.mods.isLeftButtonDown() && e.mods.isCtrlDown()) {
				setValue(getDoubleClickReturnValue(), juce::sendNotificationAsync);
			}
			else if(isInBounds(e.getMouseDownPosition())) {
				juce::Slider::mouseDown(e);
			}
		}

		/// @brief Mouse button released event handler
		///
		/// @param e - The mouse event to handle
		inline auto mouseUp(const juce::MouseEvent& e) noexcept -> void override {
			if(isInBounds(e.getMouseDownPosition())) {
				juce::Slider::mouseUp(e);
			}
		}

		/// @brief Mouse button double clicked event handler
		///
		/// @param e - The mouse event to handle
		auto mouseDoubleClick(const juce::MouseEvent& e) noexcept -> void override;

		/// @brief Mouse button pressed and dragged event handler
		///
		/// @param e - The mouse event to handle
		auto mouseDrag(const juce::MouseEvent& e) noexcept -> void override;

		/// @brief Mouse wheel moved event handler
		///
		/// @param e - The mouse event to handle
		/// @param wheel - The corresponding details of the mouse wheel
		auto mouseWheelMove(const juce::MouseEvent& e,
							const juce::MouseWheelDetails& wheel) noexcept -> void override;

		/// @brief Global component focus change event handler
		/// Used to detect when the popup text entry box has lost focus
		///
		/// @param focusedComponent - The newly focused component
		inline auto
		globalFocusChanged(juce::Component* focusedComponent) noexcept -> void override {
			ignoreUnused(focusedComponent);
			mPopupTextBox->setVisible(false);
		}

		/// @brief Text editor return key pressed event handler
		/// Used to detect when the popup text entry box has had its value changed and committed
		///
		/// @param editor - The text editor in focus when the return key was pressed
		inline auto textEditorReturnKeyPressed(juce::TextEditor& editor) noexcept -> void override {
			double value = getValueFromText(editor.getText());
			isValueValid(value) ? setValue(value) : setValue(getValue());
			mPopupTextBox->setVisible(false);
		}

		/// @brief Text editor escape key pressed event handler
		/// Used to detect when the popup text entry box has been escaped
		///
		/// @param editor - The text editor in focus when the escape key was pressed
		inline auto textEditorEscapeKeyPressed(juce::TextEditor& editor) noexcept -> void override {
			juce::ignoreUnused(editor);
			mPopupTextBox->setVisible(false);
		}

		/// @brief Text editor focus lost event handler
		/// Used to detect when the popup text entry box has lost focus
		///
		/// @param editor - The text editor that lost focus
		inline auto textEditorFocusLost(juce::TextEditor& editor) noexcept -> void override {
			juce::ignoreUnused(editor);
			if(mPopupTextBox->isVisible() && !mPopupTextBox->hasKeyboardFocus(true)) {
				mPopupTextBox->setVisible(false);
			}
		}

		/// @brief Text label text changed event handler
		/// Used to detect when the value label/editor has had its contents changed and committed
		///
		/// @param label - The label whose text changed
		inline auto labelTextChanged(juce::Label* label) noexcept -> void override {
			if(label != nullptr) {
				double val = getValueFromText(label->getText());
				isValueValid(val) ?
					  setValue(val, juce::sendNotificationAsync) :
					  label->setText(getTextFromValue(getValue()), juce::dontSendNotification);
			}
		}

		/// @brief Sets the drawing scaling factor in the x-axis from the initial (usually maximum)
		/// size
		///
		/// @param scaleFactor - The new scale factor
		inline auto setXScaleFactor(float scaleFactor) noexcept -> void {
			mXScaleFactor = scaleFactor;
		}

		/// @brief Returns the current x-axis drawing scale factor
		///
		/// @return - The current x scale factor
		[[nodiscard]] inline auto getXScaleFactor() const noexcept -> float {
			return mXScaleFactor;
		}

		/// @brief Sets the drawing scaling factor in the y-axis from the initial (usually maximum)
		/// size
		///
		/// @param scaleFactor - The new scale factor
		inline auto setYScaleFactor(float scaleFactor) noexcept -> void {
			mYScaleFactor = scaleFactor;
		}

		/// @brief Returns the current y-axis drawing scale factor
		///
		/// @return - The current y scale factor
		[[nodiscard]] inline auto getYScaleFactor() const noexcept -> float {
			return mYScaleFactor;
		}

		/// @brief Determines if the given value is valid for this slider
		///
		/// @param value - The value in question
		///
		/// @return - Whether the value is valid
		[[nodiscard]] inline auto isValueValid(double value) const noexcept -> bool {
			return value <= 1.00 && value >= 0.00;
		}

		/// @brief Sets the font height of the popup text box to the given value
		///
		/// @param font - The new font height
		inline auto setPopupTextBoxFont(juce::Font font) noexcept -> void {
			mPopupTextBoxFont = std::move(font);
			mPopupTextBox->applyFontToAllText(mPopupTextBoxFont, true);
		}

		/// @brief Sets the font height for the value label/editor to the give value
		///
		/// @param font - The new font height
		inline auto setTextBoxFont(juce::Font font) noexcept -> void {
			mTextBoxFont = std::move(font);
			mTextBox.setFont(mTextBoxFont);
		}

		/// @brief Sets the font color for the value label/editor to the given value
		///
		/// @param newColour - The new color of the value label/editor text
		inline auto setTextBoxFontColour(juce::Colour fontColour) noexcept -> void {
			mTextBoxFontColour = fontColour;
			mTextBox.setColour(juce::Label::textColourId, fontColour);
		}

		/// @brief Sets the bounds of the value label/editor to the given ones
		///
		/// @param bounds - The new bounds for the value label/editor
		inline auto setTextBoxBounds(juce::Rectangle<int> bounds) noexcept -> void {
			mTextBox.setBounds(bounds);
		}

		/// @brief Returns the value label/editor as a pointer
		///
		/// @return - The value label/editor as a pointer
		inline auto getTextBox() noexcept -> juce::Label* {
			return &mTextBox;
		}

		/// @brief Sets the normal click+drag movement sensitivity.
		/// Only applies if normal mode is NOT velocity-based, eg:
		/// `setNormalModeIsVelocityBased(true)` has NOT been called
		///
		/// @param sensitivity - The sensitivity rate. Default is 2.4; higher is more sensitive
		/// @param initialVelocity - The initial velocity. Default is 0.08; higher is more sensitive
		inline auto
		setNormalSensitivity(double sensitivity, double initialVelocity) noexcept -> void {
			mNormalSensitivity = sensitivity;
			mNormalInitialVelocity = initialVelocity;
		}

		/// @brief Sets the fine (shift+click) click+drag movement sensitivity
		///
		/// @param sensitivity - The sensitivity rate. Default is 0.8; higher is more sensitive
		/// @param initialVelocity - The initial velocity. Default is 0.04; higher is more sensitive
		inline auto
		setFineSensitivity(double sensitivity, double initialVelocity) noexcept -> void {
			mFineSensitivity = sensitivity;
			mFineInitialVelocity = initialVelocity;
		}

		/// @brief Sets if normal click+drag movement is velocity based
		///
		/// @param set - Whether normal mode click+drag movement is velocity based
		inline auto setNormalModeIsVelocityBased(bool set) noexcept -> void {
			mNormalModeIsVelocityMode = set;
		}

		/// @brief Hides the value label/editor
		inline auto hideEditor() noexcept -> void {
			mPopupTextBox->setVisible(false);
		}

		/// @brief Returns the film strip image asset used to draw this slider, if it uses one
		///
		/// @return - `Some(ApexFilmStrip)` if this uses a film strip for drawing, otherwise `None`
		[[nodiscard]] inline auto getFilmStrip() const noexcept -> Option<ApexFilmStrip> {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip) :
									  Option<ApexFilmStrip>::None();
		}

		/// @brief Gets the user/dsp facing value corresponding to the given proportion of this
		/// slider
		///
		/// @param prop - The proportion to get the corresponding user/dsp facing value for
		///
		/// @return - The user/dsp facing value
		[[nodiscard]] inline auto getValueFromProportion(double prop) const noexcept -> double {
			return mProportionToValueFunc(prop);
		}

		/// @brief Gets the proportion corresponding to the given user/dsp facing value
		///
		/// @param value - The user/dsp facing value to get the corresponding proportion for
		///
		/// @return - The proporiton of this slider
		[[nodiscard]] inline auto getProportionFromValue(double value) const noexcept -> double {
			return mValueToProportionFunc(value);
		}

		/// @brief Sets the look and feel used to draw this slider to the given one
		///
		/// @param lookNFeel - The new look and feel
		inline auto setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) noexcept -> void {
			mLookAndFeel = std::move(lookNFeel);
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}
		/// @brief Draws this slider to the screen
		///
		/// @param g - The graphics context to use for drawing
		auto paint(juce::Graphics& g) noexcept -> void override;

	  protected:
		std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
		ApexFilmStrip mFilmStrip;
		bool mUsesFilmStrip = false;

		std::function<double(double)> mProportionToValueFunc;
		std::function<double(double)> mValueToProportionFunc;

		float mXScaleFactor = 1.0F;
		float mYScaleFactor = 1.0F;
		double mNormalSensitivity = 2.4;
		double mNormalInitialVelocity = 0.08;
		double mFineSensitivity = 0.8;
		double mFineInitialVelocity = 0.04;
		bool mNormalModeIsVelocityMode = false;

		int mNumTextBoxCharacters = 6;

		juce::Font mPopupTextBoxFont;
		std::unique_ptr<juce::TextEditor> mPopupTextBox;

		juce::Font mTextBoxFont;
		juce::Label mTextBox;

		juce::Colour mTextBoxFontColour = juce::Colour(0xffcde3ec);

		[[nodiscard]] virtual auto isInBounds(juce::Point<int> p) const noexcept -> bool;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexSlider)
	};
} // namespace apex::ui
