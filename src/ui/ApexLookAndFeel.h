#pragma once

#include "../base/StandardIncludes.h"

namespace apex {
	namespace ui {

		class ApexSlider;
		class ApexComboBox;
		class ApexToggleButton;
		class ApexMeter;

		class ApexLookAndFeel : public LookAndFeel_V4 {
			public:

				//#define USE_PHYSICAL_ROTARIES
				//#define USE_2D_SEMICIRCULAR_ROTARIES

				enum ApexColourId {
					backgroundColourId = 0x11110000,
					buttonShadowColourId = 0x11110001,
					buttonNormalColourId = 0x11110002,
					buttonPressedColourId = 0x11110003,
					buttonTroughColourId = 0x11110004,
					buttonTextColourId = 0x11110005,
					comboBoxBackgroundColourId = 0x11110006,
					comboBoxShadowColourId = 0x11110007,
					comboBoxTroughColourId = 0x11110008,
					comboBoxTextColourId = 0x11110009,
					meterClipColourId = 0x11110010,
					meterLowerColourId = 0x11110011,
					meterTroughColourId = 0x11110012,
					meterUpperColourId = 0x11110013,
					popupMenuBackgroundColourId = 0x11110014,
					popupMenuTextColourId = 0x11110015,
					popupMenuHighlightColourId = 0x11110016,
					rotarySliderFillColourId = 0x11110017,
					rotarySliderIndicatorColourId = 0x11110018,
					sliderStrokeColourId = 0x11110019,
					sliderShadowColourId = 0x11110020,
					sliderTroughColourId = 0x11110021,
					sliderGlowColourId = 0x11110022,
					sliderTextColourId = 0x11110023
				};

				ApexLookAndFeel();

				virtual void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
						float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
						Slider& slider) override;
				virtual void drawRotaryApexSlider(Graphics& g, int x, int y, int width, int height,
						float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
						ApexSlider& slider);

				virtual void drawLinearSliderBackground(Graphics& g, int x, int y, int width,
						int height, float sliderPos, float minSliderPos, float maxSliderPos,
						const Slider::SliderStyle style, Slider& slider) override;
				virtual void drawLinearApexSliderBackground(Graphics& g, int x, int y, int width,
						int height, float sliderPosProp, const Slider::SliderStyle style,
						ApexSlider& slider);

				virtual void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
						float sliderPos, float minSliderPos, float maxSliderPos,
						const Slider::SliderStyle style, Slider& slider) override;
				virtual void drawLinearApexSliderThumb(Graphics& g, int x, int y, int width,
						int height, float sliderPos, ApexSlider& slider);

				virtual void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
						float sliderPos, float minSliderPos, float maxSliderPos,
						const Slider::SliderStyle style, Slider& slider) override;
				virtual void drawLinearApexSlider(Graphics& g, int x, int y, int width,
						int height, float sliderPos, const Slider::SliderStyle style,
						ApexSlider& slider);

				virtual void drawPopupMenuBackground(Graphics& g, int width, int height) override;

				virtual void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
						bool isSeparator, bool isActive, bool isHighlighted,
						bool isTicked, bool hasSubMenu, const String& text,
						const String& shortcutKeyText, const Drawable* icon,
						const Colour* textColour) override;

				virtual void drawComboBox(Graphics& g, int width, int height, bool isButtonDown,
						int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;
				virtual void drawApexComboBox(Graphics& g, int width, int height, bool isButtonDown,
						int buttonX, int buttonY, int buttonW, int buttonH, ApexComboBox& box);

				virtual void positionComboBoxText(ComboBox& box, Label& label) override;
				virtual void positionApexComboBoxText(ApexComboBox& box, Label& label);

				virtual void drawComboBoxTextWhenNothingSelected(Graphics& g,
						ComboBox& box, Label& label) override;
				virtual void drawApexComboBoxTextWhenNothingSelected(Graphics& g,
						ApexComboBox& box, Label& label);

				virtual void drawToggleButton(Graphics& g, ToggleButton& button,
						bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
				virtual void drawApexToggleButton(Graphics& g, ApexToggleButton& button,
						bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown);

				virtual void drawApexMeter(Graphics& g, float levelProportional,
						float clipLevelProportional, size_t numSteps, ApexMeter& meter);

				virtual void setFont(Font newFont);

				virtual void setColour(ApexColourId id, Colour colour);
				virtual Colour getColour(ApexColourId id);

				virtual Rectangle<int> getActualRotaryBounds(int x, int y, int width, int height);

			protected:
				virtual void drawPhysicalRotary(Graphics& g, int x, int y, int width, int height,
						float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
						Slider& slider);
				virtual void drawSemiCircularRotary(Graphics& g, int x, int y, int width, int height,
						float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
						Slider& slider);
				virtual void drawCircularFillRotary(Graphics& g, int x, int y, int width, int height,
						float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle,
						Slider& slider);

				Colour mBackgroundColour = Colour(0xff282c34);
				Colour mButtonShadowColour = Colour(0xff14161a);
				Colour mButtonNormalColour = Colour(0xff6a90af);
				Colour mButtonPressedColour = Colour(0xff3d5264);
				Colour mButtonTroughColour = Colour(0xff1e2128);
				Colour mButtonTextColour = Colour(0xff8598b5);
				Colour mComboBoxBackgroundColour = Colour(0xff282c34);
				Colour mComboBoxShadowColour = Colour(0xff14161a);
				Colour mComboBoxTroughColour = Colour(0xff1e2128);
				Colour mComboBoxTextColour = Colour(0xff8598b5);
				Colour mMeterTroughColour = Colour(0xff1e2128);
				Colour mMeterLowerColour = Colour(0xff29967f);
				Colour mMeterUpperColour = Colour(0xff297dff);
				Colour mMeterClipColour = Colour(0xffffff7f);
				Colour mPopupMenuBackgroundColour = Colour(0xff1e2128);
				Colour mPopupMenuTextColour = Colour(0xff8598b5);
				Colour mPopupMenuHighlightColour = Colour(0xff1c1f26);
				Colour mRotarySliderFillColour = Colour(0xff2e324c);
				Colour mRotarySliderIndicatorColour = Colour(0xff1276c8);
				Colour mSliderStrokeColour = Colour(0xff1276c8);
				Colour mSliderShadowColour = Colour(0xff14161a);
				Colour mSliderTroughColour = Colour(0xff1e2128);
				Colour mSliderGlowColour = Colour(0xff1793ff);
				Colour mSliderTextColour = Colour (0xff8598b5);

				Font mFont;

				void registerColours();
		};
	}
}
