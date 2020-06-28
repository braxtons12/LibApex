#pragma once

namespace apex {
	namespace dsp {
		namespace waveshapers {
			double softSaturation(double input, double amount = 1.0, double slope = 0.4);
			float softSaturation(float input, float amount = 1.0f, float slope = 0.4f);

			double softClip(double input, double amount = 1.0);
			float softClip(float input, float amount = 1.0f);

			double hardClip(double input, double clipLevel, double amount = 1.0);
			float hardClip(float input , float clipLevel, float amount = 1.0f);
		}
	}
}
