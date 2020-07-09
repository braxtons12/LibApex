#pragma once

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex {
	namespace ui{
		class ApexToggleButton : public juce::ToggleButton {
			public:
				ApexToggleButton(ApexFilmStrip filmStrip);
				ApexToggleButton(juce::Image buttonToggled, juce::Image buttonHovered, juce::Image buttonNormal);
				ApexToggleButton(const juce::String& text);

				~ApexToggleButton() override;

				Option<ApexFilmStrip> getFilmStrip();
				Option<juce::Image> getToggledImage();
				Option<juce::Image> getHoveredImage();
				Option<juce::Image> getNormalImage();

				void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel);

			protected:
				virtual void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
						bool shouldDrawButtonAsDown) override;
				std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
				ApexFilmStrip mFilmStrip;
				juce::Image mToggledImage;
				juce::Image mHoveredImage;
				juce::Image mNormalImage;
				bool mUsesFilmStrip = false;
				bool mUsesImages = false;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexToggleButton)
		};
	}
}
