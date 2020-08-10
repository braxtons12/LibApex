#pragma once

#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexLookAndFeel.h"

namespace apex {
	namespace ui {

		class ApexComboBox : public juce::ComboBox {
		  public:
			ApexComboBox(juce::Image activeArrowImage,
						 juce::Image hoveredArrowImage,
						 juce::Image normalArrowImage);
			ApexComboBox();

			virtual void mouseDown(const juce::MouseEvent& e) override;
			virtual void mouseUp(const juce::MouseEvent& e) override;
			virtual void mouseEnter(const juce::MouseEvent& e) override;
			virtual void mouseExit(const juce::MouseEvent& e) override;

			bool isHovered();

			void setHovered(bool hovered);

			void setXScaleFactor(float scaleFactor);
			float getXScaleFactor();
			void setYScaleFactor(float scaleFactor);
			float getYScaleFactor();

			void setFont(juce::Font newFont);

			juce::Font getFont();

			void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel);

			Option<juce::Image> getActiveArrowImage();
			Option<juce::Image> getHoveredArrowImage();
			Option<juce::Image> getNormalArrowImage();

			virtual void paint(juce::Graphics& g) override;

		  protected:
			std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
			juce::Image mActiveArrowImage;
			juce::Image mHoveredArrowImage;
			juce::Image mNormalArrowImage;
			bool mUsesArrowImages = false;

			bool mHovered = false;
			float mXScaleFactor = 1.0f;
			float mYScaleFactor = 1.0f;
			juce::Font mFont;

			bool mIsButtonDown = false;

		  private:
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexComboBox)
		};
	} // namespace ui
} // namespace apex
