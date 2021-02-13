#pragma once

#include <functional>
#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexFilmStrip.h"
#include "ApexLookAndFeel.h"

namespace apex::ui {
	/// @brief Apex Audio implementatio of a digital-style level meter interface component
	class ApexMeter : public juce::Component {
	  public:
		/// @brief Constructs an `ApexMeter` with the given parameters
		///
		/// @param proportionOfHeightToValueFunc - The function to convert the proportion of height
		/// to the user/dsp facing value
		/// @param valueToProportionOfHeightFunc - The function to convert the user/dsp facing value
		/// to the proportion of height
		/// @param clipLevelProportional - The proportion of the height for which "clipping"
		/// indication should start
		ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				  std::function<double(double)> valueToProportionOfHeightFunc,
				  double clipLevelProportional) noexcept;

		/// @brief Constructs an `ApexMeter` with the given parameters
		///
		/// @param proportionOfHeightToValueFunc - The function to convert the proportion of height
		/// to the user/dsp facing value
		/// @param valueToProportionOfHeightFunc - The function to convert the user/dsp facing value
		/// to the proportion of height
		/// @param clipLevelProportional - The proportion of the height for which "clipping"
		/// indication should start
		/// @param numSteps - The number of equally spaced level "ticks" on the meter
		ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				  std::function<double(double)> valueToProportionOfHeightFunc,
				  double clipLevelProportional,
				  size_t numSteps) noexcept;

		/// @brief Constructs an `ApexMeter` that uses a film strip image asset to draw itself
		///
		/// @param proportionOfFilmStripToValueFunc - The function to convert the proportion of the
		/// number of film strip frames to the user/dsp facing value
		/// @param valueToProportionOfFilmStripFunc - The function to convert the user/dsp facing
		/// value to the proportion of the number of film strip frames
		/// @param filmStrip - The film strip asset used to visually represent this
		ApexMeter(std::function<double(double)> proportionOfFilmStripToValueFunc,
				  std::function<double(double)> valueToProportionOfFilmStripFunc,
				  ApexFilmStrip filmStrip) noexcept;

		/// @brief Constructs an `ApexMeter` that uses an image of the meter at its max level to
		/// draw itself
		///
		/// @param proportionOfHeightToValueFunc - The function to convert the proportion of the
		/// meter height to the corresponding user/dsp facing value
		/// @param valueToProportionOfHeightFunc - The function to convert the user/dsp facing value
		/// to the corresponding proportion of meter height
		/// @param maxedMeterImage - The image of the meter at its maximum level
		ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				  std::function<double(double)> valueToProportionOfHeightFunc,
				  juce::Image maxedMeterImage) noexcept;

		~ApexMeter() noexcept override = default;

		/// @brief Returns the current level of the meter
		///
		/// @return - The current level
		[[nodiscard]] inline auto getLevel() const noexcept -> double {
			return mLevel;
		}

		/// @brief - Sets the current level of the meter. This must make sense with the proportion
		/// and value conversion functions
		///
		/// @param level - The new level
		inline auto setLevel(double level) noexcept -> void {
			jassert(level == getValueFromProportionOfHeight(getProportionOfHeightFromValue(level)));
			mLevel = level;
			repaint();
		}

		/// @brief Returns the number of equally spaced level "ticks" displayed on the meter
		///
		/// @return - The number of ticks
		[[nodiscard]] inline auto getNumSteps() const noexcept -> size_t {
			return mNumSteps;
		}

		/// @brief Sets the number of equally spaced level "ticks" displayed on the meter
		///
		/// @param numSteps - The number of ticks
		inline auto setNumSteps(size_t numSteps) noexcept -> void {
			mNumSteps = numSteps;
			repaint();
		}

		/// @brief Returns the proportion of the meter at which "clipping" indication should start
		///
		/// @return - The proportion at which "clipping" indication should start
		[[nodiscard]] inline auto getClipProportion() const noexcept -> double {
			return mClipLevelProportion;
		}

		/// @brief Sets the proportion at which "clipping" indication should start
		///
		/// @param proportion - The proportion
		inline auto setClipProportion(double proportion) noexcept -> void {
			mClipLevelProportion = proportion;
			repaint();
		}

		/// @brief Sets the look and feel used to draw the meter to the given one
		///
		/// @param lookNFeel - The look and feel to use
		inline auto setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) noexcept -> void {
			mLookAndFeel = std::move(lookNFeel);
			juce::Component::setLookAndFeel(mLookAndFeel.get());
			repaint();
		}

		/// @brief Gets the user/dsp facing value for the given proportion of the meter
		///
		/// @param proportion - The proportion to get the corresponding user/dsp facing value for
		///
		/// @return - The corresponding user/dsp facing value
		[[nodiscard]] inline auto
		getValueFromProportionOfHeight(double proportion) const noexcept -> double {
			return mProportionOfHeightToValueFunc(proportion);
		}

		/// @brief Gets the proportion of the meter corresponding to the given user/dsp facing value
		///
		/// @param value - The value to get the corresponding proportion for
		///
		/// @return - The corresponding proportion of the meter
		[[nodiscard]] inline auto
		getProportionOfHeightFromValue(double value) const noexcept -> double {
			return mValueToProportionOfHeightFunc(value);
		}

		/// @brief Returns the film strip asset used to draw this meter, if this uses one
		///
		/// @return - `Some(ApexFilmStrip)` if this uses a film strip asset, or `None` otherwise
		[[nodiscard]] inline auto getFilmStrip() const noexcept -> Option<ApexFilmStrip> {
			return mUsesFilmStrip ? Some(mFilmStrip) : None<ApexFilmStrip>();
		}

		/// @brief Returns the image asset used to draw this meter, if this uses one
		///
		/// @return - `Some(juce::Image)` if this uses one, or `None` otherwise
		[[nodiscard]] inline auto getMaxedImage() const noexcept -> Option<juce::Image> {
			return mUsesMaxedImage ? Some(mMaxedMeterImage) : None<juce::Image>();
		}

		/// @brief Draws this meter to the screen
		///
		/// @param g - The graphics context to use to draw this
		auto paint(juce::Graphics& g) noexcept -> void override;

	  protected:
		std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
		std::function<double(double)> mProportionOfHeightToValueFunc;
		std::function<double(double)> mValueToProportionOfHeightFunc;

		ApexFilmStrip mFilmStrip;
		bool mUsesFilmStrip = false;

		juce::Image mMaxedMeterImage;
		bool mUsesMaxedImage = false;

		size_t mNumSteps = 10;
		double mLevel = 0.0F;
		double mClipLevelProportion = 1.0F;

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApexMeter)
	};
} // namespace apex::ui
