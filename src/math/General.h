#pragma once
#include <cstdint>

namespace apex::math {

#ifndef _MSC_VER
	using std::size_t;
#endif //_MSC_VER

	/// @brief Fast approximation calculation of the square root of the given value
	///
	/// @param x - The value to take the square root of
	/// @return - The square root of x
	[[nodiscard]]
		constexpr inline auto sqrtf(float x) noexcept -> float {
			float n = (x > 10.0F ? (x / 10.0F) : (x / 2.0F));
			float y = 1;
			double e = 0.0000000001;
			while(n - y > e) {
				n = (n + y) / 2.0F;
				y = x / n;
			}
			return n;
		}

	/// @brief Calculates the absolute value of x
	///
	/// @param x - The value to take the absolute value of
	/// @return - The absolute value of x
	[[nodiscard]]
		constexpr inline auto fabsf(float x) noexcept -> float {
			return sqrtf(x * x);
		}

	/// @brief Calculates the truncation of x
	///
	/// @param x - The value to truncate
	/// @return - The truncated value
	[[nodiscard]]
		constexpr inline auto truncf(float x) noexcept -> float {
			return static_cast<float>(static_cast<std::int64_t>(x));
		}

	/// @brief Calculates the floating point modulus x mod y
	///
	/// @param x - The moduland
	/// @param y - The dividend
	/// @return x mod y
	[[nodiscard]]
		constexpr inline auto fmodf(float x, float y) noexcept -> float {
			return x - truncf(x / y) * y;
		}

	/// @brief Calculates the maximum of the two values
	///
	/// @param x
	/// @param y
	/// @return The maximum of the two
	[[nodiscard]]
		constexpr inline auto max(float x, float y) noexcept -> float {
			return (x > y ? x : y);
		}

	/// @brief Fast approximation calculation of the square root of the given value
	///
	/// @param x - The value to take the square root of
	/// @return - The square root of x
	[[nodiscard]]
		constexpr inline auto sqrt(double x) noexcept -> double {
			double n = (x > 10.0 ? (x / 10.0) : (x / 2.0));
			double y = 1;
			double e = 0.0000000001;
			while(n - y > e) {
				n = (n + y) / 2.0;
				y = x / n;
			}
			return n;
		}

	/// @brief Calculates the absolute value of x
	///
	/// @param x - The value to take the absolute value of
	/// @return - The absolute value of x
	[[nodiscard]]
		constexpr inline auto fabs(double x) noexcept -> double {
			return sqrt(x * x);
		}

	/// @brief Calculates the truncation of x
	///
	/// @param x - The value to truncate
	/// @return - The truncated value
	[[nodiscard]]
		constexpr inline auto trunc(double x) noexcept -> double {
			return static_cast<float>(static_cast<std::int64_t>(x));
		}

	/// @brief Calculates the floating point modulus x mod y
	///
	/// @param x - The moduland
	/// @param y - The dividend
	/// @return x mod y
	[[nodiscard]]
		constexpr inline auto fmod(double x, double y) noexcept -> double {
			return x - trunc(x / y) * y;
		}

	/// @brief Calculates the maximum of the two values
	///
	/// @param x
	/// @param y
	/// @return The maximum of the two
	[[nodiscard]]
		constexpr inline auto max(double x, double y) noexcept -> double {
			return (x > y ? x : y);
		}

	/// @brief Calculates the maximum of the two values
	///
	/// @param x
	/// @param y
	/// @return The maximum of the two
	[[nodiscard]]
		constexpr inline auto max(int x, int y) noexcept -> int {
			return (x > y ? x : y);
		}

	/// @brief Calculates the maximum of the two values
	///
	/// @param x
	/// @param y
	/// @return The maximum of the two
	[[nodiscard]]
		constexpr inline auto max(size_t x, size_t y) noexcept -> size_t {
			return (x > y ? x : y);
		}
} //namespace apex::math
