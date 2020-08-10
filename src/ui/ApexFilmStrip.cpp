#include "ApexFilmStrip.h"

namespace apex {
	namespace ui {

		ApexFilmStrip::ApexFilmStrip() {
		}

		ApexFilmStrip::ApexFilmStrip(juce::Image image, size_t frameSize, bool isHorizontal) {

			mFilmStrip = image;
			mFrameSize = frameSize;
			mIsHorizontal = isHorizontal;
			mNumFrames = mIsHorizontal ? (size_t(mFilmStrip.getWidth()) / mFrameSize) :
										   (size_t(mFilmStrip.getHeight()) / mFrameSize);
		}

		ApexFilmStrip::ApexFilmStrip(const ApexFilmStrip& strip) {
			mFilmStrip = strip.mFilmStrip;
			mFrameSize = strip.mFrameSize;
			mIsHorizontal = strip.mIsHorizontal;
			mNumFrames = strip.mNumFrames;
		}

		ApexFilmStrip::~ApexFilmStrip() {
		}

		size_t ApexFilmStrip::getNumFrames() {
			return mNumFrames;
		}

		juce::Image ApexFilmStrip::getFrame(size_t index) {
			if(mIsHorizontal) {
				size_t height = size_t(mFilmStrip.getHeight());
				return mFilmStrip.getClippedImage(
					juce::Rectangle<int>(static_cast<int>(index * mFrameSize),
										 0,
										 static_cast<int>(mFrameSize),
										 static_cast<int>(height)));
			}
			else {
				size_t width = size_t(mFilmStrip.getWidth());
				return mFilmStrip.getClippedImage(
					juce::Rectangle<int>(0,
										 static_cast<int>(index * mFrameSize),
										 static_cast<int>(width),
										 static_cast<int>(mFrameSize)));
			}
		}

		juce::Image ApexFilmStrip::getFrameScaled(size_t index, size_t width, size_t height) {
			return getFrame(index).rescaled(static_cast<int>(width),
											static_cast<int>(height),
											juce::Graphics::highResamplingQuality);
		}

		ApexFilmStrip ApexFilmStrip::operator=(const ApexFilmStrip& strip) {
			return ApexFilmStrip(strip);
		}

	} // namespace ui
} // namespace apex
