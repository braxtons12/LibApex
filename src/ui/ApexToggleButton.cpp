#include "ApexToggleButton.h"

namespace apex::ui {
	/// @brief Constructs an `ApexToggleButton` with that uses the given film strip image asset
	/// for drawing
	///
	/// @param filmStrip - The film strip asset to use to draw this button
	ApexToggleButton::ApexToggleButton(ApexFilmStrip filmStrip) noexcept
		: mFilmStrip(std::move(filmStrip)), mUsesFilmStrip(true) {
	}

	/// @brief Constructs an `ApexToggleButton` with the given text label
	///
	/// @param text - The text label
	ApexToggleButton::ApexToggleButton(const juce::String& text) noexcept
		: juce::ToggleButton(text) {
	}

	/// @brief Constructs an `ApexToggleButton` that uses the given image assets to draw it
	///
	/// @param buttonToggled - The image to use when the button is toggled
	/// @param buttonHovered - The image to use when the button is hovered
	/// @param buttonNormal - The image to use when the button is in its normal state
	ApexToggleButton::ApexToggleButton(juce::Image buttonToggled,
									   juce::Image buttonHovered,
									   juce::Image buttonNormal) noexcept
		: mToggledImage(std::move(buttonToggled)), mHoveredImage(std::move(buttonHovered)),
		  mNormalImage(std::move(buttonNormal)), mUsesImages(true) {
	}

	/// @brief Draws this button to the screen
	///
	/// @param g - The graphics context to use to draw this
	/// @param shouldDrawButtonAsHighlighted - Whether the button should be drawn as hovered
	/// @param shouldDrawButtonAsDown - Whether the button should be drawn as pressed
	auto ApexToggleButton::paintButton(juce::Graphics& g,
									   bool shouldDrawButtonAsHighlighted,
									   bool shouldDrawButtonAsDown) noexcept -> void {
		juce::ignoreUnused(shouldDrawButtonAsDown);
		if(mLookAndFeel != nullptr) {
			mLookAndFeel->drawApexToggleButton(g,
											   *this,
											   shouldDrawButtonAsHighlighted,
											   getToggleState());
		}
	}
} // namespace apex::ui
