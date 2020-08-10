#include "ApexComboBox.h"

namespace apex::ui {
	/// @brief Constructs an `ApexComboBox` with the given drop down arrow images
	///
	/// @param activeArrowImage - The drop down arrow image to use when the drop down is active
	/// @param hoveredArrowImage - The drop down arrow image to use when the combo box is
	/// hovered
	/// @param normalArrowImage - The drop down arrow image to use normally (ie, not active or
	/// hovered)
	ApexComboBox::ApexComboBox(juce::Image activeArrowImage,
							   juce::Image hoveredArrowImage,
							   juce::Image normalArrowImage) noexcept
		: juce::ComboBox(), mActiveArrowImage(std::move(activeArrowImage)),
		  mHoveredArrowImage(std::move(hoveredArrowImage)),
		  mNormalArrowImage(std::move(normalArrowImage)), mUsesArrowImages(true) {
		setJustificationType(juce::Justification::centred);
	}

	/// @brief Constructs a default `ApexComboBox`
	ApexComboBox::ApexComboBox() noexcept : juce::ComboBox() {
		setJustificationType(juce::Justification::centred);
	}

	/// @brief Mouse button pressed event handler
	///
	/// @param e - The mouse event to handle
	inline auto ApexComboBox::mouseDown(const juce::MouseEvent& e) noexcept -> void {
		mIsButtonDown = isEnabled() && !e.mods.isPopupMenu();

		juce::ComboBox::mouseDown(e);
	}

	/// @brief Mouse button released event handler
	///
	/// @param e - The mouse event to handle
	inline auto ApexComboBox::mouseUp(const juce::MouseEvent& e) noexcept -> void {
		if(mIsButtonDown) {
			mIsButtonDown = false;
		}
		juce::ComboBox::mouseUp(e);
	}

	/// @brief Mouse entered component bounds handler
	///
	/// @param e - The mouse event to handle
	inline auto ApexComboBox::mouseEnter(const juce::MouseEvent& e) noexcept -> void {
		mHovered = true;
		juce::ComboBox::mouseEnter(e);
	}

	/// @brief Mouse left component bounds handler
	///
	/// @param e - The mouse event to handle
	inline auto ApexComboBox::mouseExit(const juce::MouseEvent& e) noexcept -> void {
		mHovered = false;
		juce::ComboBox::mouseExit(e);
	}

	/// @brief Returns whether the mouse is currently over this component
	///
	/// @return - Whether the mouse is over this
	inline auto ApexComboBox::isHovered() const noexcept -> bool {
		return mHovered;
	}

	/// @brief Force-sets whether the mouse is over this component
	///
	/// @param hovered - Whether the mouse is over this
	inline auto ApexComboBox::setHovered(bool hovered) noexcept -> void {
		mHovered = hovered;
	}

	/// @brief Sets the x-scale factor from the full size, useful when using graphics assets
	/// instead of drawn elements to determine the ratio they should be scaled by
	///
	/// @param scaleFactor - The new scale factor
	inline auto ApexComboBox::setXScaleFactor(float scaleFactor) noexcept -> void {
		mXScaleFactor = scaleFactor;
	}

	/// @brief Returns the current x-scale factor from the full size, useful when using graphics
	/// assets instead of drawn elements to determine the ratio they should be scaled by
	///
	/// @return - The current x-scale factor
	inline auto ApexComboBox::getXScaleFactor() const noexcept -> float {
		return mXScaleFactor;
	}

	/// @brief Sets the y-scale factor from the full size, useful when using graphics assets
	/// instead of drawn elements to determine the ratio they should be scaled by
	///
	/// @param scaleFactor - The new scale factor
	inline auto ApexComboBox::setYScaleFactor(float scaleFactor) noexcept -> void {
		mYScaleFactor = scaleFactor;
	}

	/// @brief Returns the current y-scale factor from the full size, useful when using graphics
	/// assets instead of drawn elements to determine the ratio they should be scaled by
	///
	/// @return - The current y-scale factor
	inline auto ApexComboBox::getYScaleFactor() const noexcept -> float {
		return mYScaleFactor;
	}

	/// @brief Sets the font size for text elements controlled/drawn by this component
	///
	/// @param newFont - The new font size
	inline auto ApexComboBox::setFont(juce::Font newFont) noexcept -> void {
		mFont = std::move(newFont);
	}

	/// @brief Returns the current font size
	///
	/// @return - The current font size
	inline auto ApexComboBox::getFont() const noexcept -> juce::Font {
		return mFont;
	}

	/// @brief Sets the look and feel of this component to the given one
	///
	/// @param lookNFeel - The new look and feel
	inline auto
	ApexComboBox::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) noexcept -> void {
		mLookAndFeel = std::move(lookNFeel);
		juce::Component::setLookAndFeel(mLookAndFeel.get());
	}

	/// @brief Returns the drop down arrow image to use when the drop down is active, if this is
	/// using graphics assets instead of drawn components
	///
	/// @return - The drop down arrow active image, or `None`
	inline auto ApexComboBox::getActiveArrowImage() const noexcept -> Option<juce::Image> {
		return mUsesArrowImages ? Option<juce::Image>::Some(mActiveArrowImage) :
									Option<juce::Image>::None();
	}

	/// @brief Returns the drop down arrow image to use when this is hovered, if this is
	/// using graphics assets instead of drawn components
	///
	/// @return - The drop down arrow hovered image, or `None`
	inline auto ApexComboBox::getHoveredArrowImage() const noexcept -> Option<juce::Image> {
		return mUsesArrowImages ? Option<juce::Image>::Some(mHoveredArrowImage) :
									Option<juce::Image>::None();
	}

	/// @brief Returns the drop down arrow image to use normally, if this is
	/// using graphics assets instead of drawn components
	///
	/// @return - The drop down arrow normal image, or `None`
	inline auto ApexComboBox::getNormalArrowImage() const noexcept -> Option<juce::Image> {
		return mUsesArrowImages ? Option<juce::Image>::Some(mNormalArrowImage) :
									Option<juce::Image>::None();
	}

	/// @brief Draws this component to the screen
	///
	/// @param g - The graphics context to draw with
	void ApexComboBox::paint(juce::Graphics& g) noexcept {
		auto* temp = dynamic_cast<juce::Label*>(getChildComponent(0));
		if(mLookAndFeel != nullptr) {
			mLookAndFeel->drawApexComboBox(g,
										   getWidth(),
										   getHeight(),
										   mIsButtonDown,
										   temp->getRight(),
										   0,
										   getWidth() - temp->getRight(),
										   getHeight(),
										   *this);

			if(getTextWhenNothingSelected().isNotEmpty() && temp->getText().isEmpty()
			   && !temp->isBeingEdited()) {
				mLookAndFeel->drawApexComboBoxTextWhenNothingSelected(g, *this, *temp);
			}
		}
	}
} // namespace apex::ui
