#include "ApexLookAndFeel.h"
#include "ApexSlider.h"
#include "ApexComboBox.h"
#include "ApexToggleButton.h"
#include "ApexMeter.h"

namespace apex {
	namespace ui {
		ApexLookAndFeel::ApexLookAndFeel() : juce::LookAndFeel_V4() {
			registerColours();
		}

		void ApexLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				juce::Slider& slider)
		{
#if defined USE_PHYSICAL_ROTARIES
			drawPhysicalRotary(g, x, y, width, height, sliderPos, rotaryStartAngle,
					rotaryEndAngle, slider);
#elif defined USE_2D_SEMICIRCULAR_ROTARIES
			drawSemiCircularRotary(g, x, y, width, height, sliderPos, rotaryStartAngle,
					rotaryEndAngle, slider);
#else
			drawCircularFillRotary(g, x, y, width, height, sliderPos, rotaryStartAngle,
					rotaryEndAngle, slider);
#endif
		}

		void ApexLookAndFeel::drawRotaryApexSlider(juce::Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				ApexSlider& slider)
		{
			Option<ApexFilmStrip> maybeStrip = slider.getFilmStrip();
			if(maybeStrip.isSome()) {
				ApexFilmStrip strip = *maybeStrip.getConst();
				size_t index = size_t(sliderPos * strip.getNumFrames());
				juce::Image image = strip.getFrameScaled(index, size_t(width), size_t(height));
				g.drawImageAt(image, x, y);
			}
			else {
				drawRotarySlider(g, x, y, width, height, sliderPos, rotaryStartAngle,
						rotaryEndAngle, slider);
			}
		}

