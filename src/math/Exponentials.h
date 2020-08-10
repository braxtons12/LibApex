#pragma once

#include "General.h"

namespace apex::math {

	/// @brief Calculates the mantissa and exponent of `x`,
	/// in the representation x = mantissa * 2^exponent
	///
	/// @param x - The value to calculate the mantissa and exponent for
	/// @param e - The `int` to store the exponent in
	/// @return - The mantissa
	[[nodiscard]] inline constexpr auto frexpf(float x, int* e) noexcept -> float {
		union {
			float f;
			uint32_t i;
		} y = {x};
		int ee = y.i >> 23 & 0xff;

		if(!ee) {
			if(static_cast<bool>(x)) {
				x = frexpf(static_cast<float>(x * 0x1p64), e);
				*e -= 64;
			}
			else {
				*e = 0;
			}
			return x;
		}
		else if(ee == 0xff) {
			return x;
		}

		*e = ee - 0x7e;
		y.i &= 0x807fffffUL;
		y.i |= 0x3f000000UL;
		return y.f;
	}

	/// @brief Helper function for `expf`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]] inline constexpr auto exp_helperf(float x) noexcept -> float {
		return (5040.0F
				+ x
					  * (5040.0F
						 + x
							   * (2520.0F
								  + x * (840.0F + x * (210.0F + x * (42.0F + x * (7.0F + x)))))))
			   * 0.00019841269F;
	}

	/// @brief Fast approximiation calculation of e^x
	///
	/// @param x - The exponent
	/// @return - e^x
	[[nodiscard]] inline constexpr auto expf(float x) noexcept -> float {
		if(x < 2.5F) {
			return 2.7182818F * exp_helperf(x - 1.0F);
		}
		else if(x < 5.0F) {
			return 33.115452F * exp_helperf(x - 3.5F);
		}
		else {
			return 483.42879F * exp_helperf(x - 6.0F);
		}
	}

	/// @brief Fast approximation calculation of log_2(x)
	///
	/// @param x - The input
	/// @return - log_2(x)
	[[nodiscard]] inline constexpr auto log2f(float x) noexcept -> float {
		float y = 0.0F;
		float f = 0.0F;
		int e = 0;
		f = frexpf(fabsf(x), &e);
		y = 1.23149591368684F;
		y *= f;
		y += -4.11852516267426F;
		y *= f;
		y += 6.02197014179219F;
		y *= f;
		y += -3.13396450166353F;
		y += static_cast<float>(e);
		return (y);
	}

	/// @brief Fast approximation calculation of log_10(x)
	///
	/// @param x - The input
	/// @return - log_10(x)
	[[nodiscard]] inline constexpr auto log10f(float x) noexcept -> float {
		return log2f(x) * 0.3010299956639812F;
	}

	/// @brief Fast approximation calculation of ln(x)
	///
	/// @param x - The input
	/// @return - ln(x)
	[[nodiscard]] inline constexpr auto logf(float x) noexcept -> float {
		return log2f(x) * 0.69314718055995F;
	}

	/// @brief Fast approximation calculation of 2^x
	///
	/// @param x - The exponent
	/// @return 2^x
	[[nodiscard]] inline constexpr auto pow2f(float x) noexcept -> float {
		return expf(x * 0.69314718055995F);
	}

	/// @brief Fast approximation calculation of 10^x
	///
	/// @param x - The exponent
	/// @return 10^x
	[[nodiscard]] inline constexpr auto pow10f(float x) noexcept -> float {
		return expf(2.302585092994046F * x);
	}

	/// @brief Fast approximation calculation of base^exponent
	///
	/// @param base - The base to use
	/// @param exponent - The exponent to use
	/// @return - base^exponent
	[[nodiscard]] inline constexpr auto powf(float base, float exponent) noexcept -> float {
		return pow2f(exponent * log2f(base));
	}

	/// @brief Calculates the mantissa and exponent of `x`,
	/// in the representation x = mantissa * 2^exponent
	///
	/// @param x - The value to calculate the mantissa and exponent for
	/// @param e - The `int` to store the exponent in
	/// @return - The mantissa
	[[nodiscard]] inline constexpr auto frexp(double x, int* e) noexcept -> double {
		union {
			double d;
			uint64_t i;
		} y = {x};
		int ee = y.i >> 52 & 0x7ff;

		if(!ee) {
			if(static_cast<bool>(x)) {
				x = frexp(x * 0x1p64, e);
				*e -= 64;
			}
			else {
				*e = 0;
			}
			return x;
		}
		else if(ee == 0x7ff) {
			return x;
		}

		*e = ee - 0x3fe;
		y.i &= 0x800fffffffffffffULL;
		y.i |= 0x3fe0000000000000ULL;
		return y.d;
	}

	/// @brief Helper function for `exp`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]] inline constexpr auto exp_helper(double x) noexcept -> double {
		return (5040.0
				+ x
					  * (5040.0
						 + x * (2520.0 + x * (840.0 + x * (210.0 + x * (42.0 + x * (7.0 + x)))))))
			   * 0.00019841269;
	}

	/// @brief Fast approximiation calculation of e^x
	///
	/// @param x - The exponent
	/// @return - e^x
	[[nodiscard]] inline constexpr auto exp(double x) noexcept -> double {
		if(x < 2.5) {
			return 2.7182818 * exp_helper(x - 1.0);
		}
		else if(x < 5.0) {
			return 33.115452 * exp_helper(x - 3.5);
		}
		else {
			return 483.42879 * exp_helper(x - 6.0);
		}
	}

	/// @brief Fast approximation calculation of log_2(x)
	///
	/// @param x - The input
	/// @return - log_2(x)
	[[nodiscard]] inline constexpr auto log2(double x) noexcept -> double {
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
		y += static_cast<double>(e);
		return (y);
	}

	/// @brief Fast approximation calculation of log_10(x)
	///
	/// @param x - The input
	/// @return - log_10(x)
	[[nodiscard]] inline constexpr auto log10(double x) noexcept -> double {
		return log2(x) * 0.3010299956639812;
	}

	/// @brief Fast approximation calculation of ln(x)
	///
	/// @param x - The input
	/// @return - ln(x)
	[[nodiscard]] inline constexpr auto log(double x) noexcept -> double {
		return log2(x) * 0.69314718055995;
	}

	/// @brief Fast approximation calculation of 2^x
	///
	/// @param x - The exponent
	/// @return 2^x
	[[nodiscard]] inline constexpr auto pow2(double x) noexcept -> double {
		return exp(x * 0.69314718055995);
	}

	/// @brief Fast approximation calculation of 10^x
	///
	/// @param x - The exponent
	/// @return 10^x
	[[nodiscard]] inline constexpr auto pow10(double x) noexcept -> double {
		return exp(2.302585092994046 * x);
	}

	/// @brief Fast approximation calculation of base^exponent
	///
	/// @param base - The base to use
	/// @param exponent - The exponent to use
	/// @return - base^exponent
	[[nodiscard]] inline constexpr auto pow(double base, double exponent) noexcept -> double {
		return pow2(exponent * log2(base));
	}
} // namespace apex::math
