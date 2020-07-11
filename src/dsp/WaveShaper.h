#pragma once

namespace apex {
	namespace dsp {
		namespace waveshapers {
			float softSaturation(float input, float amount = 1.0f, float slope = 0.4f) noexcept;
			double softSaturation(double input, double amount = 1.0, double slope = 0.4) noexcept;

			float softClip(float input, float amount = 1.0f) noexcept;
			double softClip(double input, double amount = 1.0) noexcept;

			float hardClip(float input , float clipLevel, float amount = 1.0f) noexcept;
			double hardClip(double input, double clipLevel, double amount = 1.0) noexcept;
		}
	}
}
