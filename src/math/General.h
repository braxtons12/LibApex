#pragma once

namespace apex {
	namespace math {
		float sqrtf(float x) noexcept;
		float fabsf(float x) noexcept;
		float truncf(float x) noexcept;
		float fmodf(float x, float y) noexcept;

		double sqrt(double x) noexcept;
		double fabs(double x) noexcept;
		double trunc(double x) noexcept;
		double fmod(double x, double y) noexcept;
	}
}
