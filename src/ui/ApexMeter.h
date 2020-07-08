#pragma once

#include <functional>
#include <memory>

#include "../base/StandardIncludes.h"
#include "ApexLookAndFeel.h"
#include "ApexFilmStrip.h"

namespace apex {
	namespace ui {
		class ApexMeter : public Component {
			public:
				ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						std::function<double(double)> valueToProportionOfHeightFunc,
						double clipLevelProportional);
				ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						std::function<double(double)> valueToProportionOfHeightFunc,
						double clipLevelProportional, size_t numSteps);
				ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						std::function<double(double)> valueToProportionOfHeightFunc,
						ApexFilmStrip filmStrip);
				ApexMeter(std::function<double(double)> proportionOfHeightToValueFunc,
						std::function<double(double)> valueToProportionOfHeightFunc,
						Image maxedMeterImage);

				~ApexMeter();

				double getLevel();
				void setLevel(double level);

				size_t getNumSteps();
				void setNumSteps(size_t numSteps);

				double getClipProportion();
				void setClipProportion(double proportion);

				void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookNFeel);

				double getValueFromProportionOfHeight(double proportion);
				double getProportionOfHeightFromValue(double value);

				Option<ApexFilmStrip> getFilmStrip();
				Option<Image> getMaxedImage();

				virtual void paint(Graphics& g) override;

			protected:
				std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
				std::function<double(double)> mProportionOfHeightToValueFunc;
				std::function<double(double)> mValueToProportionOfHeightFunc;

				ApexFilmStrip mFilmStrip;
				bool mUsesFilmStrip = false;

				Image mMaxedMeterImage;
				bool mUsesMaxedImage = false;

				size_t mNumSteps = 10;
				double mLevel = 0.0f;
				double mClipLevelProportion = 1.0f;
		};
	}
}
