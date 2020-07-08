#pragma once

#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexSlider.h"

namespace apex {
	namespace ui {
		class ApexFader: public ApexSlider {
			public:
				ApexFader(Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, ApexFilmStrip strip);
				ApexFader(Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, Image thumbImage);
				ApexFader(Slider::SliderStyle style, std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, bool isThumbOnlySlider,
						int initialThumbWidth, int initialThumbHeight);

				~ApexFader();

				virtual String getTextFromValue(double value) override;
				virtual double getValueFromText(const String& text) override;

				virtual void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) override;

				virtual void paint(Graphics& g) override;

				virtual void resized() override;
			protected:
				Image mThumbImage;
				ImageComponent mThumbComponent;
				bool mUsesThumbImage = false;
				bool mIsThumbOnly = false;

				int mInitialThumbWidth = 0;
				int mInitialThumbHeight = 0;

				void resizeThumb();

				virtual bool isInBounds(Point<int> p) override;
			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexFader);
		};
	}
}
