#include "ApexFilmStrip.h"

namespace apex::ui {
	/// @brief Constructs an `ApexFilmStrip` with the given film strip image
	///
	/// @param image - The film strip image asset
	/// @param frameSize - The size of a frame in the film strip
	/// @param isHorizontal - Whether the film strip rolls horizontally or vertically
	ApexFilmStrip::ApexFilmStrip(juce::Image image, size_t frameSize, bool isHorizontal) noexcept
		: mFilmStrip(std::move(image)), mFrameSize(frameSize),
		  mNumFrames(isHorizontal ? (narrow_cast<size_t>(mFilmStrip.getWidth()) / mFrameSize) :
									  (narrow_cast<size_t>(mFilmStrip.getHeight()) / mFrameSize)),
		  mIsHorizontal(isHorizontal) {
	}

	/// @brief Returns the frame at the given index
	///
	/// @param index - The index for the desired frame
	///
	/// @return - The frame corresponding to `index`
	auto ApexFilmStrip::getFrame(size_t index) const noexcept -> juce::Image {
		if(mIsHorizontal) {
			auto height = static_cast<size_t>(mFilmStrip.getHeight());
			return mFilmStrip.getClippedImage(
				juce::Rectangle<int>(static_cast<int>(index * mFrameSize),
									 0,
									 static_cast<int>(mFrameSize),
									 static_cast<int>(height)));
		}
		else {
			auto width = static_cast<size_t>(mFilmStrip.getWidth());
			return mFilmStrip.getClippedImage(
				juce::Rectangle<int>(0,
									 static_cast<int>(index * mFrameSize),
									 static_cast<int>(width),
									 static_cast<int>(mFrameSize)));
		}
	}
} // namespace apex::ui
