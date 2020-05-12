#include "ApexLookAndFeel.h"
#include "ApexSlider.h"
#include "ApexComboBox.h"
#include "ApexToggleButton.h"
#include "ApexMeter.h"

namespace apex {
	namespace ui {
		ApexLookAndFeel::ApexLookAndFeel() : LookAndFeel_V4() {
			registerColours();
		}

		void ApexLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				Slider& slider)
		{
			ignoreUnused(slider);

			float troughWidth = width * 0.95f;
			float troughHeight = height * 0.95f;
			Rectangle<float> rawBounds = Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			Rectangle<float> troughBounds = rawBounds.reduced(width * 0.05f, height * 0.05f);
			Rectangle<float> bounds = troughBounds.reduced(jmin(5.0f, troughWidth * 0.05f),
					jmin(5.0f, troughHeight * 0.05f));
			Rectangle<float> shadowBounds = Rectangle<float>(troughBounds.getX(),
					troughBounds.getY(), troughBounds.getWidth(),
					troughBounds.getHeight() * 0.9f).withCentre(Point<float>(
						troughBounds.getCentreX(),
						troughBounds.getCentreY() + troughBounds.getHeight() * 0.1f));

			g.setColour(mSliderTroughColour);
			g.fillEllipse(troughBounds);

			//TODO: change shadow code to use a gradient
			g.setColour(mSliderShadowColour.withAlpha(0.8f));
			g.fillEllipse(troughBounds.expanded(2.0f, 2.0f));

			g.fillEllipse(shadowBounds);

			g.setColour(mRotarySliderFillColour);
			g.fillEllipse(bounds);

			float radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
			float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
			float lineWidth = jmax(4.0f, bounds.getWidth() * 0.02f);
			float arcRadius = radius - jmin(2.0f, troughHeight * 0.02f);

			Point<float> indicatorEnd(bounds.getCentreX()
					+ arcRadius * math::cosf(angle - math::piOver2f),
					bounds.getCentreY()
					+ arcRadius * math::sinf(angle - math::piOver2f));
			Point<float> indicatorStart(bounds.getCentreX()
					+ (arcRadius * math::cosf(angle - math::piOver2f)) / 2.0f,
					bounds.getCentreY()
					+ (arcRadius * math::sinf(angle - math::piOver2f)) / 2.0f);
			g.setColour(mRotarySliderIndicatorColour);
			Line<float> line = Line<float>(indicatorStart, indicatorEnd);
			Path indicator;
			indicator.addLineSegment(line, lineWidth);
			g.strokePath(indicator, PathStrokeType(lineWidth, PathStrokeType::curved, PathStrokeType::rounded));

			//TODO: change glow code to use a gradient
			float glowWidth = lineWidth * 3.0f;
			Point<float> glowEnd(indicatorEnd.getX() + glowWidth, indicatorEnd.getY() + glowWidth);
			Point<float> glowStart(indicatorStart.getX() + glowWidth, indicatorStart.getY() + glowWidth);
			g.setColour(mSliderGlowColour.withAlpha(0.7f));
			Line<float> glowLine = Line<float>(glowStart, glowEnd);
			Path glow;
			glow.addLineSegment(glowLine, glowWidth);
			g.strokePath(glow, PathStrokeType(glowWidth, PathStrokeType::curved, PathStrokeType::rounded));
		}

