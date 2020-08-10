#pragma once

#include "../base/StandardIncludes.h"
#include "ApexSlider.h"

namespace apex {
	namespace ui {
		class ApexThumbSlider : public ApexSlider {
		  public:
			ApexThumbSlider(juce::Slider::SliderStyle style,
							std::function<double(double)> proportionToValueFunc,
							std::function<double(double)> valueToProportionFunc,
							juce::Image thumbImage);
			ApexThumbSlider(juce::Slider::SliderStyle style,
							std::function<double(double)> proportionToValueFunc,
							std::function<double(double)> valueToProportionFunc,
							int initialThumbWidth,
							int initialThumbHeight);

			~ApexThumbSlider() override;

			virtual void paint(juce::Graphics& g) override;

			virtual void resized() override;

		  protected:
			juce::Image mThumbImage;
			juce::ImageComponent mThumbComponent;
			bool mUsesThumbImage = false;

			int mInitialThumbWidth = 0;
			int mInitialThumbHeight = 0;

			void resizeThumb();

			virtual bool isInBounds(juce::Point<int> p) override;

		  private:
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexThumbSlider)
		};
	} // namespace ui
} // namespace apex
