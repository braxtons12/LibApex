#pragma once

#include "General.h"

namespace apex {
	namespace math {

		inline constexpr float frexpf(float x, int* e) noexcept {
			union { float f; uint32_t i; } y = { x };
			int ee = y.i>>23 & 0xff;

			if (!ee) {
				if (static_cast<bool>(x)) {
					x = frexpf(static_cast<float>(x*0x1p64), e);
					*e -= 64;
				} else *e = 0;
				return x;
			} else if (ee == 0xff) {
				return x;
			}

			*e = ee - 0x7e;
			y.i &= 0x807ffffful;
			y.i |= 0x3f000000ul;
			return y.f;
		}

		inline constexpr float exp_helperf(float x) noexcept {
			return (5040.0f + x * (5040.0f + x * (2520.0f + x * (840.0f + x *
								(210.0f + x * (42.0f + x * (7.0f + x))))))) * 0.00019841269f;
		}

		inline constexpr float expf(float x) noexcept {
			if(x < 2.5f) return 2.7182818f * exp_helperf(x - 1.0f);
			else if (x < 5.0f) return 33.115452f * exp_helperf(x - 3.5f);
			else return 483.42879f * exp_helperf(x - 6.0f);
		}

		inline constexpr float log2f(float x) noexcept {
			float y = 0.0f;
			float f = 0.0f;
			int e = 0;
			f = frexpf(fabsf(x), &e);
			y = 1.23149591368684f;
			y *= f;
			y += -4.11852516267426f;
			y *= f;
			y += 6.02197014179219f;
			y *= f;
			y += -3.13396450166353f;
			y += e;
			return(y);
		}

		inline constexpr float log10f(float x) noexcept {
			return log2f(x) * 0.3010299956639812f;
		}

		inline constexpr float logf(float x) noexcept {
			return log2f(x) * 0.69314718055995f;
		}

		inline constexpr float pow2f(float x) noexcept {
			return expf(x * 0.69314718055995f);
		}

		inline constexpr float pow10f(float x) noexcept {
			return expf(2.302585092994046f * x);
		}

		inline constexpr float powf(float base, float exponent) noexcept {
			return pow2f(exponent * log2f(base));
		}

		inline constexpr double frexp(double x, int *e) noexcept {
			union { double d; uint64_t i; } y = { x };
			int ee = y.i>>52 & 0x7ff;

			if (!ee) {
				if (static_cast<bool>(x)) {
					x = frexp(x*0x1p64, e);
					*e -= 64;
				} else *e = 0;
				return x;
			} else if (ee == 0x7ff) {
				return x;
			}

			*e = ee - 0x3fe;
			y.i &= 0x800fffffffffffffull;
			y.i |= 0x3fe0000000000000ull;
			return y.d;
		}

		inline constexpr double exp_helper(double x) noexcept {
			return (5040.0 + x * (5040.0 + x * (2520.0 + x * (840.0 + x *
								(210.0 + x * (42.0 + x * (7.0 + x))))))) * 0.00019841269;
		}

		inline constexpr double exp(double x) noexcept {
			if(x < 2.5) return 2.7182818 * exp_helper(x - 1.0);
			else if (x < 5.0) return 33.115452 * exp_helper(x - 3.5);
			else return 483.42879 * exp_helper(x - 6.0);
		}

		inline constexpr double log2(double x) noexcept {
			double y = 0.0;
			double f = 0.0;
			int e = 0;
			f = frexp(fabs(x), &e);
			y = 1.23149591368684;
			y *= f;
			y += -4.11852516267426;
			y *= f;
			y += 6.02197014179219;
			y *= f;
			y += -3.13396450166353;
			y += e;
			return(y);
		}

		inline constexpr double log10(double x) noexcept {
			return log2(x) * 0.3010299956639812;

		}

		inline constexpr double log(double x) noexcept {
			return log2(x) * 0.69314718055995;
		}

		inline constexpr double pow2(double x) noexcept {
			return exp(x * 0.69314718055995);
		}

		inline constexpr double pow10(double x) noexcept {
			return exp(2.302585092994046 * x);
		}

		inline constexpr double pow(double base, double exponent) noexcept {
			return pow2(exponent * log2(base));
		}
	}
}
