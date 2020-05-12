#include "ApexMeter.h"

namespace apex {
	namespace ui {
		ApexMeter::ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
				std::function<float(float)> valueToProportionOfHeightFunc,
				float clipLevelProportional)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mUsesFilmStrip(false),
			mUsesMaxedImage(false),
			mClipLevelProportion(clipLevelProportional)
		{

		}

		ApexMeter::ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
				std::function<float(float)> valueToProportionOfHeightFunc,
				int numSteps, float clipLevelProportional)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mUsesFilmStrip(false),
			mUsesMaxedImage(false),
			mNumSteps(numSteps),
			mClipLevelProportion(clipLevelProportional)
		{

		}

		ApexMeter::ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
				std::function<float(float)> valueToProportionOfHeightFunc,
				ApexFilmStrip filmStrip)
			: Component(),
			mProportionOfHeightToValueFunc(proportionOfHeightToValueFunc),
			mValueToProportionOfHeightFunc(valueToProportionOfHeightFunc),
			mFilmStrip(filmStrip),
			mUsesFilmStrip(true),
			mUsesMaxedImage(false)
		{

		}

		ApexMeter::ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
				std::function<float(float)> valueToProportionOfHeightFunc,
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

		float ApexMeter::getLevel() {
			return mLevel;
		}

		void ApexMeter::setLevel(float level) {
			mLevel = level;
		}

		int ApexMeter::getNumSteps() {
			return mNumSteps;
		}

		void ApexMeter::setNumSteps(int numSteps) {
			mNumSteps = numSteps;
		}

		float ApexMeter::getClipProportion() {
			return mClipLevelProportion;
		}

		void ApexMeter::setClipProportion(float proportion) {
			mClipLevelProportion = proportion;
		}

		void ApexMeter::setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel) {
			mLookAndFeel = lookAndFeel;
			Component::setLookAndFeel(mLookAndFeel.get());
		}

		float ApexMeter::getValueFromProportionOfHeight(float proportion) {
			return mProportionOfHeightToValueFunc(proportion);
		}

		float ApexMeter::getProportionOfHeightFromValue(float value) {
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
			mLookAndFeel->drawApexMeter(g, mLevel, mClipLevelProportion, mNumSteps, *this);
		}
	}
}
