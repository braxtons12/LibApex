#include "Exponentials.h"

#include <stdint.h>

#include "General.h"

//for explanations of these approximations, refer to:
//logs and pow10s:
//http://openaudio.blogspot.com/2017/02/faster-log10-and-pow.html
//exps:
//https://www.musicdsp.org/en/latest/Other/222-fast-exp-approximations.html

namespace apex {
	namespace math {

		inline float frexpf(float x, int *e) {
			union { float f; uint32_t i; } y = { x };
			int ee = y.i>>23 & 0xff;

			if (!ee) {
				if (x) {
					x = frexpf(x*0x1p64, e);
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

		inline float exp_helperf(float x) {
			return (5040.0f + x * (5040.0f + x * (2520.0f + x * (840.0f + x *
								(210.0f + x * (42.0f + x * (7.0f + x))))))) * 0.00019841269f;
		}

		inline float expf(float x) {
			if(x < 2.5f) return 2.7182818f * exp_helperf(x - 1.0f);
			else if (x < 5.0f) return 33.115452f * exp_helperf(x - 3.5f);
			else return 483.42879f * exp_helperf(x - 6.0f);
		}

		inline float log2f(float x) {
			float y, f;
			int e;
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

		inline float log10f(float x) {
			return log2f(x) * 0.3010299956639812f;
		}

		inline float pow10f(float x) {
			return expf(2.302585092994046f * x);
		}

		inline double frexp(double x, int *e) {
			union { double d; uint64_t i; } y = { x };
			int ee = y.i>>52 & 0x7ff;

			if (!ee) {
				if (x) {
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

		inline double exp_helper(double x) {
			return (5040.0 + x * (5040.0 + x * (2520.0 + x * (840.0 + x *
								(210.0 + x * (42.0 + x * (7.0 + x))))))) * 0.00019841269;
		}

		inline double exp(double x) {
			if(x < 2.5) return 2.7182818 * exp_helperf(x - 1.0);
			else if (x < 5.0) return 33.115452 * exp_helperf(x - 3.5);
			else return 483.42879 * exp_helperf(x - 6.0);
		}

		inline double log2(double x) {
			float y, f;
			int e;
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

		inline double log10(float x) {
			return log2(x) * 0.3010299956639812;

		}

		inline double pow10(double x) {
			return exp(2.302585092994046 * x);
		}
	}
}
