#pragma once

#include "../base/StandardIncludes.h"

namespace apex::ui {
	/// @brief Apex Audio implementation of a wrapper around a film strip image asset
	class ApexFilmStrip {
	  public:
		/// @brief Constructs a default `ApexFilmStrip`
		ApexFilmStrip() = default;

		/// @brief Constructs an `ApexFilmStrip` with the given film strip image
		///
		/// @param image - The film strip image asset
		/// @param frameSize - The size of a frame in the film strip
		/// @param isHorizontal - Whether the film strip rolls horizontally or vertically
		ApexFilmStrip(juce::Image image, size_t frameSize, bool isHorizontal) noexcept;

		/// @brief Copy constructs an `ApexFilmStrip` from the given one
		///
		/// @param strip - The `ApexFilmStrip` to copy
		ApexFilmStrip(const ApexFilmStrip& strip) noexcept = default;

		/// @brief Move constructs the given `ApexFilmStrip`
		///
		/// @param strip - The `ApexFilmStrip` to move
		ApexFilmStrip(ApexFilmStrip&& strip) noexcept = default;
		~ApexFilmStrip() noexcept = default;

		/// @brief Returns the number of frames in the film strip
		///
		/// @return - The number of frames
		[[nodiscard]] inline auto getNumFrames() const noexcept -> size_t {
			return mNumFrames;
		}

		/// @brief Returns the frame at the given index
		///
		/// @param index - The index for the desired frame
		///
		/// @return - The frame corresponding to `index`
		[[nodiscard]] auto getFrame(size_t index) const noexcept -> juce::Image;

		/// @brief Returns the frame at the given index, scaled to the given width and height
		///
		/// @param index - The index for the desired frame
		/// @param width - The desired width
		/// @param height - The desired height
		///
		/// @return - The frame corresponding to `index`, scaled to the given dimensions
		[[nodiscard]] inline auto
		getFrameScaled(size_t index, size_t width, size_t height) const noexcept -> juce::Image {
			return getFrame(index).rescaled(static_cast<int>(width),
											static_cast<int>(height),
											juce::Graphics::highResamplingQuality);
		}

		/// @brief Sets the film strip image of this to the given one
		///
		/// @param image - The film strip image asset
		/// @param frameSize - The size of a frame in the film strip
		/// @param isHorizontal - Whether the film strip rolls horizontally or vertically
		auto
		setFilmStripImage(juce::Image image, size_t frameSize, bool isHorizontal) noexcept -> void;

		auto operator=(const ApexFilmStrip& strip) noexcept -> ApexFilmStrip& = default;
		auto operator=(ApexFilmStrip&& strip) noexcept -> ApexFilmStrip& = default;

	  private:
		juce::Image mFilmStrip = juce::Image();
		size_t mFrameSize = static_cast<size_t>(mFilmStrip.getHeight());
		size_t mNumFrames = 0;
		bool mIsHorizontal = false;
	};
} // namespace apex::ui
