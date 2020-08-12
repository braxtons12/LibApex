#include "ApexLookAndFeel.h"

#include "ApexComboBox.h"
#include "ApexMeter.h"
#include "ApexSlider.h"
#include "ApexToggleButton.h"

namespace apex::ui {
	/// @brief Creates a default `ApexLookAndFeel`
	ApexLookAndFeel::ApexLookAndFeel() noexcept : juce::LookAndFeel_V4() {
		registerColours();
	}

	/// @brief Draws a rotary `juce::Slider`. If `USE_PHYSICAL_ROTARIES` is defined, it will
	/// draw a more physical-style knob. If `USE_2D_SEMICIRCULAR_ROTARIES` is defined, it will
	/// draw a traditional juce-style rotary slider with a circular knob in the center and
	/// elliptical slider around it. Otherwise, will draw an elliptical-fill based slider which
	/// fills up as the value increases
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
#if defined USE_PHYSICAL_ROTARIES
		drawPhysicalRotary(g,
						   x,
						   y,
						   width,
						   height,
						   sliderPos,
						   rotaryStartAngle,
						   rotaryEndAngle,
						   slider);
#elif defined USE_2D_SEMICIRCULAR_ROTARIES
		drawSemiCircularRotary(g,
							   x,
							   y,
							   width,
							   height,
							   sliderPos,
							   rotaryStartAngle,
							   rotaryEndAngle,
							   slider);
#else
		drawCircularFillRotary(g,
							   x,
							   y,
							   width,
							   height,
							   sliderPos,
							   rotaryStartAngle,
							   rotaryEndAngle,
							   slider);
#endif
	}

	/// @brief Draws a rotary `ApexSlider`. If `USE_PHYSICAL_ROTARIES` is defined, it will draw
	/// a more physical-style knob. If `USE_2D_SEMICIRCULAR_ROTARIES` is defined, it will draw a
	/// traditional juce-style rotary slider with a circular knob in the center and elliptical
	/// slider around it. Otherwise, will draw an elliptical-fill based slider which fills up as
	/// the value increases
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
			auto index = static_cast<size_t>(sliderPos * strip.getNumFrames());
			juce::Image image = strip.getFrameScaled(index,
													 static_cast<size_t>(width),
													 static_cast<size_t>(height));
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

		juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.8F),
											shadowBounds.getCentreX(),
											shadowBounds.getCentreY(),
											mSliderShadowColour.withAlpha(0.2F),
											shadowBounds.getRight(),
											shadowBounds.getBottom(),
											true);
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
			= static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
		auto sliderMinPos = static_cast<float>(
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
			mSliderStrokeColour.withAlpha(0.5F),
			sliderPosX,
			sliderPosY,
			mSliderStrokeColour.withAlpha(0.5F),
			sliderPosX + (slider.isHorizontal() ? 0.0F : strokeWidth),
			sliderPosY + (slider.isHorizontal() ? strokeHeight : 0.0F),
			false);
		strokeGradient.addColour(0.5F, mSliderStrokeColour.withAlpha(0.2F));
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
			auto index = static_cast<size_t>(sliderPos * filmStrip.getNumFrames());
			juce::Image image = filmStrip.getFrameScaled(index,
														 static_cast<size_t>(width),
														 static_cast<size_t>(height));
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
									  static_cast<float>(width),
									  static_cast<float>(height));
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
			r.removeFromTop(juce::roundToInt((r.getHeight() * 0.5F) - 0.5F));

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

			juce::Font font = juce::jmin(r.getWidth(), r.getHeight()) * 0.33F;
			float maxFontHeight = r.getHeight() * 0.33F;

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

		float cornerSize = juce::jmin(width, height) * 0.1F;
		juce::Rectangle<float> boxBounds = juce::Rectangle<int>(0, 0, width, height).toFloat();
		juce::Rectangle<float> troughBounds = boxBounds.reduced(2.0F, 2.0F);
		juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0F, 2.0F);
		juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0F, 2.0F);
		juce::Rectangle<int> arrowZone(static_cast<int>(width * 0.75),
									   0,
									   static_cast<int>(width * 0.2),
									   height);

		g.setColour(mComboBoxBackgroundColour);
		g.fillRect(boxBounds);

		g.setColour(mComboBoxTroughColour);
		g.fillRoundedRectangle(troughBounds, cornerSize);

		juce::ColourGradient shadowGradient(mComboBoxShadowColour.withAlpha(0.8F),
											shadowBounds.getCentre(),
											mComboBoxShadowColour.withAlpha(0.2F),
											shadowBounds.getBottomRight(),
											true);
		g.setGradientFill(shadowGradient);
		g.fillRoundedRectangle(shadowBounds, cornerSize);

		juce::Colour buttonColour
			= (box.isPopupActive() ?
					 mButtonNormalColour.brighter(0.2F) :
					 (box.isMouseOver() ?
						  mButtonNormalColour.brighter(0.1F) :
						  (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
		juce::ColourGradient buttonGradient(buttonColour.withAlpha(0.4F),
											buttonBounds.getCentre(),
											buttonColour.withAlpha(0.1F),
											buttonBounds.getBottomRight(),
											true);
		g.setGradientFill(buttonGradient);
		g.fillRoundedRectangle(buttonBounds, cornerSize);

		juce::Path p;
		p.startNewSubPath(arrowZone.getX() + 3.0F,
						  arrowZone.getY() + arrowZone.getHeight() * 0.45F);
		p.lineTo(static_cast<float>(arrowZone.getCentreX()),
				 arrowZone.getY() + arrowZone.getHeight() * 0.575F);
		p.lineTo(arrowZone.getRight() - 3.0F, arrowZone.getY() + arrowZone.getHeight() * 0.45F);

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
			juce::Rectangle<int> arrowZone(static_cast<int>(width * 0.75),
										   0,
										   static_cast<int>(width * 0.25),
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

			float cornerSize = juce::jmin(width, height) * 0.1F;
			juce::Rectangle<float> boxBounds = juce::Rectangle<int>(0, 0, width, height).toFloat();
			juce::Rectangle<float> troughBounds = boxBounds.reduced(2.0F, 2.0F);
			juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0F, 2.0F);
			juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0F, 2.0F);
			juce::Rectangle<int> arrowZone(static_cast<int>(width * 0.75),
										   0,
										   static_cast<int>(width * 0.2),
										   height);

			g.setColour(mComboBoxBackgroundColour);
			g.fillRect(boxBounds);

			g.setColour(mComboBoxTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			juce::ColourGradient shadowGradient(mComboBoxShadowColour.withAlpha(0.8F),
												shadowBounds.getCentre(),
												mComboBoxShadowColour.withAlpha(0.2F),
												shadowBounds.getBottomRight(),
												true);
			g.setGradientFill(shadowGradient);
			g.fillRoundedRectangle(shadowBounds, cornerSize);

			juce::Colour buttonColour
				= (box.isPopupActive() ?
						 mButtonNormalColour.brighter(0.2F) :
						 (box.isHovered() ?
							  mButtonNormalColour.brighter(0.1F) :
							  (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
			juce::ColourGradient buttonGradient(buttonColour.withAlpha(0.4F),
												buttonBounds.getCentre(),
												buttonColour.withAlpha(0.1F),
												buttonBounds.getBottomRight(),
												true);
			g.setGradientFill(buttonGradient);
			g.fillRoundedRectangle(buttonBounds, cornerSize);

			juce::Path p;
			p.startNewSubPath(arrowZone.getX() + 3.0F,
							  arrowZone.getY() + arrowZone.getHeight() * 0.45F);
			p.lineTo(static_cast<float>(arrowZone.getCentreX()),
					 arrowZone.getY() + arrowZone.getHeight() * 0.575F);
			p.lineTo(arrowZone.getRight() - 3.0F, arrowZone.getY() + arrowZone.getHeight() * 0.45F);

			juce::Colour textColour
				= (box.isPopupActive() ?
						 mComboBoxTextColour.brighter(0.2F) :
						 (box.isHovered() ? mComboBoxTextColour.brighter(0.1F) :
											(box.isEnabled() ? mComboBoxTextColour :
															   mComboBoxTextColour.darker(0.3F))));
			g.setColour(textColour);
			g.strokePath(p, juce::PathStrokeType(2.0F));
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
		label.setBounds(0, 0, static_cast<int>(box.getWidth() * 0.7), box.getHeight());
		label.setFont(juce::jmin(box.getWidth(), box.getHeight()) * 0.33F);
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
		label.setBounds(0, 0, static_cast<int>(box.getWidth() * 0.7), box.getHeight());
		label.setFont(juce::jmin(box.getWidth(), box.getHeight()) * 0.33F);
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
						 juce::jmax(1, static_cast<int>(textArea.getHeight() / font.getHeight())),
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
						 juce::jmax(1, static_cast<int>(textArea.getHeight() / font.getHeight())),
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
		auto cornerSize = gsl::narrow_cast<float>(
			juce::jmax(5.0, button.getWidth() * 0.1, button.getHeight() * 0.1));
		auto width = gsl::narrow_cast<float>(button.getWidth());
		auto height = gsl::narrow_cast<float>(button.getHeight());
		float fontSize = juce::jmin(width * 0.33F, height * 0.33F);
		juce::Rectangle<float> bounds(0.0F, 0.0F, width, height);
		juce::Rectangle<float> troughBounds = bounds.reduced(2.0F, 2.0F);
		juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0F, 2.0F);
		juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0F, 2.0F);

		g.setColour(mBackgroundColour);
		g.fillRect(bounds);

		g.setColour(mButtonTroughColour);
		g.fillRoundedRectangle(troughBounds, cornerSize);

		juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.8F),
											shadowBounds.getCentreX(),
											shadowBounds.getCentreY(),
											mSliderShadowColour.withAlpha(0.2F),
											shadowBounds.getRight(),
											shadowBounds.getBottom(),
											true);
		g.setGradientFill(shadowGradient);
		g.fillRoundedRectangle(shadowBounds, cornerSize);

		juce::Colour buttonColour
			= ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown) ?
					 mButtonPressedColour.brighter(0.1F) :
					 (shouldDrawButtonAsHighlighted ?
						  mButtonNormalColour.brighter(0.1F) :
						  (shouldDrawButtonAsDown ? mButtonPressedColour : mButtonNormalColour)));
		juce::ColourGradient buttonGradient(buttonColour.withAlpha(0.4F),
											buttonBounds.getCentreX(),
											buttonBounds.getCentreY(),
											buttonColour.withAlpha(0.1F),
											buttonBounds.getRight(),
											buttonBounds.getBottom(),
											true);
		g.setGradientFill(buttonGradient);
		g.fillRoundedRectangle(buttonBounds, cornerSize);

		g.setColour(mButtonTextColour);

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
			auto index = static_cast<size_t>(filmStrip.getNumFrames() * levelProportional);
			g.drawImageAt(filmStrip.getFrameScaled(index,
												   static_cast<size_t>(meterWidth),
												   static_cast<size_t>(meterHeight)),
						  meterX,
						  meterY);
		}
		else if(maybeMaxedImage.isSome()) {
			juce::Image maxedImage = *maybeMaxedImage.getConst();
			int meterPeak = static_cast<int>(gsl::narrow_cast<float>(meterY)
											 + (1.0F - levelProportional)
												   * gsl::narrow_cast<float>(meterHeight));
			int meterDiff = meterHeight - meterPeak;
			juce::Rectangle<int> clip(0, meterDiff, meterWidth, meterHeight);
			juce::Image clipped = maxedImage.getClippedImage(clip);
			g.drawImageAt(clipped, meterX, meterY + meterDiff);
		}
		else {
			int meterLevelY = static_cast<int>(gsl::narrow_cast<float>(meterY)
											   + (1.0F - levelProportional)
													 * gsl::narrow_cast<float>(meterHeight));

			float stepHeight
				= gsl::narrow_cast<float>(meterHeight) / gsl::narrow_cast<float>(numSteps);

			juce::Rectangle<float> bounds(static_cast<float>(meterX),
										  static_cast<float>(meterY),
										  static_cast<float>(meterWidth),
										  static_cast<float>(meterHeight));
			juce::Rectangle<float> troughBounds = bounds.reduced(2.0F, 2.0F);

			juce::Rectangle<float> meterBounds = troughBounds.reduced(2.0F, 2.0F);
			auto meterHeightDiff = gsl::narrow_cast<float>(meterLevelY - (meterY + 2.0));
			float meterBoundsHeight = meterBounds.getHeight();
			meterBounds = meterBounds.withY(static_cast<float>(meterLevelY) + 2.0F)
							  .withHeight(static_cast<float>(meterBoundsHeight) + 2.0F
										  - static_cast<float>(meterHeightDiff));

			float troughX = troughBounds.getX() + 2.0F;
			float troughWidth = troughBounds.getWidth() - 2.0F;

			juce::ColourGradient meterGradient(mMeterClipColour,
											   troughX,
											   static_cast<float>(meterY),
											   mMeterLowerColour,
											   troughX,
											   static_cast<float>(meterY + meterHeight),
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
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::backgroundColourId),
												 mBackgroundColour);
					juce::LookAndFeel::setColour(juce::AlertWindow::outlineColourId,
												 mBackgroundColour);
				}
				break;
			case ApexColourId::buttonShadowColourId:
				{
					mButtonShadowColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::buttonShadowColourId),
						mButtonShadowColour);
				}
				break;
			case ApexColourId::buttonNormalColourId:
				{
					mButtonNormalColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::buttonNormalColourId),
						mButtonNormalColour);
				}
				break;
			case ApexColourId::buttonPressedColourId:
				{
					mButtonPressedColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::buttonPressedColourId),
						mButtonPressedColour);
				}
				break;
			case ApexColourId::buttonTroughColourId:
				{
					mButtonTroughColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::buttonTroughColourId),
						mButtonTroughColour);
				}
				break;
			case ApexColourId::buttonTextColourId:
				{
					mButtonTextColour = colour;
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::buttonTextColourId),
												 mButtonTextColour);
				}
				break;
			case ApexColourId::comboBoxBackgroundColourId:
				{
					mComboBoxBackgroundColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::comboBoxBackgroundColourId),
						mComboBoxBackgroundColour);
				}
				break;
			case ApexColourId::comboBoxShadowColourId:
				{
					mComboBoxShadowColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::comboBoxShadowColourId),
						mComboBoxShadowColour);
				}
				break;
			case ApexColourId::comboBoxTroughColourId:
				{
					mComboBoxTroughColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::comboBoxTroughColourId),
						mComboBoxTroughColour);
				}
				break;
			case ApexColourId::comboBoxTextColourId:
				{
					mComboBoxTextColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::comboBoxTextColourId),
						mComboBoxTextColour);
				}
				break;
			case ApexColourId::meterClipColourId:
				{
					mMeterClipColour = colour;
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterClipColourId),
												 mMeterClipColour);
				}
				break;
			case ApexColourId::meterLowerColourId:
				{
					mMeterLowerColour = colour;
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterLowerColourId),
												 mMeterLowerColour);
				}
				break;
			case ApexColourId::meterTroughColourId:
				{
					mMeterTroughColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::meterTroughColourId),
						mMeterTroughColour);
				}
				break;
			case ApexColourId::meterUpperColourId:
				{
					mMeterUpperColour = colour;
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterUpperColourId),
												 mMeterUpperColour);
				}
				break;
			case ApexColourId::popupMenuBackgroundColourId:
				{
					mPopupMenuBackgroundColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::popupMenuBackgroundColourId),
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
						static_cast<int>(ApexColourId::popupMenuTextColourId),
						mPopupMenuTextColour);
				}
				break;
			case ApexColourId::popupMenuHighlightColourId:
				{
					mPopupMenuHighlightColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::popupMenuHighlightColourId),
						mPopupMenuHighlightColour);
				}
				break;
			case ApexColourId::rotarySliderFillColourId:
				{
					mRotarySliderFillColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::rotarySliderFillColourId),
						mRotarySliderFillColour);
				}
				break;
			case ApexColourId::rotarySliderIndicatorColourId:
				{
					mRotarySliderIndicatorColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::rotarySliderIndicatorColourId),
						mRotarySliderIndicatorColour);
				}
				break;
			case ApexColourId::sliderStrokeColourId:
				{
					mSliderStrokeColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::sliderStrokeColourId),
						mSliderStrokeColour);
				}
				break;
			case ApexColourId::sliderShadowColourId:
				{
					mSliderShadowColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::sliderShadowColourId),
						mSliderShadowColour);
				}
				break;
			case ApexColourId::sliderTroughColourId:
				{
					mSliderTroughColour = colour;
					juce::LookAndFeel::setColour(
						static_cast<int>(ApexColourId::sliderTroughColourId),
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
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderGlowColourId),
												 mSliderGlowColour);
				}
				break;
			case ApexColourId::sliderTextColourId:
				{
					mSliderTextColour = colour;
					juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderTextColourId),
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
		return findColour(static_cast<int>(id));
	}

	auto ApexLookAndFeel::registerColours() noexcept -> void {
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::backgroundColourId),
									 mBackgroundColour);

		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::buttonShadowColourId),
									 mButtonShadowColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::buttonNormalColourId),
									 mButtonNormalColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::buttonPressedColourId),
									 mButtonPressedColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::buttonTroughColourId),
									 mButtonTroughColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::buttonTextColourId),
									 mButtonTextColour);

		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::comboBoxBackgroundColourId),
									 mComboBoxBackgroundColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::comboBoxShadowColourId),
									 mComboBoxShadowColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::comboBoxTroughColourId),
									 mComboBoxTroughColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::comboBoxTextColourId),
									 mComboBoxTextColour);

		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterClipColourId),
									 mMeterClipColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterLowerColourId),
									 mMeterLowerColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterTroughColourId),
									 mMeterTroughColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::meterUpperColourId),
									 mMeterUpperColour);

		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::popupMenuBackgroundColourId),
									 mPopupMenuBackgroundColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::popupMenuTextColourId),
									 mPopupMenuTextColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::popupMenuHighlightColourId),
									 mPopupMenuHighlightColour);
		juce::LookAndFeel::setColour(juce::PopupMenu::backgroundColourId,
									 mPopupMenuBackgroundColour.withAlpha(0.9F));
		juce::LookAndFeel::setColour(juce::PopupMenu::highlightedBackgroundColourId,
									 mPopupMenuBackgroundColour.brighter(0.2F).withAlpha(0.9F));

		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::rotarySliderFillColourId),
									 mRotarySliderFillColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::rotarySliderIndicatorColourId),
									 mRotarySliderIndicatorColour);

		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderStrokeColourId),
									 mSliderStrokeColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderShadowColourId),
									 mSliderShadowColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderTroughColourId),
									 mSliderTroughColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderGlowColourId),
									 mSliderGlowColour);
		juce::LookAndFeel::setColour(static_cast<int>(ApexColourId::sliderTextColourId),
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

	/// @brief Returns the actual rectangulary bounds of a rotary slider, given the raw bounds
	///
	/// @param x - The raw x position
	/// @param y - The raw y position
	/// @param width - The raw width
	/// @param height - The raw height
	///
	/// @return - The actualy bounds of the rotary slider's on screen position
	auto ApexLookAndFeel::getActualRotaryBounds(int x, int y, int width, int height) const noexcept
		-> juce::Rectangle<int> {
#if defined USE_PHYSICAL_ROTARIES
		auto diameter = static_cast<double>(juce::jmin(width, height));
		juce::Rectangle<double> bounds(x + (width - diameter) / 2.0,
									   y + (height - diameter) / 2.0,
									   diameter,
									   diameter);
		return bounds.toNearestInt();
#elif defined USE_2D_SEMICIRCULAR_ROTARIES
		auto diameter = static_cast<double>(juce::jmin(width, height));
		juce::Rectangle<double> bounds(x + (width - diameter) / 2.0,
									   y + (height - diameter) / 2.0,
									   diameter,
									   diameter / 2.0);
		return bounds.toNearestInt();
#else
		auto diameter = static_cast<double>(juce::jmin(width, height));
		juce::Rectangle<double> bounds(x + (width - diameter) / 2.0,
									   y + (height - diameter) / 2.0,
									   diameter,
									   diameter);
		return bounds.toNearestInt();
#endif
	}

	auto ApexLookAndFeel::drawPhysicalRotary(juce::Graphics& g,
											 int x,
											 int y,
											 int width,
											 int height,
											 float sliderPos,
											 const float rotaryStartAngle,
											 const float rotaryEndAngle,
											 juce::Slider& slider) noexcept -> void {
		juce::ignoreUnused(slider);

		juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();

		g.setColour(mBackgroundColour);
		g.fillRect(rawBounds);

		float diameter = juce::jmin(rawBounds.getWidth(), rawBounds.getHeight());

		juce::Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
												  .withHeight(diameter)
												  .reduced(2.0F, 2.0F)
												  .withCentre(rawBounds.getCentre());
		juce::Rectangle<float> circleBounds = troughBounds.reduced(2.0F, 2.0F);
		juce::Rectangle<float> innerShadowBounds = troughBounds.expanded(2.0F, 2.0F);
		juce::Rectangle<float> outerShadowBounds
			= juce::Rectangle<float>(innerShadowBounds.getX() - 3.0F,
									 innerShadowBounds.getY(),
									 innerShadowBounds.getWidth() + 3.0F,
									 innerShadowBounds.getHeight() + 8.0F);

		float lineWidth = diameter * 0.02F;

		juce::ColourGradient innerShadowGradient(
			mSliderShadowColour.withAlpha(0.8F),
			innerShadowBounds.getX() + innerShadowBounds.getWidth() / 2.0F,
			innerShadowBounds.getY() + innerShadowBounds.getHeight() / 2.0F,
			mSliderShadowColour.withAlpha(0.2F),
			innerShadowBounds.getX() + innerShadowBounds.getWidth(),
			innerShadowBounds.getY() + innerShadowBounds.getHeight(),
			true);
		juce::ColourGradient outerShadowGradient(
			mSliderShadowColour.withAlpha(0.8F),
			outerShadowBounds.getX() + outerShadowBounds.getWidth() / 2.0F,
			outerShadowBounds.getY() + outerShadowBounds.getHeight() / 2.0F,
			mSliderShadowColour.darker(0.2F).withAlpha(0.0F),
			outerShadowBounds.getX(),
			outerShadowBounds.getY() + outerShadowBounds.getHeight(),
			true);
		juce::ColourGradient circleGradient(mRotarySliderFillColour.brighter(0.1F),
											circleBounds.getX() + circleBounds.getWidth() / 2,
											circleBounds.getY() + circleBounds.getHeight() / 2,
											mRotarySliderFillColour.darker(0.3F),
											circleBounds.getX() + circleBounds.getWidth(),
											circleBounds.getY() + circleBounds.getHeight(),
											true);
		juce::ColourGradient circleHighlight(mRotarySliderFillColour.darker(0.2F).withAlpha(0.3F),
											 circleBounds.getX(),
											 circleBounds.getY(),
											 mRotarySliderFillColour.darker(0.2F).withAlpha(0.3F),
											 circleBounds.getX() + circleBounds.getWidth(),
											 circleBounds.getY() + circleBounds.getHeight(),
											 false);
		circleHighlight.addColour(0.5F, mRotarySliderFillColour.brighter(0.1F).withAlpha(0.5F));

		g.setColour(mSliderTroughColour);
		g.fillEllipse(troughBounds);

		g.setGradientFill(innerShadowGradient);
		g.fillEllipse(innerShadowBounds);

		g.setGradientFill(outerShadowGradient);
		g.fillEllipse(outerShadowBounds);

		g.setGradientFill(circleGradient);
		g.fillEllipse(circleBounds);
		g.setGradientFill(circleHighlight);
		g.fillEllipse(circleBounds);

		float radius = circleBounds.getWidth() / 2.0F;
		float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		juce::Point<float> indicatorStart(
			circleBounds.getCentreX() + (radius / 2.0F) * math::cosf(angle - math::piOver2f),
			circleBounds.getCentreY() + (radius / 2.0F) * math::sinf(angle - math::piOver2f));
		juce::Point<float> indicatorEnd(
			circleBounds.getCentreX() + radius * math::cosf(angle - math::piOver2f),
			circleBounds.getCentreY() + radius * math::sinf(angle - math::piOver2f));

		juce::ColourGradient indicatorGradient(mRotarySliderIndicatorColour.brighter(0.2F),
											   indicatorStart,
											   mRotarySliderIndicatorColour.darker(0.3F),
											   indicatorEnd,
											   false);
		juce::ColourGradient indicatorBackgroundGradient(
			mSliderShadowColour.darker(0.2F).withAlpha(0.5F),
			indicatorStart,
			mSliderShadowColour.darker(0.2F).withAlpha(0.2F),
			indicatorEnd,
			false);

		juce::Line<float> indicatorLine = juce::Line<float>(indicatorStart, indicatorEnd);
		juce::Path indicator;
		indicator.addLineSegment(indicatorLine, lineWidth);
		juce::Path indicatorBackground;
		indicatorBackground.addLineSegment(indicatorLine, lineWidth + 2.0F);

		g.setGradientFill(indicatorBackgroundGradient);
		g.strokePath(indicatorBackground,
					 juce::PathStrokeType(lineWidth + 2.0F,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::rounded));
		g.setGradientFill(indicatorGradient);
		g.strokePath(indicator,
					 juce::PathStrokeType(lineWidth,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::rounded));

		juce::ColourGradient glowGradient(
			mRotarySliderIndicatorColour.withAlpha(0.0F),
			indicatorStart,
			mRotarySliderIndicatorColour.brighter(0.4F).withAlpha(0.3F),
			indicatorEnd,
			false);
		g.setGradientFill(glowGradient);
		g.strokePath(indicator,
					 juce::PathStrokeType(lineWidth,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::rounded));
	}

	auto ApexLookAndFeel::drawSemiCircularRotary(juce::Graphics& g,
												 int x,
												 int y,
												 int width,
												 int height,
												 float sliderPos,
												 const float rotaryStartAngle,
												 const float rotaryEndAngle,
												 juce::Slider& slider) noexcept -> void {
		juce::ignoreUnused(rotaryStartAngle, rotaryEndAngle, slider);

		juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();

		g.setColour(mBackgroundColour);
		g.fillRect(rawBounds);

		float diameter = juce::jmin(rawBounds.getWidth(), rawBounds.getHeight());

		juce::Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
												  .withHeight(diameter)
												  .reduced(2.0F, 2.0F)
												  .withCentre(rawBounds.getCentre());
		juce::Rectangle<float> circleBounds
			= troughBounds.reduced(2.0F, 2.0F)
				  .withCentre(juce::Point<float>(troughBounds.getCentreX(),
												 troughBounds.getCentreY() - 2.0F));
		juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0F, 2.0F);

		juce::Point<float> troughCentre = troughBounds.getCentre();
		juce::Point<float> shadowCentre = shadowBounds.getCentre();
		juce::Point<float> circleCentre = circleBounds.getCentre();

		juce::Point<float> troughLeft(troughBounds.getX(), troughBounds.getCentreY());
		juce::Point<float> shadowLeft(shadowBounds.getX(), shadowBounds.getCentreY());

		juce::Point<float> troughRight(troughBounds.getX() + troughBounds.getWidth(),
									   troughBounds.getCentreY());
		juce::Point<float> shadowRight(shadowBounds.getX() + shadowBounds.getWidth(),
									   shadowBounds.getCentreY());

		float troughRadius = troughBounds.getWidth() / 2.0F;
		float shadowRadius = shadowBounds.getWidth() / 2.0F;
		float circleRadius = circleBounds.getWidth() / 2.0F;

		float endAngle = 2.0F * math::pif;
		float startAngle = math::pif;
		float angle = startAngle + sliderPos * (endAngle - startAngle);

		juce::Path trough;
		trough.addCentredArc(troughCentre.getX(),
							 troughCentre.getY(),
							 troughRadius / 2.0F,
							 troughRadius / 2.0F,
							 0.0F,
							 startAngle + math::piOver2f,
							 endAngle + math::piOver2f,
							 true);

		g.setColour(mSliderTroughColour);
		g.strokePath(trough,
					 juce::PathStrokeType(troughRadius,
										  juce::PathStrokeType::mitered,
										  juce::PathStrokeType::butt));

		juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.9F),
											shadowBounds.getX() + shadowBounds.getWidth() / 2.0F,
											shadowBounds.getY() + shadowBounds.getHeight() / 2.0F,
											mSliderShadowColour.withAlpha(0.3F),
											shadowBounds.getX(),
											shadowBounds.getY(),
											true);

		juce::Path shadow;
		shadow.addCentredArc(shadowCentre.getX(),
							 shadowCentre.getY(),
							 shadowRadius / 2.0F,
							 shadowRadius / 2.0F,
							 0.0F,
							 startAngle + math::piOver2f,
							 endAngle + math::piOver2f,
							 true);
		g.setGradientFill(shadowGradient);
		g.strokePath(shadow,
					 juce::PathStrokeType(shadowRadius,
										  juce::PathStrokeType::mitered,
										  juce::PathStrokeType::butt));

		juce::Point<float> thumbStart = circleCentre;
		juce::Point<float> thumbEnd(circleCentre.getX() + circleRadius * math::cosf(angle),
									circleCentre.getY() + circleRadius * math::sinf(angle));

		float thumbWidth = circleRadius * 0.1F;

		float strokeRadius = circleRadius + 5.0F;

		juce::Path fillPath;
		fillPath.addCentredArc(circleCentre.getX(),
							   circleCentre.getY(),
							   circleRadius / 2.0F,
							   circleRadius / 2.0F,
							   0.0F,
							   startAngle + math::piOver2f,
							   angle + math::piOver2f,
							   true);

		juce::ColourGradient strokeGradient(
			mSliderStrokeColour.withAlpha(0.5F),
			thumbStart,
			mSliderStrokeColour.withAlpha(0.5F),
			juce::Point<float>(thumbStart.getX(), thumbStart.getY() + strokeRadius),
			true);
		strokeGradient.addColour(0.5F, mSliderStrokeColour.withAlpha(0.2F));
		g.setGradientFill(strokeGradient);
		g.strokePath(fillPath,
					 juce::PathStrokeType(circleRadius,
										  juce::PathStrokeType::mitered,
										  juce::PathStrokeType::butt));

		juce::Line<float> thumbLine(thumbStart, thumbEnd);
		juce::Path thumb;
		thumb.addLineSegment(thumbLine, thumbWidth);
		juce::ColourGradient thumbGradient(mSliderStrokeColour.brighter(0.2F),
										   thumbStart,
										   mSliderStrokeColour.brighter(0.2F),
										   thumbEnd,
										   true);
		thumbGradient.addColour(0.5F, mSliderStrokeColour.darker(0.8F));
		g.setGradientFill(thumbGradient);
		g.strokePath(thumb,
					 juce::PathStrokeType(thumbWidth,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::rounded));
		float cornerSize = thumbWidth * 0.1F;
		juce::ColourGradient glowGradient(mSliderGlowColour.brighter(0.2F).withAlpha(0.5F),
										  thumbStart,
										  mSliderGlowColour.brighter(0.2F).withAlpha(0.5F),
										  thumbEnd,
										  true);
		glowGradient.addColour(0.5F, mSliderGlowColour.darker(0.2F).withAlpha(0.5F));
		g.setGradientFill(glowGradient);
		g.strokePath(thumb,
					 juce::PathStrokeType(thumbWidth - cornerSize,
										  juce::PathStrokeType::curved,
										  juce::PathStrokeType::rounded));
	}

	auto ApexLookAndFeel::drawCircularFillRotary(juce::Graphics& g,
												 int x,
												 int y,
												 int width,
												 int height,
												 float sliderPos,
												 const float rotaryStartAngle,
												 const float rotaryEndAngle,
												 juce::Slider& slider) noexcept -> void {
		juce::ignoreUnused(rotaryStartAngle, rotaryEndAngle, slider);

		juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();

		g.setColour(mBackgroundColour);
		g.fillRect(rawBounds);

		auto diameter = gsl::narrow_cast<float>(juce::jmin(width, height));

		juce::Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
												  .withHeight(diameter)
												  .reduced(2.0F, 2.0F)
												  .withCentre(rawBounds.getCentre());
		juce::Rectangle<float> circleBounds
			= troughBounds.withWidth(juce::jmax(diameter * sliderPos, 20.0F))
				  .withHeight(juce::jmax(diameter * sliderPos, 20.0F))
				  .reduced(2.0F, 2.0F)
				  .withCentre(troughBounds.getCentre());
		juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0F, 2.0F);

		juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.8F),
											shadowBounds.getCentreX(),
											shadowBounds.getCentreY(),
											mSliderShadowColour.withAlpha(0.2F),
											shadowBounds.getRight(),
											shadowBounds.getBottom(),
											true);
		juce::ColourGradient circleGradient(mSliderStrokeColour.withAlpha(0.1F),
											circleBounds.getCentreX(),
											circleBounds.getCentreY(),
											mSliderStrokeColour.withAlpha(0.5F),
											circleBounds.getRight(),
											circleBounds.getBottom(),
											true);

		g.setColour(mSliderTroughColour);
		g.fillEllipse(troughBounds);
		g.setGradientFill(shadowGradient);
		g.fillEllipse(shadowBounds);
		g.setGradientFill(circleGradient);
		g.fillEllipse(circleBounds);
	}
} // namespace apex::ui
