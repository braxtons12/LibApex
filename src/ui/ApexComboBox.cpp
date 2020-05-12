#include "ApexComboBox.h"

namespace apex {
	namespace ui {

		ApexComboBox::ApexComboBox(Image activeArrowImage, Image hoveredArrowImage,
				Image normalArrowImage)
			: ComboBox(), mActiveArrowImage(activeArrowImage),
			mHoveredArrowImage(hoveredArrowImage), mNormalArrowImage(normalArrowImage),
			mUsesArrowImages(true)
		{
			setJustificationType(Justification::centred);
		}

		ApexComboBox::ApexComboBox()
			: ComboBox(), mUsesArrowImages(false)
		{
			setJustificationType(Justification::centred);
		}

		void ApexComboBox::mouseDown(const MouseEvent& e) {
			mIsButtonDown = isEnabled() && !e.mods.isPopupMenu();

			ComboBox::mouseDown(e);
		}

		void ApexComboBox::mouseUp(const MouseEvent& e) {
			if(mIsButtonDown) mIsButtonDown = false;
			ComboBox::mouseUp(e);
		}

		void ApexComboBox::mouseEnter(const MouseEvent& e) {
			mHovered = true;
			ComboBox::mouseEnter(e);
		}

		void ApexComboBox::mouseExit(const MouseEvent& e) {
			mHovered = false;
			ComboBox::mouseExit(e);
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

		void ApexComboBox::setFont(Font newFont) {
			mFont = newFont;
		}

		Font ApexComboBox::getFont() {
			return mFont;
		}

		void ApexComboBox::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel) {
			mLookAndFeel = lookAndFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

		Option<Image> ApexComboBox::getActiveArrowImage() {
			return mUsesArrowImages ? Option<Image>::Some(mActiveArrowImage)
				: Option<Image>::None();
		}

		Option<Image> ApexComboBox::getHoveredArrowImage() {
			return mUsesArrowImages ? Option<Image>::Some(mHoveredArrowImage)
				: Option<Image>::None();
		}

		Option<Image> ApexComboBox::getNormalArrowImage() {
			return mUsesArrowImages ? Option<Image>::Some(mNormalArrowImage)
				: Option<Image>::None();
		}

		void ApexComboBox::paint(Graphics& g) {
			Label* temp = static_cast<Label*>(getChildComponent(0));
			mLookAndFeel->drawApexComboBox(g, getWidth(), getHeight(), mIsButtonDown,
					temp->getRight(), 0, getWidth() - temp->getRight(), getHeight(), *this);

			if(getTextWhenNothingSelected().isNotEmpty() && temp->getText().isEmpty()
					&& !temp->isBeingEdited())
				mLookAndFeel->drawApexComboBoxTextWhenNothingSelected(g, *this, *temp);
		}
	}
}
