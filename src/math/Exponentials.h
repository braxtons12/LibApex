#pragma once

namespace apex {
	namespace math {
		float frexpf(float x, int* e) noexcept;
		float expf(float x) noexcept;
		float log2f(float x) noexcept;
		float log10f(float x) noexcept;
		float logf(float x) noexcept;
		float pow2f(float x) noexcept;
		float pow10f(float x) noexcept;
		float powf(float base, float exponent) noexcept;

		double frexp(double x, int* e) noexcept;
		double exp(double x) noexcept;
		double log2(double x) noexcept;
		double log10(double x) noexcept;
		double log(double x) noexcept;
		double pow2(double x) noexcept;
		double pow10(double x) noexcept;
		double pow(double base, double exponent) noexcept;

	}
}
