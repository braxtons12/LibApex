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
				ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
						std::function<float(float)> valueToProportionOfHeightFunc,
						float clipLevelProportional);
				ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
						std::function<float(float)> valueToProportionOfHeightFunc,
						float clipLevelProportional, int numSteps);
				ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
						std::function<float(float)> valueToProportionOfHeightFunc,
						ApexFilmStrip filmStrip);
				ApexMeter(std::function<float(float)> proportionOfHeightToValueFunc,
						std::function<float(float)> valueToProportionOfHeightFunc,
						Image maxedMeterImage);

				~ApexMeter();

				float getLevel();
				void setLevel(float level);

				int getNumSteps();
				void setNumSteps(int numSteps);

				float getClipProportion();
				void setClipProportion(float proportion);

				void setLookAndFeel(std::shared_ptr<ApexLookAndFeel> lookAndFeel);

				float getValueFromProportionOfHeight(float proportion);
				float getProportionOfHeightFromValue(float value);

				Option<ApexFilmStrip> getFilmStrip();
				Option<Image> getMaxedImage();

				virtual void paint(Graphics& g) override;

			protected:
				std::shared_ptr<ApexLookAndFeel> mLookAndFeel;
				std::function<float(float)> mProportionOfHeightToValueFunc;
				std::function<float(float)> mValueToProportionOfHeightFunc;

				ApexFilmStrip mFilmStrip;
				bool mUsesFilmStrip = false;

				Image mMaxedMeterImage;
				bool mUsesMaxedImage = false;

				int mNumSteps = 10;
				float mLevel = 0.0f;
				float mClipLevelProportion = 1.0f;
		};
	}
}
