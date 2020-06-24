#include "ApexToggleButton.h"

namespace apex {
	namespace ui {
		ApexToggleButton::ApexToggleButton(ApexFilmStrip filmStrip)
			: ToggleButton(),
			mFilmStrip(filmStrip), mUsesFilmStrip(true), mUsesImages(false)
		{

		}

		ApexToggleButton::ApexToggleButton(Image buttonToggled, Image buttonHovered,
				Image buttonNormal)
			: ToggleButton(),
			mToggledImage(buttonToggled), mHoveredImage(buttonHovered),
			mNormalImage(buttonNormal), mUsesFilmStrip(false), mUsesImages(true)
		{

		}

		ApexToggleButton::ApexToggleButton(const String& text)
			: ToggleButton(text),
			mUsesFilmStrip(false), mUsesImages(false)
		{

		}

		ApexToggleButton::~ApexToggleButton() {

		}

		Option<ApexFilmStrip> ApexToggleButton::getFilmStrip() {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip)
				: Option<ApexFilmStrip>::None();
		}

		Option<Image> ApexToggleButton::getToggledImage() {
			return mUsesImages ? Option<Image>::Some(mToggledImage)
				: Option<Image>::None();
		}

		Option<Image> ApexToggleButton::getHoveredImage() {
			return mUsesImages ? Option<Image>::Some(mHoveredImage)
				: Option<Image>::None();
		}

		Option<Image> ApexToggleButton::getNormalImage() {
			return mUsesImages ? Option<Image>::Some(mNormalImage)
				: Option<Image>::None();
		}

		void ApexToggleButton::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel) {
			mLookAndFeel = lookAndFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

		void ApexToggleButton::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted,
				bool shouldDrawButtonAsDown)
		{
			ignoreUnused(shouldDrawButtonAsDown);
			if(mLookAndFeel != nullptr)
				mLookAndFeel->drawApexToggleButton(g, *this, shouldDrawButtonAsHighlighted,
						getToggleState());
		}
	}
}
