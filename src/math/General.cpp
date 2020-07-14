#include "General.h"

namespace apex {
	namespace math {
		inline float sqrtf(float x) noexcept {
			float n = x > 10.0f ? x / 10.0f : x / 2.0f;
			float y = 1;
			double e = 0.0000000001;
			while(n - y > e) {
				n = (n + y) / 2.0f;
				y = x / n;
			}
			return n;
		}

		inline float fabsf(float x) noexcept {
			return sqrtf(x * x);
		}

		inline float truncf(float x) noexcept {
			return static_cast<float>(static_cast<int>(x));
		}

		inline float fmodf(float x, float y) noexcept {
			return x - truncf(x / y) * y;
		}

		inline float max(float x, float y) noexcept {
			if(x > y) return x;
			else return y;
		}

		inline double sqrt(double x) noexcept {
			double n = x > 10.0 ? x / 10.0 : x / 2.0;
			double y = 1;
			double e = 0.0000000001;
			while(n - y > e) {
				n = (n + y) / 2.0;
				y = x / n;
			}
			return n;
		}

		inline double fabs(double x) noexcept {
			return sqrt(x * x);
		}

		inline double trunc(double x) noexcept {
			return int(x);
		}

		inline double fmod(double x, double y) noexcept {
			return x - trunc(x / y) * y;
		}

		inline double max(double x, double y) noexcept {
			if(x > y) return x;
			else return y;
		}

		inline int max(int x, int y) noexcept {
			if(x > y) return x;
			else return y;
		}

		inline size_t max(size_t x, size_t y) noexcept {
			if(x > y) return x;
			else return y;
		}
	}
}
