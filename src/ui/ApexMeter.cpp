#include "ApexMeter.h"

namespace apex {
	namespace ui {
		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				std::function<double(double)> valueToProportionOfHeightFunc,
				double clipLevelProportional)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mUsesFilmStrip(false),
			mUsesMaxedImage(false),
			mClipLevelProportion(clipLevelProportional)
		{

		}

		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				std::function<double(double)> valueToProportionOfHeightFunc,
				double clipLevelProportional, size_t numSteps)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mUsesFilmStrip(false),
			mUsesMaxedImage(false),
			mNumSteps(numSteps),
			mClipLevelProportion(clipLevelProportional)
		{

		}

		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				std::function<double(double)> valueToProportionOfHeightFunc,
				ApexFilmStrip filmStrip)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mFilmStrip(filmStrip),
			mUsesFilmStrip(true),
			mUsesMaxedImage(false)
		{

		}

		ApexMeter::ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
				std::function<double(double)> valueToProportionOfHeightFunc,
				Image maxedMeterImage)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mUsesFilmStrip(false),
			mMaxedMeterImage(maxedMeterImage),
			mUsesMaxedImage(true)
		{

		}

		ApexMeter::~ApexMeter() {

		}

		double ApexMeter::getLevel() {
			return mLevel;
		}

		void ApexMeter::setLevel(double level) {
			mLevel = level;
		}

		size_t ApexMeter::getNumSteps() {
			return mNumSteps;
		}

		void ApexMeter::setNumSteps(size_t numSteps) {
			mNumSteps = numSteps;
		}

		double ApexMeter::getClipProportion() {
			return mClipLevelProportion;
		}

		void ApexMeter::setClipProportion(double proportion) {
			mClipLevelProportion = proportion;
		}

		void ApexMeter::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel) {
			mLookAndFeel = lookNFeel;
			Component::setLookAndFeel(mLookAndFeel.get());
		}

		double ApexMeter::getValueFromProportionOfHeight(double proportion) {
			return mProportionOfHeightToValueFunc(proportion);
		}

		double ApexMeter::getProportionOfHeightFromValue(double value) {
			return mValueToProportionOfHeightFunc(value);
		}

		Option<ApexFilmStrip> ApexMeter::getFilmStrip() {
			return mUsesFilmStrip ? Option<ApexFilmStrip>::Some(mFilmStrip)
				: Option<ApexFilmStrip>::None();
		}

		Option<Image> ApexMeter::getMaxedImage() {
			return mUsesMaxedImage ? Option<Image>::Some(mMaxedMeterImage)
				: Option<Image>::None();
		}

		void ApexMeter::paint(Graphics& g) {
			if(mLookAndFeel != nullptr)
				mLookAndFeel->drawApexMeter(
					g,
					static_cast<float>(mLevel),
					static_cast<float>(mClipLevelProportion),
					mNumSteps,
					*this);
		}
	}
}
