#pragma once

#include <functional>
#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex {
	namespace ui {

#define SliderFloatingTextBoxStartWidth	 25
#define SliderFloatingTextBoxStartHeight 30

		class ApexSlider : public juce::Slider,
						   public juce::TextEditor::Listener,
						   public juce::FocusChangeListener,
						   public juce::Label::Listener {
		  public:
			ApexSlider(juce::Slider::SliderStyle style,
					   std::function<double(double)> proportionToValueFunc,
					   std::function<double(double)> valueToProportionFunc,
					   ApexFilmStrip strip);
			ApexSlider(juce::Slider::SliderStyle style,
					   std::function<double(double)> proportionToValueFunc,
					   std::function<double(double)> valueToProportionFunc);

			~ApexSlider() override;

			virtual juce::String getTextFromValue(double value) override;
			virtual double getValueFromText(const juce::String& text) override;

			virtual void mouseDown(const juce::MouseEvent& e) override;
			virtual void mouseUp(const juce::MouseEvent& e) override;
			virtual void mouseDoubleClick(const juce::MouseEvent& e) override;
			virtual void mouseDrag(const juce::MouseEvent& e) override;
			virtual void mouseWheelMove(const juce::MouseEvent& e,
										const juce::MouseWheelDetails& wheel) override;

			void globalFocusChanged(juce::Component* focusedComponent) override;

			void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
			void textEditorEscapeKeyPressed(juce::TextEditor& editor) override;
			void textEditorFocusLost(juce::TextEditor& editor) override;

			void labelTextChanged(juce::Label* label) override;

			void setXScaleFactor(float scaleFactor);
			float getXScaleFactor() const;
			void setYScaleFactor(float scaleFactor);
			float getYScaleFactor() const;

			virtual bool isValueValid(double value) const;

			void setPopupTextBoxFont(juce::Font font);
			void setTextBoxFont(juce::Font font);
			void setTextBoxFontColour(juce::Colour newColour);
			void setTextBoxBounds(juce::Rectangle<int> bounds);
			juce::Label* getTextBox();

			void setNormalSensitivity(double sensitivity, double initialVelocity);
			void setFineSensitivity(double sensitivity, double initialVelocity);
			void setNormalModeIsVelocityBased(bool set);

			void hideEditor();

			Option<ApexFilmStrip> getFilmStrip();

			double getValueFromProportion(double prop) const;
			double getProportionFromValue(double value) const;

			void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel);

			virtual void paint(juce::Graphics& g) override;

		  protected:
			std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
			ApexFilmStrip mFilmStrip;
			bool mUsesFilmStrip = false;

			std::function<double(double)> mProportionToValueFunc;
			std::function<double(double)> mValueToProportionFunc;

			float mXScaleFactor = 1.0f;
			float mYScaleFactor = 1.0f;
			double mNormalSensitivity = 2.4;
			double mNormalInitialVelocity = 0.08;
			double mFineSensitivity = 0.8;
			double mFineInitialVelocity = 0.04;
			bool mNormalModeIsVelocityMode = false;

			int mNumTextBoxCharacters = 6;

			juce::Font mPopupTextBoxFont;
			std::unique_ptr<juce::TextEditor> mPopupTextBox;

			juce::Font mTextBoxFont;
			juce::Label mTextBox;

			juce::Colour mTextBoxFontColour = juce::Colour(0xffcde3ec);

			virtual bool isInBounds(juce::Point<int> p);

		  private:
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexSlider)
		};
	} // namespace ui
} // namespace apex
