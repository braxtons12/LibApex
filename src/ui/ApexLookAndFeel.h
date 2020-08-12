#pragma once

#include "../base/StandardIncludes.h"

namespace apex::ui {
	class ApexSlider;
	class ApexComboBox;
	class ApexToggleButton;
	class ApexMeter;

	/// @brief Apex Audio look and feel implementation
	class ApexLookAndFeel : public juce::LookAndFeel_V4 {
	  public:
		//#define USE_PHYSICAL_ROTARIES
		//#define USE_2D_SEMICIRCULAR_ROTARIES

		/// @brief list of available Apex Audio specific colors
		enum class ApexColourId
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

		/// @brief Creates a default `ApexLookAndFeel`
		ApexLookAndFeel() noexcept;

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
		auto drawRotarySlider(juce::Graphics& g,
							  int x,
							  int y,
							  int width,
							  int height,
							  float sliderPos,
							  float rotaryStartAngle,
							  float rotaryEndAngle,
							  juce::Slider& slider) noexcept -> void override;

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
		virtual auto drawRotaryApexSlider(juce::Graphics& g,
										  int x,
										  int y,
										  int width,
										  int height,
										  float sliderPos,
										  float rotaryStartAngle,
										  float rotaryEndAngle,
										  ApexSlider& slider) noexcept -> void;

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
		auto drawLinearSliderBackground(juce::Graphics& g,
										int x,
										int y,
										int width,
										int height,
										float sliderPos,
										float minSliderPos,
										float maxSliderPos,
										juce::Slider::SliderStyle style,
										juce::Slider& slider) noexcept -> void override;

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
		virtual auto drawLinearApexSliderBackground(juce::Graphics& g,
													int x,
													int y,
													int width,
													int height,
													float sliderPosProp,
													juce::Slider::SliderStyle style,
													ApexSlider& slider) noexcept -> void;

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
		auto drawLinearSliderThumb(juce::Graphics& g,
								   int x,
								   int y,
								   int width,
								   int height,
								   float sliderPos,
								   float minSliderPos,
								   float maxSliderPos,
								   juce::Slider::SliderStyle style,
								   juce::Slider& slider) noexcept -> void override;

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
		virtual auto drawLinearApexSliderThumb(juce::Graphics& g,
											   int x,
											   int y,
											   int width,
											   int height,
											   float sliderPos,
											   ApexSlider& slider) noexcept -> void;

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
		auto drawLinearSlider(juce::Graphics& g,
							  int x,
							  int y,
							  int width,
							  int height,
							  float sliderPos,
							  float minSliderPos,
							  float maxSliderPos,
							  juce::Slider::SliderStyle style,
							  juce::Slider& slider) noexcept -> void override;

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
		virtual auto drawLinearApexSlider(juce::Graphics& g,
										  int x,
										  int y,
										  int width,
										  int height,
										  float sliderPos,
										  juce::Slider::SliderStyle style,
										  ApexSlider& slider) noexcept -> void;

		/// @brief Draws the background for a popupmenu
		///
		/// @param g - The graphics context to use for drawing
		/// @param width - The width of the menu
		/// @param height - The height of the menu
		auto
		drawPopupMenuBackground(juce::Graphics& g, int width, int height) noexcept -> void override;

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
		auto drawPopupMenuItem(juce::Graphics& g,
							   const juce::Rectangle<int>& area,
							   bool isSeparator,
							   bool isActive,
							   bool isHighlighted,
							   bool isTicked,
							   bool hasSubMenu,
							   const juce::String& text,
							   const juce::String& shortcutKeyText,
							   const juce::Drawable* icon,
							   const juce::Colour* textColour) noexcept -> void override;

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
		auto drawComboBox(juce::Graphics& g,
						  int width,
						  int height,
						  bool isButtonDown,
						  int buttonX,
						  int buttonY,
						  int buttonW,
						  int buttonH,
						  juce::ComboBox& box) noexcept -> void override;

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
		virtual auto drawApexComboBox(juce::Graphics& g,
									  int width,
									  int height,
									  bool isButtonDown,
									  int buttonX,
									  int buttonY,
									  int buttonW,
									  int buttonH,
									  ApexComboBox& box) noexcept -> void;

		/// @brief Positions the text of a `juce::ComboBox` within the combobox's bounds
		///
		/// @param box - The combo box to position the text for
		/// @param label - The label making up the text
		auto
		positionComboBoxText(juce::ComboBox& box, juce::Label& label) noexcept -> void override;

