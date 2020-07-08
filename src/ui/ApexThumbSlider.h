#pragma once

#include "../base/StandardIncludes.h"
#include "ApexSlider.h"

namespace apex {
	namespace ui {
		class ApexThumbSlider : public ApexSlider {
			public:
				ApexThumbSlider(Slider::SliderStyle style,
						std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc, Image thumbImage);
				ApexThumbSlider(Slider::SliderStyle style,
						std::function<double(double)> proportionToValueFunc,
						std::function<double(double)> valueToProportionFunc,
						int initialThumbWidth, int initialThumbHeight);

				~ApexThumbSlider();

				virtual void paint(Graphics& g) override;

				virtual void resized() override;

			protected:
				Image mThumbImage;
				ImageComponent mThumbComponent;
				bool mUsesThumbImage = false;

				int mInitialThumbWidth = 0;
				int mInitialThumbHeight = 0;

				void resizeThumb();

				virtual bool isInBounds(Point<int> p) override;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexThumbSlider)
		};
	}
}
