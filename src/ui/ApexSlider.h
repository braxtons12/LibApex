#pragma once

#include <functional>
#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex {
	namespace ui {

#define SliderFloatingTextBoxStartWidth  25
#define SliderFloatingTextBoxStartHeight 30

		class ApexSlider: public Slider, public TextEditor::Listener, public FocusChangeListener, public Label::Listener {

			public:
				ApexSlider(Slider::SliderStyle style, std::function<float(float)> proportionToValueFunc,
						std::function<float(float)> valueToProportionFunc, ApexFilmStrip strip);
				ApexSlider(Slider::SliderStyle style, std::function<float(float)> proportionToValueFunc,
						std::function<float(float)> valueToProportionFunc);

				~ApexSlider();

				virtual String getTextFromValue(double value) override;
				virtual double getValueFromText(const String& text) override;

				virtual void mouseDown(const MouseEvent& e) override;
				virtual void mouseUp(const MouseEvent& e) override;
				virtual void mouseDoubleClick(const MouseEvent& e) override;
				virtual void mouseDrag(const MouseEvent& e) override;
				virtual void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) override;

				void globalFocusChanged(Component* focusedComponent) override;

				void textEditorReturnKeyPressed(TextEditor& editor) override;
				void textEditorEscapeKeyPressed(TextEditor& editor) override;
				void textEditorFocusLost(TextEditor& editor) override;

				void labelTextChanged(Label* label) override;

				void setXScaleFactor(float scaleFactor);
				float getXScaleFactor() const;
				void setYScaleFactor(float scaleFactor);
				float getYScaleFactor() const;

				virtual bool isValueValid(double value) const;

				void setPopupTextBoxFont(Font font);
				void setTextBoxFont(Font font);
				void setTextBoxFontColour(Colour newColour);
				void setTextBoxBounds(Rectangle<int> bounds);
				Label* getTextBox();

				void setNormalSensitivity(double sensitivity, double initialVelocity);
				void setFineSensitivity(double sensitivity, double initialVelocity);
				void setNormalModeIsVelocityBased(bool set);

				void hideEditor();

				Option<ApexFilmStrip> getFilmStrip();

				float getValueFromProportion(float prop) const;
				float getProportionFromValue(float value) const;

				void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel);

				virtual void paint(Graphics& g) override;

			protected:
				std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
				ApexFilmStrip mFilmStrip;
				bool mUsesFilmStrip = false;

				std::function<float(float)> mProportionToValueFunc;
				std::function<float(float)> mValueToProportionFunc;

				float mXScaleFactor = 1.0f;
				float mYScaleFactor = 1.0f;
				double mNormalSensitivity = 2.4;
				double mNormalInitialVelocity = 0.08;
				double mFineSensitivity = 0.8;
				double mFineInitialVelocity = 0.04;
				bool mNormalModeIsVelocityMode = false;

				int mNumTextBoxCharacters = 6;

				Font mPopupTextBoxFont;
				std::unique_ptr<TextEditor> mPopupTextBox;

				Font mTextBoxFont;
				Label mTextBox;

				Colour mTextBoxFontColour = Colour(0xffcde3ec);

				virtual bool isInBounds(Point<int> p);

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexSlider)
		};
	}
}
