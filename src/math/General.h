#pragma once
#include <cstdint>

#ifndef _MSC_VER
	using std::size_t;
#endif

namespace apex {
	namespace math {
		float sqrtf(float x) noexcept;
		float fabsf(float x) noexcept;
		float truncf(float x) noexcept;
		float fmodf(float x, float y) noexcept;
		float max(float x, float y) noexcept;

		double sqrt(double x) noexcept;
		double fabs(double x) noexcept;
		double trunc(double x) noexcept;
		double fmod(double x, double y) noexcept;
		double max(double x, double y) noexcept;

		int max(int x, int y) noexcept;
		size_t max(size_t x, size_t y) noexcept;
	}
}
