#include "ApexComboBox.h"

namespace apex {
	namespace ui {

		ApexComboBox::ApexComboBox(juce::Image activeArrowImage, juce::Image hoveredArrowImage,
				juce::Image normalArrowImage)
			: juce::ComboBox(), mActiveArrowImage(activeArrowImage),
			mHoveredArrowImage(hoveredArrowImage), mNormalArrowImage(normalArrowImage),
			mUsesArrowImages(true)
		{
			setJustificationType(juce::Justification::centred);
		}

		ApexComboBox::ApexComboBox()
			: juce::ComboBox(), mUsesArrowImages(false)
		{
			setJustificationType(juce::Justification::centred);
		}

		void ApexComboBox::mouseDown(const juce::MouseEvent& e) {
			mIsButtonDown = isEnabled() && !e.mods.isPopupMenu();

			juce::ComboBox::mouseDown(e);
		}

		void ApexComboBox::mouseUp(const juce::MouseEvent& e) {
			if(mIsButtonDown) mIsButtonDown = false;
			juce::ComboBox::mouseUp(e);
		}

		void ApexComboBox::mouseEnter(const juce::MouseEvent& e) {
			mHovered = true;
			juce::ComboBox::mouseEnter(e);
		}

		void ApexComboBox::mouseExit(const juce::MouseEvent& e) {
			mHovered = false;
			juce::ComboBox::mouseExit(e);
		}

		bool ApexComboBox::isHovered() {
			return mHovered;
		}

		void ApexComboBox::setHovered(bool hovered) {
			mHovered = hovered;
		}

		void ApexComboBox::setXScaleFactor(float scaleFactor) {
			mXScaleFactor = scaleFactor;
		}

		float ApexComboBox::getXScaleFactor() {
			return mXScaleFactor;
		}

		void ApexComboBox::setYScaleFactor(float scaleFactor) {
			mYScaleFactor = scaleFactor;
		}

		float ApexComboBox::getYScaleFactor() {
			return mYScaleFactor;
		}

		void ApexComboBox::setFont(juce::Font newFont) {
			mFont = newFont;
		}

		juce::Font ApexComboBox::getFont() {
			return mFont;
		}

		void ApexComboBox::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) {
			mLookAndFeel = lookNFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

		Option<juce::Image> ApexComboBox::getActiveArrowImage() {
			return mUsesArrowImages ? Option<juce::Image>::Some(mActiveArrowImage)
				: Option<juce::Image>::None();
		}

		Option<juce::Image> ApexComboBox::getHoveredArrowImage() {
			return mUsesArrowImages ? Option<juce::Image>::Some(mHoveredArrowImage)
				: Option<juce::Image>::None();
		}

		Option<juce::Image> ApexComboBox::getNormalArrowImage() {
			return mUsesArrowImages ? Option<juce::Image>::Some(mNormalArrowImage)
				: Option<juce::Image>::None();
		}

		void ApexComboBox::paint(juce::Graphics& g) {
			juce::Label* temp = static_cast<juce::Label*>(getChildComponent(0));
			if(mLookAndFeel != nullptr) {
				mLookAndFeel->drawApexComboBox(g, getWidth(), getHeight(), mIsButtonDown,
						temp->getRight(), 0, getWidth() - temp->getRight(), getHeight(), *this);

				if(getTextWhenNothingSelected().isNotEmpty() && temp->getText().isEmpty()
						&& !temp->isBeingEdited())
					mLookAndFeel->drawApexComboBoxTextWhenNothingSelected(g, *this, *temp);
			}
		}
	}
}
