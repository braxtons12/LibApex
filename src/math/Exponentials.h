#pragma once

namespace apex {
	namespace math {
		float frexpf(float x, int* e);
		float expf(float x);
		float log2f(float x);
		float log10f(float x);
		float logf(float x);
		float pow2f(float x);
		float pow10f(float x);
		float powf(float base, float exponent);

		double frexp(double x, int* e);
		double exp(double x);
		double log2(double x);
		double log10(double x);
		double log(double x);
		double pow2(float x);
		double pow10(double x);
		double pow(double base, double exponent);

	}
}
