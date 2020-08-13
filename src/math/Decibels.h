#pragma once

#include <gsl/gsl>

#include "Exponentials.h"

namespace apex::math {
	/// @brief Basic class for working with decibel values and converting from decibels to linear
	/// and vice versa
	class Decibels {
	  public:
		static const constexpr double MINUS_INFINITY_DB = -100.0;

		/// @brief Constructs a `Decibels` with a value of minus infinity db
		constexpr Decibels() noexcept = default;

		/// @brief Constructs a `Decibels` with the given decibel value
		///
		/// @param decibels - The decibel value
		explicit constexpr Decibels(double decibels) noexcept : mValue(decibels) {
		}

		/// @brief Copy constructs a `Decibels` from the given one
		///
		/// @param decibels - The `Decibels` to copy
		constexpr Decibels(const Decibels& decibels) noexcept = default;

		/// @brief Move constructs the given `Decibels`
		///
		/// @param decibels - The `Decibels` to move
		constexpr Decibels(Decibels&& decibels) noexcept = default;
		~Decibels() noexcept = default;

		/// @brief Converts the given linear value to the corresponding Decibel value
		///
		/// @param linear - The linear value to convert
		/// @return - The Decibel value
		[[nodiscard]] static inline constexpr auto
		linearToDecibels(float linear) noexcept -> float {
			return 20.0F * log10f(linear);
		}

		/// @brief Converts the given Decibel value to the corresponding linear value
		///
		/// @param decibels - The Decibel value to convert
		/// @return - The linear value
		[[nodiscard]] static inline constexpr auto
		decibelsToLinear(float decibels) noexcept -> float {
			return pow10f(decibels / 20.0F);
		}

		/// @brief Converts the given linear value to the corresponding Decibel value
		///
		/// @param linear - The linear value to convert
		/// @return - The Decibel value
		[[nodiscard]] static inline constexpr auto
		linearToDecibels(double linear) noexcept -> double {
			return 20.0 * log10(linear);
		}

		/// @brief Converts the given Decibel value to the corresponding linear value
		///
		/// @param decibels - The Decibel value to convert
		/// @return - The linear value
		[[nodiscard]] static inline constexpr auto
		decibelsToLinear(double decibels) noexcept -> double {
			return pow10(decibels / 20.0);
		}

		/// @brief Converts the given decibels to a linear value
		///
		/// @param decibels - The decibel value to convert
		/// @return - The corresponding linear value
		[[nodiscard]] static inline constexpr auto toLinear(Decibels decibels) noexcept -> double {
			return pow10(decibels.mValue / 20.0);
		}

		/// @brief Converts the given linear value to a decibel value
		///
		/// @param linear - The linear value to convert
		/// @return - The corresponding decibel value
		[[nodiscard]] static inline constexpr auto fromLinear(float linear) noexcept -> Decibels {
			return Decibels(20.0F * log10f(linear));
		}

		/// @brief Converts the given linear value to a decibel value
		///
		/// @param linear - The linear value to convert
		/// @return - The corresponding decibel value
		[[nodiscard]] static inline constexpr auto fromLinear(double linear) noexcept -> Decibels {
			return Decibels(20.0 * log10(linear));
		}

		/// @brief Returns the decibel value of this
		///
		/// @return - The value of this
		[[nodiscard]] constexpr inline auto getDecibels() const noexcept -> double {
			return mValue;
		}

		/// @brief Sets this to the given decibel value
		///
		/// @param decibels - The value to set this to, in decibels
		constexpr inline auto setFromDecibels(float decibels) noexcept -> void {
			mValue = decibels;
		}

		/// @brief Sets this to the given decibel value
		///
		/// @param decibels - The value to set this to, in decibels
		constexpr inline auto setFromDecibels(double decibels) noexcept -> void {
			mValue = decibels;
		}

		/// @brief Sets this to the given decibel vlaue
		///
		/// @param decibels - The value to set this to
		constexpr inline auto setFromDecibels(Decibels decibels) noexcept -> void {
			mValue = decibels.mValue;
		}

		/// @brief Returns the linear value of this
		///
		/// @return - The linear value of this
		[[nodiscard]] constexpr inline auto getLinear() const noexcept -> double {
			return pow10(mValue / 20.0);
		}

		/// @brief Sets the this to the given linear value
		///
		/// @param linear - The linear value to set this to
		constexpr inline auto setFromLinear(double linear) noexcept -> void {
			mValue = 20.0 * log10(linear);
		}

		/// @brief Sets the this to the given linear value
		///
		/// @param linear - The linear value to set this to
		constexpr inline auto setFromLinear(float linear) noexcept -> void {
			mValue = 20.0F * log10f(linear);
		}

		friend constexpr inline auto operator+(const Decibels& lhs, float x) noexcept -> Decibels {
			return Decibels(lhs.mValue + x);
		}

		friend constexpr inline auto operator+(const Decibels& lhs, double x) noexcept -> Decibels {
			return Decibels(lhs.mValue + x);
		}

		friend constexpr inline auto operator+(float x, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(x + rhs.mValue);
		}

		friend constexpr inline auto operator+(double x, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(x + rhs.mValue);
		}

		friend constexpr inline auto
		operator+(const Decibels& lhs, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(lhs.mValue + rhs.mValue);
		}

		constexpr inline auto operator+=(float x) noexcept -> Decibels& {
			mValue += x;
			return *this;
		}

		constexpr inline auto operator+=(double x) noexcept -> Decibels& {
			mValue += x;
			return *this;
		}

		constexpr inline auto operator+=(const Decibels& rhs) noexcept -> Decibels& {
			mValue += rhs.mValue;
			return *this;
		}