		/// @brief Positions the text of an `ApexComboBox` within the combobox's bounds
		///
		/// @param box - The combo box to position the text for
		/// @param label - The label making up the text
		virtual auto
		positionApexComboBoxText(ApexComboBox& box, juce::Label& label) noexcept -> void;

		/// @brief Draws the correct text for a `juce::ComboBox` when no entry has been chosen
		///
		/// @param g - The graphics context to draw with
		/// @param box - The combo box to draw for
		/// @param label - The label making up the text
		auto drawComboBoxTextWhenNothingSelected(juce::Graphics& g,
												 juce::ComboBox& box,
												 juce::Label& label) noexcept -> void override;

		/// @brief Draws the correct text for an `ApexComboBox` when no entry has been chosen
		///
		/// @param g - The graphics context to draw with
		/// @param box - The combo box to draw for
		/// @param label - The label making up the text
		virtual auto drawApexComboBoxTextWhenNothingSelected(juce::Graphics& g,
															 ApexComboBox& box,
															 juce::Label& label) noexcept -> void;

		/// @brief Draws a `juce::ToggleButton`
		///
		/// @param g - The graphics context to draw with
		/// @param button - The button to draw
		/// @param shouldDrawButtonAsHighlighted - If the button is hovered
		/// @param shouldDrawButtonAsDown - If the button is currently pressed
		auto drawToggleButton(juce::Graphics& g,
							  juce::ToggleButton& button,
							  bool shouldDrawButtonAsHighlighted,
							  bool shouldDrawButtonAsDown) noexcept -> void override;

		/// @brief Draws an `ApexToggleButton`
		///
		/// @param g - The graphics context to draw with
		/// @param button - The button to draw
		/// @param shouldDrawButtonAsHighlighted - If the button is hovered
		/// @param shouldDrawButtonAsDown - If the button is currently pressed
		virtual auto drawApexToggleButton(juce::Graphics& g,
										  ApexToggleButton& button,
										  bool shouldDrawButtonAsHighlighted,
										  bool shouldDrawButtonAsDown) noexcept -> void;

		/// @brief Draws an `ApexMeter`
		///
		/// @param g - The graphics context to draw with
		/// @param levelProportional - The current level of the meter, proportional to its size
		/// @param clipLevelProportional - The level clipping indication should start, proportional
		/// to the meter's size
		/// @param numSteps - The number of equally spaced level ticks in the meter
		/// @param meter - The meter to draw
		virtual auto drawApexMeter(juce::Graphics& g,
								   float levelProportional,
								   float clipLevelProportional,
								   size_t numSteps,
								   ApexMeter& meter) noexcept -> void;

		/// @brief Sets the default font size used by this `ApexLookAndFeel`
		///
		/// @param newFont - The new font size
		virtual auto setFont(juce::Font newFont) noexcept -> void;

		/// @brief Sets the color indicated by id to the given one
		///
		/// @param id - The id of the color to set
		/// @param colour - The new colour for the given id
		virtual auto setColour(ApexColourId id, juce::Colour colour) noexcept -> void;

		/// @brief Returns the color corresponding to the given id
		///
		/// @param id - The id to get the color for
		///
		/// @return - The corresponding color
		[[nodiscard]] virtual auto getColour(ApexColourId id) const noexcept -> juce::Colour;

		/// @brief Returns the actual rectangulary bounds of a rotary slider, given the raw bounds
		///
		/// @param x - The raw x position
		/// @param y - The raw y position
		/// @param width - The raw width
		/// @param height - The raw height
		///
		/// @return - The actualy bounds of the rotary slider's on screen position
		[[nodiscard]] virtual auto
		getActualRotaryBounds(int x, int y, int width, int height) const noexcept
			-> juce::Rectangle<int>;

	  protected:
		virtual auto drawPhysicalRotary(juce::Graphics& g,
										int x,
										int y,
										int width,
										int height,
										float sliderPos,
										float rotaryStartAngle,
										float rotaryEndAngle,
										juce::Slider& slider) noexcept -> void;
		virtual auto drawSemiCircularRotary(juce::Graphics& g,
											int x,
											int y,
											int width,
											int height,
											float sliderPos,
											float rotaryStartAngle,
											float rotaryEndAngle,
											juce::Slider& slider) noexcept -> void;
		virtual auto drawCircularFillRotary(juce::Graphics& g,
											int x,
											int y,
											int width,
											int height,
											float sliderPos,
											float rotaryStartAngle,
											float rotaryEndAngle,
											juce::Slider& slider) noexcept -> void;

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

		auto registerColours() noexcept -> void;
	};
} // namespace apex::ui
