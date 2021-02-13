#include "ApexFader.h"

namespace apex::ui {
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
	ApexFader::ApexFader(juce::Slider::SliderStyle style,
						 std::function<double(double)> proportionToValueFunc,
						 std::function<double(double)> valueToProportionFunc,
						 ApexFilmStrip filmStrip) noexcept
		: ApexSlider(style,
					 std::move(proportionToValueFunc),
					 std::move(valueToProportionFunc),
					 std::move(filmStrip)) {
	}

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
	ApexFader::ApexFader(juce::Slider::SliderStyle style,
						 std::function<double(double)> proportionToValueFunc,
						 std::function<double(double)> valueToProportionFunc,
						 juce::Image thumbImage) noexcept
		: ApexSlider(style, std::move(proportionToValueFunc), std::move(valueToProportionFunc)),
		  mThumbImage(std::move(thumbImage)), mUsesThumbImage(true), mIsThumbOnly(true),
		  mInitialThumbWidth(static_cast<size_t>(mThumbImage.getWidth())),
		  mInitialThumbHeight(static_cast<size_t>(mThumbImage.getHeight())) {
		mThumbComponent.setImage(mThumbImage);
		mThumbComponent.setImagePlacement(
			juce::RectanglePlacement(narrow_cast<size_t>(juce::RectanglePlacement::xMid)
									 | narrow_cast<size_t>(juce::RectanglePlacement::yTop)));
		addAndMakeVisible(mThumbComponent);
	}

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
	ApexFader::ApexFader(juce::Slider::SliderStyle style,
						 std::function<double(double)> proportionToValueFunc,
						 std::function<double(double)> valueToProportionFunc,
						 bool isThumbOnlySlider,
						 size_t initialThumbWidth,
						 size_t initialThumbHeight) noexcept
		: ApexSlider(style, std::move(proportionToValueFunc), std::move(valueToProportionFunc)),
		  mIsThumbOnly(isThumbOnlySlider), mInitialThumbWidth(initialThumbWidth),
		  mInitialThumbHeight(initialThumbHeight) {
	}

	/// @brief Draws this component to the screen
	///
	/// @param g - The graphics context to draw with
	auto ApexFader::paint(juce::Graphics& g) noexcept -> void {
		double sliderPos = getProportionFromValue(getValue());
		jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

		auto style = getSliderStyle();

		if(mUsesThumbImage) {
			resizeThumb();
		}
		else if(mIsThumbOnly && mLookAndFeel != nullptr) {
			mLookAndFeel->drawLinearApexSliderThumb(g,
													getX(),
													getY(),
													getWidth(),
													getHeight(),
													static_cast<float>(sliderPos),
													*this);
		}
		else if(mLookAndFeel != nullptr) {
			mLookAndFeel->drawLinearApexSlider(g,
											   getX(),
											   getY(),
											   getWidth(),
											   getHeight(),
											   static_cast<float>(sliderPos),
											   style,
											   *this);
		}
	}

	/// @brief Resizes this's thumb to match its current bounds
	auto ApexFader::resizeThumb() noexcept -> void {
		double sliderPos = getProportionFromValue(getValue());
		jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

		size_t thumbWidth
			= General<>::roundU(narrow_cast<float>(mInitialThumbWidth) * mXScaleFactor);
		size_t thumbHeight
			= General<>::roundU(narrow_cast<float>(mInitialThumbHeight) * mYScaleFactor);

		if(isHorizontal()) {
			int thumbX = static_cast<int>(sliderPos * getWidth()
										  - (narrow_cast<float>(thumbWidth) / 2.0F));
			int thumbY = static_cast<int>(narrow_cast<float>(getHeight()) * 0.5F
										  - (narrow_cast<float>(thumbHeight) / 2.0F));
			juce::Rectangle<int> bounds
				= {thumbX, thumbY, narrow_cast<int>(thumbWidth), narrow_cast<int>(thumbHeight)};
			mThumbComponent.setBounds(bounds);
		}
		else {
			int thumbX = static_cast<int>(narrow_cast<float>(getWidth()) * 0.5F
										  - (narrow_cast<float>(thumbWidth) / 2.0F));
			int thumbY = static_cast<int>(sliderPos * getHeight()
										  - (narrow_cast<float>(thumbHeight) / 2.0F));
			juce::Rectangle<int> bounds
				= {thumbX, thumbY, narrow_cast<int>(thumbWidth), narrow_cast<int>(thumbHeight)};
			mThumbComponent.setBounds(bounds);
		}
	}

	/// @brief Returns whether the given point is within this component's bounds
	///
	/// @param p - The point in question
	///
	/// @return Whether the point is within this's bounds
	auto ApexFader::isInBounds(juce::Point<int> p) const noexcept -> bool {
		auto x = static_cast<size_t>(p.x);
		auto y = static_cast<size_t>(p.y);

		auto localX = static_cast<size_t>(getX());
		auto localY = static_cast<size_t>(getY());
		auto width = static_cast<size_t>(getWidth());
		auto height = static_cast<size_t>(getHeight());
		x += localX;
		y += localY;

		if(mIsThumbOnly) {
			double sliderPos = getProportionFromValue(getValue());
			jassert(sliderPos >= 0.00 && sliderPos <= 1.00);

			size_t thumbWidth
				= General<>::roundU(narrow_cast<float>(mInitialThumbWidth) * mXScaleFactor);
			size_t thumbHeight
				= General<>::roundU(narrow_cast<float>(mInitialThumbHeight) * mYScaleFactor);
			size_t thumbX = 0;
			size_t thumbY = 0;
			if(isHorizontal()) {
				thumbX = static_cast<size_t>(sliderPos * narrow_cast<float>(width)
											 - (narrow_cast<float>(thumbWidth) / 2.0F));
				thumbY = static_cast<size_t>(narrow_cast<float>(height) * 0.5F
											 - (narrow_cast<float>(thumbHeight) / 2.0F));
			}
			else {
				thumbX = static_cast<size_t>(narrow_cast<float>(width) * 0.5F
											 - (narrow_cast<float>(thumbWidth) / 2.0F));
				thumbY = static_cast<size_t>(sliderPos * narrow_cast<float>(height)
											 - (narrow_cast<float>(thumbHeight) / 2.0F));
			}
			return (x >= thumbX && x <= thumbX + thumbWidth && y >= thumbY
					&& y <= thumbY + thumbWidth);
		}
		else {
			return (x >= localX && x <= localY + width && y >= localY && y <= localY + height);
		}
	}
} // namespace apex::ui
