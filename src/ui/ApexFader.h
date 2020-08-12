#pragma once

#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexSlider.h"

namespace apex::ui {
	/// @brief Apex Audio implementation of a "fader" UI component
	class ApexFader : public ApexSlider {
	  public:
		/// @brief Constructs an `ApexFader` with the given parameters. This will use a film strip
		/// based visual representation, provided by the given film strip
		///
		/// @param style - The `SliderStyle` of the fader, eg: linear horizontal, linear vertical,
		/// etc
		/// @param proportionToValueFunc - The function to use to convert the proportion of length
		/// corresponding to the fader's current setting to its user and dsp facing value
		/// @param valueToProportionFunc - The function to use to convert the user/dsp facing value
		/// of the fader's current setting to the corresponding proportion of its length
		/// @param strip - The film strip graphics asset containing the images(frames) for the
		/// fader's values
		ApexFader(juce::Slider::SliderStyle style,
				  std::function<double(double)> proportionToValueFunc,
				  std::function<double(double)> valueToProportionFunc,
				  ApexFilmStrip strip) noexcept;

		/// @brief Constructs an `ApexFader` with the given parameters. This will use a "thumb only"
		/// visual representation via the given thumb image
		///
		/// @param style - The `SliderStyle` of the fader, eg: linear horizontal, linear vertical,
		/// etc
		/// @param proportionToValueFunc - The function to use to convert the proportion of length
		/// corresponding to the fader's current setting to its user and dsp facing value
		/// @param valueToProportionFunc - The function to use to convert the user/dsp facing value
		/// of the fader's current setting to the corresponding proportion of its length
		/// @param thumbImage - The image to use as the fader's thumb (the cap, knob, handle,etc)
		ApexFader(juce::Slider::SliderStyle style,
				  std::function<double(double)> proportionToValueFunc,
				  std::function<double(double)> valueToProportionFunc,
				  juce::Image thumbImage) noexcept;

		/// @brief Constructs an `ApexFader` with the given parameters. This will use drawn graphics
		/// for the visual representation, either using a "filled meter" style or "thumb only" style
		/// depending on the value of `isThumbOnlySlider`
		///
		/// @param style - The `SliderStyle` of the fader, eg: linear horizontal, linear vertical,
		/// etc
		/// @param proportionToValueFunc - The function to use to convert the proportion of length
		/// corresponding to the fader's current setting to its user and dsp facing value
		/// @param valueToProportionFunc - The function to use to convert the user/dsp facing value
		/// of the fader's current setting to the corresponding proportion of its length
		/// @param isThumbOnlySlider - Whether the visual representation should only use a thumb
		/// @param initialThumbWidth - The initial (typically the maximum) width of the thumb
		/// @param initialThumbHeight - The initial (typically the maximum) height of the thumb
		ApexFader(juce::Slider::SliderStyle style,
				  std::function<double(double)> proportionToValueFunc,
				  std::function<double(double)> valueToProportionFunc,
				  bool isThumbOnlySlider,
				  size_t initialThumbWidth,
				  size_t initialThumbHeight) noexcept;

		~ApexFader() noexcept override = default;

		/// @brief Returns the user-facing value, as a `juce::String`, corresponding to the given
		/// proportion
		///
		/// @param proportion - The proportion to the get corresponding user-facing value string for
		///
		/// @return - The user-facing value as a `juce::String`
		[[nodiscard]] inline auto getTextFromValue(double value) noexcept -> juce::String override {
			double val = getValueFromProportion(value);
			juce::String temp = juce::String(val);
			if(math::fabs(val) <= 0.010) {
				return juce::String("0.0");
			}
			else {
				return temp.dropLastCharacters(
					temp.length() - (temp.upToFirstOccurrenceOf(".", true, true).length() + 2));
			}
		}

		/// @brief Returns the proportion corresponding to the given user-facing value string
		///
		/// @param text - The user-facing value string to get the corresponding proportion for
		///
		/// @return - The proportion corresponding to the given user-facing value string
		[[nodiscard]] inline auto
		getValueFromText(const juce::String& text) noexcept -> double override {
			return getProportionFromValue(text.getDoubleValue());
		}

		/// @brief Mouse wheel movement event handler
		///
		/// @param e - The mouse event to handle
		/// @param wheel - The details of the mouse wheel
		inline auto mouseWheelMove(const juce::MouseEvent& e,
								   const juce::MouseWheelDetails& wheel) noexcept -> void override {
			double reversed = wheel.isReversed ? -1.0 : 1.0;
			double val = getValueFromProportion(getValue());
			if(e.mods.isShiftDown()) {
				setValue(getProportionFromValue(val + 1.0 * reversed), juce::sendNotificationAsync);
			}
			else {
				setValue(getProportionFromValue(val + 3.0 * reversed), juce::sendNotificationAsync);
			}
		}

		/// @brief Draws this component to the screen
		///
		/// @param g - The graphics context to draw with
		auto paint(juce::Graphics& g) noexcept -> void override;

		/// @brief Resizes this component's contents to match its current bounds
		inline auto resized() noexcept -> void override {
			if(mUsesThumbImage) {
				resizeThumb();
			}
		}

	  protected:
		juce::Image mThumbImage;
		juce::ImageComponent mThumbComponent;
		bool mUsesThumbImage = false;
		bool mIsThumbOnly = false;

		size_t mInitialThumbWidth = 0;
		size_t mInitialThumbHeight = 0;

		/// @brief Resizes this's thumb to match its current bounds
		auto resizeThumb() noexcept -> void;

		/// @brief Returns whether the given point is within this component's bounds
		///
		/// @param p - The point in question
		///
		/// @return Whether the point is within this's bounds
		[[nodiscard]] auto isInBounds(juce::Point<int> p) const noexcept -> bool override;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexFader)
	};
} // namespace apex::ui