		friend constexpr inline auto operator-(const Decibels& lhs, float x) noexcept -> Decibels {
			return Decibels(lhs.mValue - x);
		}

		friend constexpr inline auto operator-(const Decibels& lhs, double x) noexcept -> Decibels {
			return Decibels(lhs.mValue - x);
		}

		friend constexpr inline auto operator-(float x, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(x - rhs.mValue);
		}

		friend constexpr inline auto operator-(double x, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(x - rhs.mValue);
		}

		friend constexpr inline auto
		operator-(const Decibels& lhs, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(lhs.mValue - rhs.mValue);
		}

		constexpr inline auto operator-=(float x) noexcept -> Decibels& {
			mValue -= x;
			return *this;
		}

		constexpr inline auto operator-=(double x) noexcept -> Decibels& {
			mValue -= x;
			return *this;
		}

		constexpr inline auto operator-=(const Decibels& rhs) noexcept -> Decibels& {
			mValue -= rhs.mValue;
			return *this;
		}

		friend constexpr inline auto operator-(const Decibels& lhs) noexcept -> Decibels {
			return Decibels(-lhs.mValue);
		}

		friend constexpr inline auto operator*(const Decibels& lhs, float x) noexcept -> Decibels {
			return Decibels(lhs.mValue * x);
		}

		friend constexpr inline auto operator*(const Decibels& lhs, double x) noexcept -> Decibels {
			return Decibels(lhs.mValue * x);
		}

		friend constexpr inline auto operator*(float x, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(x * rhs.mValue);
		}

		friend constexpr inline auto operator*(double x, const Decibels& rhs) noexcept -> Decibels {
			return Decibels(x * rhs.mValue);
		}

		constexpr inline auto operator*=(float x) noexcept -> Decibels& {
			mValue *= x;
			return *this;
		}

		constexpr inline auto operator*=(double x) noexcept -> Decibels& {
			mValue *= x;
			return *this;
		}

		friend constexpr inline auto operator/(const Decibels& lhs, float x) noexcept -> Decibels {
			return Decibels(lhs.mValue / x);
		}

		friend constexpr inline auto operator/(const Decibels& lhs, double x) noexcept -> Decibels {
			return Decibels(lhs.mValue / x);
		}

		friend constexpr inline auto operator/(float x, const Decibels& rhs) noexcept -> float {
			return gsl::narrow_cast<float>(x / rhs.mValue);
		}

		friend constexpr inline auto operator/(double x, const Decibels& rhs) noexcept -> double {
			return x / rhs.mValue;
		}

		constexpr inline auto operator/=(float x) noexcept -> Decibels& {
			mValue /= x;
			return *this;
		}

		constexpr inline auto operator/=(double x) noexcept -> Decibels& {
			mValue /= x;
			return *this;
		}

		friend constexpr inline auto operator<(const Decibels& lhs, float rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Decibels& lhs, double rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(float lhs, const Decibels& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(double lhs, const Decibels& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto
		operator<(const Decibels& lhs, const Decibels& rhs) noexcept -> bool {
			return lhs.mValue < rhs.mValue;
		}

		friend constexpr inline auto operator>(const Decibels& lhs, float rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Decibels& lhs, double rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(float lhs, const Decibels& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(double lhs, const Decibels& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto
		operator>(const Decibels& lhs, const Decibels& rhs) noexcept -> bool {
			return lhs.mValue > rhs.mValue;
		}

		friend constexpr inline auto operator==(const Decibels& lhs, float rhs) noexcept -> bool {
			return lhs.mValue == rhs;
		}

		friend constexpr inline auto operator==(const Decibels& lhs, double rhs) noexcept -> bool {
			return lhs.mValue == rhs;
		}

		friend constexpr inline auto operator==(float lhs, const Decibels& rhs) noexcept -> bool {
			return lhs == rhs.mValue;
		}

		friend constexpr inline auto operator==(double lhs, const Decibels& rhs) noexcept -> bool {
			return lhs == rhs.mValue;
		}

		friend constexpr inline auto
		operator==(const Decibels& lhs, const Decibels& rhs) noexcept -> bool {
			return lhs.mValue == rhs.mValue;
		}

		friend constexpr inline auto operator<=(const Decibels& lhs, float rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(const Decibels& lhs, double rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(float lhs, const Decibels& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(double lhs, const Decibels& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto
		operator<=(const Decibels& lhs, const Decibels& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Decibels& lhs, float rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Decibels& lhs, double rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(float lhs, const Decibels& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(double lhs, const Decibels& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto
		operator>=(const Decibels& lhs, const Decibels& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		constexpr inline auto operator=(float x) noexcept -> Decibels& {
			mValue = x;
			return *this;
		}

		constexpr inline auto operator=(double x) noexcept -> Decibels& {
			mValue = x;
			return *this;
		}

		constexpr inline auto operator=(const Decibels& rhs) noexcept -> Decibels& = default;
		constexpr inline auto operator=(Decibels&& rhs) noexcept -> Decibels& = default;

		explicit constexpr inline operator float() const noexcept {
			return gsl::narrow_cast<float>(mValue);
		}

		explicit constexpr inline operator double() const noexcept {
			return mValue;
		}

		explicit constexpr inline operator int() const noexcept {
			return gsl::narrow_cast<int>(mValue);
		}

	  private:
		double mValue = MINUS_INFINITY_DB;
	};

	constexpr inline auto operator"" _dB(long double x) noexcept -> Decibels {
		return Decibels(gsl::narrow_cast<double>(x));
	}

	constexpr inline auto operator""_dB(unsigned long long x) noexcept -> Decibels {
		return Decibels(gsl::narrow_cast<double>(x));
	}
} // namespace apex::math
