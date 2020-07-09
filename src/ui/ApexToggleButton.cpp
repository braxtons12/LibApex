#include "ApexToggleButton.h"

namespace apex {
	namespace ui {
		ApexToggleButton::ApexToggleButton(ApexFilmStrip filmStrip)
			: juce::ToggleButton(),
			mFilmStrip(filmStrip), mUsesFilmStrip(true), mUsesImages(false)
		{

		}

		ApexToggleButton::ApexToggleButton(juce::Image buttonToggled, juce::Image buttonHovered,
				juce::Image buttonNormal)
			: juce::ToggleButton(),
			mToggledImage(buttonToggled), mHoveredImage(buttonHovered),
			mNormalImage(buttonNormal), mUsesFilmStrip(false), mUsesImages(true)
		{

		}

		ApexToggleButton::ApexToggleButton(const juce::String& text)
			: juce::ToggleButton(text),
			mUsesFilmStrip(false), mUsesImages(false)
		{

		}

		ApexToggleButton::~ApexToggleButton() {

		}

		Option<ApexFilmStrip> ApexToggleButton::getFilmStrip() {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip)
				: Option<ApexFilmStrip>::None();
		}

		Option<juce::Image> ApexToggleButton::getToggledImage() {
			return mUsesImages ? Option<juce::Image>::Some(mToggledImage)
				: Option<juce::Image>::None();
		}

		Option<juce::Image> ApexToggleButton::getHoveredImage() {
			return mUsesImages ? Option<juce::Image>::Some(mHoveredImage)
				: Option<juce::Image>::None();
		}

		Option<juce::Image> ApexToggleButton::getNormalImage() {
			return mUsesImages ? Option<juce::Image>::Some(mNormalImage)
				: Option<juce::Image>::None();
		}

		void ApexToggleButton::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) {
			mLookAndFeel = lookNFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

		void ApexToggleButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
				bool shouldDrawButtonAsDown)
		{
			juce::ignoreUnused(shouldDrawButtonAsDown);
			if(mLookAndFeel != nullptr)
				mLookAndFeel->drawApexToggleButton(g, *this, shouldDrawButtonAsHighlighted,
						getToggleState());
		}
	}
}
