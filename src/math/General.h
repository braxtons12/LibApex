#pragma once

#include <cstdint>
#include <type_traits>

namespace apex::math {

#ifndef _MSC_VER
	using std::int32_t;
	using std::size_t;
#endif //_MSC_VER

	template<typename NumericType = float,
			 std::enable_if_t<std::is_arithmetic_v<NumericType>, bool> = true>
	class General {
	  public:
		/// @brief Calculates the maximum of the two values
		///
		/// @param x
		/// @param y
		/// @return The maximum of the two
		[[nodiscard]] inline static constexpr auto
		max(NumericType left, NumericType right) noexcept -> NumericType {
			return (left > right ? left : right);
		}

		/// @brief Calculates the absolute value of x
		///
		/// @param x - The value to take the absolute value of
		/// @return - The absolute value of x
		[[nodiscard]] inline static constexpr auto abs(NumericType x) noexcept -> NumericType {
			if constexpr(std::is_same_v<NumericType, float>) {
				return sqrtf_internal(x * x);
			}
			else if constexpr(std::is_same_v<NumericType, double>) {
				return sqrt_internal(x * x);
			}
			else {
				return (x >= 0 ? x : -x);
			}
		}

		/// @brief Fast approximation calculation of the square root of the given value
		///
		/// @param x - The value to take the square root of
		/// @return - The square root of x
		template<typename FloatType = NumericType,
				 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
		[[nodiscard]] inline static constexpr auto sqrt(FloatType x) noexcept -> FloatType {
			if constexpr(std::is_same_v<FloatType, float>) {
				return sqrtf_internal(x);
			}
			else {
				return sqrt_internal(x);
			}
		}

		/// @brief Calculates the truncation of x
		///
		/// @param x - The value to truncate
		/// @return - The truncated value
		template<typename FloatType = NumericType,
				 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
		[[nodiscard]] inline static constexpr auto trunc(FloatType x) noexcept -> FloatType {
			if constexpr(std::is_same_v<FloatType, float>) {
				return static_cast<float>(static_cast<std::int64_t>(x));
			}
			else {
				return static_cast<double>(static_cast<std::int64_t>(x));
			}
		}

		/// @brief Calculates the floating point modulus, x mod y
		///
		/// @param x - The moduland
		/// @param y - The dividend
		/// @return x mod y
		template<typename FloatType = NumericType,
				 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
		[[nodiscard]] inline static constexpr auto
		fmod(FloatType x, FloatType y) noexcept -> FloatType {
			if constexpr(std::is_same_v<FloatType, float>) {
				return x - trunc<float>(x / y) * y;
			}
			else {
				return x - trunc<double>(x / y) * y;
			}
		}

		/// @brief Calculates the integer rounded version of the given value
		///
		/// @param x - The value to round
		/// @return - The rounded value
		template<typename FloatType = NumericType,
				 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
		[[nodiscard]] inline static constexpr auto round(FloatType x) noexcept -> int32_t {
			if constexpr(std::is_same_v<FloatType, float>) {
				return static_cast<int32_t>(x + 0.5F);
			}
			else {
				return static_cast<int32_t>(x + 0.5);
			}
		}

		/// @brief Calculates the unsigned integer rounded version of the given value
		///
		/// @param x - The value to round
		/// @return - The rounded value
		template<typename FloatType = NumericType,
				 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
		[[nodiscard]] inline static constexpr auto roundU(FloatType x) noexcept -> size_t {
			if constexpr(std::is_same_v<FloatType, float>) {
				return static_cast<size_t>(x + 0.5F);
			}
			else {
				return static_cast<size_t>(x + 0.5);
			}
		}

	  private:
		/// @brief Fast approximation calculation of the square root of the given value
		///
		/// @param x - The value to take the square root of
		/// @return - The square root of x
		[[nodiscard]] static constexpr inline auto sqrtf_internal(float x) noexcept -> float {
			const float xhalf = 0.5F * x;
			union // get bits for floating value
			{
				float f = 0.0F;
				int i;
			} u;
			u.f = x;
			u.i = 0x5F375A86
				  - (u.i >> 1); // gives initial guess y0. use 0x5fe6ec85e7de30da for double
			u.f = u.f * (1.5F - xhalf * u.f * u.f); // Newton method, repeating increases accuracy
			u.f = u.f * (1.5F - xhalf * u.f * u.f); // Newton method, repeating increases accuracy
			return x * u.f;
		}

		/// @brief Fast approximation calculation of the square root of the given value
		///
		/// @param x - The value to take the square root of
		/// @return - The square root of x
		[[nodiscard]] static constexpr inline auto sqrt_internal(double x) noexcept -> double {
			const double xhalf = 0.5 * x;
			union // get bits for floating value
			{
				double f = 0.0;
				int64_t i;
			} u;
			u.f = x;
			u.i = 0x5fe6ec85e7de30da
				  - (u.i >> 1); // gives initial guess y0. use 0x5fe6ec85e7de30da for double
			u.f = u.f * (1.5 - xhalf * u.f * u.f); // Newton method, repeating increases accuracy
			u.f = u.f * (1.5 - xhalf * u.f * u.f); // Newton method, repeating increases accuracy
			return x * u.f;
		}
	};
} // namespace apex::math
