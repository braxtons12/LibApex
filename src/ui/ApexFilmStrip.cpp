#include "ApexFilmStrip.h"

namespace apex {
	namespace ui {

		ApexFilmStrip::ApexFilmStrip() {

		}

		ApexFilmStrip::ApexFilmStrip(Image image, size_t frameSize, bool isHorizontal) {

			mFilmStrip = image;
			mFrameSize = frameSize;
			mIsHorizontal = isHorizontal;
			mNumFrames = mIsHorizontal ? (size_t(mFilmStrip.getWidth()) / mFrameSize) : (size_t(mFilmStrip.getHeight()) / mFrameSize);
		}

		ApexFilmStrip::~ApexFilmStrip() {

		}

		size_t ApexFilmStrip::getNumFrames() {
			return mNumFrames;
		}

		Image ApexFilmStrip::getFrame(size_t index) {
			if(mIsHorizontal) {
				size_t height = size_t(mFilmStrip.getHeight());
				return mFilmStrip.getClippedImage(Rectangle<int>(index * mFrameSize, 0, mFrameSize, height));
			}
			else {
				size_t width = size_t(mFilmStrip.getWidth());
				return mFilmStrip.getClippedImage(Rectangle<int>(0, index * mFrameSize, width, mFrameSize));
			}
		}

		Image ApexFilmStrip::getFrameScaled(size_t index, size_t width, size_t height) {
			return getFrame(index).rescaled(width, height, Graphics::highResamplingQuality);
		}

	}
}
