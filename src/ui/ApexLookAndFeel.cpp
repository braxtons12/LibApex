#include "ApexLookAndFeel.h"

#include "ApexComboBox.h"
#include "ApexMeter.h"
#include "ApexSlider.h"
#include "ApexToggleButton.h"

namespace apex::ui {
	/// @brief Creates a default `ApexLookAndFeel`
	ApexLookAndFeel::ApexLookAndFeel() noexcept {
		registerColours();
	}

	/// @brief Draws a rotary `juce::Slider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param rotaryStartAngle - The starting angle to draw the slider. The angle corresponding
	/// to 0.0 proportion
	/// @param rotaryEndAngle - The ending angle for the slider. The angle corresponding t0 1.0
	/// proportion
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawRotarySlider(juce::Graphics& g,
										   int x,
										   int y,
										   int width,
										   int height,
										   float sliderPos,
										   const float rotaryStartAngle,
										   const float rotaryEndAngle,
										   juce::Slider& slider) noexcept -> void {
		g.setFont(mFont);
		int size = juce::jmin(width, height);
		float reduction = gsl::narrow_cast<float>(size) * 0.5F * 0.2F;
		juce::Rectangle<int> initialBounds = {x, y, width, height};
		juce::Rectangle<float> bounds
			= juce::Rectangle<int>(x, y, size, size).toFloat().reduced(reduction);
		bounds.setCentre(initialBounds.getCentre().toFloat());
		float radius = bounds.getWidth() * 0.5F;
		float endAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		float lineWidth = radius * 0.2F;
		float troughWidth = lineWidth * 1.03F;
		float arcRadius = radius - lineWidth * 0.5F;
		float troughRadius = radius - lineWidth * 0.5F;
		float thumbWidth = lineWidth;
		float radiusReductionForKnobTrough = thumbWidth;
		float radiusReductionForKnob = radiusReductionForKnobTrough * 1.03F;
		juce::Rectangle<float> knobTroughBounds = bounds.reduced(radiusReductionForKnobTrough);
		juce::Rectangle<float> knobBounds = bounds.reduced(radiusReductionForKnob);
		float indicatorInnerRadius = (knobBounds.getWidth() * 0.8F) * 0.5F;
		float indicatorOuterRadius = arcRadius;

		juce::Path backgroundArc;
		backgroundArc.addCentredArc(bounds.getCentreX(),
									bounds.getCentreY(),
									troughRadius,
									troughRadius,
									0.0F,
									rotaryStartAngle,
									rotaryEndAngle,
									true);
		g.setColour(mSliderTroughColour);
		g.strokePath(backgroundArc,
					 juce::PathStrokeType(troughWidth,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::square));
		juce::Path knobTrough;
		knobTrough.addEllipse(knobTroughBounds);
		g.fillPath(knobTrough);
		juce::Rectangle<float> shadowBounds = knobBounds.translated(-reduction * 0.6F, reduction);
		juce::Path shadow;
		shadow.addEllipse(shadowBounds);
		juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.5F),
											shadowBounds.getTopRight(),
											mSliderShadowColour.withAlpha(0.0F),
											shadowBounds.getBottomLeft(),
											false);
		g.setGradientFill(shadowGradient);
		g.fillPath(shadow);
		juce::ColourGradient knobGradient(mRotarySliderFillColour.darker(0.1F),
										  bounds.getCentre(),
										  mRotarySliderFillColour.darker(0.1F),
										  bounds.getBottomRight(),
										  true);
		knobGradient.addColour(0.5F, mRotarySliderFillColour.darker(0.1F));
		knobGradient.addColour(0.65F, mRotarySliderFillColour.brighter(0.1F));
		knobGradient.addColour(0.75F, mRotarySliderFillColour.darker(0.1F));
		g.setGradientFill(knobGradient);
		juce::Path knob;
		knob.addEllipse(knobBounds);
		g.fillPath(knob);
		juce::ColourGradient knobLight(mRotarySliderFillColour.darker(0.4F).withAlpha(0.2F),
									   bounds.getTopLeft(),
									   mRotarySliderFillColour.darker(0.4F).withAlpha(0.2F),
									   bounds.getBottomRight(),
									   false);
		knobLight.addColour(0.5F, mRotarySliderFillColour.brighter(0.3F).withAlpha(0.6F));
		g.setGradientFill(knobLight);
		g.fillPath(knob);

		juce::Point<float> indicatorStart(
			bounds.getCentreX()
				+ indicatorInnerRadius * Trig<>::cos(endAngle - Constants<>::piOver2),
			bounds.getCentreY()
				+ indicatorInnerRadius * Trig<>::sin(endAngle - Constants<>::piOver2));
		juce::Point<float> indicatorEnd(
			bounds.getCentreX()
				+ indicatorOuterRadius * Trig<>::cos(endAngle - Constants<>::piOver2),
			bounds.getCentreY()
				+ indicatorOuterRadius * Trig<>::sin(endAngle - Constants<>::piOver2));
		juce::ColourGradient indicatorGradient(mRotarySliderIndicatorColour,
											   bounds.getCentre(),
											   mRotarySliderIndicatorColour,
											   bounds.getBottomRight(),
											   true);
		indicatorGradient.addColour(0.45F, mRotarySliderIndicatorColour);
		indicatorGradient.addColour(0.65F, mRotarySliderIndicatorColour.darker(0.3F));
		indicatorGradient.addColour(0.75F, mRotarySliderIndicatorColour);
		juce::Path indicatorPath;
		indicatorPath.startNewSubPath(indicatorStart);
		indicatorPath.lineTo(indicatorEnd);
		indicatorPath.closeSubPath();
		g.setColour(mSliderTroughColour);
		g.strokePath(indicatorPath,
					 juce::PathStrokeType(lineWidth * 1.03F,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::square));
		g.setGradientFill(indicatorGradient);
		g.strokePath(indicatorPath,
					 juce::PathStrokeType(lineWidth,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::square));
		if(slider.isEnabled()) {
			juce::ColourGradient arcGradient(mSliderStrokeColour.brighter(0.5F),
											 bounds.getCentre(),
											 mSliderStrokeColour.brighter(0.5F),
											 bounds.getBottomRight(),
											 true);
			arcGradient.addColour(0.52F, mSliderStrokeColour.darker(0.3F).withAlpha(0.7F));
			juce::Path valueArc;
			valueArc.addCentredArc(bounds.getCentreX(),
								   bounds.getCentreY(),
								   arcRadius,
								   arcRadius,
								   0.0F,
								   rotaryStartAngle,
								   endAngle,
								   true);
			g.setGradientFill(arcGradient);
			g.strokePath(valueArc,
						 juce::PathStrokeType(lineWidth,
											  juce::PathStrokeType::curved,
											  juce::PathStrokeType::square));
		}

		juce::Point<float> thumbStart(
			bounds.getCentreX()
				+ (arcRadius - thumbWidth * 0.5F) * Trig<>::cos(endAngle - Constants<>::piOver2),
			bounds.getCentreY()
				+ (arcRadius - thumbWidth * 0.5F) * Trig<>::sin(endAngle - Constants<>::piOver2));
		juce::Point<float> thumbEnd(
			bounds.getCentreX()
				+ (arcRadius + thumbWidth * 0.5F) * Trig<>::cos(endAngle - Constants<>::piOver2),
			bounds.getCentreY()
				+ (arcRadius + thumbWidth * 0.5F) * Trig<>::sin(endAngle - Constants<>::piOver2));
		juce::Path thumbPath;
		thumbPath.startNewSubPath(thumbStart);
		thumbPath.lineTo(thumbEnd);
		thumbPath.closeSubPath();
		juce::ColourGradient thumbGradient(mSliderGlowColour,
										   bounds.getCentre(),
										   mSliderGlowColour,
										   bounds.getBottomRight(),
										   true);
		thumbGradient.addColour(0.52F, mSliderStrokeColour.darker(0.2F));
		g.setGradientFill(thumbGradient);
		g.strokePath(thumbPath,
					 juce::PathStrokeType(thumbWidth,
										  juce::PathStrokeType::mitered,
										  juce::PathStrokeType::square));
	}

	/// @brief Draws a rotary `ApexSlider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param rotaryStartAngle - The starting angle to draw the slider. The angle corresponding
	/// to 0.0 proportion
	/// @param rotaryEndAngle - The ending angle for the slider. The angle corresponding t0 1.0
	/// proportion
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawRotaryApexSlider(juce::Graphics& g,
											   int x,
											   int y,
											   int width,
											   int height,
											   float sliderPos,
											   const float rotaryStartAngle,
											   const float rotaryEndAngle,
											   ApexSlider& slider) noexcept -> void {
		Option<ApexFilmStrip> maybeStrip = slider.getFilmStrip();
		if(maybeStrip.isSome()) {
			ApexFilmStrip strip = *maybeStrip.getConst();
			auto index = narrow_cast<size_t>(sliderPos * narrow_cast<float>(strip.getNumFrames()));
			juce::Image image = strip.getFrameScaled(index,
													 narrow_cast<size_t>(width),
													 narrow_cast<size_t>(height));
			g.drawImageAt(image, x, y);
		}
		else {
			drawRotarySlider(g,
							 x,
							 y,
							 width,
							 height,
							 sliderPos,
							 rotaryStartAngle,
							 rotaryEndAngle,
							 slider);
		}
	}

	/// @brief Draws the background for a linear `juce::Slider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param minSliderPos - The minimum proportion the slider's value can take
	/// @param maxSliderPos - The maximum proportion the slider's value can take
	/// @param style - the style of the slider
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawLinearSliderBackground(juce::Graphics& g,
													 int x,
													 int y,
													 int width,
													 int height,
													 float sliderPos,
													 float minSliderPos,
													 float maxSliderPos,
													 const juce::Slider::SliderStyle style,
													 juce::Slider& slider) noexcept -> void {
		ignoreUnused(sliderPos, minSliderPos, maxSliderPos, style, slider);

		juce::Rectangle<int> rawBounds = juce::Rectangle<int>(x, y, width, height);

		g.fillAll(mBackgroundColour);

		juce::Rectangle<float> troughBounds = rawBounds.toFloat().reduced(2.0F, 2.0F);
		juce::Rectangle<float> shadowBounds = troughBounds.expanded(1.0F, 1.0F);

		juce::ColourGradient shadowGradient(mSliderShadowColour,
											shadowBounds.getTopRight(),
											mSliderShadowColour.withAlpha(0.0F),
											shadowBounds.getBottomLeft(),
											false);
		g.setColour(mSliderTroughColour);
		g.fillRect(troughBounds);

		g.setGradientFill(shadowGradient);
		g.fillRect(shadowBounds);
	}

	/// @brief Draws the background for a linear `ApexSlider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPosProp - The current value of the slider, as a proportion
	/// @param style - the style of the slider
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawLinearApexSliderBackground(juce::Graphics& g,
														 int x,
														 int y,
														 int width,
														 int height,
														 float sliderPosProp,
														 const juce::Slider::SliderStyle style,
														 ApexSlider& slider) noexcept -> void {
		// technically we don't need this function, but we'll keep it for future use
		// incase we decide we DO want to do something different for our own types
		// vs built-ins
		// also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
		auto sliderMaxPos
			= narrow_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
		auto sliderMinPos = narrow_cast<float>(
			dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x + width : y + height);
		drawLinearSliderBackground(g,
								   x,
								   y,
								   width,
								   height,
								   sliderPosProp,
								   sliderMinPos,
								   sliderMaxPos,
								   style,
								   dynamic_cast<juce::Slider&>(slider));
	}

	/// @brief Draws the thumb (the knob/handle/fader cap/etc) for a linear `juce::Slider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param minSliderPos - The minimum proportion the slider's value can take
	/// @param maxSliderPos - The maximum proportion the slider's value can take
	/// @param style - the style of the slider
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawLinearSliderThumb(juce::Graphics& g,
												int x,
												int y,
												int width,
												int height,
												float sliderPos,
												float minSliderPos,
												float maxSliderPos,
												const juce::Slider::SliderStyle style,
												juce::Slider& slider) noexcept -> void {
		ignoreUnused(minSliderPos, maxSliderPos, style);

		juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
		juce::Rectangle<float> bounds = rawBounds.reduced(1.0F, 1.0F);
		float boundsW = bounds.getWidth();
		float boundsH = bounds.getHeight();
		float boundsX = bounds.getX();
		float boundsY = bounds.getY();

		float thumbWidth
			= slider.isHorizontal() ? juce::jmin(20.0F, boundsW * 0.10F) : boundsW - 4.0F;
		float thumbHeight
			= slider.isHorizontal() ? boundsH - 4.0F : juce::jmin(20.0F, boundsH * 0.10F);
		float sliderPosX
			= slider.isHorizontal() ? sliderPos * boundsW + boundsX : boundsX + boundsW / 2.0F;
		float sliderPosY = slider.isHorizontal() ? boundsY + boundsH / 2.0F :
													 (1.0F - sliderPos) * boundsH + boundsY;
		sliderPosX -= thumbWidth / 2.0F;
		sliderPosY -= thumbHeight / 2.0F;

		float wDiffRight = sliderPosX + thumbWidth - (boundsX + boundsW);
		float hDiffBottom = sliderPosY + thumbHeight - (boundsY + boundsH);
		float wDiffLeft = boundsX - sliderPosX;
		float hDiffTop = boundsY - sliderPosY;
		if(slider.isHorizontal() && wDiffRight >= 0.0F) {
			thumbWidth -= wDiffRight;
		}
		else if(!slider.isHorizontal() && hDiffBottom >= 0.0F) {
			thumbHeight -= hDiffBottom;
		}
		else if(slider.isHorizontal() && wDiffLeft >= 0.0F) {
			sliderPosX += wDiffLeft;
			thumbWidth -= wDiffLeft;
		}
		else if(!slider.isHorizontal() && hDiffTop >= 0.0F) {
			sliderPosY += hDiffTop;
			thumbHeight -= hDiffTop;
		}

		juce::Rectangle<float> thumb
			= juce::Rectangle<float>(sliderPosX, sliderPosY, thumbWidth, thumbHeight);

		juce::ColourGradient strokeGradient(
			mSliderStrokeColour.brighter(0.2F).withAlpha(0.5F),
			sliderPosX,
			sliderPosY,
			mSliderStrokeColour.brighter(0.2F).withAlpha(0.5F),
			sliderPosX + (slider.isHorizontal() ? 0.0F : thumbWidth),
			sliderPosY + (slider.isHorizontal() ? thumbHeight : 0.0F),
			false);
		strokeGradient.addColour(0.5F, mSliderStrokeColour.withAlpha(0.2F));
		juce::ColourGradient glowGradient(mSliderGlowColour.brighter(0.2F).withAlpha(0.5F),
										  sliderPosX,
										  sliderPosY,
										  mSliderGlowColour.brighter(0.2F).withAlpha(0.5F),
										  sliderPosX + (slider.isHorizontal() ? 0.0F : thumbWidth),
										  sliderPosY + (slider.isHorizontal() ? thumbHeight : 0.0F),
										  false);
		glowGradient.addColour(0.5F, mSliderGlowColour.darker(0.2F).withAlpha(0.2F));
		float cornerSize = slider.isHorizontal() ? thumbWidth * 0.1F : thumbHeight * 0.1F;
		g.setGradientFill(strokeGradient);
		g.fillRect(thumb);
		g.setGradientFill(glowGradient);
		g.fillRoundedRectangle(thumb, cornerSize);
	}

	/// @brief Draws the thumb (the knob/handle/fader cap/etc) for a linear `ApexSlider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawLinearApexSliderThumb(juce::Graphics& g,
													int x,
													int y,
													int width,
													int height,
													float sliderPos,
													ApexSlider& slider) noexcept -> void {
		// technically we don't need this function, but we'll keep it for future use
		// incase we decide we DO want to do something different for our own types
		// vs built-ins
		// also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
		auto sliderMaxPos
			= gsl::narrow_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
		auto sliderMinPos = gsl::narrow_cast<float>(
			dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x + width : y + height);
		drawLinearSliderThumb(g,
							  x,
							  y,
							  width,
							  height,
							  sliderPos,
							  sliderMinPos,
							  sliderMaxPos,
							  dynamic_cast<juce::Slider&>(slider).getSliderStyle(),
							  dynamic_cast<juce::Slider&>(slider));
	}

	/// @brief Draws a linear `juce::Slider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param minSliderPos - The minimum proportion the slider's value can take
	/// @param maxSliderPos - The maximum proportion the slider's value can take
	/// @param style - the style of the slider
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawLinearSlider(juce::Graphics& g,
										   int x,
										   int y,
										   int width,
										   int height,
										   float sliderPos,
										   float minSliderPos,
										   float maxSliderPos,
										   const juce::Slider::SliderStyle style,
										   juce::Slider& slider) noexcept -> void {
		drawLinearSliderBackground(g,
								   x,
								   y,
								   width,
								   height,
								   sliderPos,
								   minSliderPos,
								   maxSliderPos,
								   style,
								   slider);

		juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
		juce::Rectangle<float> bounds = rawBounds.reduced(1.0F, 1.0F);
		float boundsW = bounds.getWidth();
		float boundsH = bounds.getHeight();
		float boundsX = bounds.getX();
		float boundsY = bounds.getY();

		float thumbWidth
			= slider.isHorizontal() ? juce::jmin(20.0F, boundsW * 0.10F) : boundsW - 4.0F;
		float thumbHeight
			= slider.isHorizontal() ? boundsH - 4.0F : juce::jmin(20.0F, boundsH * 0.10F);
		float sliderPosX = boundsX + 2.0F;
		float sliderPosY
			= slider.isHorizontal() ? boundsY + 2.0F : (1.0F - sliderPos) * boundsH + boundsY;
		float strokeWidth = slider.isHorizontal() ? sliderPos * boundsW : boundsW - 4.0F;
		float strokeHeight = slider.isHorizontal() ? boundsH - 4.0F : sliderPos * boundsH;

		float wDiffRight = sliderPosX + thumbWidth - (boundsX + boundsW);
		float hDiffBottom = sliderPosY + thumbHeight - (boundsY + boundsH);
		float wDiffLeft = boundsX - sliderPosX;
		float hDiffTop = boundsY - sliderPosY;
		if(slider.isHorizontal() && wDiffRight >= 0.0F) {
			thumbWidth -= wDiffRight;
		}
		else if(!slider.isHorizontal() && hDiffBottom >= 0.0F) {
			thumbHeight -= hDiffBottom;
		}
		else if(slider.isHorizontal() && wDiffLeft >= 0.0F) {
			sliderPosX += wDiffLeft;
			thumbWidth -= wDiffLeft;
		}
		else if(!slider.isHorizontal() && hDiffTop >= 0.0F) {
			sliderPosY += hDiffTop;
			thumbHeight -= hDiffTop;
		}

		sliderPosY -= slider.isHorizontal() ? 0.0F : thumbHeight / 2.0F;
		strokeWidth += slider.isHorizontal() ? -1.0F + thumbWidth / 2.0F : 0.0F;
		strokeHeight += slider.isHorizontal() ? 0.0F : thumbHeight / 2.0F;

		juce::Rectangle<float> stroke
			= juce::Rectangle<float>(sliderPosX, sliderPosY, strokeWidth, strokeHeight);
		juce::ColourGradient strokeGradient(
			mSliderStrokeColour.darker(0.3F).withAlpha(0.7F),
			sliderPosX,
			sliderPosY,
			mSliderStrokeColour.darker(0.3F).withAlpha(0.7F),
			sliderPosX + (slider.isHorizontal() ? 0.0F : strokeWidth),
			sliderPosY + (slider.isHorizontal() ? strokeHeight : 0.0F),
			false);
		strokeGradient.addColour(0.5F, mSliderStrokeColour.darker(0.2F));
		g.setGradientFill(strokeGradient);
		g.fillRect(stroke);

		drawLinearSliderThumb(g,
							  x,
							  y,
							  width,
							  height,
							  sliderPos,
							  minSliderPos,
							  maxSliderPos,
							  style,
							  slider);
	}

	/// @brief Draws a linear `ApexSlider`.
	///
	/// @param g - The graphics context to use for draw
	/// @param x - The x position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param y - The y position, relative to the bounds set for the graphics context. (Usually
	/// this should be 0)
	/// @param width - The width of the slider
	/// @param height - The height of the slider
	/// @param sliderPos - The current value of the slider, as a proportion
	/// @param style - the style of the slider
	/// @param slider - The slider to draw
	auto ApexLookAndFeel::drawLinearApexSlider(juce::Graphics& g,
											   int x,
											   int y,
											   int width,
											   int height,
											   float sliderPos,
											   const juce::Slider::SliderStyle style,
											   ApexSlider& slider) noexcept -> void {
		Option<ApexFilmStrip> maybeFilmStrip = slider.getFilmStrip();
		if(maybeFilmStrip.isSome()) {
			ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
			auto index
				= narrow_cast<size_t>(sliderPos * narrow_cast<float>(filmStrip.getNumFrames()));
			juce::Image image = filmStrip.getFrameScaled(index,
														 narrow_cast<size_t>(width),
														 narrow_cast<size_t>(height));
			g.drawImageAt(image, x, y);
		}
		else {
			auto sliderMaxPos = gsl::narrow_cast<float>(
				dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
			auto sliderMinPos = gsl::narrow_cast<float>(
				dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x + width : y + height);
			drawLinearSlider(g,
							 x,
							 y,
							 width,
							 height,
							 sliderPos,
							 sliderMinPos,
							 sliderMaxPos,
							 style,
							 dynamic_cast<juce::Slider&>(slider));
		}
	}

	/// @brief Draws the background for a popupmenu
	///
	/// @param g - The graphics context to use for drawing
	/// @param width - The width of the menu
	/// @param height - The height of the menu
	auto ApexLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height) noexcept
		-> void {
		juce::Rectangle<float> bounds(0.0F,
									  0.0F,
									  gsl::narrow_cast<float>(width),
									  gsl::narrow_cast<float>(height));
		g.setColour(mPopupMenuBackgroundColour.darker(0.4F).withAlpha(0.9F));
		g.fillAll();
		g.setColour(mComboBoxShadowColour.withAlpha(0.8F));
		g.fillAll();
		g.setColour(mPopupMenuBackgroundColour.withAlpha(0.8F));
		g.fillRect(bounds.reduced(3.0F, 3.0F));
	}

	/// @brief draws a popup menu entry
	///
	/// @param g - The graphics context to use for drawing
	/// @param area - the bounds of the menu entry
	/// @param isSeparator - Whether the entry is a separator
	/// @param isActive - Whether the entry is currently enabled
	/// @param isHighlighted - Whether the entry is currently hovered
	/// @param isTicked - If the entry is a toggle, whether it is currently toggled
	/// @param hasSubMenu - If the entry is a submenu
	/// @param text - The text to draw for the entry
	/// @param shortcutKeyText - The text for the shorcut key
	/// @param icon - The icon for the entry
	/// @param textColour - The color of the text for the entry
	auto ApexLookAndFeel::drawPopupMenuItem(juce::Graphics& g,
											const juce::Rectangle<int>& area,
											bool isSeparator,
											bool isActive,
											bool isHighlighted,
											bool isTicked,
											bool hasSubMenu,
											const juce::String& text,
											const juce::String& shortcutKeyText,
											const juce::Drawable* icon,
											const juce::Colour* textColour) noexcept -> void {
		ignoreUnused(textColour);

		if(isSeparator) {
			juce::Rectangle<int> r = area.reduced(5, 0);
			r.removeFromTop(juce::roundToInt((narrow_cast<float>(r.getHeight()) * 0.5F) - 0.5F));

			g.setColour(mPopupMenuTextColour.withAlpha(0.3F));
			g.fillRect(r.removeFromTop(1));
		}
		else {
			juce::Rectangle<int> r = area.reduced(1);

			if(isHighlighted && isActive) {
				g.setColour(mPopupMenuHighlightColour.withAlpha(0.5F).brighter(0.1F));
				g.fillRect(r);
				g.setColour(mPopupMenuTextColour.brighter(0.2F));
			}
			else {
				g.setColour(mPopupMenuTextColour.withAlpha(isActive ? 1.0F : 0.5F));
			}

			r.reduce(juce::jmin(5, area.getWidth() / 20), 0);

			juce::Font font = narrow_cast<float>(juce::jmin(r.getWidth(), r.getHeight())) * 0.33F;
			float maxFontHeight = narrow_cast<float>(r.getHeight()) * 0.33F;

			g.setFont(font);

			juce::Rectangle<float> iconArea;
			if(icon != nullptr) {
				iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();
				icon->drawWithin(g,
								 iconArea,
								 juce::RectanglePlacement::centred
									 | juce::RectanglePlacement::onlyReduceInSize,
								 1.0F);
				r.removeFromLeft(juce::roundToInt(maxFontHeight * 0.5F));
			}
			else if(isTicked) {
				juce::Colour fill = mPopupMenuHighlightColour.withAlpha(0.5F).brighter(0.1F);
				if(isHighlighted) {
					fill = fill.brighter(0.2F);
				}
				g.setColour(fill);
				g.fillRect(area.reduced(1));
				iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();
				if(isHighlighted && isActive) {
					g.setColour(mPopupMenuTextColour.brighter(0.2F));
				}
				else {
					g.setColour(mPopupMenuTextColour.withAlpha(isActive ? 1.0F : 0.5F));
				}
			}

			if(hasSubMenu) {
				float arrowH = 0.6F * getPopupMenuFont().getAscent();

				auto x = gsl::narrow_cast<float>(
					r.removeFromRight(gsl::narrow_cast<int>(arrowH)).getX());
				auto halfH = gsl::narrow_cast<float>(r.getCentreY());

				juce::Path p;
				p.startNewSubPath(x, halfH - arrowH * 0.5F);
				p.lineTo(x + arrowH * 0.6F, halfH);
				p.lineTo(x, halfH + arrowH * 0.5F);

				g.strokePath(p, juce::PathStrokeType(2.0F));
			}

			r.removeFromRight(3);
			g.drawFittedText(text, r, juce::Justification::centredLeft, 1);

			if(shortcutKeyText.isNotEmpty()) {
				juce::Font font2 = font;
				font2.setHeight(font2.getHeight() * 0.75F);
				font2.setHorizontalScale(0.95F);
				g.setFont(font2);
				g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
			}
		}
	}

	/// @brief Draws a `juce::ComboBox`
	///
	/// @param g - The graphics context to draw with
	/// @param width - The width of the combo box
	/// @param height - The height of the combo boxc
	/// @param isButtonDown - Whether the combo box is currently being clicked
	/// @param buttonX - The x position of the button making up the box
	/// @param buttonY - The y position of the butotn making up the box
	/// @param buttonW - The width of the button making up the box
	/// @param buttonH - The height of the button making up the box
	/// @param box - The combo box to draw
	auto ApexLookAndFeel::drawComboBox(juce::Graphics& g,
									   int width,
									   int height,
									   bool isButtonDown,
									   int buttonX,
									   int buttonY,
									   int buttonW,
									   int buttonH,
									   juce::ComboBox& box) noexcept -> void {
		juce::ignoreUnused(buttonX, buttonY, buttonW, buttonH, isButtonDown);

		float reducedWidth = gsl::narrow_cast<float>(width) * 0.1F;
		float reducedHeight = gsl::narrow_cast<float>(height) * 0.1F;
		float cornerSize = juce::jmin(gsl::narrow_cast<float>(width) - reducedWidth,
									  gsl::narrow_cast<float>(height) - reducedHeight)
						   * 0.1F;
		juce::Rectangle<float> boxBounds = juce::Rectangle<int>(0, 0, width, height).toFloat();
		juce::Rectangle<float> troughBounds = boxBounds.reduced(reducedWidth, reducedHeight);
		troughBounds.setCentre(boxBounds.getCentre());
		juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0F);
		juce::Rectangle<float> shadowBounds
			= troughBounds.translated(-reducedWidth * 0.2F, reducedHeight * 0.4F);
		juce::Rectangle<int> arrowZone(
			narrow_cast<int>(buttonBounds.getX() + buttonBounds.getWidth() * 0.75),
			narrow_cast<int>(buttonBounds.getY()),
			narrow_cast<int>(buttonBounds.getWidth() * 0.2),
			narrow_cast<int>(buttonBounds.getHeight()));

		g.setColour(mComboBoxBackgroundColour);
		g.fillRect(boxBounds);

		g.setColour(mComboBoxTroughColour);
		g.fillRoundedRectangle(troughBounds, cornerSize);

		juce::ColourGradient shadowGradient(mComboBoxShadowColour.withAlpha(0.8F),
											shadowBounds.getTopRight(),
											mComboBoxShadowColour.withAlpha(0.2F),
											shadowBounds.getBottomLeft(),
											false);
		g.setGradientFill(shadowGradient);
		g.fillRoundedRectangle(shadowBounds, cornerSize);

		juce::Colour buttonColour
			= (box.isPopupActive() ?
					 mButtonNormalColour.brighter(0.2F) :
					 (box.isMouseOver() ?
						  mButtonNormalColour.brighter(0.1F) :
						  (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
		juce::ColourGradient buttonGradient(buttonColour,
											buttonBounds.getCentre(),
											buttonColour.darker(0.3F),
											buttonBounds.getBottomRight(),
											true);
		g.setGradientFill(buttonGradient);
		g.fillRoundedRectangle(buttonBounds, cornerSize);
		juce::ColourGradient lightGradient(buttonColour.darker(0.4F).withAlpha(0.2F),
										   boxBounds.getTopLeft(),
										   buttonColour.darker(0.4F).withAlpha(0.2F),
										   boxBounds.getBottomRight(),
										   false);
		lightGradient.addColour(0.5F, buttonColour.brighter(0.3F).withAlpha(0.4F));
		g.setGradientFill(lightGradient);
		g.fillRoundedRectangle(buttonBounds, cornerSize);
		juce::Path p;
		p.startNewSubPath(narrow_cast<float>(arrowZone.getX()) + 3.0F,
						  narrow_cast<float>(arrowZone.getY())
							  + narrow_cast<float>(arrowZone.getHeight()) * 0.45F);
		p.lineTo(narrow_cast<float>(arrowZone.getCentreX()),
				 narrow_cast<float>(arrowZone.getY())
					 + narrow_cast<float>(arrowZone.getHeight()) * 0.575F);
		p.lineTo(narrow_cast<float>(arrowZone.getRight()) - 3.0F,
				 narrow_cast<float>(arrowZone.getY())
					 + narrow_cast<float>(arrowZone.getHeight()) * 0.5F);

		juce::Colour textColour
			= (box.isPopupActive() ?
					 mComboBoxTextColour.brighter(0.2F) :
					 (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1F) :
										  (box.isEnabled() ? mComboBoxTextColour :
															 mComboBoxTextColour.darker(0.3F))));
		g.setColour(textColour);
		g.strokePath(p, juce::PathStrokeType(2.0F));
	}

	/// @brief Draws an `ApexComboBox`
	///
	/// @param g - The graphics context to draw with
	/// @param width - The width of the combo box
	/// @param height - The height of the combo boxc
	/// @param isButtonDown - Whether the combo box is currently being clicked
	/// @param buttonX - The x position of the button making up the box
	/// @param buttonY - The y position of the butotn making up the box
	/// @param buttonW - The width of the button making up the box
	/// @param buttonH - The height of the button making up the box
	/// @param box - The combo box to draw
	auto ApexLookAndFeel::drawApexComboBox(juce::Graphics& g,
										   int width,
										   int height,
										   bool isButtonDown,
										   int buttonX,
										   int buttonY,
										   int buttonW,
										   int buttonH,
										   ApexComboBox& box) noexcept -> void {
		juce::ignoreUnused(isButtonDown, buttonX, buttonY, buttonW, buttonH);

		Option<juce::Image> maybeActiveArrow = box.getActiveArrowImage();
		if(maybeActiveArrow.isSome()) {
			juce::Rectangle<int> arrowZone(narrow_cast<int>(width * 0.75),
										   0,
										   narrow_cast<int>(width * 0.25),
										   height);

			juce::Image activeArrow = *maybeActiveArrow.getConst();
			juce::Image hoveredArrow = *box.getHoveredArrowImage().getConst();
			juce::Image normalArrow = *box.getNormalArrowImage().getConst();

			if(box.isPopupActive()) {
				g.drawImageAt(activeArrow.rescaled(arrowZone.getWidth(),
												   arrowZone.getHeight(),
												   juce::Graphics::highResamplingQuality),
							  arrowZone.getX(),
							  arrowZone.getY());
			}
			else if(box.isHovered()) {
				g.drawImageAt(hoveredArrow.rescaled(arrowZone.getWidth(),
													arrowZone.getHeight(),
													juce::Graphics::highResamplingQuality),
							  arrowZone.getX(),
							  arrowZone.getY());
			}
			else {
				g.drawImageAt(normalArrow.rescaled(arrowZone.getWidth(),
												   arrowZone.getHeight(),
												   juce::Graphics::highResamplingQuality),
							  arrowZone.getX(),
							  arrowZone.getY());
			}
		}
		else {
			drawComboBox(g, width, height, isButtonDown, buttonX, buttonY, buttonW, buttonH, box);
		}
	}

	/// @brief Positions the text of a `juce::ComboBox` within the combobox's bounds
	///
	/// @param box - The combo box to position the text for
	/// @param label - The label making up the text
	auto ApexLookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label) noexcept
		-> void {
		juce::Colour textColour
			= (box.isPopupActive() ?
					 mComboBoxTextColour.brighter(0.2F) :
					 (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1F) :
										  (box.isEnabled() ? mComboBoxTextColour :
															 mComboBoxTextColour.darker(0.3F))));
		label.setColour(juce::Label::textColourId, textColour);
		label.setBounds(0, 0, narrow_cast<int>(box.getWidth() * 0.7), box.getHeight());
		label.setFont(narrow_cast<float>(juce::jmin(box.getWidth(), box.getHeight())) * 0.33F);
	}

	/// @brief Positions the text of an `ApexComboBox` within the combobox's bounds
	///
	/// @param box - The combo box to position the text for
	/// @param label - The label making up the text
	auto ApexLookAndFeel::positionApexComboBoxText(ApexComboBox& box, juce::Label& label) noexcept
		-> void {
		juce::Colour textColour
			= (box.isPopupActive() ?
					 mComboBoxTextColour.brighter(0.2F) :
					 (box.isHovered() ? mComboBoxTextColour.brighter(0.1F) :
										(box.isEnabled() ? mComboBoxTextColour :
														   mComboBoxTextColour.darker(0.3F))));
		label.setColour(juce::Label::textColourId, textColour);
		label.setBounds(0, 0, narrow_cast<int>(box.getWidth() * 0.7), box.getHeight());
		label.setFont(narrow_cast<float>(juce::jmin(box.getWidth(), box.getHeight())) * 0.33F);
	}

	/// @brief Draws the correct text for a `juce::ComboBox` when no entry has been chosen
	///
	/// @param g - The graphics context to draw with
	/// @param box - The combo box to draw for
	/// @param label - The label making up the text
	auto ApexLookAndFeel::drawComboBoxTextWhenNothingSelected(juce::Graphics& g,
															  juce::ComboBox& box,
															  juce::Label& label) noexcept -> void {
		juce::Colour textColour
			= (box.isPopupActive() ?
					 mComboBoxTextColour.brighter(0.2F) :
					 (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1F) :
										  (box.isEnabled() ? mComboBoxTextColour :
															 mComboBoxTextColour.darker(0.3F))));
		g.setColour(textColour);
		juce::Font font = getLabelFont(label);
		g.setFont(font);
		juce::Rectangle<int> textArea
			= getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
		g.drawFittedText(box.getTextWhenNothingSelected(),
						 textArea,
						 label.getJustificationType(),
						 juce::jmax(1,
									narrow_cast<int>(narrow_cast<float>(textArea.getHeight())
													 / font.getHeight())),
						 label.getMinimumHorizontalScale());
	}

	/// @brief Draws the correct text for an `ApexComboBox` when no entry has been chosen
	///
	/// @param g - The graphics context to draw with
	/// @param box - The combo box to draw for
	/// @param label - The label making up the text
	auto
	ApexLookAndFeel::drawApexComboBoxTextWhenNothingSelected(juce::Graphics& g,
															 ApexComboBox& box,
															 juce::Label& label) noexcept -> void {
		juce::Colour textColour
			= (box.isPopupActive() ?
					 mComboBoxTextColour.brighter(0.2F) :
					 (box.isHovered() ? mComboBoxTextColour.brighter(0.1F) :
										(box.isEnabled() ? mComboBoxTextColour :
														   mComboBoxTextColour.darker(0.3F))));
		g.setColour(textColour);
		juce::Font font = getLabelFont(label);
		g.setFont(font);
		juce::Rectangle<int> textArea
			= getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
		g.drawFittedText(box.getTextWhenNothingSelected(),
						 textArea,
						 label.getJustificationType(),
						 juce::jmax(1,
									narrow_cast<int>(narrow_cast<float>(textArea.getHeight())
													 / font.getHeight())),
						 label.getMinimumHorizontalScale());
	}

	/// @brief Draws a `juce::ToggleButton`
	///
	/// @param g - The graphics context to draw with
	/// @param button - The button to draw
	/// @param shouldDrawButtonAsHighlighted - If the button is hovered
	/// @param shouldDrawButtonAsDown - If the button is currently pressed
	auto ApexLookAndFeel::drawToggleButton(juce::Graphics& g,
										   juce::ToggleButton& button,
										   bool shouldDrawButtonAsHighlighted,
										   bool shouldDrawButtonAsDown) noexcept -> void {
		auto initialWidth = gsl::narrow_cast<float>(button.getWidth());
		auto initialHeight = gsl::narrow_cast<float>(button.getHeight());
		float reducedWidth = initialWidth * 0.1F;
		float reducedHeight = initialHeight * 0.1F;
		float cornerSize
			= juce::jmin(initialWidth - reducedWidth, initialHeight - reducedHeight) * 0.1F;
		juce::Rectangle<float> initialBounds(0.0F, 0.0F, initialWidth, initialHeight);
		juce::Rectangle<float> bounds = initialBounds.reduced(reducedWidth, reducedHeight);
		float fontSize = juce::jmin(bounds.getWidth() * 0.33F, bounds.getHeight() * 0.33F);
		juce::Rectangle<float> troughBounds = bounds.reduced(2.0F, 2.0F);
		juce::Rectangle<float> shadowBounds
			= troughBounds.translated(-reducedWidth * 0.4F, reducedHeight * 0.4F);
		juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0F, 2.0F);

		g.setColour(mBackgroundColour);
		g.fillRect(initialBounds);

		g.setColour(mButtonTroughColour);
		g.fillRoundedRectangle(troughBounds, cornerSize);

		juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.8F),
											shadowBounds.getTopRight(),
											mSliderShadowColour.withAlpha(0.2F),
											shadowBounds.getBottomLeft(),
											false);
		g.setGradientFill(shadowGradient);
		g.fillRoundedRectangle(shadowBounds, cornerSize);

		juce::Colour buttonColour
			= ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown) ?
					 mButtonPressedColour.brighter(0.1F) :
					 (shouldDrawButtonAsHighlighted ?
						  mButtonNormalColour.brighter(0.1F) :
						  (shouldDrawButtonAsDown ? mButtonPressedColour : mButtonNormalColour)));
		juce::ColourGradient buttonGradient(buttonColour,
											buttonBounds.getCentre(),
											buttonColour.darker(0.4F),
											buttonBounds.getBottomRight(),
											true);
		g.setGradientFill(buttonGradient);
		g.fillRoundedRectangle(buttonBounds, cornerSize);

		juce::ColourGradient lightGradient(buttonColour.darker(0.4F).withAlpha(0.2F),
										   initialBounds.getTopLeft(),
										   buttonColour.darker(0.4F).withAlpha(0.2F),
										   initialBounds.getBottomRight(),
										   false);
		lightGradient.addColour(0.5F, buttonColour.brighter(0.3F).withAlpha(0.4F));
		g.setGradientFill(lightGradient);
		g.fillRoundedRectangle(buttonBounds, cornerSize);

		juce::Colour textColour
			= ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown) ?
					 mButtonTextColour.brighter(0.05F) :
					 (shouldDrawButtonAsHighlighted ?
						  mButtonTextColour.brighter(0.1F) :
						  (shouldDrawButtonAsDown ? mButtonTextColour.darker(0.1F) :
													mButtonTextColour)));
		juce::ColourGradient textGradient(textColour.withAlpha(0.8F),
										  buttonBounds.getCentreX(),
										  buttonBounds.getCentreY(),
										  textColour.withAlpha(1.0F),
										  buttonBounds.getRight(),
										  buttonBounds.getBottom(),
										  true);
		g.setGradientFill(textGradient);
		g.setFont(fontSize);

		if(!button.isEnabled()) {
			g.setOpacity(0.5F);
		}

		g.drawFittedText(button.getButtonText(),
						 troughBounds.reduced(2.0F, 2.0F).toNearestInt(),
						 juce::Justification::centred,
						 1,
						 1.0F);
	}

	/// @brief Draws an `ApexToggleButton`
	///
	/// @param g - The graphics context to draw with
	/// @param button - The button to draw
	/// @param shouldDrawButtonAsHighlighted - If the button is hovered
	/// @param shouldDrawButtonAsDown - If the button is currently pressed
	auto ApexLookAndFeel::drawApexToggleButton(juce::Graphics& g,
											   ApexToggleButton& button,
											   bool shouldDrawButtonAsHighlighted,
											   bool shouldDrawButtonAsDown) noexcept -> void {
		Option<ApexFilmStrip> maybeFilmStrip = button.getFilmStrip();
		Option<juce::Image> maybeToggledImage = button.getToggledImage();
		if(maybeFilmStrip.isSome()) {
			ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
			if(shouldDrawButtonAsDown) {
				g.drawImageAt(filmStrip.getFrameScaled(0,
													   size_t(button.getWidth()),
													   size_t(button.getHeight())),
							  button.getX(),
							  button.getY());
			}
			else if(shouldDrawButtonAsHighlighted) {
				g.drawImageAt(filmStrip.getFrameScaled(1,
													   size_t(button.getWidth()),
													   size_t(button.getHeight())),
							  button.getX(),
							  button.getY());
			}
			else {
				g.drawImageAt(filmStrip.getFrameScaled(2,
													   size_t(button.getWidth()),
													   size_t(button.getHeight())),
							  button.getX(),
							  button.getY());
			}
		}
		else if(maybeToggledImage.isSome()) {
			juce::Image toggledImage = *maybeToggledImage.getConst();
			juce::Image hoveredImage = *button.getHoveredImage().getConst();
			juce::Image normalImage = *button.getNormalImage().getConst();
			if(shouldDrawButtonAsDown) {
				g.drawImageAt(toggledImage.rescaled(button.getWidth(), button.getHeight()),
							  button.getX(),
							  button.getY());
			}
			else if(shouldDrawButtonAsHighlighted) {
				g.drawImageAt(hoveredImage.rescaled(button.getWidth(), button.getHeight()),
							  button.getX(),
							  button.getY());
			}
			else {
				g.drawImageAt(normalImage.rescaled(button.getWidth(), button.getHeight()),
							  button.getX(),
							  button.getY());
			}
		}
		else {
			drawToggleButton(g,
							 dynamic_cast<juce::ToggleButton&>(button),
							 shouldDrawButtonAsHighlighted,
							 shouldDrawButtonAsDown);
		}
	}

	/// @brief Draws an `ApexMeter`
	///
	/// @param g - The graphics context to draw with
	/// @param levelProportional - The current level of the meter, proportional to its size
	/// @param clipLevelProportional - The level clipping indication should start, proportional
	/// to the meter's size
	/// @param numSteps - The number of equally spaced level ticks in the meter
	/// @param meter - The meter to draw
	auto ApexLookAndFeel::drawApexMeter(juce::Graphics& g,
										float levelProportional,
										float clipLevelProportional,
										size_t numSteps,
										ApexMeter& meter) noexcept -> void {
		int meterX = 0;
		int meterY = 0;
		int meterWidth = meter.getWidth();
		int meterHeight = meter.getHeight();

		Option<ApexFilmStrip> maybeFilmStrip = meter.getFilmStrip();
		Option<juce::Image> maybeMaxedImage = meter.getMaxedImage();
		if(maybeFilmStrip.isSome()) {
			ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
			auto index = narrow_cast<size_t>(narrow_cast<float>(filmStrip.getNumFrames())
											 * levelProportional);
			g.drawImageAt(filmStrip.getFrameScaled(index,
												   narrow_cast<size_t>(meterWidth),
												   narrow_cast<size_t>(meterHeight)),
						  meterX,
						  meterY);
		}
		else if(maybeMaxedImage.isSome()) {
			juce::Image maxedImage = *maybeMaxedImage.getConst();
			int meterPeak = narrow_cast<int>(gsl::narrow_cast<float>(meterY)
											 + (1.0F - levelProportional)
												   * gsl::narrow_cast<float>(meterHeight));
			int meterDiff = meterHeight - meterPeak;
			juce::Rectangle<int> clip(0, meterDiff, meterWidth, meterHeight);
			juce::Image clipped = maxedImage.getClippedImage(clip);
			g.drawImageAt(clipped, meterX, meterY + meterDiff);
		}
		else {
			int meterLevelY = narrow_cast<int>(gsl::narrow_cast<float>(meterY)
											   + (1.0F - levelProportional)
													 * gsl::narrow_cast<float>(meterHeight));

			float stepHeight
				= gsl::narrow_cast<float>(meterHeight) / gsl::narrow_cast<float>(numSteps);

			juce::Rectangle<float> bounds(narrow_cast<float>(meterX),
										  narrow_cast<float>(meterY),
										  narrow_cast<float>(meterWidth),
										  narrow_cast<float>(meterHeight));
			juce::Rectangle<float> troughBounds = bounds.reduced(2.0F, 2.0F);

			juce::Rectangle<float> meterBounds = troughBounds.reduced(2.0F, 2.0F);
			auto meterHeightDiff = gsl::narrow_cast<float>(meterLevelY - (meterY + 2.0));
			float meterBoundsHeight = meterBounds.getHeight();
			meterBounds = meterBounds.withY(narrow_cast<float>(meterLevelY) + 2.0F)
							  .withHeight(narrow_cast<float>(meterBoundsHeight) + 2.0F
										  - narrow_cast<float>(meterHeightDiff));

			float troughX = troughBounds.getX() + 2.0F;
			float troughWidth = troughBounds.getWidth() - 2.0F;

			juce::ColourGradient meterGradient(mMeterClipColour,
											   troughX,
											   narrow_cast<float>(meterY),
											   mMeterLowerColour,
											   troughX,
											   narrow_cast<float>(meterY + meterHeight),
											   false);
			meterGradient.addColour(clipLevelProportional, mMeterUpperColour);

			g.setColour(mBackgroundColour);
			g.fillRect(bounds);

			g.setColour(mMeterTroughColour);
			g.fillRect(troughBounds);

			g.setGradientFill(meterGradient);
			g.fillRect(meterBounds);

			g.setColour(mBackgroundColour.brighter(0.2F).withAlpha(0.3F));
			float stepY = gsl::narrow_cast<float>(meterY) + stepHeight;
			for(size_t i = 1; i < numSteps; ++i) {
				g.drawLine(troughX, stepY, troughX + troughWidth, stepY);
				stepY += stepHeight;
			}
		}
	}

	/// @brief Sets the default font size used by this `ApexLookAndFeel`
	///
	/// @param newFont - The new font size
	auto ApexLookAndFeel::setFont(juce::Font newFont) noexcept -> void {
		mFont = std::move(newFont);
	}

	/// @brief Sets the color indicated by id to the given one
	///
	/// @param id - The id of the color to set
	/// @param colour - The new colour for the given id
	auto ApexLookAndFeel::setColour(ApexColourId id, juce::Colour colour) noexcept -> void {
		switch(id) {
			case ApexColourId::backgroundColourId:
				{
					mBackgroundColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::backgroundColourId),
												 mBackgroundColour);
					juce::LookAndFeel::setColour(juce::AlertWindow::outlineColourId,
												 mBackgroundColour);
				}
				break;
			case ApexColourId::buttonShadowColourId:
				{
					mButtonShadowColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::buttonShadowColourId),
						mButtonShadowColour);
				}
				break;
			case ApexColourId::buttonNormalColourId:
				{
					mButtonNormalColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::buttonNormalColourId),
						mButtonNormalColour);
				}
				break;
			case ApexColourId::buttonPressedColourId:
				{
					mButtonPressedColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::buttonPressedColourId),
						mButtonPressedColour);
				}
				break;
			case ApexColourId::buttonTroughColourId:
				{
					mButtonTroughColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::buttonTroughColourId),
						mButtonTroughColour);
				}
				break;
			case ApexColourId::buttonTextColourId:
				{
					mButtonTextColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::buttonTextColourId),
												 mButtonTextColour);
				}
				break;
			case ApexColourId::comboBoxBackgroundColourId:
				{
					mComboBoxBackgroundColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::comboBoxBackgroundColourId),
						mComboBoxBackgroundColour);
				}
				break;
			case ApexColourId::comboBoxShadowColourId:
				{
					mComboBoxShadowColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::comboBoxShadowColourId),
						mComboBoxShadowColour);
				}
				break;
			case ApexColourId::comboBoxTroughColourId:
				{
					mComboBoxTroughColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::comboBoxTroughColourId),
						mComboBoxTroughColour);
				}
				break;
			case ApexColourId::comboBoxTextColourId:
				{
					mComboBoxTextColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::comboBoxTextColourId),
						mComboBoxTextColour);
				}
				break;
			case ApexColourId::meterClipColourId:
				{
					mMeterClipColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterClipColourId),
												 mMeterClipColour);
				}
				break;
			case ApexColourId::meterLowerColourId:
				{
					mMeterLowerColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterLowerColourId),
												 mMeterLowerColour);
				}
				break;
			case ApexColourId::meterTroughColourId:
				{
					mMeterTroughColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::meterTroughColourId),
						mMeterTroughColour);
				}
				break;
			case ApexColourId::meterUpperColourId:
				{
					mMeterUpperColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterUpperColourId),
												 mMeterUpperColour);
				}
				break;
			case ApexColourId::popupMenuBackgroundColourId:
				{
					mPopupMenuBackgroundColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::popupMenuBackgroundColourId),
						mPopupMenuBackgroundColour);
					juce::LookAndFeel::setColour(juce::PopupMenu::backgroundColourId,
												 mPopupMenuBackgroundColour.withAlpha(0.9F));
					juce::LookAndFeel::setColour(
						juce::PopupMenu::highlightedBackgroundColourId,
						mPopupMenuBackgroundColour.brighter(0.2F).withAlpha(0.9F));
				}
				break;
			case ApexColourId::popupMenuTextColourId:
				{
					mPopupMenuTextColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::popupMenuTextColourId),
						mPopupMenuTextColour);
				}
				break;
			case ApexColourId::popupMenuHighlightColourId:
				{
					mPopupMenuHighlightColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::popupMenuHighlightColourId),
						mPopupMenuHighlightColour);
				}
				break;
			case ApexColourId::rotarySliderFillColourId:
				{
					mRotarySliderFillColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::rotarySliderFillColourId),
						mRotarySliderFillColour);
				}
				break;
			case ApexColourId::rotarySliderIndicatorColourId:
				{
					mRotarySliderIndicatorColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::rotarySliderIndicatorColourId),
						mRotarySliderIndicatorColour);
				}
				break;
			case ApexColourId::sliderStrokeColourId:
				{
					mSliderStrokeColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::sliderStrokeColourId),
						mSliderStrokeColour);
				}
				break;
			case ApexColourId::sliderShadowColourId:
				{
					mSliderShadowColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::sliderShadowColourId),
						mSliderShadowColour);
				}
				break;
			case ApexColourId::sliderTroughColourId:
				{
					mSliderTroughColour = colour;
					juce::LookAndFeel::setColour(
						narrow_cast<int>(ApexColourId::sliderTroughColourId),
						mSliderTroughColour);

					juce::LookAndFeel::setColour(juce::Slider::textBoxOutlineColourId,
												 mSliderTroughColour);
					juce::LookAndFeel::setColour(juce::AlertWindow::backgroundColourId,
												 mSliderTroughColour);
					juce::LookAndFeel::setColour(juce::TextEditor::backgroundColourId,
												 mSliderTroughColour.withAlpha(0.85F));
					juce::LookAndFeel::setColour(
						juce::TextEditor::focusedOutlineColourId,
						mSliderTroughColour.brighter(0.1F).withAlpha(0.85F));
				}
				break;
			case ApexColourId::sliderGlowColourId:
				{
					mSliderGlowColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderGlowColourId),
												 mSliderGlowColour);
				}
				break;
			case ApexColourId::sliderTextColourId:
				{
					mSliderTextColour = colour;
					juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderTextColourId),
												 mSliderTextColour);
					juce::LookAndFeel::setColour(juce::TextEditor::textColourId, mSliderTextColour);
					juce::LookAndFeel::setColour(juce::AlertWindow::textColourId,
												 mSliderTextColour);
				}
				break;
			default: break;
		}
	}

	/// @brief Returns the color corresponding to the given id
	///
	/// @param id - The id to get the color for
	///
	/// @return - The corresponding color
	auto ApexLookAndFeel::getColour(ApexColourId id) const noexcept -> juce::Colour {
		return findColour(narrow_cast<int>(id));
	}

	auto ApexLookAndFeel::registerColours() noexcept -> void {
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::backgroundColourId),
									 mBackgroundColour);

		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::buttonShadowColourId),
									 mButtonShadowColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::buttonNormalColourId),
									 mButtonNormalColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::buttonPressedColourId),
									 mButtonPressedColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::buttonTroughColourId),
									 mButtonTroughColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::buttonTextColourId),
									 mButtonTextColour);

		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::comboBoxBackgroundColourId),
									 mComboBoxBackgroundColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::comboBoxShadowColourId),
									 mComboBoxShadowColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::comboBoxTroughColourId),
									 mComboBoxTroughColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::comboBoxTextColourId),
									 mComboBoxTextColour);

		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterClipColourId),
									 mMeterClipColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterLowerColourId),
									 mMeterLowerColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterTroughColourId),
									 mMeterTroughColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::meterUpperColourId),
									 mMeterUpperColour);

		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::popupMenuBackgroundColourId),
									 mPopupMenuBackgroundColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::popupMenuTextColourId),
									 mPopupMenuTextColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::popupMenuHighlightColourId),
									 mPopupMenuHighlightColour);
		juce::LookAndFeel::setColour(juce::PopupMenu::backgroundColourId,
									 mPopupMenuBackgroundColour.withAlpha(0.9F));
		juce::LookAndFeel::setColour(juce::PopupMenu::highlightedBackgroundColourId,
									 mPopupMenuBackgroundColour.brighter(0.2F).withAlpha(0.9F));

		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::rotarySliderFillColourId),
									 mRotarySliderFillColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::rotarySliderIndicatorColourId),
									 mRotarySliderIndicatorColour);

		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderStrokeColourId),
									 mSliderStrokeColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderShadowColourId),
									 mSliderShadowColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderTroughColourId),
									 mSliderTroughColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderGlowColourId),
									 mSliderGlowColour);
		juce::LookAndFeel::setColour(narrow_cast<int>(ApexColourId::sliderTextColourId),
									 mSliderTextColour);
		juce::LookAndFeel::setColour(juce::Slider::textBoxOutlineColourId, mSliderTroughColour);

		juce::LookAndFeel::setColour(juce::AlertWindow::backgroundColourId, mSliderTroughColour);
		juce::LookAndFeel::setColour(juce::AlertWindow::outlineColourId, mBackgroundColour);
		juce::LookAndFeel::setColour(juce::AlertWindow::textColourId, mSliderTextColour);

		juce::LookAndFeel::setColour(juce::TextEditor::backgroundColourId,
									 mSliderTroughColour.withAlpha(0.85F));
		juce::LookAndFeel::setColour(juce::TextEditor::outlineColourId,
									 mSliderTroughColour.withAlpha(0.85F));
		juce::LookAndFeel::setColour(juce::TextEditor::focusedOutlineColourId,
									 mSliderTroughColour.brighter(0.1F).withAlpha(0.85F));
		juce::LookAndFeel::setColour(juce::TextEditor::textColourId, mSliderTextColour);
	}

	/// @brief Returns the actual rectangular bounds of a rotary slider, given the raw bounds
	///
	/// @param x - The raw x position
	/// @param y - The raw y position
	/// @param width - The raw width
	/// @param height - The raw height
	///
	/// @return - The actualy bounds of the rotary slider's on screen position
	auto ApexLookAndFeel::getActualRotaryBounds(int x, int y, int width, int height) const noexcept
		-> juce::Rectangle<int> {
		int size = juce::jmin(width, height);
		juce::Rectangle<int> bounds = {x, y, size, size};
		auto newBounds
			= bounds.toFloat().reduced(gsl::narrow_cast<float>(size) * 0.5F * 0.2F).toNearestInt();
		newBounds.setCentre(bounds.getCentre());
		return newBounds;
	}

	/// @brief Returns the actual rectangular bounds of a combo box, given the raw bounds
	///
	/// @param x - Thew raw x position
	/// @param y - The raw y position
	/// @param width - The raw width
	/// @param height - The raw height
	///
	/// @return - The actual bounds of the combo box's on screen position
	auto ApexLookAndFeel::getActualComboBounds(int x, int y, int width, int height) const noexcept
		-> juce::Rectangle<int> {
		float reducedWidth = gsl::narrow_cast<float>(width) * 0.1F;
		float reducedHeight = gsl::narrow_cast<float>(height) * 0.1F;
		juce::Rectangle<int> initBounds = {x, y, width, height};
		auto newBounds = initBounds.toFloat().reduced(reducedWidth, reducedHeight).toNearestInt();
		newBounds.setCentre(initBounds.getCentre());
		return newBounds;
	}

	/// @brief Returns the actual rectangular bounds of a button, given the raw bounds
	///
	/// @param x - Thew raw x position
	/// @param y - The raw y position
	/// @param width - The raw width
	/// @param height - The raw height
	///
	/// @return - The actual bounds of the button's on screen position
	auto ApexLookAndFeel::getActualButtonBounds(int x, int y, int width, int height) const noexcept
		-> juce::Rectangle<int> {
		float reducedWidth = gsl::narrow_cast<float>(width) * 0.1F;
		float reducedHeight = gsl::narrow_cast<float>(height) * 0.1F;
		juce::Rectangle<int> initBounds = {x, y, width, height};
		auto newBounds = initBounds.toFloat().reduced(reducedWidth, reducedHeight).toNearestInt();
		newBounds.setCentre(initBounds.getCentre());
		return newBounds;
	}
} // namespace apex::ui
