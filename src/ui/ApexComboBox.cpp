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
		: mActiveArrowImage(std::move(activeArrowImage)),
		  mHoveredArrowImage(std::move(hoveredArrowImage)),
		  mNormalArrowImage(std::move(normalArrowImage)), mUsesArrowImages(true) {
		setJustificationType(juce::Justification::centred);
	}

	/// @brief Constructs a default `ApexComboBox`
	ApexComboBox::ApexComboBox() noexcept {
		setJustificationType(juce::Justification::centred);
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
