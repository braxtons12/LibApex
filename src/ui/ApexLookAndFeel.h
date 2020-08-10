#pragma once

#include "../base/StandardIncludes.h"

namespace apex {
	namespace ui {

		class ApexSlider;
		class ApexComboBox;
		class ApexToggleButton;
		class ApexMeter;

		class ApexLookAndFeel : public juce::LookAndFeel_V4 {
		  public:
			//#define USE_PHYSICAL_ROTARIES
			//#define USE_2D_SEMICIRCULAR_ROTARIES

			enum ApexColourId
			{
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

			virtual void drawRotarySlider(juce::Graphics& g,
										  int x,
										  int y,
										  int width,
										  int height,
										  float sliderPos,
										  const float rotaryStartAngle,
										  const float rotaryEndAngle,
										  juce::Slider& slider) override;
			virtual void drawRotaryApexSlider(juce::Graphics& g,
											  int x,
											  int y,
											  int width,
											  int height,
											  float sliderPos,
											  const float rotaryStartAngle,
											  const float rotaryEndAngle,
											  ApexSlider& slider);

			virtual void drawLinearSliderBackground(juce::Graphics& g,
													int x,
													int y,
													int width,
													int height,
													float sliderPos,
													float minSliderPos,
													float maxSliderPos,
													const juce::Slider::SliderStyle style,
													juce::Slider& slider) override;
			virtual void drawLinearApexSliderBackground(juce::Graphics& g,
														int x,
														int y,
														int width,
														int height,
														float sliderPosProp,
														const juce::Slider::SliderStyle style,
														ApexSlider& slider);

			virtual void drawLinearSliderThumb(juce::Graphics& g,
											   int x,
											   int y,
											   int width,
											   int height,
											   float sliderPos,
											   float minSliderPos,
											   float maxSliderPos,
											   const juce::Slider::SliderStyle style,
											   juce::Slider& slider) override;
			virtual void drawLinearApexSliderThumb(juce::Graphics& g,
												   int x,
												   int y,
												   int width,
												   int height,
												   float sliderPos,
												   ApexSlider& slider);

			virtual void drawLinearSlider(juce::Graphics& g,
										  int x,
										  int y,
										  int width,
										  int height,
										  float sliderPos,
										  float minSliderPos,
										  float maxSliderPos,
										  const juce::Slider::SliderStyle style,
										  juce::Slider& slider) override;
			virtual void drawLinearApexSlider(juce::Graphics& g,
											  int x,
											  int y,
											  int width,
											  int height,
											  float sliderPos,
											  const juce::Slider::SliderStyle style,
											  ApexSlider& slider);

			virtual void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;

			virtual void drawPopupMenuItem(juce::Graphics& g,
										   const juce::Rectangle<int>& area,
										   bool isSeparator,
										   bool isActive,
										   bool isHighlighted,
										   bool isTicked,
										   bool hasSubMenu,
										   const juce::String& text,
										   const juce::String& shortcutKeyText,
										   const juce::Drawable* icon,
										   const juce::Colour* textColour) override;

			virtual void drawComboBox(juce::Graphics& g,
									  int width,
									  int height,
									  bool isButtonDown,
									  int buttonX,
									  int buttonY,
									  int buttonW,
									  int buttonH,
									  juce::ComboBox& box) override;
			virtual void drawApexComboBox(juce::Graphics& g,
										  int width,
										  int height,
										  bool isButtonDown,
										  int buttonX,
										  int buttonY,
										  int buttonW,
										  int buttonH,
										  ApexComboBox& box);

			virtual void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override;
			virtual void positionApexComboBoxText(ApexComboBox& box, juce::Label& label);

			virtual void drawComboBoxTextWhenNothingSelected(juce::Graphics& g,
															 juce::ComboBox& box,
															 juce::Label& label) override;
			virtual void drawApexComboBoxTextWhenNothingSelected(juce::Graphics& g,
																 ApexComboBox& box,
																 juce::Label& label);

			virtual void drawToggleButton(juce::Graphics& g,
										  juce::ToggleButton& button,
										  bool shouldDrawButtonAsHighlighted,
										  bool shouldDrawButtonAsDown) override;
			virtual void drawApexToggleButton(juce::Graphics& g,
											  ApexToggleButton& button,
											  bool shouldDrawButtonAsHighlighted,
											  bool shouldDrawButtonAsDown);

			virtual void drawApexMeter(juce::Graphics& g,
									   float levelProportional,
									   float clipLevelProportional,
									   size_t numSteps,
									   ApexMeter& meter);

			virtual void setFont(juce::Font newFont);

			virtual void setColour(ApexColourId id, juce::Colour colour);
			virtual juce::Colour getColour(ApexColourId id);

			virtual juce::Rectangle<int> getActualRotaryBounds(int x, int y, int width, int height);

		  protected:
			virtual void drawPhysicalRotary(juce::Graphics& g,
											int x,
											int y,
											int width,
											int height,
											float sliderPos,
											const float rotaryStartAngle,
											const float rotaryEndAngle,
											juce::Slider& slider);
			virtual void drawSemiCircularRotary(juce::Graphics& g,
												int x,
												int y,
												int width,
												int height,
												float sliderPos,
												const float rotaryStartAngle,
												const float rotaryEndAngle,
												juce::Slider& slider);
			virtual void drawCircularFillRotary(juce::Graphics& g,
												int x,
												int y,
												int width,
												int height,
												float sliderPos,
												const float rotaryStartAngle,
												const float rotaryEndAngle,
												juce::Slider& slider);

			juce::Colour mBackgroundColour = juce::Colour(0xff282c34);
			juce::Colour mButtonShadowColour = juce::Colour(0xff14161a);
			juce::Colour mButtonNormalColour = juce::Colour(0xff6a90af);
			juce::Colour mButtonPressedColour = juce::Colour(0xff3d5264);
			juce::Colour mButtonTroughColour = juce::Colour(0xff1e2128);
			juce::Colour mButtonTextColour = juce::Colour(0xff8598b5);
			juce::Colour mComboBoxBackgroundColour = juce::Colour(0xff282c34);
			juce::Colour mComboBoxShadowColour = juce::Colour(0xff14161a);
			juce::Colour mComboBoxTroughColour = juce::Colour(0xff1e2128);
			juce::Colour mComboBoxTextColour = juce::Colour(0xff8598b5);
			juce::Colour mMeterTroughColour = juce::Colour(0xff1e2128);
			juce::Colour mMeterLowerColour = juce::Colour(0xff29967f);
			juce::Colour mMeterUpperColour = juce::Colour(0xff297dff);
			juce::Colour mMeterClipColour = juce::Colour(0xffffff7f);
			juce::Colour mPopupMenuBackgroundColour = juce::Colour(0xff1e2128);
			juce::Colour mPopupMenuTextColour = juce::Colour(0xff8598b5);
			juce::Colour mPopupMenuHighlightColour = juce::Colour(0xff1c1f26);
			juce::Colour mRotarySliderFillColour = juce::Colour(0xff2e324c);
			juce::Colour mRotarySliderIndicatorColour = juce::Colour(0xff1276c8);
			juce::Colour mSliderStrokeColour = juce::Colour(0xff1276c8);
			juce::Colour mSliderShadowColour = juce::Colour(0xff14161a);
			juce::Colour mSliderTroughColour = juce::Colour(0xff1e2128);
			juce::Colour mSliderGlowColour = juce::Colour(0xff1793ff);
			juce::Colour mSliderTextColour = juce::Colour(0xff8598b5);

			juce::Font mFont;

			void registerColours();
		};
	} // namespace ui
} // namespace apex
