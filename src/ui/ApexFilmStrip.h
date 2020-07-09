#pragma once

#include "../base/StandardIncludes.h"

namespace apex {
	namespace ui {

		class ApexFilmStrip {

			public:
				ApexFilmStrip();
				ApexFilmStrip(juce::Image image, size_t frameSize, bool isHorizontal);
				ApexFilmStrip(const ApexFilmStrip& strip);
				~ApexFilmStrip();

				size_t getNumFrames();
				juce::Image getFrame(size_t index);
				juce::Image getFrameScaled(size_t index, size_t width, size_t height);
				ApexFilmStrip operator=(const ApexFilmStrip& strip);

			private:
				juce::Image mFilmStrip;
				size_t mFrameSize;
				size_t mNumFrames;
				bool mIsHorizontal;
		};
	}
}
