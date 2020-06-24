#pragma once

#include "../base/StandardIncludes.h"

namespace apex {
	namespace ui {

		class ApexFilmStrip {

			public:
				ApexFilmStrip();
				ApexFilmStrip(Image image, size_t frameSize, bool isHorizontal);
				~ApexFilmStrip();

				size_t getNumFrames();
				Image getFrame(size_t index);
				Image getFrameScaled(size_t index, size_t width, size_t height);

			private:
				Image mFilmStrip;
				size_t mFrameSize;
				size_t mNumFrames;
				bool mIsHorizontal;
		};
	}
}
