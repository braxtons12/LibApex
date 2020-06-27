#pragma once

namespace apex {
	namespace dsp {
		namespace waveshapers {
			static double softSaturation(double input, double amount = 1.0, double slope = 0.4);
			static float softSaturation(float input, float amount = 1.0f, float slope = 0.4f);

			static double softClip(double input, double amount = 1.0);
			static float softClip(float input, float amount = 1.0f);

			static double hardClip(double input, double clipLevel, double amount = 1.0);
			static float hardClip(float input , float clipLevel, float amount = 1.0f);
		}
	}
}