		void ApexLookAndFeel::drawRotaryApexSlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				ApexSlider& slider)
		{
			Option<ApexFilmStrip> maybeStrip = slider.getFilmStrip();
			if(maybeStrip.isSome()) {
				ApexFilmStrip strip = maybeStrip.get();
				size_t index = size_t(sliderPos * strip.getNumFrames());
				Image image = strip.getFrameScaled(index, size_t(width), size_t(height));
				g.drawImageAt(image, x, y);
			}
			else {
				drawRotarySlider(g, x, y, width, height, sliderPos, rotaryStartAngle,
						rotaryEndAngle, dynamic_cast<Slider&>(slider));
			}
		}

		void ApexLookAndFeel::drawLinearSliderBackground(Graphics& g, int x, int y, int width,
				int height, float sliderPos, float minSliderPos, float maxSliderPos,
				const Slider::SliderStyle style, Slider& slider)
		{
			ignoreUnused(sliderPos, minSliderPos, maxSliderPos, style);

			float troughWidth = width * 0.95f;
			float troughHeight = height * 0.95f;
			Rectangle<int> rawBounds = Rectangle<int>(x, y, width, height);

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			Rectangle<float> troughBounds = Rectangle<float>(x + width * 0.05f, y + height * 0.05f,
					troughWidth, troughHeight);
			Rectangle<float> shadowBounds = Rectangle<float>(troughBounds.getX(),
					troughBounds.getY(), troughBounds.getWidth(),
					troughBounds.getHeight() * 0.9f).withCentre(Point<float>(
						troughBounds.getCentreX(),
						troughBounds.getCentreY() + troughBounds.getHeight() * 0.1f));
			g.setColour(mSliderTroughColour);
			g.fillRect(troughBounds);

			if(slider.isHorizontal()) {
				//TODO: change shadow code below to use a gradient dependent
				//(size/position-wise) on style
			}

			g.setColour(mSliderShadowColour.withAlpha(0.8f));
			g.fillRect(troughBounds.expanded(2.0f, 2.0f));

			g.fillRect(shadowBounds);
		}

		void ApexLookAndFeel::drawLinearApexSliderBackground(Graphics& g, int x, int y, int width,
				int height, float sliderPosProp, const Slider::SliderStyle style,
				ApexSlider& slider)
		{
			//technically we don't need this function, but we'll keep it for future use
			//incase we decide we DO want to do something different for our own types
			//vs built-ins
			//also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
			float sliderMaxPos = dynamic_cast<Slider&>(slider).isHorizontal() ? x : y;
			float sliderMinPos = dynamic_cast<Slider&>(slider).isHorizontal() ? x + width : y + height;
			drawLinearSliderBackground(g, x, y, width, height, sliderPosProp, sliderMinPos,
					sliderMaxPos, style, dynamic_cast<Slider&>(slider));
		}

		void ApexLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
				float sliderPos, float minSliderPos, float maxSliderPos,
				const Slider::SliderStyle style, Slider& slider)
		{
			ignoreUnused(minSliderPos, maxSliderPos, style);
			float thumbWidth = slider.isHorizontal() ? jmin(20.0f, width * 0.15f)
				: jmin(4.0f, width * 0.9f);
			float thumbHeight = slider.isHorizontal() ? jmin(4.0f, height * 0.9f)
				: jmin(20.0f, height * 0.15f);
			float sliderPosX = slider.isHorizontal() ? sliderPos * width + x : x + width / 2.0f;
			float sliderPosY = slider.isHorizontal() ? y + height / 2.0f : sliderPos * height + y;
			sliderPosX -= thumbWidth / 2.0f;
			sliderPosY -= thumbHeight / 2.0f;
			Rectangle<float> thumb = Rectangle<float>(sliderPosX, sliderPosY, thumbWidth, thumbHeight);
			float cornerSize = jmax(thumbWidth * 0.1f, thumbHeight * 0.1f);
			g.setColour(mSliderStrokeColour.brighter(0.1f));
			g.fillRoundedRectangle(thumb, cornerSize);
			//TODO: change glow to use a gradient fill
			g.setColour(mSliderGlowColour.withAlpha(0.7f));
			g.fillRoundedRectangle(thumb.expanded(thumbWidth * 2.0f, thumbHeight * 2.0f),
					cornerSize * 2.0f);
		}

		void ApexLookAndFeel::drawLinearApexSliderThumb(Graphics& g, int x, int y, int width,
				int height, float sliderPos, ApexSlider& slider)
		{
			//technically we don't need this function, but we'll keep it for future use
			//incase we decide we DO want to do something different for our own types
			//vs built-ins
			//also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
			float sliderMaxPos = dynamic_cast<Slider&>(slider).isHorizontal() ? x : y;
			float sliderMinPos = dynamic_cast<Slider&>(slider).isHorizontal() ? x + width : y + height;
			drawLinearSliderThumb(g, x, y, width, height, sliderPos, sliderMinPos, sliderMaxPos,
					dynamic_cast<Slider&>(slider).getSliderStyle(), dynamic_cast<Slider&>(slider));
		}

		void ApexLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, float minSliderPos, float maxSliderPos,
				const Slider::SliderStyle style, Slider& slider)
		{
			drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos,
					maxSliderPos, style, slider);

			float strokeWidth = slider.isHorizontal() ? jmin(15.0f, width * 0.1f)
				: jmin(3.0f, width * 0.85f);
			float strokeHeight = slider.isHorizontal() ? jmin(3.0f, height * 0.85f)
				: jmin(15.0f, height * 0.1f);
			float sliderPosX = slider.isHorizontal() ? sliderPos * width + x : x + width / 2.0f;
			float sliderPosY = slider.isHorizontal() ? y + height / 2.0f : sliderPos * height + y;
			sliderPosX -= strokeWidth / 2.0f;
			sliderPosY -= strokeHeight / 2.0f;
			Rectangle<float> stroke = Rectangle<float>(sliderPosX, sliderPosY, strokeWidth, strokeHeight);
			g.setColour(mSliderStrokeColour);
			g.fillRect(stroke);

			drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos,
					maxSliderPos, style, slider);
		}

		void ApexLookAndFeel::drawLinearApexSlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, const Slider::SliderStyle style, ApexSlider& slider)
		{
			Option<ApexFilmStrip> maybeFilmStrip = slider.getFilmStrip();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = maybeFilmStrip.get();
				size_t index = size_t(sliderPos * filmStrip.getNumFrames());
				Image image = filmStrip.getFrameScaled(index, size_t(width), size_t(height));
				g.drawImageAt(image, x, y);
			}
			else {
				float sliderMaxPos = dynamic_cast<Slider&>(slider).isHorizontal() ? x : y;
				float sliderMinPos = dynamic_cast<Slider&>(slider).isHorizontal() ? x + width : y + height;
				drawLinearSlider(g, x, y, width, height, sliderPos, sliderMinPos, sliderMaxPos,
						style, dynamic_cast<Slider&>(slider));
			}
		}

		void ApexLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height) {
			g.setColour(mPopupMenuBackgroundColour.withAlpha(0.9f));
			g.fillRect(Rectangle<int>(width, height));
		}

		void ApexLookAndFeel::drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
				bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
				bool hasSubMenu, const String& text, const String& shortcutKeyText,
				const Drawable* icon, const Colour* textColour)
		{
			ignoreUnused(textColour);

			if(isSeparator) {
				Rectangle<int> r = area.reduced(5, 0);
				r.removeFromTop(roundToInt((r.getHeight() * 0.5f) - 0.5f));

				g.setColour(mPopupMenuTextColour.withAlpha(0.3f));
				g.fillRect(r.removeFromTop(1));
			}
			else {
				Rectangle<int> r = area.reduced(1);

				if(isHighlighted && isActive) {
					g.setColour(mPopupMenuBackgroundColour.brighter(0.2f));
					g.fillRect(r);
					g.setColour(mPopupMenuTextColour.brighter(0.2f));
				}
				else {
					g.setColour(mPopupMenuTextColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
				}

				r.reduce(jmin(5, area.getWidth() / 20), 0);

				Font font = getPopupMenuFont();
				float maxFontHeight = r.getHeight() / 1.3f;

				if(font.getHeight() > maxFontHeight) font.setHeight(maxFontHeight);

				g.setFont(font);

				Rectangle<float> iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();

				if(icon != nullptr) {
					icon->drawWithin(g, iconArea,
							RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
					r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
				}
				else if (isTicked) {
					Path tick = getTickShape(1.0f);
					g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(
									iconArea.getWidth() / 5, 0).toFloat(), true));
				}

				if(hasSubMenu) {
					float arrowH = 0.6f * getPopupMenuFont().getAscent();

					float x = static_cast<float>(r.removeFromRight((int)arrowH).getX());
					float halfH = static_cast<float>(r.getCentreY());

					Path p;
					p.startNewSubPath(x, halfH - arrowH * 0.5f);
					p.lineTo(x + arrowH * 0.6f, halfH);
					p.lineTo(x, halfH + arrowH * 0.5f);

					g.strokePath(p, PathStrokeType(2.0f));
				}

				r.removeFromRight(3);
				g.drawFittedText(text, r, Justification::centredLeft, 1);

				if(shortcutKeyText.isNotEmpty()) {
					Font font2 = font;
					font2.setHeight(font2.getHeight() * 0.75f);
					font2.setHorizontalScale(0.95f);
					g.setFont(font2);
					g.drawText(shortcutKeyText, r, Justification::centredRight, true);
				}
			}
		}

		void ApexLookAndFeel::drawComboBox(Graphics& g, int width, int height, bool isButtonDown,
				int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
		{
			ignoreUnused(buttonX, buttonY, buttonW, buttonH, isButtonDown);

			auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
			Rectangle<float> boxBounds = Rectangle<int>(0, 0, width, height).toFloat();

			g.setColour(mComboBoxBackgroundColour);
			g.fillRect(boxBounds);

			Rectangle<float> troughBounds = boxBounds.reduced(
					boxBounds.getWidth() * 0.05f, boxBounds.getHeight() * 0.05f);

			g.setColour(mComboBoxTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
			g.setColour(mComboBoxShadowColour.withAlpha(0.8f));
			g.fillRoundedRectangle(shadowBounds, cornerSize);

			Rectangle<int> arrowZone(width - 30, 0, 20, height);
			Path p;
			p.startNewSubPath(arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
			p.lineTo(static_cast<float>(arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
			p.lineTo(arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

			g.setColour(mComboBoxTextColour.withAlpha((
							box.isMouseOver() ? 9.0f : (box.isPopupActive() ? 1.0f
								: (box.isEnabled() ? 0.8f : 0.2f)))));
			g.strokePath(p, PathStrokeType(2.0f));
		}

		void ApexLookAndFeel::drawApexComboBox(Graphics& g, int width, int height, bool isButtonDown,
				int buttonX, int buttonY, int buttonW, int buttonH, ApexComboBox& box)
		{
			Rectangle<int> arrowZone(width - 30, 0, 20, height);

			Option<Image> maybeActiveArrow = box.getActiveArrowImage();
			if(maybeActiveArrow.isSome()) {
				Image activeArrow = maybeActiveArrow.get();
				Image hoveredArrow = box.getHoveredArrowImage().get();
				Image normalArrow = box.getNormalArrowImage().get();

				if(box.isPopupActive()) {
					g.drawImageAt(activeArrow.rescaled(arrowZone.getWidth(), arrowZone.getHeight(),
								Graphics::highResamplingQuality), arrowZone.getX(), arrowZone.getY());
				}
				else if(box.isHovered()) {
					g.drawImageAt(hoveredArrow.rescaled(arrowZone.getWidth(), arrowZone.getHeight(),
								Graphics::highResamplingQuality), arrowZone.getX(), arrowZone.getY());
				}
				else {
					g.drawImageAt(normalArrow.rescaled(arrowZone.getWidth(), arrowZone.getHeight(),
								Graphics::highResamplingQuality), arrowZone.getX(), arrowZone.getY());
				}
			}
			else {
				drawComboBox(g, width, height, isButtonDown, buttonX, buttonY, buttonW,
						buttonH, dynamic_cast<ComboBox&>(box));
			}
		}

		void ApexLookAndFeel::positionComboBoxText(ComboBox& box, Label& label) {
			label.setBounds(0, 0, box.getWidth(), box.getHeight());
			label.setFont(mFont);
		}

		void ApexLookAndFeel::positionApexComboBoxText(ApexComboBox& box, Label& label) {
			//technically we don't need this function, but we'll keep it incase we
			//decide we want to do ApexComboBox specific things in the future
			positionComboBoxText(dynamic_cast<ComboBox&>(box), label);
		}

		void ApexLookAndFeel::drawComboBoxTextWhenNothingSelected(Graphics& g, ComboBox& box, Label& label) {
			g.setColour(mComboBoxTextColour);
			Font font = getLabelFont(label);
			g.setFont(font);
			Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
			g.drawFittedText(box.getTextWhenNothingSelected(), textArea, label.getJustificationType(),
					jmax(1, (int)(textArea.getHeight() / font.getHeight())),
					label.getMinimumHorizontalScale());
		}

		void ApexLookAndFeel::drawApexComboBoxTextWhenNothingSelected(Graphics& g,
				ApexComboBox& box, Label& label)
		{
			//technically dont' need this function, but we'll keep it incase we decide we want
			//to do ApexComboBox specific things in the future
			drawComboBoxTextWhenNothingSelected(g, dynamic_cast<ComboBox&>(box), label);
		}

		void ApexLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
				bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
		{
			float fontSize = jmin(15.0f, button.getHeight() * 0.75f);
			float cornerSize = jmax(5.0f, button.getWidth() * 0.1f, button.getHeight() * 0.1f);
			float width = float(button.getWidth());
			float height = float(button.getHeight());
			Rectangle<float> bounds(button.getX(), button.getY(), width, height);
			Rectangle<float> troughBounds = bounds.reduced(width * 0.05f, height * 0.05f);
			Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
			Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);

			g.setColour(mBackgroundColour);
			g.fillRect(bounds);

			g.setColour(mButtonTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			g.setColour(mButtonShadowColour.withAlpha(0.8f));
			g.fillRoundedRectangle(shadowBounds, cornerSize);
			g.fillRoundedRectangle(shadowBounds.withCentre(
						Point<float>(
							shadowBounds.getX(),
							shadowBounds.getY() + shadowBounds.getHeight() * 0.25f)
						), cornerSize);

			g.setColour(shouldDrawButtonAsHighlighted ? mButtonNormalColour.brighter(0.2f)
					: (shouldDrawButtonAsDown ? mButtonPressedColour : mButtonNormalColour));
			g.fillRoundedRectangle(buttonBounds, cornerSize);

			g.setColour(mButtonTextColour);
			g.setFont(fontSize);

			if(!button.isEnabled()) g.setOpacity(0.5f);

			g.drawFittedText(button.getButtonText(), troughBounds.reduced(2.0f, 2.0f).toNearestInt(),
					Justification::centred, 10);
		}

		void ApexLookAndFeel::drawApexToggleButton(Graphics& g, ApexToggleButton& button,
				bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
		{
			Option<ApexFilmStrip> maybeFilmStrip = button.getFilmStrip();
			Option<Image> maybeToggledImage = button.getToggledImage();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = maybeFilmStrip.get();
				if(shouldDrawButtonAsDown) g.drawImageAt(
						filmStrip.getFrameScaled(
							0,
							size_t(button.getWidth()),
							size_t(button.getHeight())
							),
						button.getX(), button.getY());
				else if(shouldDrawButtonAsHighlighted) g.drawImageAt(
						filmStrip.getFrameScaled(
							1,
							size_t(button.getWidth()),
							size_t(button.getHeight())
							),
						button.getX(), button.getY());
				else g.drawImageAt(filmStrip.getFrameScaled(
							2,
							size_t(button.getWidth()),
							size_t(button.getHeight())
							),
						button.getX(), button.getY());
			}
			else if(maybeToggledImage.isSome()) {
				Image toggledImage = maybeToggledImage.get();
				Image hoveredImage = button.getHoveredImage().get();
				Image normalImage = button.getNormalImage().get();
				if(shouldDrawButtonAsDown) g.drawImageAt(
						toggledImage.rescaled(
							button.getWidth(),
							button.getHeight()
							),
						button.getX(), button.getY());
				else if(shouldDrawButtonAsHighlighted) g.drawImageAt(
						hoveredImage.rescaled(
							button.getWidth(),
							button.getHeight()
							),
						button.getX(), button.getY());
				else g.drawImageAt(normalImage.rescaled(
							button.getWidth(),
							button.getHeight()
							),
						button.getX(), button.getY());
			}
			else {
				drawToggleButton(g, dynamic_cast<ToggleButton&>(button),
						shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
			}
		}

		void ApexLookAndFeel::drawApexMeter(Graphics& g, float levelProportional,
				float clipLevelProportional, int numSteps, ApexMeter& meter) {

			int meterX = meter.getX();;
			int meterY = meter.getY();
			int meterWidth = meter.getWidth();
			int meterHeight = meter.getHeight();

			Option<ApexFilmStrip> maybeFilmStrip = meter.getFilmStrip();
			Option<Image> maybeMaxedImage = meter.getMaxedImage();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = maybeFilmStrip.get();
				size_t index = filmStrip.getNumFrames() * levelProportional;
				g.drawImageAt(filmStrip.getFrameScaled(index,
							size_t(meterWidth),
							size_t(meterHeight)
							),
						meterX, meterY);
			}
			else if (maybeMaxedImage.isSome()) {
				Image maxedImage = maybeMaxedImage.get();
				int meterPeak = meterY + (1.0f - levelProportional) * meterHeight;
				int meterDiff = meterHeight - meterPeak;
				Rectangle<int> clip(0, meterDiff, meterWidth, meterHeight);
				Image clipped = maxedImage.getClippedImage(clip);
				g.drawImageAt(clipped, meterX, meterY + meterDiff);
			}
			else {
				float cornerSize = jmax(meterWidth * 0.1f, meterHeight * 0.1f);
				int meterLevelY = meterY + (1.0f - levelProportional) * meterHeight;

				float stepHeight = meterHeight / float(numSteps);

				Rectangle<float> bounds = meter.getBounds().toFloat();
				Rectangle<float> troughBounds = bounds.reduced(meterWidth * 0.05f, meterHeight * 0.05f);
				Rectangle<float> meterBounds = troughBounds.reduced(2.0f, 2.0f).withY(meterLevelY);
				Rectangle<float> glowBounds = meterBounds.expanded(meterBounds.getWidth() * 2.0f,
						meterBounds.getWidth() * 2.0f);

				float troughX = troughBounds.getX();
				float troughWidth = troughBounds.getWidth();

				ColourGradient meterGradient(mMeterClipColour, troughX, meterY,
						mMeterLowerColour, troughX, meterY + meterHeight, false);
				meterGradient.addColour(clipLevelProportional, mMeterUpperColour);

				ColourGradient glowGradient(mMeterClipColour.brighter(0.2f).withAlpha(0.5f), troughX,
						meterY, mMeterLowerColour.brighter(0.2f).withAlpha(0.5f),
						troughX, meterY + meterHeight, false);
				glowGradient.addColour(clipLevelProportional, mMeterUpperColour.brighter(0.2f).withAlpha(0.5f));

				g.setColour(mBackgroundColour);
				g.fillRoundedRectangle(bounds, cornerSize);

				g.setColour(mMeterTroughColour);
				g.fillRoundedRectangle(troughBounds, cornerSize);

				g.setGradientFill(meterGradient);
				g.fillRoundedRectangle(meterBounds, cornerSize);

				g.setColour(mBackgroundColour.brighter(0.2f).withAlpha(0.7f));
				float stepY = meterY + stepHeight;
				for(int i = 1; i < numSteps; ++i) {
					g.drawLine(troughX, stepY, troughX + troughWidth, stepY);
					stepY += stepHeight;
				}

				g.setGradientFill(glowGradient);
				g.fillRoundedRectangle(glowBounds, cornerSize);
			}
		}

		void ApexLookAndFeel::setFont(Font newFont) {
			mFont = newFont;
		}

		void ApexLookAndFeel::setColour(ApexColourId id, Colour colour) {
			switch(id) {
				case backgroundColourId: {
											 mBackgroundColour = colour;
											 LookAndFeel::setColour(backgroundColourId,
													 mBackgroundColour);
										 }
										 break;
				case buttonShadowColourId: {
											   mButtonShadowColour = colour;
											   LookAndFeel::setColour(buttonShadowColourId,
													   mButtonShadowColour);
										   }
										   break;
				case buttonNormalColourId: {
											   mButtonNormalColour = colour;
											   LookAndFeel::setColour(buttonNormalColourId,
													   mButtonNormalColour);
										   }
										   break;
				case buttonPressedColourId: {
												mButtonPressedColour = colour;
												LookAndFeel::setColour(buttonPressedColourId,
														mButtonPressedColour);
											}
											break;
				case buttonTroughColourId: {
											   mButtonTroughColour = colour;
											   LookAndFeel::setColour(buttonTroughColourId,
													   mButtonTroughColour);
										   }
										   break;
				case buttonTextColourId: {
											 mButtonTextColour = colour;
											 LookAndFeel::setColour(buttonTextColourId,
													 mButtonTextColour);
										 }
										 break;
				case comboBoxBackgroundColourId: {
													 mComboBoxBackgroundColour = colour;
													 LookAndFeel::setColour(comboBoxBackgroundColourId,
															 mComboBoxBackgroundColour);
												 }
												 break;
				case comboBoxShadowColourId: {
												 mComboBoxShadowColour = colour;
												 LookAndFeel::setColour(comboBoxShadowColourId,
														 mComboBoxShadowColour);
											 }
											 break;
				case comboBoxTroughColourId: {
												 mComboBoxTroughColour = colour;
												 LookAndFeel::setColour(comboBoxTroughColourId,
														 mComboBoxTroughColour);
											 }
											 break;
				case comboBoxTextColourId: {
											   mComboBoxTextColour = colour;
											   LookAndFeel::setColour(comboBoxTextColourId,
													   mComboBoxTextColour);
										   }
										   break;
				case meterClipColourId: {
											mMeterClipColour = colour;
											LookAndFeel::setColour(meterClipColourId,
													mMeterClipColour);
										}
										break;
				case meterLowerColourId: {
											 mMeterLowerColour = colour;
											 LookAndFeel::setColour(meterLowerColourId,
													 mMeterLowerColour);
										 }
										 break;
				case meterTroughColourId: {
											  mMeterTroughColour = colour;
											  LookAndFeel::setColour(meterTroughColourId,
													  mMeterTroughColour);
										  }
										  break;
				case meterUpperColourId: {
											 mMeterUpperColour = colour;
											 LookAndFeel::setColour(meterUpperColourId,
													 mMeterUpperColour);
										 }
										 break;
				case popupMenuBackgroundColourId: {
													  mPopupMenuBackgroundColour = colour;
													  LookAndFeel::setColour(popupMenuBackgroundColourId,
															  mPopupMenuBackgroundColour);
												  }
												  break;
				case popupMenuTextColourId: {
												mPopupMenuTextColour = colour;
												LookAndFeel::setColour(popupMenuTextColourId,
														mPopupMenuTextColour);
											}
											break;
				case rotarySliderFillColourId: {
												   mRotarySliderFillColour = colour;
												   LookAndFeel::setColour(rotarySliderFillColourId,
														   mRotarySliderFillColour);
											   }
											   break;
				case rotarySliderIndicatorColourId: {
														mRotarySliderIndicatorColour = colour;
														LookAndFeel::setColour(rotarySliderIndicatorColourId,
																mRotarySliderIndicatorColour);
													}
													break;
				case sliderStrokeColourId: {
											   mSliderStrokeColour = colour;
											   LookAndFeel::setColour(sliderStrokeColourId,
													   mSliderStrokeColour);
										   }
										   break;
				case sliderShadowColourId: {
											   mSliderShadowColour = colour;
											   LookAndFeel::setColour(sliderShadowColourId,
													   mSliderShadowColour);
										   }
										   break;
				case sliderTroughColourId: {
											   mSliderTroughColour = colour;
											   LookAndFeel::setColour(sliderTroughColourId,
													   mSliderTroughColour);
										   }
										   break;
				case sliderGlowColourId: {
											 mSliderGlowColour = colour;
											 LookAndFeel::setColour(sliderGlowColourId,
													 mSliderGlowColour);
										 }
										 break;
				case sliderTextColourId: {
											 mSliderTextColour = colour;
											 LookAndFeel::setColour(sliderTextColourId,
													 mSliderTextColour);
										 }
										 break;
			}
		}

		Colour ApexLookAndFeel::getColour(ApexColourId id) {
			return findColour(id);
		}

		void ApexLookAndFeel::registerColours() {
			LookAndFeel::setColour(backgroundColourId, mBackgroundColour);

			LookAndFeel::setColour(buttonShadowColourId, mButtonShadowColour);
			LookAndFeel::setColour(buttonNormalColourId, mButtonNormalColour);
			LookAndFeel::setColour(buttonPressedColourId, mButtonPressedColour);
			LookAndFeel::setColour(buttonTroughColourId, mButtonTroughColour);
			LookAndFeel::setColour(buttonTextColourId, mButtonTextColour);

			LookAndFeel::setColour(comboBoxBackgroundColourId, mComboBoxBackgroundColour);
			LookAndFeel::setColour(comboBoxShadowColourId, mComboBoxShadowColour);
			LookAndFeel::setColour(comboBoxTroughColourId, mComboBoxTroughColour);
			LookAndFeel::setColour(comboBoxTextColourId, mComboBoxTextColour);

			LookAndFeel::setColour(meterClipColourId, mMeterClipColour);
			LookAndFeel::setColour(meterLowerColourId, mMeterLowerColour);
			LookAndFeel::setColour(meterTroughColourId, mMeterTroughColour);
			LookAndFeel::setColour(meterUpperColourId, mMeterUpperColour);

			LookAndFeel::setColour(popupMenuBackgroundColourId, mPopupMenuBackgroundColour);
			LookAndFeel::setColour(popupMenuTextColourId, mPopupMenuTextColour);

			LookAndFeel::setColour(rotarySliderFillColourId, mRotarySliderFillColour);
			LookAndFeel::setColour(rotarySliderIndicatorColourId, mRotarySliderIndicatorColour);

			LookAndFeel::setColour(sliderStrokeColourId, mSliderStrokeColour);
			LookAndFeel::setColour(sliderShadowColourId, mSliderShadowColour);
			LookAndFeel::setColour(sliderTroughColourId, mSliderTroughColour);
			LookAndFeel::setColour(sliderGlowColourId, mSliderGlowColour);
			LookAndFeel::setColour(sliderTextColourId, mSliderTextColour);
		}
	}
}
