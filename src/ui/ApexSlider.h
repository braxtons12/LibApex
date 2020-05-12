#pragma once

#include <functional>
#include <memory>

#include "../StandardIncludes.h"
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
				float getXScaleFactor();
				void setYScaleFactor(float scaleFactor);
				float getYScaleFactor();

				virtual bool isValueValid(double value);

				void setPopupTextBoxFont(Font font);
				void setTextBoxFont(Font font);
				void setTextBoxFontColour(Colour newColour);
				void setTextBoxBounds(Rectangle<int> bounds);
				Label* getTextBox();

				void setNormalSensitivity(double set);
				void setFineSensitivity(double set);

				void hideEditor();

				Option<ApexFilmStrip> getFilmStrip();

				float getValueFromProportion(float prop);
				float getProportionFromValue(float value);

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
				double mFineSensitivity = 0.8;

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
