#pragma once

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex {
	namespace ui{
		class ApexToggleButton : public ToggleButton {
			public:
				ApexToggleButton(ApexFilmStrip filmStrip);
				ApexToggleButton(Image buttonToggled, Image buttonHovered, Image buttonNormal);
				ApexToggleButton(const String& text);

				~ApexToggleButton();

				Option<ApexFilmStrip> getFilmStrip();
				Option<Image> getToggledImage();
				Option<Image> getHoveredImage();
				Option<Image> getNormalImage();

				void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel);

			protected:
				virtual void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted,
						bool shouldDrawButtonAsDown) override;
				std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
				ApexFilmStrip mFilmStrip;
				Image mToggledImage;
				Image mHoveredImage;
				Image mNormalImage;
				bool mUsesFilmStrip = false;
				bool mUsesImages = false;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexToggleButton)
		};
	}
}
