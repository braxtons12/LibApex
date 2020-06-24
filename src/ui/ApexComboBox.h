#pragma once

#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexLookAndFeel.h"

namespace apex {
	namespace ui {

		class ApexComboBox : public ComboBox {
			public:
				ApexComboBox(Image activeArrowImage, Image hoveredArrowImage, Image normalArrowImage);
				ApexComboBox();

				virtual void mouseDown(const MouseEvent& e) override;
				virtual void mouseUp(const MouseEvent& e) override;
				virtual void mouseEnter(const MouseEvent& e) override;
				virtual void mouseExit(const MouseEvent& e) override;

				bool isHovered();

				void setHovered(bool hovered);

				void setXScaleFactor(float scaleFactor);
				float getXScaleFactor();
				void setYScaleFactor(float scaleFactor);
				float getYScaleFactor();

				void setFont(Font newFont);

				Font getFont();

				void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel);

				Option<Image> getActiveArrowImage();
				Option<Image> getHoveredArrowImage();
				Option<Image> getNormalArrowImage();

				virtual void paint(Graphics& g) override;

			protected:
				std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
				Image mActiveArrowImage;
				Image mHoveredArrowImage;
				Image mNormalArrowImage;
				bool mUsesArrowImages = false;

				bool mHovered = false;
				float mXScaleFactor = 1.0f;
				float mYScaleFactor = 1.0f;
				Font mFont;

				bool mIsButtonDown = false;

			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexComboBox)
		};
	}
}
