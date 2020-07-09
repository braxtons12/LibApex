#pragma once

#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexSlider.h"

namespace apex {
	namespace ui {
		class ApexFader: public ApexSlider {
			public:
				ApexFader(juce::Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, ApexFilmStrip strip);
				ApexFader(juce::Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, juce::Image thumbImage);
				ApexFader(juce::Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, bool isThumbOnlySlider,
						int initialThumbWidth, int initialThumbHeight);

				~ApexFader() override;

				virtual juce::String getTextFromValue(double value) override;
				virtual double getValueFromText(const juce::String& text) override;

				virtual void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

				virtual void paint(juce::Graphics& g) override;

				virtual void resized() override;
			protected:
				juce::Image mThumbImage;
				juce::ImageComponent mThumbComponent;
				bool mUsesThumbImage = false;
				bool mIsThumbOnly = false;

				int mInitialThumbWidth = 0;
				int mInitialThumbHeight = 0;

				void resizeThumb();

				virtual bool isInBounds(juce::Point<int> p) override;
			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexFader)
		};
	}
}
