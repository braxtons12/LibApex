#pragma once

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex::ui {
	/// @brief Apex Audio implementation of a basic toggle button
	class ApexToggleButton : public juce::ToggleButton {
	  public:
		/// @brief Constructs an `ApexToggleButton` with that uses the given film strip image asset
		/// for drawing
		///
		/// @param filmStrip - The film strip asset to use to draw this button
		explicit ApexToggleButton(ApexFilmStrip filmStrip) noexcept;

		/// @brief Constructs an `ApexToggleButton` with the given text label
		///
		/// @param text - The text label
		explicit ApexToggleButton(const juce::String& text) noexcept;

		/// @brief Constructs an `ApexToggleButton` that uses the given image assets to draw it
		///
		/// @param buttonToggled - The image to use when the button is toggled
		/// @param buttonHovered - The image to use when the button is hovered
		/// @param buttonNormal - The image to use when the button is in its normal state
		ApexToggleButton(juce::Image buttonToggled,
						 juce::Image buttonHovered,
						 juce::Image buttonNormal) noexcept;

		~ApexToggleButton() noexcept override = default;

		/// @brief Returns the film strip used to draw this toggle button, if it uses one
		///
		/// @return - `Some(ApexFilmStrip)` if this uses a film strip, or `None`
		[[nodiscard]] inline auto getFilmStrip() const noexcept -> Option<ApexFilmStrip> {
			return mUsesFilmStrip ? Some(mFilmStrip) : None<ApexFilmStrip>();
		}

		/// @brief Returns the image to draw when this is in the toggled state, if it uses separated
		/// image assets
		///
		/// @return - `Some(juce::Image)` if this uses separated image assets for its states, or
		/// `None` otherwise
		[[nodiscard]] inline auto getToggledImage() const noexcept -> Option<juce::Image> {
			return mUsesImages ? Some(mToggledImage) : None<juce::Image>();
		}

		/// @brief Returns the image to draw when this is in the hovered state, if it uses separated
		/// image assets
		///
		/// @return - `Some(juce::Image)` if this uses separated image assets for its states, or
		/// `None` otherwise
		[[nodiscard]] inline auto getHoveredImage() const noexcept -> Option<juce::Image> {
			return mUsesImages ? Some(mHoveredImage) : None<juce::Image>();
		}

		/// @brief Returns the image to draw when this is in the normal state, if it uses separated
		/// image assets
		///
		/// @return - `Some(juce::Image)` if this uses separated image assets for its states, or
		/// `None` otherwise
		[[nodiscard]] inline auto getNormalImage() const noexcept -> Option<juce::Image> {
			return mUsesImages ? Some(mNormalImage) : None<juce::Image>();
		}

		/// @brief Sets the LookAndFeel used to draw this button to the given one
		///
		/// @param lookNFeel - The new look and feel
		inline auto setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) noexcept -> void {
			mLookAndFeel = std::move(lookNFeel);
			juce::Component::setLookAndFeel(mLookAndFeel.get());
		}

	  protected:
		/// @brief Draws this button to the screen
		///
		/// @param g - The graphics context to use to draw this
		/// @param shouldDrawButtonAsHighlighted - Whether the button should be drawn as hovered
		/// @param shouldDrawButtonAsDown - Whether the button should be drawn as pressed
		auto paintButton(juce::Graphics& g,
						 bool shouldDrawButtonAsHighlighted,
						 bool shouldDrawButtonAsDown) noexcept -> void override;
		std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
		ApexFilmStrip mFilmStrip;
		juce::Image mToggledImage;
		juce::Image mHoveredImage;
		juce::Image mNormalImage;
		bool mUsesFilmStrip = false;
		bool mUsesImages = false;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexToggleButton)
	};
} // namespace apex::ui