		void ApexLookAndFeel::drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width,
				int height, float sliderPos, float minSliderPos, float maxSliderPos,
				const juce::Slider::SliderStyle style, juce::Slider& slider)
		{
			ignoreUnused(sliderPos, minSliderPos, maxSliderPos, style, slider);

			juce::Rectangle<int> rawBounds = juce::Rectangle<int>(x, y, width, height);

			g.fillAll(mBackgroundColour);

			juce::Rectangle<float> troughBounds = rawBounds.toFloat().reduced(2.0f, 2.0f);
			juce::Rectangle<float> shadowBounds = troughBounds.expanded(1.0f, 1.0f);

			juce::ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.8f),
					shadowBounds.getCentreX(),
					shadowBounds.getCentreY(),
					mSliderShadowColour.withAlpha(0.2f),
					shadowBounds.getRight(),
					shadowBounds.getBottom(),
					true);
			g.setColour(mSliderTroughColour);
			g.fillRect(troughBounds);

			g.setGradientFill(shadowGradient);
			g.fillRect(shadowBounds);
		}

		void ApexLookAndFeel::drawLinearApexSliderBackground(juce::Graphics& g, int x, int y, int width,
				int height, float sliderPosProp, const juce::Slider::SliderStyle style,
				ApexSlider& slider)
		{
			//technically we don't need this function, but we'll keep it for future use
			//incase we decide we DO want to do something different for our own types
			//vs built-ins
			//also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
			float sliderMaxPos = static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
			float sliderMinPos = static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x + width : y + height);
			drawLinearSliderBackground(g, x, y, width, height, sliderPosProp, sliderMinPos,
					sliderMaxPos, style, dynamic_cast<juce::Slider&>(slider));
		}

		void ApexLookAndFeel::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
				float sliderPos, float minSliderPos, float maxSliderPos,
				const juce::Slider::SliderStyle style, juce::Slider& slider)
		{
			ignoreUnused(minSliderPos, maxSliderPos, style);

			juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
			juce::Rectangle<float> bounds = rawBounds.reduced(1.0f, 1.0f);
			float boundsW = bounds.getWidth();
			float boundsH = bounds.getHeight();
			float boundsX = bounds.getX();
			float boundsY = bounds.getY();

			float thumbWidth = slider.isHorizontal() ? juce::jmin(20.0f, boundsW * 0.10f)
				: boundsW - 4.0f;
			float thumbHeight = slider.isHorizontal() ? boundsH - 4.0f
				: juce::jmin(20.0f, boundsH * 0.10f);
			float sliderPosX = slider.isHorizontal() ? sliderPos * boundsW + boundsX
				: boundsX + boundsW / 2.0f;
			float sliderPosY = slider.isHorizontal() ? boundsY + boundsH / 2.0f
				: (1.0f - sliderPos) * boundsH + boundsY;
			sliderPosX -= thumbWidth / 2.0f;
			sliderPosY -= thumbHeight / 2.0f;

			float wDiffRight = sliderPosX + thumbWidth - (boundsX + boundsW);
			float hDiffBottom = sliderPosY + thumbHeight - (boundsY + boundsH);
			float wDiffLeft = boundsX - sliderPosX;
			float hDiffTop = boundsY - sliderPosY;
			if(slider.isHorizontal() && wDiffRight >= 0.0f) {
				thumbWidth -= wDiffRight;
			}
			else if(!slider.isHorizontal() && hDiffBottom >= 0.0f) {
				thumbHeight -= hDiffBottom;
			}
			else if(slider.isHorizontal() && wDiffLeft >= 0.0f) {
				sliderPosX += wDiffLeft;
				thumbWidth -= wDiffLeft;
			}
			else if(!slider.isHorizontal() && hDiffTop >= 0.0f) {
				sliderPosY += hDiffTop;
				thumbHeight -= hDiffTop;
			}

			juce::Rectangle<float> thumb = juce::Rectangle<float>(sliderPosX, sliderPosY, thumbWidth, thumbHeight);

			juce::ColourGradient strokeGradient(
					mSliderStrokeColour.brighter(0.2f).withAlpha(0.5f),
					sliderPosX,
					sliderPosY,
					mSliderStrokeColour.brighter(0.2f).withAlpha(0.5f),
					sliderPosX + (slider.isHorizontal() ? 0.0f : thumbWidth),
					sliderPosY + (slider.isHorizontal() ? thumbHeight : 0.0f),
					false);
			strokeGradient.addColour(0.5f, mSliderStrokeColour.withAlpha(0.2f));
			juce::ColourGradient glowGradient(
					mSliderGlowColour.brighter(0.2f).withAlpha(0.5f),
					sliderPosX,
					sliderPosY,
					mSliderGlowColour.brighter(0.2f).withAlpha(0.5f),
					sliderPosX + (slider.isHorizontal() ? 0.0f : thumbWidth),
					sliderPosY + (slider.isHorizontal() ? thumbHeight : 0.0f),
					false);
			glowGradient.addColour(0.5f, mSliderGlowColour.darker(0.2f).withAlpha(0.2f));
			float cornerSize = slider.isHorizontal() ? thumbWidth * 0.1f : thumbHeight * 0.1f;
			g.setGradientFill(strokeGradient);
			g.fillRect(thumb);
			g.setGradientFill(glowGradient);
			g.fillRoundedRectangle(thumb, cornerSize);
		}

		void ApexLookAndFeel::drawLinearApexSliderThumb(juce::Graphics& g, int x, int y, int width,
				int height, float sliderPos, ApexSlider& slider)
		{
			//technically we don't need this function, but we'll keep it for future use
			//incase we decide we DO want to do something different for our own types
			//vs built-ins
			//also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
			float sliderMaxPos = static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
			float sliderMinPos = static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x + width : y + height);
			drawLinearSliderThumb(g, x, y, width, height, sliderPos, sliderMinPos, sliderMaxPos,
					dynamic_cast<juce::Slider&>(slider).getSliderStyle(), dynamic_cast<juce::Slider&>(slider));
		}

		void ApexLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
				float sliderPos, float minSliderPos, float maxSliderPos,
				const juce::Slider::SliderStyle style, juce::Slider& slider)
		{
			drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos,
					maxSliderPos, style, slider);

			juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
			juce::Rectangle<float> bounds = rawBounds.reduced(1.0f, 1.0f);
			float boundsW = bounds.getWidth();
			float boundsH = bounds.getHeight();
			float boundsX = bounds.getX();
			float boundsY = bounds.getY();

			float thumbWidth = slider.isHorizontal() ? juce::jmin(20.0f, boundsW * 0.10f)
				: boundsW - 4.0f;
			float thumbHeight = slider.isHorizontal() ? boundsH - 4.0f
				: juce::jmin(20.0f, boundsH * 0.10f);
			float sliderPosX = boundsX + 2.0f;
			float sliderPosY = slider.isHorizontal() ? boundsY + 2.0f
				: (1.0f - sliderPos) * boundsH + boundsY;
			float strokeWidth = slider.isHorizontal() ? sliderPos * boundsW
				: boundsW - 4.0f;
			float strokeHeight = slider.isHorizontal() ? boundsH - 4.0f
				: sliderPos * boundsH;

			float wDiffRight = sliderPosX + thumbWidth - (boundsX + boundsW);
			float hDiffBottom = sliderPosY + thumbHeight - (boundsY + boundsH);
			float wDiffLeft = boundsX - sliderPosX;
			float hDiffTop = boundsY - sliderPosY;
			if(slider.isHorizontal() && wDiffRight >= 0.0f) {
				thumbWidth -= wDiffRight;
			}
			else if(!slider.isHorizontal() && hDiffBottom >= 0.0f) {
				thumbHeight -= hDiffBottom;
			}
			else if(slider.isHorizontal() && wDiffLeft >= 0.0f) {
				sliderPosX += wDiffLeft;
				thumbWidth -= wDiffLeft;
			}
			else if(!slider.isHorizontal() && hDiffTop >= 0.0f) {
				sliderPosY += hDiffTop;
				thumbHeight -= hDiffTop;
			}

			sliderPosY -= slider.isHorizontal() ? 0.0f : thumbHeight / 2.0f;
			strokeWidth += slider.isHorizontal() ? -1.0f + thumbWidth / 2.0f : 0.0f;
			strokeHeight += slider.isHorizontal() ? 0.0f : thumbHeight / 2.0f;

			juce::Rectangle<float> stroke = juce::Rectangle<float>(sliderPosX, sliderPosY, strokeWidth, strokeHeight);
			juce::ColourGradient strokeGradient(
					mSliderStrokeColour.withAlpha(0.5f),
					sliderPosX,
					sliderPosY,
					mSliderStrokeColour.withAlpha(0.5f),
					sliderPosX + (slider.isHorizontal() ? 0.0f : strokeWidth),
					sliderPosY + (slider.isHorizontal() ? strokeHeight : 0.0f),
					false);
			strokeGradient.addColour(0.5f, mSliderStrokeColour.withAlpha(0.2f));
			g.setGradientFill(strokeGradient);
			g.fillRect(stroke);

			drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos,
					maxSliderPos, style, slider);
		}

		void ApexLookAndFeel::drawLinearApexSlider(juce::Graphics& g, int x, int y, int width, int height,
				float sliderPos, const juce::Slider::SliderStyle style, ApexSlider& slider)
		{
			Option<ApexFilmStrip> maybeFilmStrip = slider.getFilmStrip();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
				size_t index = size_t(sliderPos * filmStrip.getNumFrames());
				juce::Image image = filmStrip.getFrameScaled(index, size_t(width), size_t(height));
				g.drawImageAt(image, x, y);
			}
			else {
				float sliderMaxPos = static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x : y);
				float sliderMinPos = static_cast<float>(dynamic_cast<juce::Slider&>(slider).isHorizontal() ? x + width : y + height);
				drawLinearSlider(g, x, y, width, height, sliderPos, sliderMinPos, sliderMaxPos,
						style, dynamic_cast<juce::Slider&>(slider));
			}
		}

		void ApexLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height) {
			juce::Rectangle<float> bounds(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
			g.setColour(mPopupMenuBackgroundColour.darker(0.4f).withAlpha(0.9f));
			g.fillAll();
			g.setColour(mComboBoxShadowColour.withAlpha(0.8f));
			g.fillAll();
			g.setColour(mPopupMenuBackgroundColour.withAlpha(0.8f));
			g.fillRect(bounds.reduced(3.0f, 3.0f));
		}

		void ApexLookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
				bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
				bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText,
				const juce::Drawable* icon, const juce::Colour* textColour)
		{
			ignoreUnused(textColour);

			if(isSeparator) {
				juce::Rectangle<int> r = area.reduced(5, 0);
				r.removeFromTop(juce::roundToInt((r.getHeight() * 0.5f) - 0.5f));

				g.setColour(mPopupMenuTextColour.withAlpha(0.3f));
				g.fillRect(r.removeFromTop(1));
			}
			else {
				juce::Rectangle<int> r = area.reduced(1);

				if(isHighlighted && isActive) {
					g.setColour(mPopupMenuHighlightColour.withAlpha(0.5f).brighter(0.1f));
					g.fillRect(r);
					g.setColour(mPopupMenuTextColour.brighter(0.2f));
				}
				else {
					g.setColour(mPopupMenuTextColour.withAlpha(isActive ? 1.0f : 0.5f));
				}

				r.reduce(juce::jmin(5, area.getWidth() / 20), 0);

				juce::Font font = juce::jmin(r.getWidth(), r.getHeight()) * 0.33f;
				float maxFontHeight = r.getHeight() * 0.33f;

				g.setFont(font);

				juce::Rectangle<float> iconArea;
				if(icon != nullptr) {
					iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();
					icon->drawWithin(g, iconArea,
							juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
					r.removeFromLeft(juce::roundToInt(maxFontHeight * 0.5f));
				}
				else if (isTicked) {
					juce::Colour fill = mPopupMenuHighlightColour.withAlpha(0.5f).brighter(0.1f);
					if(isHighlighted) fill = fill.brighter(0.2f);
					g.setColour(fill);
					g.fillRect(area.reduced(1));
					iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();
					if(isHighlighted && isActive)
						g.setColour(mPopupMenuTextColour.brighter(0.2f));
					else
						g.setColour(mPopupMenuTextColour.withAlpha(isActive ? 1.0f : 0.5f));
				}

				if(hasSubMenu) {
					float arrowH = 0.6f * getPopupMenuFont().getAscent();

					float x = static_cast<float>(r.removeFromRight((int)arrowH).getX());
					float halfH = static_cast<float>(r.getCentreY());

					juce::Path p;
					p.startNewSubPath(x, halfH - arrowH * 0.5f);
					p.lineTo(x + arrowH * 0.6f, halfH);
					p.lineTo(x, halfH + arrowH * 0.5f);

					g.strokePath(p, juce::PathStrokeType(2.0f));
				}

				r.removeFromRight(3);
				g.drawFittedText(text, r, juce::Justification::centredLeft, 1);

				if(shortcutKeyText.isNotEmpty()) {
					juce::Font font2 = font;
					font2.setHeight(font2.getHeight() * 0.75f);
					font2.setHorizontalScale(0.95f);
					g.setFont(font2);
					g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
				}
			}
		}

		void ApexLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
				int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box)
		{
			juce::ignoreUnused(buttonX, buttonY, buttonW, buttonH, isButtonDown);

			float cornerSize = juce::jmin(width, height) * 0.1f;
			juce::Rectangle<float> boxBounds = juce::Rectangle<int>(0, 0, width, height).toFloat();
			juce::Rectangle<float> troughBounds = boxBounds.reduced(2.0f, 2.0f);
			juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);
			juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
			juce::Rectangle<int> arrowZone(static_cast<int>(width * 0.75f), 0, static_cast<int>(width * 0.2f), height);

			g.setColour(mComboBoxBackgroundColour);
			g.fillRect(boxBounds);


			g.setColour(mComboBoxTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			juce::ColourGradient shadowGradient(
					mComboBoxShadowColour.withAlpha(0.8f),
					shadowBounds.getCentre(),
					mComboBoxShadowColour.withAlpha(0.2f),
					shadowBounds.getBottomRight(),
					true);
			g.setGradientFill(shadowGradient);
			g.fillRoundedRectangle(shadowBounds, cornerSize);

			juce::Colour buttonColour = (box.isPopupActive() ? mButtonNormalColour.brighter(0.2f)
					: (box.isMouseOver() ? mButtonNormalColour.brighter(0.1f)
						: (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
			juce::ColourGradient buttonGradient(
					buttonColour.withAlpha(0.4f),
					buttonBounds.getCentre(),
					buttonColour.withAlpha(0.1f),
					buttonBounds.getBottomRight(),
					true);
			g.setGradientFill(buttonGradient);
			g.fillRoundedRectangle(buttonBounds, cornerSize);

			juce::Path p;
			p.startNewSubPath(arrowZone.getX() + 3.0f,
					arrowZone.getY() + arrowZone.getHeight() * 0.45f);
			p.lineTo(static_cast<float>(arrowZone.getCentreX()),
					arrowZone.getY() + arrowZone.getHeight() * 0.575f);
			p.lineTo(arrowZone.getRight() - 3.0f,
					arrowZone.getY() + arrowZone.getHeight() * 0.45f);

			juce::Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour : mComboBoxTextColour.darker(0.3f))));
			g.setColour(textColour);
			g.strokePath(p, juce::PathStrokeType(2.0f));
		}

		void ApexLookAndFeel::drawApexComboBox(juce::Graphics& g, int width, int height,
				bool isButtonDown, int buttonX, int buttonY, int buttonW,
				int buttonH, ApexComboBox& box)
		{
			juce::ignoreUnused(isButtonDown, buttonX, buttonY, buttonW, buttonH);

			Option<juce::Image> maybeActiveArrow = box.getActiveArrowImage();
			if(maybeActiveArrow.isSome()) {
				juce::Rectangle<int> arrowZone(static_cast<int>(width * 0.75f), 0, static_cast<int>(width * 0.25f), height);

				juce::Image activeArrow = *maybeActiveArrow.getConst();
				juce::Image hoveredArrow = *box.getHoveredArrowImage().getConst();
				juce::Image normalArrow = *box.getNormalArrowImage().getConst();

				if(box.isPopupActive()) {
					g.drawImageAt(activeArrow.rescaled(arrowZone.getWidth(), arrowZone.getHeight(),
								juce::Graphics::highResamplingQuality), arrowZone.getX(), arrowZone.getY());
				}
				else if(box.isHovered()) {
					g.drawImageAt(hoveredArrow.rescaled(arrowZone.getWidth(), arrowZone.getHeight(),
								juce::Graphics::highResamplingQuality), arrowZone.getX(), arrowZone.getY());
				}
				else {
					g.drawImageAt(normalArrow.rescaled(arrowZone.getWidth(), arrowZone.getHeight(),
								juce::Graphics::highResamplingQuality), arrowZone.getX(), arrowZone.getY());
				}
			}
			else {

				float cornerSize = juce::jmin(width, height) * 0.1f;
				juce::Rectangle<float> boxBounds = juce::Rectangle<int>(0, 0, width, height).toFloat();
				juce::Rectangle<float> troughBounds = boxBounds.reduced(2.0f, 2.0f);
				juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);
				juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
				juce::Rectangle<int> arrowZone(static_cast<int>(width * 0.75f), 0, static_cast<int>(width * 0.2f), height);

				g.setColour(mComboBoxBackgroundColour);
				g.fillRect(boxBounds);


				g.setColour(mComboBoxTroughColour);
				g.fillRoundedRectangle(troughBounds, cornerSize);

				juce::ColourGradient shadowGradient(
						mComboBoxShadowColour.withAlpha(0.8f),
						shadowBounds.getCentre(),
						mComboBoxShadowColour.withAlpha(0.2f),
						shadowBounds.getBottomRight(),
						true);
				g.setGradientFill(shadowGradient);
				g.fillRoundedRectangle(shadowBounds, cornerSize);

				juce::Colour buttonColour = (box.isPopupActive() ? mButtonNormalColour.brighter(0.2f)
						: (box.isHovered() ? mButtonNormalColour.brighter(0.1f)
							: (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
				juce::ColourGradient buttonGradient(
						buttonColour.withAlpha(0.4f),
						buttonBounds.getCentre(),
						buttonColour.withAlpha(0.1f),
						buttonBounds.getBottomRight(),
						true);
				g.setGradientFill(buttonGradient);
				g.fillRoundedRectangle(buttonBounds, cornerSize);

				juce::Path p;
				p.startNewSubPath(arrowZone.getX() + 3.0f,
						arrowZone.getY() + arrowZone.getHeight() * 0.45f);
				p.lineTo(static_cast<float>(arrowZone.getCentreX()),
						arrowZone.getY() + arrowZone.getHeight() * 0.575f);
				p.lineTo(arrowZone.getRight() - 3.0f,
						arrowZone.getY() + arrowZone.getHeight() * 0.45f);

				juce::Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
						: (box.isHovered() ? mComboBoxTextColour.brighter(0.1f)
							: (box.isEnabled() ? mComboBoxTextColour
								: mComboBoxTextColour.darker(0.3f))));
				g.setColour(textColour);
				g.strokePath(p, juce::PathStrokeType(2.0f));
			}
		}

		void ApexLookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label) {
			juce::Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			label.setColour(juce::Label::textColourId, textColour);
			label.setBounds(0, 0, static_cast<int>(box.getWidth() * 0.7f), box.getHeight());
			label.setFont(juce::jmin(box.getWidth(), box.getHeight()) * 0.33f);
		}

		void ApexLookAndFeel::positionApexComboBoxText(ApexComboBox& box, juce::Label& label) {
			juce::Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isHovered() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			label.setColour(juce::Label::textColourId, textColour);
			label.setBounds(0, 0, static_cast<int>(box.getWidth() * 0.7f), box.getHeight());
			label.setFont(juce::jmin(box.getWidth(), box.getHeight()) * 0.33f);
		}

		void ApexLookAndFeel::drawComboBoxTextWhenNothingSelected(juce::Graphics& g, juce::ComboBox& box,
				juce::Label& label) {
			juce::Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			g.setColour(textColour);
			juce::Font font = getLabelFont(label);
			g.setFont(font);
			juce::Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
			g.drawFittedText(box.getTextWhenNothingSelected(), textArea, label.getJustificationType(),
					juce::jmax(1, static_cast<int>(textArea.getHeight() / font.getHeight())),
					label.getMinimumHorizontalScale());
		}

		void ApexLookAndFeel::drawApexComboBoxTextWhenNothingSelected(juce::Graphics& g,
				ApexComboBox& box, juce::Label& label)
		{
			juce::Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isHovered() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			g.setColour(textColour);
			juce::Font font = getLabelFont(label);
			g.setFont(font);
			juce::Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
			g.drawFittedText(box.getTextWhenNothingSelected(), textArea, label.getJustificationType(),
					juce::jmax(1, static_cast<int>(textArea.getHeight() / font.getHeight())),
					label.getMinimumHorizontalScale());
		}

		void ApexLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
				bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
		{
			float cornerSize = juce::jmax(5.0f, button.getWidth() * 0.1f, button.getHeight() * 0.1f);
			float width = float(button.getWidth());
			float height = float(button.getHeight());
			float fontSize = juce::jmin(width * 0.33f, height * 0.33f);
			juce::Rectangle<float> bounds(0.0f, 0.0f, width, height);
			juce::Rectangle<float> troughBounds = bounds.reduced(2.0f, 2.0f);
			juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
			juce::Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);

			g.setColour(mBackgroundColour);
			g.fillRect(bounds);

			g.setColour(mButtonTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			juce::ColourGradient shadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					shadowBounds.getCentreX(),
					shadowBounds.getCentreY(),
					mSliderShadowColour.withAlpha(0.2f),
					shadowBounds.getRight(),
					shadowBounds.getBottom(),
					true);
			g.setGradientFill(shadowGradient);
			g.fillRoundedRectangle(shadowBounds, cornerSize);

			juce::Colour buttonColour = ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown)
					? mButtonPressedColour.brighter(0.1f)
					: (shouldDrawButtonAsHighlighted ? mButtonNormalColour.brighter(0.1f)
						: (shouldDrawButtonAsDown ? mButtonPressedColour
							: mButtonNormalColour)));
			juce::ColourGradient buttonGradient(
					buttonColour.withAlpha(0.4f),
					buttonBounds.getCentreX(),
					buttonBounds.getCentreY(),
					buttonColour.withAlpha(0.1f),
					buttonBounds.getRight(),
					buttonBounds.getBottom(),
					true);
			g.setGradientFill(buttonGradient);
			g.fillRoundedRectangle(buttonBounds, cornerSize);

			g.setColour(mButtonTextColour);

			juce::Colour textColour = ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown)
					? mButtonTextColour.brighter(0.05f)
					: (shouldDrawButtonAsHighlighted ? mButtonTextColour.brighter(0.1f)
						: (shouldDrawButtonAsDown ? mButtonTextColour.darker(0.1f)
							: mButtonTextColour)));
			juce::ColourGradient textGradient(
					textColour.withAlpha(0.8f),
					buttonBounds.getCentreX(),
					buttonBounds.getCentreY(),
					textColour.withAlpha(1.0f),
					buttonBounds.getRight(),
					buttonBounds.getBottom(),
					true);
			g.setGradientFill(textGradient);
			g.setFont(fontSize);

			if(!button.isEnabled()) g.setOpacity(0.5f);

			g.drawFittedText(button.getButtonText(),
					troughBounds.reduced(2.0f, 2.0f).toNearestInt(),
					juce::Justification::centred, 1, 1.0f);
		}

		void ApexLookAndFeel::drawApexToggleButton(juce::Graphics& g, ApexToggleButton& button,
				bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
		{
			Option<ApexFilmStrip> maybeFilmStrip = button.getFilmStrip();
			Option<juce::Image> maybeToggledImage = button.getToggledImage();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
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
				juce::Image toggledImage = *maybeToggledImage.getConst();
				juce::Image hoveredImage = *button.getHoveredImage().getConst();
				juce::Image normalImage = *button.getNormalImage().getConst();
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
				drawToggleButton(g, dynamic_cast<juce::ToggleButton&>(button),
						shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
			}
		}

		void ApexLookAndFeel::drawApexMeter(juce::Graphics& g, float levelProportional,
				float clipLevelProportional, size_t numSteps, ApexMeter& meter) {

			int meterX = 0;
			int meterY = 0;
			int meterWidth = meter.getWidth();
			int meterHeight = meter.getHeight();

			Option<ApexFilmStrip> maybeFilmStrip = meter.getFilmStrip();
			Option<juce::Image> maybeMaxedImage = meter.getMaxedImage();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
				size_t index = static_cast<size_t>(filmStrip.getNumFrames() * levelProportional);
				g.drawImageAt(filmStrip.getFrameScaled(index,
							static_cast<size_t>(meterWidth),
							static_cast<size_t>(meterHeight)
							),
						meterX, meterY);
			}
			else if (maybeMaxedImage.isSome()) {
				juce::Image maxedImage = *maybeMaxedImage.getConst();
				int meterPeak = static_cast<int>(meterY + (1.0f - levelProportional) * meterHeight);
				int meterDiff = meterHeight - meterPeak;
				juce::Rectangle<int> clip(0, meterDiff, meterWidth, meterHeight);
				juce::Image clipped = maxedImage.getClippedImage(clip);
				g.drawImageAt(clipped, meterX, meterY + meterDiff);
			}
			else {
				int meterLevelY = static_cast<int>(meterY + (1.0f - levelProportional) * meterHeight);

				float stepHeight = meterHeight / static_cast<float>(numSteps);

				juce::Rectangle<float> bounds(
						static_cast<float>(meterX),
						static_cast<float>(meterY),
						static_cast<float>(meterWidth),
						static_cast<float>(meterHeight)
						);
				juce::Rectangle<float> troughBounds = bounds.reduced(2.0f, 2.0f);

				juce::Rectangle<float> meterBounds = troughBounds.reduced(2.0f, 2.0f);
				float meterHeightDiff = meterLevelY - (meterY + 2.0f);
				float meterBoundsHeight = meterBounds.getHeight();
				meterBounds = meterBounds.withY(static_cast<float>(meterLevelY) + 2.0f)
					.withHeight(static_cast<float>(meterBoundsHeight)  + 2.0f - static_cast<float>(meterHeightDiff));

				float troughX = troughBounds.getX() + 2.0f;
				float troughWidth = troughBounds.getWidth() - 2.0f;

				juce::ColourGradient meterGradient(mMeterClipColour, troughX, static_cast<float>(meterY),
						mMeterLowerColour, troughX, static_cast<float>(meterY + meterHeight), false);
				meterGradient.addColour(clipLevelProportional, mMeterUpperColour);

				g.setColour(mBackgroundColour);
				g.fillRect(bounds);

				g.setColour(mMeterTroughColour);
				g.fillRect(troughBounds);

				g.setGradientFill(meterGradient);
				g.fillRect(meterBounds);

				g.setColour(mBackgroundColour.brighter(0.2f).withAlpha(0.3f));
				float stepY = meterY + stepHeight;
				for(size_t i = 1; i < numSteps; ++i) {
					g.drawLine(troughX, stepY, troughX + troughWidth, stepY);
					stepY += stepHeight;
				}
			}
		}

		void ApexLookAndFeel::setFont(juce::Font newFont) {
			mFont = newFont;
		}

		void ApexLookAndFeel::setColour(ApexColourId id, juce::Colour colour) {
			switch(id) {
				case backgroundColourId: {
											 mBackgroundColour = colour;
											 juce::LookAndFeel::setColour(backgroundColourId,
													 mBackgroundColour);
											 juce::LookAndFeel::setColour(juce::AlertWindow::outlineColourId,
													 mBackgroundColour);
										 }
										 break;
				case buttonShadowColourId: {
											   mButtonShadowColour = colour;
											   juce::LookAndFeel::setColour(buttonShadowColourId,
													   mButtonShadowColour);
										   }
										   break;
				case buttonNormalColourId: {
											   mButtonNormalColour = colour;
											   juce::LookAndFeel::setColour(buttonNormalColourId,
													   mButtonNormalColour);
										   }
										   break;
				case buttonPressedColourId: {
												mButtonPressedColour = colour;
												juce::LookAndFeel::setColour(buttonPressedColourId,
														mButtonPressedColour);
											}
											break;
				case buttonTroughColourId: {
											   mButtonTroughColour = colour;
											   juce::LookAndFeel::setColour(buttonTroughColourId,
													   mButtonTroughColour);
										   }
										   break;
				case buttonTextColourId: {
											 mButtonTextColour = colour;
											 juce::LookAndFeel::setColour(buttonTextColourId,
													 mButtonTextColour);
										 }
										 break;
				case comboBoxBackgroundColourId: {
													 mComboBoxBackgroundColour = colour;
													 juce::LookAndFeel::setColour(comboBoxBackgroundColourId,
															 mComboBoxBackgroundColour);
												 }
												 break;
				case comboBoxShadowColourId: {
												 mComboBoxShadowColour = colour;
												 juce::LookAndFeel::setColour(comboBoxShadowColourId,
														 mComboBoxShadowColour);
											 }
											 break;
				case comboBoxTroughColourId: {
												 mComboBoxTroughColour = colour;
												 juce::LookAndFeel::setColour(comboBoxTroughColourId,
														 mComboBoxTroughColour);
											 }
											 break;
				case comboBoxTextColourId: {
											   mComboBoxTextColour = colour;
											   juce::LookAndFeel::setColour(comboBoxTextColourId,
													   mComboBoxTextColour);
										   }
										   break;
				case meterClipColourId: {
											mMeterClipColour = colour;
											juce::LookAndFeel::setColour(meterClipColourId,
													mMeterClipColour);
										}
										break;
				case meterLowerColourId: {
											 mMeterLowerColour = colour;
											 juce::LookAndFeel::setColour(meterLowerColourId,
													 mMeterLowerColour);
										 }
										 break;
				case meterTroughColourId: {
											  mMeterTroughColour = colour;
											  juce::LookAndFeel::setColour(meterTroughColourId,
													  mMeterTroughColour);
										  }
										  break;
				case meterUpperColourId: {
											 mMeterUpperColour = colour;
											 juce::LookAndFeel::setColour(meterUpperColourId,
													 mMeterUpperColour);
										 }
										 break;
				case popupMenuBackgroundColourId: {
													  mPopupMenuBackgroundColour = colour;
													  juce::LookAndFeel::setColour(popupMenuBackgroundColourId,
															  mPopupMenuBackgroundColour);
													  juce::LookAndFeel::setColour(juce::PopupMenu::backgroundColourId,
															  mPopupMenuBackgroundColour
															  .withAlpha(0.9f));
													  juce::LookAndFeel::setColour(juce::PopupMenu::highlightedBackgroundColourId,
															  mPopupMenuBackgroundColour
															  .brighter(0.2f)
															  .withAlpha(0.9f));
												  }
												  break;
				case popupMenuTextColourId: {
												mPopupMenuTextColour = colour;
												juce::LookAndFeel::setColour(popupMenuTextColourId,
														mPopupMenuTextColour);
											}
											break;
				case popupMenuHighlightColourId: {
													 mPopupMenuHighlightColour = colour;
													 juce::LookAndFeel::setColour(popupMenuHighlightColourId,
															 mPopupMenuHighlightColour);
												 }
												 break;
				case rotarySliderFillColourId: {
												   mRotarySliderFillColour = colour;
												   juce::LookAndFeel::setColour(rotarySliderFillColourId,
														   mRotarySliderFillColour);
											   }
											   break;
				case rotarySliderIndicatorColourId: {
														mRotarySliderIndicatorColour = colour;
														juce::LookAndFeel::setColour(rotarySliderIndicatorColourId,
																mRotarySliderIndicatorColour);
													}
													break;
				case sliderStrokeColourId: {
											   mSliderStrokeColour = colour;
											   juce::LookAndFeel::setColour(sliderStrokeColourId,
													   mSliderStrokeColour);
										   }
										   break;
				case sliderShadowColourId: {
											   mSliderShadowColour = colour;
											   juce::LookAndFeel::setColour(sliderShadowColourId,
													   mSliderShadowColour);
										   }
										   break;
				case sliderTroughColourId: {
											   mSliderTroughColour = colour;
											   juce::LookAndFeel::setColour(sliderTroughColourId,
													   mSliderTroughColour);

											   juce::LookAndFeel::setColour(juce::Slider::textBoxOutlineColourId,
													   mSliderTroughColour);
											   juce::LookAndFeel::setColour(juce::AlertWindow::backgroundColourId,
													   mSliderTroughColour);
											   juce::LookAndFeel::setColour(juce::TextEditor::backgroundColourId,
													   mSliderTroughColour
													   .withAlpha(0.85f));
											   juce::LookAndFeel::setColour(juce::TextEditor::focusedOutlineColourId,
													   mSliderTroughColour
													   .brighter(0.1f)
													   .withAlpha(0.85f));
										   }
										   break;
				case sliderGlowColourId: {
											 mSliderGlowColour = colour;
											 juce::LookAndFeel::setColour(sliderGlowColourId,
													 mSliderGlowColour);
										 }
										 break;
				case sliderTextColourId: {
											 mSliderTextColour = colour;
											 juce::LookAndFeel::setColour(sliderTextColourId,
													 mSliderTextColour);
											 juce::LookAndFeel::setColour(juce::TextEditor::textColourId,
													 mSliderTextColour);
											 juce::LookAndFeel::setColour(juce::AlertWindow::textColourId,
													 mSliderTextColour);
										 }
										 break;
				default: break;
			}
		}

		juce::Colour ApexLookAndFeel::getColour(ApexColourId id) {
			return findColour(id);
		}

		void ApexLookAndFeel::registerColours() {
			juce::LookAndFeel::setColour(backgroundColourId, mBackgroundColour);

			juce::LookAndFeel::setColour(buttonShadowColourId, mButtonShadowColour);
			juce::LookAndFeel::setColour(buttonNormalColourId, mButtonNormalColour);
			juce::LookAndFeel::setColour(buttonPressedColourId, mButtonPressedColour);
			juce::LookAndFeel::setColour(buttonTroughColourId, mButtonTroughColour);
			juce::LookAndFeel::setColour(buttonTextColourId, mButtonTextColour);

			juce::LookAndFeel::setColour(comboBoxBackgroundColourId, mComboBoxBackgroundColour);
			juce::LookAndFeel::setColour(comboBoxShadowColourId, mComboBoxShadowColour);
			juce::LookAndFeel::setColour(comboBoxTroughColourId, mComboBoxTroughColour);
			juce::LookAndFeel::setColour(comboBoxTextColourId, mComboBoxTextColour);

			juce::LookAndFeel::setColour(meterClipColourId, mMeterClipColour);
			juce::LookAndFeel::setColour(meterLowerColourId, mMeterLowerColour);
			juce::LookAndFeel::setColour(meterTroughColourId, mMeterTroughColour);
			juce::LookAndFeel::setColour(meterUpperColourId, mMeterUpperColour);

			juce::LookAndFeel::setColour(popupMenuBackgroundColourId, mPopupMenuBackgroundColour);
			juce::LookAndFeel::setColour(popupMenuTextColourId, mPopupMenuTextColour);
			juce::LookAndFeel::setColour(popupMenuHighlightColourId, mPopupMenuHighlightColour);
			juce::LookAndFeel::setColour(juce::PopupMenu::backgroundColourId, mPopupMenuBackgroundColour
					.withAlpha(0.9f));
			juce::LookAndFeel::setColour(juce::PopupMenu::highlightedBackgroundColourId, mPopupMenuBackgroundColour
					.brighter(0.2f)
					.withAlpha(0.9f));

			juce::LookAndFeel::setColour(rotarySliderFillColourId, mRotarySliderFillColour);
			juce::LookAndFeel::setColour(rotarySliderIndicatorColourId, mRotarySliderIndicatorColour);

			juce::LookAndFeel::setColour(sliderStrokeColourId, mSliderStrokeColour);
			juce::LookAndFeel::setColour(sliderShadowColourId, mSliderShadowColour);
			juce::LookAndFeel::setColour(sliderTroughColourId, mSliderTroughColour);
			juce::LookAndFeel::setColour(sliderGlowColourId, mSliderGlowColour);
			juce::LookAndFeel::setColour(sliderTextColourId, mSliderTextColour);
			juce::LookAndFeel::setColour(juce::Slider::textBoxOutlineColourId, mSliderTroughColour);

			juce::LookAndFeel::setColour(juce::AlertWindow::backgroundColourId, mSliderTroughColour);
			juce::LookAndFeel::setColour(juce::AlertWindow::outlineColourId, mBackgroundColour);
			juce::LookAndFeel::setColour(juce::AlertWindow::textColourId, mSliderTextColour);

			juce::LookAndFeel::setColour(juce::TextEditor::backgroundColourId, mSliderTroughColour
					.withAlpha(0.85f));
			juce::LookAndFeel::setColour(juce::TextEditor::outlineColourId, mSliderTroughColour
					.withAlpha(0.85f));
			juce::LookAndFeel::setColour(juce::TextEditor::focusedOutlineColourId, mSliderTroughColour
					.brighter(0.1f)
					.withAlpha(0.85f));
			juce::LookAndFeel::setColour(juce::TextEditor::textColourId, mSliderTextColour);
		}

		juce::Rectangle<int> ApexLookAndFeel::getActualRotaryBounds(int x, int y, int width, int height) {
#if defined USE_PHYSICAL_ROTARIES
			float diameter = static_cast<float>(juce::jmin(width, height));
			juce::Rectangle<float> bounds(x + (width - diameter) / 2.0f, y + (height - diameter) / 2.0f,
					diameter, diameter);
			return bounds.toNearestInt();
#elif defined USE_2D_SEMICIRCULAR_ROTARIES
			float diameter = static_cast<float>(juce::jmin(width, height));
			juce::Rectangle<float> bounds(x + (width - diameter) / 2.0f, y + (height - diameter) / 2.0f,
					diameter, diameter / 2.0f);
			return bounds.toNearestInt();
#else
			float diameter = static_cast<float>(juce::jmin(width, height));
			juce::Rectangle<float> bounds(x + (width - diameter) / 2.0f, y + (height - diameter) / 2.0f,
					diameter, diameter);
			return bounds.toNearestInt();
#endif
		}

		void ApexLookAndFeel::drawPhysicalRotary(juce::Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				juce::Slider& slider)
		{
			juce::ignoreUnused(slider);

			juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			float diameter = juce::jmin(rawBounds.getWidth(), rawBounds.getHeight());

			juce::Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
				.withHeight(diameter)
				.reduced(2.0f, 2.0f)
				.withCentre(rawBounds.getCentre());
			juce::Rectangle<float> circleBounds = troughBounds.reduced(2.0f, 2.0f);
			juce::Rectangle<float> innerShadowBounds = troughBounds.expanded(2.0f, 2.0f);
			juce::Rectangle<float> outerShadowBounds = juce::Rectangle<float>(
					innerShadowBounds.getX() - 3.0f,
					innerShadowBounds.getY(),
					innerShadowBounds.getWidth() + 3.0f,
					innerShadowBounds.getHeight() + 8.0f);

			float lineWidth = diameter * 0.02f;

			juce::ColourGradient innerShadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					innerShadowBounds.getX() + innerShadowBounds.getWidth() / 2.0f,
					innerShadowBounds.getY() + innerShadowBounds.getHeight() / 2.0f,
					mSliderShadowColour.withAlpha(0.2f),
					innerShadowBounds.getX() + innerShadowBounds.getWidth(),
					innerShadowBounds.getY() + innerShadowBounds.getHeight(),
					true);
			juce::ColourGradient outerShadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					outerShadowBounds.getX() + outerShadowBounds.getWidth() / 2.0f,
					outerShadowBounds.getY() + outerShadowBounds.getHeight() / 2.0f,
					mSliderShadowColour.darker(0.2f).withAlpha(0.0f),
					outerShadowBounds.getX(),
					outerShadowBounds.getY() + outerShadowBounds.getHeight(),
					true);
			juce::ColourGradient circleGradient(
					mRotarySliderFillColour.brighter(0.1f),
					circleBounds.getX() + circleBounds.getWidth() / 2,
					circleBounds.getY() + circleBounds.getHeight() / 2,
					mRotarySliderFillColour.darker(0.3f),
					circleBounds.getX() + circleBounds.getWidth(),
					circleBounds.getY() + circleBounds.getHeight(),
					true);
			juce::ColourGradient circleHighlight(
					mRotarySliderFillColour.darker(0.2f).withAlpha(0.3f),
					circleBounds.getX(),
					circleBounds.getY(),
					mRotarySliderFillColour.darker(0.2f).withAlpha(0.3f),
					circleBounds.getX() + circleBounds.getWidth(),
					circleBounds.getY() + circleBounds.getHeight(),
					false);
			circleHighlight.addColour(0.5f, mRotarySliderFillColour.brighter(0.1f).withAlpha(0.5f));

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

			float radius = circleBounds.getWidth() / 2.0f;
			float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

			juce::Point<float> indicatorStart(circleBounds.getCentreX()
					+ (radius / 2.0f) * math::cosf(angle - math::piOver2f),
					circleBounds.getCentreY()
					+ (radius / 2.0f) * math::sinf(angle - math::piOver2f));
			juce::Point<float> indicatorEnd(circleBounds.getCentreX()
					+ radius * math::cosf(angle - math::piOver2f),
					circleBounds.getCentreY()
					+ radius * math::sinf(angle - math::piOver2f));

			juce::ColourGradient indicatorGradient(
					mRotarySliderIndicatorColour.brighter(0.2f),
					indicatorStart,
					mRotarySliderIndicatorColour.darker(0.3f),
					indicatorEnd,
					false);
			juce::ColourGradient indicatorBackgroundGradient(
					mSliderShadowColour.darker(0.2f).withAlpha(0.5f),
					indicatorStart,
					mSliderShadowColour.darker(0.2f).withAlpha(0.2f),
					indicatorEnd,
					false);

			juce::Line<float> indicatorLine = juce::Line<float>(indicatorStart, indicatorEnd);
			juce::Path indicator;
			indicator.addLineSegment(indicatorLine, lineWidth);
			juce::Path indicatorBackground;
			indicatorBackground.addLineSegment(indicatorLine, lineWidth + 2.0f);

			g.setGradientFill(indicatorBackgroundGradient);
			g.strokePath(indicatorBackground, juce::PathStrokeType(lineWidth + 2.0f,
						juce::PathStrokeType::curved,
						juce::PathStrokeType::rounded));
			g.setGradientFill(indicatorGradient);
			g.strokePath(indicator, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved,
						juce::PathStrokeType::rounded));

			juce::ColourGradient glowGradient(
					mRotarySliderIndicatorColour.withAlpha(0.0f),
					indicatorStart,
					mRotarySliderIndicatorColour.brighter(0.4f).withAlpha(0.3f),
					indicatorEnd,
					false);
			g.setGradientFill(glowGradient);
			g.strokePath(indicator, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved,
						juce::PathStrokeType::rounded));
		}

		void ApexLookAndFeel::drawSemiCircularRotary(juce::Graphics& g, int x, int y, int width,
				int height, float sliderPos, const float rotaryStartAngle,
				const float rotaryEndAngle, juce::Slider& slider)
		{
			juce::ignoreUnused(rotaryStartAngle, rotaryEndAngle, slider);

			juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			float diameter = juce::jmin(rawBounds.getWidth(), rawBounds.getHeight());

			juce::Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
				.withHeight(diameter).reduced(2.0f, 2.0f)
				.withCentre(rawBounds.getCentre());
			juce::Rectangle<float> circleBounds = troughBounds
				.reduced(2.0f, 2.0f)
				.withCentre(juce::Point<float>(
							troughBounds.getCentreX(),
							troughBounds.getCentreY() - 2.0f));
			juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);

			juce::Point<float> troughCentre = troughBounds.getCentre();
			juce::Point<float> shadowCentre = shadowBounds.getCentre();
			juce::Point<float> circleCentre = circleBounds.getCentre();

			juce::Point<float> troughLeft(troughBounds.getX(), troughBounds.getCentreY());
			juce::Point<float> shadowLeft(shadowBounds.getX(), shadowBounds.getCentreY());

			juce::Point<float> troughRight(troughBounds.getX() + troughBounds.getWidth(),
					troughBounds.getCentreY());
			juce::Point<float> shadowRight(shadowBounds.getX() + shadowBounds.getWidth(),
					shadowBounds.getCentreY());

			float troughRadius = troughBounds.getWidth() / 2.0f;
			float shadowRadius = shadowBounds.getWidth() / 2.0f;
			float circleRadius = circleBounds.getWidth() / 2.0f;

			float endAngle = 2.0f * math::pif;
			float startAngle = math::pif;
			float angle = startAngle + sliderPos * (endAngle - startAngle);

			juce::Path trough;
			trough.addCentredArc(troughCentre.getX(), troughCentre.getY(),
					troughRadius / 2.0f, troughRadius / 2.0f, 0.0f,
					startAngle + math::piOver2f, endAngle + math::piOver2f, true);

			g.setColour(mSliderTroughColour);
			g.strokePath(trough, juce::PathStrokeType(troughRadius, juce::PathStrokeType::mitered,
						juce::PathStrokeType::butt));

			juce::ColourGradient shadowGradient(
					mSliderShadowColour.withAlpha(0.9f),
					shadowBounds.getX() + shadowBounds.getWidth() / 2.0f,
					shadowBounds.getY() + shadowBounds.getHeight() / 2.0f,
					mSliderShadowColour.withAlpha(0.3f),
					shadowBounds.getX(),
					shadowBounds.getY(),
					true);

			juce::Path shadow;
			shadow.addCentredArc(shadowCentre.getX(), shadowCentre.getY(),
					shadowRadius / 2.0f, shadowRadius / 2.0f, 0.0f,
					startAngle + math::piOver2f, endAngle + math::piOver2f, true);
			g.setGradientFill(shadowGradient);
			g.strokePath(shadow, juce::PathStrokeType(shadowRadius, juce::PathStrokeType::mitered,
						juce::PathStrokeType::butt));

			juce::Point<float> thumbStart = circleCentre;
			juce::Point<float> thumbEnd(circleCentre.getX()
					+ circleRadius * math::cosf(angle),
					circleCentre.getY()
					+ circleRadius * math::sinf(angle));

			float thumbWidth = circleRadius * 0.1f;

			float strokeRadius = circleRadius + 5.0f;

			juce::Path fillPath;
			fillPath.addCentredArc(circleCentre.getX(), circleCentre.getY(), circleRadius / 2.0f,
					circleRadius / 2.0f, 0.0f, startAngle + math::piOver2f,
					angle + math::piOver2f, true);

			juce::ColourGradient strokeGradient(
					mSliderStrokeColour.withAlpha(0.5f),
					thumbStart,
					mSliderStrokeColour.withAlpha(0.5f),
					juce::Point<float>(thumbStart.getX(), thumbStart.getY() + strokeRadius),
					true);
			strokeGradient.addColour(0.5f, mSliderStrokeColour.withAlpha(0.2f));
			g.setGradientFill(strokeGradient);
			g.strokePath(fillPath, juce::PathStrokeType(circleRadius, juce::PathStrokeType::mitered,
						juce::PathStrokeType::butt));

			juce::Line<float> thumbLine(thumbStart, thumbEnd);
			juce::Path thumb;
			thumb.addLineSegment(thumbLine, thumbWidth);
			juce::ColourGradient thumbGradient(
					mSliderStrokeColour.brighter(0.2f),
					thumbStart,
					mSliderStrokeColour.brighter(0.2f),
					thumbEnd,
					true);
			thumbGradient.addColour(0.5f, mSliderStrokeColour.darker(0.8f));
			g.setGradientFill(thumbGradient);
			g.strokePath(thumb, juce::PathStrokeType(thumbWidth, juce::PathStrokeType::curved,
						juce::PathStrokeType::rounded));
			float cornerSize = thumbWidth * 0.1f;
			juce::ColourGradient glowGradient(
					mSliderGlowColour.brighter(0.2f).withAlpha(0.5f),
					thumbStart,
					mSliderGlowColour.brighter(0.2f).withAlpha(0.5f),
					thumbEnd,
					true);
			glowGradient.addColour(0.5f, mSliderGlowColour.darker(0.2f).withAlpha(0.5f));
			g.setGradientFill(glowGradient);
			g.strokePath(thumb, juce::PathStrokeType(thumbWidth - cornerSize, juce::PathStrokeType::curved,
						juce::PathStrokeType::rounded));
		}

		void ApexLookAndFeel::drawCircularFillRotary(juce::Graphics& g, int x, int y, int width,
				int height, float sliderPos, const float rotaryStartAngle,
				const float rotaryEndAngle, juce::Slider& slider)
		{
			juce::ignoreUnused(rotaryStartAngle, rotaryEndAngle, slider);

			juce::Rectangle<float> rawBounds = juce::Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			float diameter = static_cast<float>(juce::jmin(width, height));

			juce::Rectangle<float> troughBounds = rawBounds
				.withWidth(diameter)
				.withHeight(diameter)
				.reduced(2.0f, 2.0f)
				.withCentre(rawBounds.getCentre());
			juce::Rectangle<float> circleBounds = troughBounds
				.withWidth(juce::jmax(diameter * sliderPos, 20.0f))
				.withHeight(juce::jmax(diameter * sliderPos, 20.0f))
				.reduced(2.0f, 2.0f)
				.withCentre(troughBounds.getCentre());
			juce::Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);

			juce::ColourGradient shadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					shadowBounds.getCentreX(),
					shadowBounds.getCentreY(),
					mSliderShadowColour.withAlpha(0.2f),
					shadowBounds.getRight(),
					shadowBounds.getBottom(),
					true);
			juce::ColourGradient circleGradient(
					mSliderStrokeColour.withAlpha(0.1f),
					circleBounds.getCentreX(),
					circleBounds.getCentreY(),
					mSliderStrokeColour.withAlpha(0.5f),
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
	}
}
