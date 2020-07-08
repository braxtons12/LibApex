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

		void ApexLookAndFeel::drawRotaryApexSlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				ApexSlider& slider)
		{
			Option<ApexFilmStrip> maybeStrip = slider.getFilmStrip();
			if(maybeStrip.isSome()) {
				ApexFilmStrip strip = *maybeStrip.getConst();
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
			ignoreUnused(sliderPos, minSliderPos, maxSliderPos, style, slider);

			Rectangle<int> rawBounds = Rectangle<int>(x, y, width, height);

			g.fillAll(mBackgroundColour);

			Rectangle<float> troughBounds = rawBounds.toFloat().reduced(2.0f, 2.0f);
			Rectangle<float> shadowBounds = troughBounds.expanded(1.0f, 1.0f);

			ColourGradient shadowGradient(mSliderShadowColour.withAlpha(0.8f),
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

		void ApexLookAndFeel::drawLinearApexSliderBackground(Graphics& g, int x, int y, int width,
				int height, float sliderPosProp, const Slider::SliderStyle style,
				ApexSlider& slider)
		{
			//technically we don't need this function, but we'll keep it for future use
			//incase we decide we DO want to do something different for our own types
			//vs built-ins
			//also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
			float sliderMaxPos = static_cast<float>(dynamic_cast<Slider&>(slider).isHorizontal() ? x : y);
			float sliderMinPos = static_cast<float>(dynamic_cast<Slider&>(slider).isHorizontal() ? x + width : y + height);
			drawLinearSliderBackground(g, x, y, width, height, sliderPosProp, sliderMinPos,
					sliderMaxPos, style, dynamic_cast<Slider&>(slider));
		}

		void ApexLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
				float sliderPos, float minSliderPos, float maxSliderPos,
				const Slider::SliderStyle style, Slider& slider)
		{
			ignoreUnused(minSliderPos, maxSliderPos, style);

			Rectangle<float> rawBounds = Rectangle<int>(x, y, width, height).toFloat();
			Rectangle<float> bounds = rawBounds.reduced(1.0f, 1.0f);
			float boundsW = bounds.getWidth();
			float boundsH = bounds.getHeight();
			float boundsX = bounds.getX();
			float boundsY = bounds.getY();

			float thumbWidth = slider.isHorizontal() ? jmin(20.0f, boundsW * 0.10f)
				: boundsW - 4.0f;
			float thumbHeight = slider.isHorizontal() ? boundsH - 4.0f
				: jmin(20.0f, boundsH * 0.10f);
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

			Rectangle<float> thumb = Rectangle<float>(sliderPosX, sliderPosY, thumbWidth, thumbHeight);

			ColourGradient strokeGradient(
					mSliderStrokeColour.brighter(0.2f).withAlpha(0.5f),
					sliderPosX,
					sliderPosY,
					mSliderStrokeColour.brighter(0.2f).withAlpha(0.5f),
					sliderPosX + (slider.isHorizontal() ? 0.0f : thumbWidth),
					sliderPosY + (slider.isHorizontal() ? thumbHeight : 0.0f),
					false);
			strokeGradient.addColour(0.5f, mSliderStrokeColour.withAlpha(0.2f));
			ColourGradient glowGradient(
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

		void ApexLookAndFeel::drawLinearApexSliderThumb(Graphics& g, int x, int y, int width,
				int height, float sliderPos, ApexSlider& slider)
		{
			//technically we don't need this function, but we'll keep it for future use
			//incase we decide we DO want to do something different for our own types
			//vs built-ins
			//also, Why TF do we need to dynamic cast to Slider& when ApexSlider : Slider?????
			float sliderMaxPos = static_cast<float>(dynamic_cast<Slider&>(slider).isHorizontal() ? x : y);
			float sliderMinPos = static_cast<float>(dynamic_cast<Slider&>(slider).isHorizontal() ? x + width : y + height);
			drawLinearSliderThumb(g, x, y, width, height, sliderPos, sliderMinPos, sliderMaxPos,
					dynamic_cast<Slider&>(slider).getSliderStyle(), dynamic_cast<Slider&>(slider));
		}

		void ApexLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, float minSliderPos, float maxSliderPos,
				const Slider::SliderStyle style, Slider& slider)
		{
			drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos,
					maxSliderPos, style, slider);

			Rectangle<float> rawBounds = Rectangle<int>(x, y, width, height).toFloat();
			Rectangle<float> bounds = rawBounds.reduced(1.0f, 1.0f);
			float boundsW = bounds.getWidth();
			float boundsH = bounds.getHeight();
			float boundsX = bounds.getX();
			float boundsY = bounds.getY();

			float thumbWidth = slider.isHorizontal() ? jmin(20.0f, boundsW * 0.10f)
				: boundsW - 4.0f;
			float thumbHeight = slider.isHorizontal() ? boundsH - 4.0f
				: jmin(20.0f, boundsH * 0.10f);
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

			Rectangle<float> stroke = Rectangle<float>(sliderPosX, sliderPosY, strokeWidth, strokeHeight);
			ColourGradient strokeGradient(
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

		void ApexLookAndFeel::drawLinearApexSlider(Graphics& g, int x, int y, int width, int height,
				float sliderPos, const Slider::SliderStyle style, ApexSlider& slider)
		{
			Option<ApexFilmStrip> maybeFilmStrip = slider.getFilmStrip();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
				size_t index = size_t(sliderPos * filmStrip.getNumFrames());
				Image image = filmStrip.getFrameScaled(index, size_t(width), size_t(height));
				g.drawImageAt(image, x, y);
			}
			else {
				float sliderMaxPos = static_cast<float>(dynamic_cast<Slider&>(slider).isHorizontal() ? x : y);
				float sliderMinPos = static_cast<float>(dynamic_cast<Slider&>(slider).isHorizontal() ? x + width : y + height);
				drawLinearSlider(g, x, y, width, height, sliderPos, sliderMinPos, sliderMaxPos,
						style, dynamic_cast<Slider&>(slider));
			}
		}

		void ApexLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height) {
			Rectangle<float> bounds(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
			g.setColour(mPopupMenuBackgroundColour.darker(0.4f).withAlpha(0.9f));
			g.fillAll();
			g.setColour(mComboBoxShadowColour.withAlpha(0.8f));
			g.fillAll();
			g.setColour(mPopupMenuBackgroundColour.withAlpha(0.8f));
			g.fillRect(bounds.reduced(3.0f, 3.0f));
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
					g.setColour(mPopupMenuHighlightColour.withAlpha(0.5f).brighter(0.1f));
					g.fillRect(r);
					g.setColour(mPopupMenuTextColour.brighter(0.2f));
				}
				else {
					g.setColour(mPopupMenuTextColour.withAlpha(isActive ? 1.0f : 0.5f));
				}

				r.reduce(jmin(5, area.getWidth() / 20), 0);

				Font font = jmin(r.getWidth(), r.getHeight()) * 0.33f;
				float maxFontHeight = r.getHeight() * 0.33f;

				g.setFont(font);

				Rectangle<float> iconArea;
				if(icon != nullptr) {
					iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();
					icon->drawWithin(g, iconArea,
							RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
					r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
				}
				else if (isTicked) {
					Colour fill = mPopupMenuHighlightColour.withAlpha(0.5f).brighter(0.1f);
					if(isHighlighted) fill = fill.brighter(0.2f);
					g.setColour(fill);
					g.fillRect(area.reduced(1));
					iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();
					if(isHighlighted && isActive)
						g.setColour(mPopupMenuTextColour.brighter(0.2f));
					else
						g.setColour(mPopupMenuTextColour.withAlpha(isActive ? 1.0f : 0.5f));
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

			float cornerSize = jmin(width, height) * 0.1f;
			Rectangle<float> boxBounds = Rectangle<int>(0, 0, width, height).toFloat();
			Rectangle<float> troughBounds = boxBounds.reduced(2.0f, 2.0f);
			Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);
			Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
			Rectangle<int> arrowZone(static_cast<int>(width * 0.75f), 0, static_cast<int>(width * 0.2f), height);

			g.setColour(mComboBoxBackgroundColour);
			g.fillRect(boxBounds);


			g.setColour(mComboBoxTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			ColourGradient shadowGradient(
					mComboBoxShadowColour.withAlpha(0.8f),
					shadowBounds.getCentre(),
					mComboBoxShadowColour.withAlpha(0.2f),
					shadowBounds.getBottomRight(),
					true);
			g.setGradientFill(shadowGradient);
			g.fillRoundedRectangle(shadowBounds, cornerSize);

			Colour buttonColour = (box.isPopupActive() ? mButtonNormalColour.brighter(0.2f)
					: (box.isMouseOver() ? mButtonNormalColour.brighter(0.1f)
						: (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
			ColourGradient buttonGradient(
					buttonColour.withAlpha(0.4f),
					buttonBounds.getCentre(),
					buttonColour.withAlpha(0.1f),
					buttonBounds.getBottomRight(),
					true);
			g.setGradientFill(buttonGradient);
			g.fillRoundedRectangle(buttonBounds, cornerSize);

			Path p;
			p.startNewSubPath(arrowZone.getX() + 3.0f,
					arrowZone.getY() + arrowZone.getHeight() * 0.45f);
			p.lineTo(static_cast<float>(arrowZone.getCentreX()),
					arrowZone.getY() + arrowZone.getHeight() * 0.575f);
			p.lineTo(arrowZone.getRight() - 3.0f,
					arrowZone.getY() + arrowZone.getHeight() * 0.45f);

			Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour : mComboBoxTextColour.darker(0.3f))));
			g.setColour(textColour);
			g.strokePath(p, PathStrokeType(2.0f));
		}

		void ApexLookAndFeel::drawApexComboBox(Graphics& g, int width, int height,
				bool isButtonDown, int buttonX, int buttonY, int buttonW,
				int buttonH, ApexComboBox& box)
		{
			ignoreUnused(isButtonDown, buttonX, buttonY, buttonW, buttonH);

			Option<Image> maybeActiveArrow = box.getActiveArrowImage();
			if(maybeActiveArrow.isSome()) {
				Rectangle<int> arrowZone(static_cast<int>(width * 0.75f), 0, static_cast<int>(width * 0.25f), height);

				Image activeArrow = *maybeActiveArrow.getConst();
				Image hoveredArrow = *box.getHoveredArrowImage().getConst();
				Image normalArrow = *box.getNormalArrowImage().getConst();

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

				float cornerSize = jmin(width, height) * 0.1f;
				Rectangle<float> boxBounds = Rectangle<int>(0, 0, width, height).toFloat();
				Rectangle<float> troughBounds = boxBounds.reduced(2.0f, 2.0f);
				Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);
				Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
				Rectangle<int> arrowZone(static_cast<int>(width * 0.75f), 0, static_cast<int>(width * 0.2f), height);

				g.setColour(mComboBoxBackgroundColour);
				g.fillRect(boxBounds);


				g.setColour(mComboBoxTroughColour);
				g.fillRoundedRectangle(troughBounds, cornerSize);

				ColourGradient shadowGradient(
						mComboBoxShadowColour.withAlpha(0.8f),
						shadowBounds.getCentre(),
						mComboBoxShadowColour.withAlpha(0.2f),
						shadowBounds.getBottomRight(),
						true);
				g.setGradientFill(shadowGradient);
				g.fillRoundedRectangle(shadowBounds, cornerSize);

				Colour buttonColour = (box.isPopupActive() ? mButtonNormalColour.brighter(0.2f)
						: (box.isHovered() ? mButtonNormalColour.brighter(0.1f)
							: (box.isEnabled() ? mButtonNormalColour : mButtonPressedColour)));
				ColourGradient buttonGradient(
						buttonColour.withAlpha(0.4f),
						buttonBounds.getCentre(),
						buttonColour.withAlpha(0.1f),
						buttonBounds.getBottomRight(),
						true);
				g.setGradientFill(buttonGradient);
				g.fillRoundedRectangle(buttonBounds, cornerSize);

				Path p;
				p.startNewSubPath(arrowZone.getX() + 3.0f,
						arrowZone.getY() + arrowZone.getHeight() * 0.45f);
				p.lineTo(static_cast<float>(arrowZone.getCentreX()),
						arrowZone.getY() + arrowZone.getHeight() * 0.575f);
				p.lineTo(arrowZone.getRight() - 3.0f,
						arrowZone.getY() + arrowZone.getHeight() * 0.45f);

				Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
						: (box.isHovered() ? mComboBoxTextColour.brighter(0.1f)
							: (box.isEnabled() ? mComboBoxTextColour
								: mComboBoxTextColour.darker(0.3f))));
				g.setColour(textColour);
				g.strokePath(p, PathStrokeType(2.0f));
			}
		}

		void ApexLookAndFeel::positionComboBoxText(ComboBox& box, Label& label) {
			Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			label.setColour(Label::textColourId, textColour);
			label.setBounds(0, 0, static_cast<int>(box.getWidth() * 0.7f), box.getHeight());
			label.setFont(jmin(box.getWidth(), box.getHeight()) * 0.33f);
		}

		void ApexLookAndFeel::positionApexComboBoxText(ApexComboBox& box, Label& label) {
			Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isHovered() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			label.setColour(Label::textColourId, textColour);
			label.setBounds(0, 0, static_cast<int>(box.getWidth() * 0.7f), box.getHeight());
			label.setFont(jmin(box.getWidth(), box.getHeight()) * 0.33f);
		}

		void ApexLookAndFeel::drawComboBoxTextWhenNothingSelected(Graphics& g, ComboBox& box,
				Label& label) {
			Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isMouseOver() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			g.setColour(textColour);
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
			Colour textColour = (box.isPopupActive() ? mComboBoxTextColour.brighter(0.2f)
					: (box.isHovered() ? mComboBoxTextColour.brighter(0.1f)
						: (box.isEnabled() ? mComboBoxTextColour
							: mComboBoxTextColour.darker(0.3f))));
			g.setColour(textColour);
			Font font = getLabelFont(label);
			g.setFont(font);
			Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
			g.drawFittedText(box.getTextWhenNothingSelected(), textArea, label.getJustificationType(),
					jmax(1, (int)(textArea.getHeight() / font.getHeight())),
					label.getMinimumHorizontalScale());
		}

		void ApexLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
				bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
		{
			float cornerSize = jmax(5.0f, button.getWidth() * 0.1f, button.getHeight() * 0.1f);
			float width = float(button.getWidth());
			float height = float(button.getHeight());
			float fontSize = jmin(width * 0.33f, height * 0.33f);
			Rectangle<float> bounds(0.0f, 0.0f, width, height);
			Rectangle<float> troughBounds = bounds.reduced(2.0f, 2.0f);
			Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);
			Rectangle<float> buttonBounds = troughBounds.reduced(2.0f, 2.0f);

			g.setColour(mBackgroundColour);
			g.fillRect(bounds);

			g.setColour(mButtonTroughColour);
			g.fillRoundedRectangle(troughBounds, cornerSize);

			ColourGradient shadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					shadowBounds.getCentreX(),
					shadowBounds.getCentreY(),
					mSliderShadowColour.withAlpha(0.2f),
					shadowBounds.getRight(),
					shadowBounds.getBottom(),
					true);
			g.setGradientFill(shadowGradient);
			g.fillRoundedRectangle(shadowBounds, cornerSize);

			Colour buttonColour = ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown)
					? mButtonPressedColour.brighter(0.1f)
					: (shouldDrawButtonAsHighlighted ? mButtonNormalColour.brighter(0.1f)
						: (shouldDrawButtonAsDown ? mButtonPressedColour
							: mButtonNormalColour)));
			ColourGradient buttonGradient(
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

			Colour textColour = ((shouldDrawButtonAsHighlighted && shouldDrawButtonAsDown)
					? mButtonTextColour.brighter(0.05f)
					: (shouldDrawButtonAsHighlighted ? mButtonTextColour.brighter(0.1f)
						: (shouldDrawButtonAsDown ? mButtonTextColour.darker(0.1f)
							: mButtonTextColour)));
			ColourGradient textGradient(
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
					Justification::centred, 1, 1.0f);
		}

		void ApexLookAndFeel::drawApexToggleButton(Graphics& g, ApexToggleButton& button,
				bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
		{
			Option<ApexFilmStrip> maybeFilmStrip = button.getFilmStrip();
			Option<Image> maybeToggledImage = button.getToggledImage();
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
				Image toggledImage = *maybeToggledImage.getConst();
				Image hoveredImage = *button.getHoveredImage().getConst();
				Image normalImage = *button.getNormalImage().getConst();
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
				float clipLevelProportional, size_t numSteps, ApexMeter& meter) {

			int meterX = 0;
			int meterY = 0;
			int meterWidth = meter.getWidth();
			int meterHeight = meter.getHeight();

			Option<ApexFilmStrip> maybeFilmStrip = meter.getFilmStrip();
			Option<Image> maybeMaxedImage = meter.getMaxedImage();
			if(maybeFilmStrip.isSome()) {
				ApexFilmStrip filmStrip = *maybeFilmStrip.getConst();
				size_t index = static_cast<size_t>(filmStrip.getNumFrames() * levelProportional);
				g.drawImageAt(filmStrip.getFrameScaled(index,
							size_t(meterWidth),
							size_t(meterHeight)
							),
						meterX, meterY);
			}
			else if (maybeMaxedImage.isSome()) {
				Image maxedImage = *maybeMaxedImage.getConst();
				int meterPeak = static_cast<int>(meterY + (1.0f - levelProportional) * meterHeight);
				int meterDiff = meterHeight - meterPeak;
				Rectangle<int> clip(0, meterDiff, meterWidth, meterHeight);
				Image clipped = maxedImage.getClippedImage(clip);
				g.drawImageAt(clipped, meterX, meterY + meterDiff);
			}
			else {
				int meterLevelY = static_cast<int>(meterY + (1.0f - levelProportional) * meterHeight);

				float stepHeight = meterHeight / static_cast<float>(numSteps);

				Rectangle<float> bounds(
					static_cast<float>(meterX),
					static_cast<float>(meterY),
					static_cast<float>(meterWidth),
					static_cast<float>(meterHeight)
					);
				Rectangle<float> troughBounds = bounds.reduced(2.0f, 2.0f);

				Rectangle<float> meterBounds = troughBounds.reduced(2.0f, 2.0f);
				float meterHeightDiff = meterLevelY - (meterY + 2.0f);
				float meterBoundsHeight = meterBounds.getHeight();
				meterBounds = meterBounds.withY(static_cast<float>(meterLevelY) + 2.0f)
					.withHeight(static_cast<float>(meterBoundsHeight)  + 2.0f - static_cast<float>(meterHeightDiff));

				float troughX = troughBounds.getX() + 2.0f;
				float troughWidth = troughBounds.getWidth() - 2.0f;

				ColourGradient meterGradient(mMeterClipColour, troughX, static_cast<float>(meterY),
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
				for(int i = 1; i < numSteps; ++i) {
					g.drawLine(troughX, stepY, troughX + troughWidth, stepY);
					stepY += stepHeight;
				}
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
											 LookAndFeel::setColour(AlertWindow::outlineColourId,
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
													  LookAndFeel::setColour(PopupMenu::backgroundColourId,
															  mPopupMenuBackgroundColour
															  .withAlpha(0.9f));
													  LookAndFeel::setColour(PopupMenu::highlightedBackgroundColourId,
															  mPopupMenuBackgroundColour
															  .brighter(0.2f)
															  .withAlpha(0.9f));
												  }
												  break;
				case popupMenuTextColourId: {
												mPopupMenuTextColour = colour;
												LookAndFeel::setColour(popupMenuTextColourId,
														mPopupMenuTextColour);
											}
											break;
				case popupMenuHighlightColourId: {
													 mPopupMenuHighlightColour = colour;
													 LookAndFeel::setColour(popupMenuHighlightColourId,
															 mPopupMenuHighlightColour);
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

											   LookAndFeel::setColour(Slider::textBoxOutlineColourId,
													   mSliderTroughColour);
											   LookAndFeel::setColour(AlertWindow::backgroundColourId,
													   mSliderTroughColour);
											   LookAndFeel::setColour(TextEditor::backgroundColourId,
													   mSliderTroughColour
													   .withAlpha(0.85f));
											   LookAndFeel::setColour(TextEditor::focusedOutlineColourId,
													   mSliderTroughColour
													   .brighter(0.1f)
													   .withAlpha(0.85f));
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
											 LookAndFeel::setColour(TextEditor::textColourId,
													 mSliderTextColour);
											 LookAndFeel::setColour(AlertWindow::textColourId,
													 mSliderTextColour);
										 }
										 break;
				default: break;
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
			LookAndFeel::setColour(popupMenuHighlightColourId, mPopupMenuHighlightColour);
			LookAndFeel::setColour(PopupMenu::backgroundColourId, mPopupMenuBackgroundColour
					.withAlpha(0.9f));
			LookAndFeel::setColour(PopupMenu::highlightedBackgroundColourId, mPopupMenuBackgroundColour
					.brighter(0.2f)
					.withAlpha(0.9f));

			LookAndFeel::setColour(rotarySliderFillColourId, mRotarySliderFillColour);
			LookAndFeel::setColour(rotarySliderIndicatorColourId, mRotarySliderIndicatorColour);

			LookAndFeel::setColour(sliderStrokeColourId, mSliderStrokeColour);
			LookAndFeel::setColour(sliderShadowColourId, mSliderShadowColour);
			LookAndFeel::setColour(sliderTroughColourId, mSliderTroughColour);
			LookAndFeel::setColour(sliderGlowColourId, mSliderGlowColour);
			LookAndFeel::setColour(sliderTextColourId, mSliderTextColour);
			LookAndFeel::setColour(Slider::textBoxOutlineColourId, mSliderTroughColour);

			LookAndFeel::setColour(AlertWindow::backgroundColourId, mSliderTroughColour);
			LookAndFeel::setColour(AlertWindow::outlineColourId, mBackgroundColour);
			LookAndFeel::setColour(AlertWindow::textColourId, mSliderTextColour);

			LookAndFeel::setColour(TextEditor::backgroundColourId, mSliderTroughColour
					.withAlpha(0.85f));
			LookAndFeel::setColour(TextEditor::outlineColourId, mSliderTroughColour
					.withAlpha(0.85f));
			LookAndFeel::setColour(TextEditor::focusedOutlineColourId, mSliderTroughColour
					.brighter(0.1f)
					.withAlpha(0.85f));
			LookAndFeel::setColour(TextEditor::textColourId, mSliderTextColour);
		}

		Rectangle<int> ApexLookAndFeel::getActualRotaryBounds(int x, int y, int width, int height) {
#if defined USE_PHYSICAL_ROTARIES
			float diameter = static_cast<float>(jmin(width, height));
			Rectangle<float> bounds(x + (width - diameter) / 2.0f, y + (height - diameter) / 2.0f,
					diameter, diameter);
			return bounds.toNearestInt();
#elif defined USE_2D_SEMICIRCULAR_ROTARIES
			float diameter = static_cast<float>(jmin(width, height));
			Rectangle<float> bounds(x + (width - diameter) / 2.0f, y + (height - diameter) / 2.0f,
					diameter, diameter / 2.0f);
			return bounds.toNearestInt();
#else
			float diameter = static_cast<float>(jmin(width, height));
			Rectangle<float> bounds(x + (width - diameter) / 2.0f, y + (height - diameter) / 2.0f,
					diameter, diameter);
			return bounds.toNearestInt();
#endif
		}

		void ApexLookAndFeel::drawPhysicalRotary(Graphics& g, int x, int y, int width, int height,
				float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
				Slider& slider)
		{
			ignoreUnused(slider);

			Rectangle<float> rawBounds = Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			float diameter = jmin(rawBounds.getWidth(), rawBounds.getHeight());

			Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
				.withHeight(diameter)
				.reduced(2.0f, 2.0f)
				.withCentre(rawBounds.getCentre());
			Rectangle<float> circleBounds = troughBounds.reduced(2.0f, 2.0f);
			Rectangle<float> innerShadowBounds = troughBounds.expanded(2.0f, 2.0f);
			Rectangle<float> outerShadowBounds = Rectangle<float>(
					innerShadowBounds.getX() - 3.0f,
					innerShadowBounds.getY(),
					innerShadowBounds.getWidth() + 3.0f,
					innerShadowBounds.getHeight() + 8.0f);

			float lineWidth = diameter * 0.02f;

			ColourGradient innerShadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					innerShadowBounds.getX() + innerShadowBounds.getWidth() / 2.0f,
					innerShadowBounds.getY() + innerShadowBounds.getHeight() / 2.0f,
					mSliderShadowColour.withAlpha(0.2f),
					innerShadowBounds.getX() + innerShadowBounds.getWidth(),
					innerShadowBounds.getY() + innerShadowBounds.getHeight(),
					true);
			ColourGradient outerShadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					outerShadowBounds.getX() + outerShadowBounds.getWidth() / 2.0f,
					outerShadowBounds.getY() + outerShadowBounds.getHeight() / 2.0f,
					mSliderShadowColour.darker(0.2f).withAlpha(0.0f),
					outerShadowBounds.getX(),
					outerShadowBounds.getY() + outerShadowBounds.getHeight(),
					true);
			ColourGradient circleGradient(
					mRotarySliderFillColour.brighter(0.1f),
					circleBounds.getX() + circleBounds.getWidth() / 2,
					circleBounds.getY() + circleBounds.getHeight() / 2,
					mRotarySliderFillColour.darker(0.3f),
					circleBounds.getX() + circleBounds.getWidth(),
					circleBounds.getY() + circleBounds.getHeight(),
					true);
			ColourGradient circleHighlight(
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

			Point<float> indicatorStart(circleBounds.getCentreX()
					+ (radius / 2.0f) * math::cosf(angle - math::piOver2f),
					circleBounds.getCentreY()
					+ (radius / 2.0f) * math::sinf(angle - math::piOver2f));
			Point<float> indicatorEnd(circleBounds.getCentreX()
					+ radius * math::cosf(angle - math::piOver2f),
					circleBounds.getCentreY()
					+ radius * math::sinf(angle - math::piOver2f));

			ColourGradient indicatorGradient(
					mRotarySliderIndicatorColour.brighter(0.2f),
					indicatorStart,
					mRotarySliderIndicatorColour.darker(0.3f),
					indicatorEnd,
					false);
			ColourGradient indicatorBackgroundGradient(
					mSliderShadowColour.darker(0.2f).withAlpha(0.5f),
					indicatorStart,
					mSliderShadowColour.darker(0.2f).withAlpha(0.2f),
					indicatorEnd,
					false);

			Line<float> indicatorLine = Line<float>(indicatorStart, indicatorEnd);
			Path indicator;
			indicator.addLineSegment(indicatorLine, lineWidth);
			Path indicatorBackground;
			indicatorBackground.addLineSegment(indicatorLine, lineWidth + 2.0f);

			g.setGradientFill(indicatorBackgroundGradient);
			g.strokePath(indicatorBackground, PathStrokeType(lineWidth + 2.0f,
						PathStrokeType::curved,
						PathStrokeType::rounded));
			g.setGradientFill(indicatorGradient);
			g.strokePath(indicator, PathStrokeType(lineWidth, PathStrokeType::curved,
						PathStrokeType::rounded));

			ColourGradient glowGradient(
					mRotarySliderIndicatorColour.withAlpha(0.0f),
					indicatorStart,
					mRotarySliderIndicatorColour.brighter(0.4f).withAlpha(0.3f),
					indicatorEnd,
					false);
			g.setGradientFill(glowGradient);
			g.strokePath(indicator, PathStrokeType(lineWidth, PathStrokeType::curved,
						PathStrokeType::rounded));
		}

		void ApexLookAndFeel::drawSemiCircularRotary(Graphics& g, int x, int y, int width,
				int height, float sliderPos, const float rotaryStartAngle,
				const float rotaryEndAngle, Slider& slider)
		{
			ignoreUnused(rotaryStartAngle, rotaryEndAngle, slider);

			Rectangle<float> rawBounds = Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			float diameter = jmin(rawBounds.getWidth(), rawBounds.getHeight());

			Rectangle<float> troughBounds = rawBounds.withWidth(diameter)
				.withHeight(diameter).reduced(2.0f, 2.0f)
				.withCentre(rawBounds.getCentre());
			Rectangle<float> circleBounds = troughBounds
				.reduced(2.0f, 2.0f)
				.withCentre(Point<float>(
							troughBounds.getCentreX(),
							troughBounds.getCentreY() - 2.0f));
			Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);

			Point<float> troughCentre = troughBounds.getCentre();
			Point<float> shadowCentre = shadowBounds.getCentre();
			Point<float> circleCentre = circleBounds.getCentre();

			Point<float> troughLeft(troughBounds.getX(), troughBounds.getCentreY());
			Point<float> shadowLeft(shadowBounds.getX(), shadowBounds.getCentreY());

			Point<float> troughRight(troughBounds.getX() + troughBounds.getWidth(),
					troughBounds.getCentreY());
			Point<float> shadowRight(shadowBounds.getX() + shadowBounds.getWidth(),
					shadowBounds.getCentreY());

			float troughRadius = troughBounds.getWidth() / 2.0f;
			float shadowRadius = shadowBounds.getWidth() / 2.0f;
			float circleRadius = circleBounds.getWidth() / 2.0f;

			float endAngle = 2.0f * math::pif;
			float startAngle = math::pif;
			float angle = startAngle + sliderPos * (endAngle - startAngle);

			Path trough;
			trough.addCentredArc(troughCentre.getX(), troughCentre.getY(),
					troughRadius / 2.0f, troughRadius / 2.0f, 0.0f,
					startAngle + math::piOver2f, endAngle + math::piOver2f, true);

			g.setColour(mSliderTroughColour);
			g.strokePath(trough, PathStrokeType(troughRadius, PathStrokeType::mitered,
						PathStrokeType::butt));

			ColourGradient shadowGradient(
					mSliderShadowColour.withAlpha(0.9f),
					shadowBounds.getX() + shadowBounds.getWidth() / 2.0f,
					shadowBounds.getY() + shadowBounds.getHeight() / 2.0f,
					mSliderShadowColour.withAlpha(0.3f),
					shadowBounds.getX(),
					shadowBounds.getY(),
					true);

			Path shadow;
			shadow.addCentredArc(shadowCentre.getX(), shadowCentre.getY(),
					shadowRadius / 2.0f, shadowRadius / 2.0f, 0.0f,
					startAngle + math::piOver2f, endAngle + math::piOver2f, true);
			g.setGradientFill(shadowGradient);
			g.strokePath(shadow, PathStrokeType(shadowRadius, PathStrokeType::mitered,
						PathStrokeType::butt));

			Point<float> thumbStart = circleCentre;
			Point<float> thumbEnd(circleCentre.getX()
					+ circleRadius * math::cosf(angle),
					circleCentre.getY()
					+ circleRadius * math::sinf(angle));

			float thumbWidth = circleRadius * 0.1f;

			float strokeRadius = circleRadius + 5.0f;

			Path fillPath;
			fillPath.addCentredArc(circleCentre.getX(), circleCentre.getY(), circleRadius / 2.0f,
					circleRadius / 2.0f, 0.0f, startAngle + math::piOver2f,
					angle + math::piOver2f, true);

			ColourGradient strokeGradient(
					mSliderStrokeColour.withAlpha(0.5f),
					thumbStart,
					mSliderStrokeColour.withAlpha(0.5f),
					Point<float>(thumbStart.getX(), thumbStart.getY() + strokeRadius),
					true);
			strokeGradient.addColour(0.5f, mSliderStrokeColour.withAlpha(0.2f));
			g.setGradientFill(strokeGradient);
			g.strokePath(fillPath, PathStrokeType(circleRadius, PathStrokeType::mitered,
						PathStrokeType::butt));

			Line<float> thumbLine(thumbStart, thumbEnd);
			Path thumb;
			thumb.addLineSegment(thumbLine, thumbWidth);
			ColourGradient thumbGradient(
					mSliderStrokeColour.brighter(0.2f),
					thumbStart,
					mSliderStrokeColour.brighter(0.2f),
					thumbEnd,
					true);
			thumbGradient.addColour(0.5f, mSliderStrokeColour.darker(0.8f));
			g.setGradientFill(thumbGradient);
			g.strokePath(thumb, PathStrokeType(thumbWidth, PathStrokeType::curved,
						PathStrokeType::rounded));
			float cornerSize = thumbWidth * 0.1f;
			ColourGradient glowGradient(
					mSliderGlowColour.brighter(0.2f).withAlpha(0.5f),
					thumbStart,
					mSliderGlowColour.brighter(0.2f).withAlpha(0.5f),
					thumbEnd,
					true);
			glowGradient.addColour(0.5f, mSliderGlowColour.darker(0.2f).withAlpha(0.5f));
			g.setGradientFill(glowGradient);
			g.strokePath(thumb, PathStrokeType(thumbWidth - cornerSize, PathStrokeType::curved,
						PathStrokeType::rounded));
		}

		void ApexLookAndFeel::drawCircularFillRotary(Graphics& g, int x, int y, int width,
				int height, float sliderPos, const float rotaryStartAngle,
				const float rotaryEndAngle, Slider& slider)
		{
			ignoreUnused(rotaryStartAngle, rotaryEndAngle, slider);

			Rectangle<float> rawBounds = Rectangle<int>(x, y, width, height).toFloat();

			g.setColour(mBackgroundColour);
			g.fillRect(rawBounds);

			float diameter = static_cast<float>(jmin(width, height));

			Rectangle<float> troughBounds = rawBounds
				.withWidth(diameter)
				.withHeight(diameter)
				.reduced(2.0f, 2.0f)
				.withCentre(rawBounds.getCentre());
			Rectangle<float> circleBounds = troughBounds
				.withWidth(jmax(diameter * sliderPos, 20.0f))
				.withHeight(jmax(diameter * sliderPos, 20.0f))
				.reduced(2.0f, 2.0f)
				.withCentre(troughBounds.getCentre());
			Rectangle<float> shadowBounds = troughBounds.expanded(2.0f, 2.0f);

			ColourGradient shadowGradient(
					mSliderShadowColour.withAlpha(0.8f),
					shadowBounds.getCentreX(),
					shadowBounds.getCentreY(),
					mSliderShadowColour.withAlpha(0.2f),
					shadowBounds.getRight(),
					shadowBounds.getBottom(),
					true);
			ColourGradient circleGradient(
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
