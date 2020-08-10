#include "ApexMeter.h"

namespace apex {
	namespace ui {
		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
							 std::function<double(double)> valueToProportionOfHeightFunc,
							 double clipLevelProportional)
			: juce::Component(), mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			  mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc), mUsesFilmStrip(false),
			  mUsesMaxedImage(false), mClipLevelProportion(clipLevelProportional) {
		}

		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
							 std::function<double(double)> valueToProportionOfHeightFunc,
							 double clipLevelProportional,
							 size_t numSteps)
			: juce::Component(), mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			  mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc), mUsesFilmStrip(false),
			  mUsesMaxedImage(false), mNumSteps(numSteps),
			  mClipLevelProportion(clipLevelProportional) {
		}

		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
							 std::function<double(double)> valueToProportionOfHeightFunc,
							 ApexFilmStrip filmStrip)
			: juce::Component(), mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			  mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc), mFilmStrip(filmStrip),
			  mUsesFilmStrip(true), mUsesMaxedImage(false) {
		}

		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
							 std::function<double(double)> valueToProportionOfHeightFunc,
							 juce::Image maxedMeterImage)
			: juce::Component(), mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			  mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc), mUsesFilmStrip(false),
			  mMaxedMeterImage(maxedMeterImage), mUsesMaxedImage(true) {
		}

		ApexMeter::~ApexMeter() {
		}

		double ApexMeter::getLevel() {
			return mLevel;
		}

		void ApexMeter::setLevel(double level) {
			mLevel = level;
			repaint();
		}

		size_t ApexMeter::getNumSteps() {
			return mNumSteps;
		}

		void ApexMeter::setNumSteps(size_t numSteps) {
			mNumSteps = numSteps;
			repaint();
		}

		double ApexMeter::getClipProportion() {
			return mClipLevelProportion;
		}

		void ApexMeter::setClipProportion(double proportion) {
			mClipLevelProportion = proportion;
			repaint();
		}

		void ApexMeter::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) {
			mLookAndFeel = lookNFeel;
			juce::Component::setLookAndFeel(mLookAndFeel.get());
			repaint();
		}

		double ApexMeter::getValueFromProportionOfHeight(double proportion) {
			return mProportionOfHeightToValueFunc(proportion);
		}

		double ApexMeter::getProportionOfHeightFromValue(double value) {
			return mValueToProportionOfHeightFunc(value);
		}

		Option<ApexFilmStrip> ApexMeter::getFilmStrip() {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip) :
									  Option<ApexFilmStrip>::None();
		}

		Option<juce::Image> ApexMeter::getMaxedImage() {
			return mUsesMaxedImage ? Option<juce::Image>::Some(mMaxedMeterImage) :
									   Option<juce::Image>::None();
		}

		void ApexMeter::paint(juce::Graphics& g) {
			if(mLookAndFeel != nullptr)
				mLookAndFeel->drawApexMeter(g,
											static_cast<float>(mLevel),
											static_cast<float>(mClipLevelProportion),
											mNumSteps,
											*this);
		}
	} // namespace ui
} // namespace apex
