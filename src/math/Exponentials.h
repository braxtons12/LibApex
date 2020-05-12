#pragma once

namespace apex {
	namespace math {
		float frexpf(float x, int* e);
		float expf(float x);
		float log2f(float x);
		float log10f(float x);
		float pow10f(float x);

		double frexp(double x, int* e);
		double exp(double x);
		double log2(double x);
		double log10(double x);
		double pow10(double x);
	}
}
