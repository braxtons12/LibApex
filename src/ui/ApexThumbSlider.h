#pragma once

#include "../base/StandardIncludes.h"
#include "ApexSlider.h"

namespace apex::ui {
	/// @brief Apex Audio implementation of a linear slider that uses only a thumb for its visual
	/// representation
	class ApexThumbSlider : public ApexSlider {
	  public:
		/// @brief Constructs an `ApexThumbSlider` with the given style and thumb image
		///
		/// @param style - The style of the slider
		/// @param proportionToValueFunc - The function to convert the proportion of this slider to
		/// the corresponding user/dsp facing value
		/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
		/// corresponding proportion of this slider
		/// @param thumbImage - The image asset to use to draw the slider thumb
		ApexThumbSlider(juce::Slider::SliderStyle style,
						std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc,
						juce::Image thumbImage) noexcept;

		/// @brief Constructs an `ApexThumbSlider` with the given style and initial width and height
		///
		/// @param style - The style of the slider
		/// @param proportionToValueFunc - The function to convert the proportion of this slider to
		/// the corresponding user/dsp facing value
		/// @param valueToProportionFunc - The function to convert the user/dsp facing value to the
		/// corresponding proportion of this slider
		/// @param initialThumbWidth - The initial width of the thumb (usually the maximum size)
		/// @param initialThumbHeight - The initial height of the thumb (usually the maximum size)
		ApexThumbSlider(juce::Slider::SliderStyle style,
						std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc,
						size_t initialThumbWidth,
						size_t initialThumbHeight) noexcept;

		~ApexThumbSlider() noexcept override = default;

		/// @brief Draws this slider to the screen
		///
		/// @param g - The graphics context to use for drawing
		auto paint(juce::Graphics& g) noexcept -> void override;

		/// @brief re-layouts this component on a size change
		inline auto resized() noexcept -> void override {
			if(mUsesThumbImage) {
				resizeThumb();
			}
		}

	  protected:
		juce::Image mThumbImage;
		juce::ImageComponent mThumbComponent;
		bool mUsesThumbImage = false;

		size_t mInitialThumbWidth = 0;
		size_t mInitialThumbHeight = 0;

		/// @brief Resizes the thumb to fit the new bounds and value of the slider
		void resizeThumb() noexcept;

		/// @brief Determines if the given point is within the control-responsive bounds of this
		/// slider
		///
		/// @param p - The point in question
		/// @return - Whether the point is within the controllable bounds
		[[nodiscard]] auto isInBounds(juce::Point<int> p) const noexcept -> bool override;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexThumbSlider)
	};
} // namespace apex::ui
