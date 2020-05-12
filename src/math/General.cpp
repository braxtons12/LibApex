#include "General.h"

namespace apex {
	namespace math {
		inline float sqrtf(float x) {
			float n = x > 10.0f ? x / 10.0f : x / 2.0f;
			float y = 1;
			double e = 0.0000000001;
			while(n - y > e) {
				n = (n + y) / 2.0f;
				y = x / n;
			}
			return n;
		}

		inline float fabsf(float x) {
			return sqrtf(x * x);
		}

		inline float truncf(float x) {
			return x - int(x);
		}

		inline float fmodf(float x, float y) {
			return x - truncf(x / y) * y;
		}

		inline double sqrt(double x) {
			double n = x > 10.0 ? x / 10.0 : x / 2.0;
			double y = 1;
			double e = 0.0000000001;
			while(n - y > e) {
				n = (n + y) / 2.0;
				y = x / n;
			}
			return n;
		}

		inline double fabs(double x) {
			return sqrt(x * x);
		}

		inline double trunc(double x) {
			return x - int(x);
		}

		inline double fmod(double x, double y) {
			return x - trunc(x / y) * y;
		}
	}
}
