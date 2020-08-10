#pragma once

#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexLookAndFeel.h"

namespace apex::ui {
	class ApexComboBox : public juce::ComboBox {
	  public:
		/// @brief Constructs an `ApexComboBox` with the given drop down arrow images
		///
		/// @param activeArrowImage - The drop down arrow image to use when the drop down is active
		/// @param hoveredArrowImage - The drop down arrow image to use when the combo box is
		/// hovered
		/// @param normalArrowImage - The drop down arrow image to use normally (ie, not active or
		/// hovered)
		ApexComboBox(juce::Image activeArrowImage,
					 juce::Image hoveredArrowImage,
					 juce::Image normalArrowImage) noexcept;

		/// @brief Constructs a default `ApexComboBox`
		ApexComboBox() noexcept;

		/// @brief Mouse button pressed event handler
		///
		/// @param e - The mouse event to handle
		auto mouseDown(const juce::MouseEvent& e) noexcept -> void override;

		/// @brief Mouse button released event handler
		///
		/// @param e - The mouse event to handle
		auto mouseUp(const juce::MouseEvent& e) noexcept -> void override;

		/// @brief Mouse entered component bounds handler
		///
		/// @param e - The mouse event to handle
		auto mouseEnter(const juce::MouseEvent& e) noexcept -> void override;

		/// @brief Mouse left component bounds handler
		///
		/// @param e - The mouse event to handle
		auto mouseExit(const juce::MouseEvent& e) noexcept -> void override;

		/// @brief Returns whether the mouse is currently over this component
		///
		/// @return - Whether the mouse is over this
		[[nodiscard]] inline auto isHovered() const noexcept -> bool;

		/// @brief Force-sets whether the mouse is over this component
		///
		/// @param hovered - Whether the mouse is over this
		inline auto setHovered(bool hovered) noexcept -> void;

		/// @brief Sets the x-scale factor from the full size, useful when using graphics assets
		/// instead of drawn elements to determine the ratio they should be scaled by
		///
		/// @param scaleFactor - The new scale factor
		inline auto setXScaleFactor(float scaleFactor) noexcept -> void;

		/// @brief Returns the current x-scale factor from the full size, useful when using graphics
		/// assets instead of drawn elements to determine the ratio they should be scaled by
		///
		/// @return - The current x-scale factor
		[[nodiscard]] inline auto getXScaleFactor() const noexcept -> float;

		/// @brief Sets the y-scale factor from the full size, useful when using graphics assets
		/// instead of drawn elements to determine the ratio they should be scaled by
		///
		/// @param scaleFactor - The new scale factor
		inline auto setYScaleFactor(float scaleFactor) noexcept -> void;

		/// @brief Returns the current y-scale factor from the full size, useful when using graphics
		/// assets instead of drawn elements to determine the ratio they should be scaled by
		///
		/// @return - The current y-scale factor
		[[nodiscard]] inline auto getYScaleFactor() const noexcept -> float;

		/// @brief Sets the font size for text elements controlled/drawn by this component
		///
		/// @param newFont - The new font size
		inline auto setFont(juce::Font newFont) noexcept -> void;

		/// @brief Returns the current font size
		///
		/// @return - The current font size
		[[nodiscard]] inline auto getFont() const noexcept -> juce::Font;

		/// @brief Sets the look and feel of this component to the given one
		///
		/// @param lookNFeel - The new look and feel
		inline auto setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) noexcept -> void;

		/// @brief Returns the drop down arrow image to use when the drop down is active, if this is
		/// using graphics assets instead of drawn components
		///
		/// @return - The drop down arrow active image, or `None`
		[[nodiscard]] inline auto getActiveArrowImage() const noexcept -> Option<juce::Image>;

		/// @brief Returns the drop down arrow image to use when this is hovered, if this is
		/// using graphics assets instead of drawn components
		///
		/// @return - The drop down arrow hovered image, or `None`
		[[nodiscard]] inline auto getHoveredArrowImage() const noexcept -> Option<juce::Image>;

		/// @brief Returns the drop down arrow image to use normally, if this is
		/// using graphics assets instead of drawn components
		///
		/// @return - The drop down arrow normal image, or `None`
		[[nodiscard]] inline auto getNormalArrowImage() const noexcept -> Option<juce::Image>;

		/// @brief Draws this component to the screen
		///
		/// @param g - The graphics context to draw with
		auto paint(juce::Graphics& g) noexcept -> void override;

	  protected:
		std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
		juce::Image mActiveArrowImage;
		juce::Image mHoveredArrowImage;
		juce::Image mNormalArrowImage;
		bool mUsesArrowImages = false;

		bool mHovered = false;
		float mXScaleFactor = 1.0F;
		float mYScaleFactor = 1.0F;
		juce::Font mFont;

		bool mIsButtonDown = false;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexComboBox)
	};
} // namespace apex::ui
