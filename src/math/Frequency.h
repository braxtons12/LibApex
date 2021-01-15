#pragma once

#include <gsl/gsl>

#include "TrigFuncs.h"

namespace apex::math {
	using gsl::narrow_cast;
	class Radians;

	/// @brief Basic class for working with frequency values in Hertz and converting from Hertz to
	/// Radians and vice-versa
	class Hertz {
	  public:
		/// @brief Constructs a `Hertz` with a value of 0 Hertz
		constexpr Hertz() noexcept = default;

		/// @brief Constructs a `Hertz` with the given frequency in Hertz
		///
		/// @param hertz - The frequency in Hertz
		constexpr Hertz(float hertz) noexcept : mValue(hertz) {
		}

		/// @brief Constructs a `Hertz` with the given frequency in Hertz
		///
		/// @param hertz - The frequency in Hertz
		constexpr Hertz(double hertz) noexcept : mValue(hertz) {
		}

		/// @brief Copy constructs a `Hertz` from the given one
		///
		/// @param hertz - The `Hertz` to copy
		constexpr Hertz(const Hertz& hertz) noexcept = default;

		/// @brief Move constructs the given `Hertz`
		///
		/// @param hertz - The `Hertz` to move
		constexpr Hertz(Hertz&& hertz) noexcept = default;
		~Hertz() noexcept = default;

		/// @brief Converts the given Hertz value to Radians
		///
		/// @param hertz - The Hertz value to convert
		///
		/// @return - The corresponding value in Radians
		[[nodiscard]] static inline constexpr auto hertzToRadians(float hertz) noexcept -> float {
			return hertz / Constants<>::twoPi;
		}

		/// @brief Converts the given Hertz value to Radians
		///
		/// @param hertz - The Hertz value to convert
		///
		/// @return - The corresponding value in Radians
		[[nodiscard]] static inline constexpr auto hertzToRadians(double hertz) noexcept -> double {
			return hertz / Constants<double>::twoPi;
		}

		/// @brief Converts the given Hertz value to Radians
		///
		/// @param hertz - The Hertz value to convert
		///
		/// @return - The corresponding value in Radians
		[[nodiscard]] static constexpr auto toRadians(Hertz hertz) noexcept -> Radians;

		/// @brief Converts the given Radians value to Hertz
		///
		/// @param radians - The Hertz value to convert
		///
		/// @return - The corresponding value in Hertz
		[[nodiscard]] static constexpr auto fromRadians(float radians) noexcept -> Hertz {
			return Hertz(radians * Constants<>::twoPi);
		}

		/// @brief Converts the given Radians value to Hertz
		///
		/// @param radians - The Hertz value to convert
		///
		/// @return - The corresponding value in Hertz
		[[nodiscard]] static constexpr auto fromRadians(double radians) noexcept -> Hertz {
			return Hertz(radians * Constants<double>::twoPi);
		}

		/// @brief Converts the given Radians value to Hertz
		///
		/// @param radians - The Hertz value to convert
		///
		/// @return - The corresponding value in Hertz
		[[nodiscard]] static constexpr auto fromRadians(Radians radians) noexcept -> Hertz;

		/// @brief Returns the Hertz value of this
		///
		/// @return - The Hertz value
		[[nodiscard]] constexpr inline auto getHertz() const noexcept -> double {
			return mValue;
		}

		/// @brief Sets the value of this to the given Hertz value
		///
		/// @param hertz - The new Hertz value
		constexpr inline auto setFromHertz(float hertz) noexcept -> void {
			mValue = hertz;
		}

		/// @brief Sets the value of this to the given Hertz value
		///
		/// @param hertz - The new Hertz value
		constexpr inline auto setFromHertz(double hertz) noexcept -> void {
			mValue = hertz;
		}

		/// @brief Sets the value of this to the given Hertz value
		///
		/// @param hertz - The new Hertz value
		constexpr inline auto setFromHertz(Hertz hertz) noexcept -> void {
			mValue = hertz.mValue;
		}

		/// @brief Returns the Radians value of this
		///
		/// @return - The Radians value
		[[nodiscard]] constexpr inline auto getRadians() const noexcept -> Radians;

		/// @brief Sets the value of this to the given Radians value
		///
		/// @param radians - The new value, in Radians
		constexpr inline auto setFromRadians(float radians) noexcept -> void {
			mValue = radians * Constants<>::twoPi;
		}

		/// @brief Sets the value of this to the given Radians value
		///
		/// @param radians - The new value, in Radians
		constexpr inline auto setFromRadians(double radians) noexcept -> void {
			mValue = radians * Constants<double>::twoPi;
		}

		/// @brief Sets the value of this to the given Radians value
		///
		/// @param radians - The new value, in Radians
		constexpr inline auto setFromRadians(Radians radians) noexcept -> void;

		friend constexpr inline auto operator+(const Hertz& lhs, float rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue + rhs);
		}

		friend constexpr inline auto operator+(const Hertz& lhs, double rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue + rhs);
		}

		friend constexpr inline auto operator+(const Hertz& lhs, int64_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue + narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator+(const Hertz& lhs, size_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue + narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator+(float lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs + rhs.mValue);
		}

		friend constexpr inline auto operator+(double lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs + rhs.mValue);
		}

		friend constexpr inline auto operator+(int64_t lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(narrow_cast<double>(lhs) + rhs.mValue);
		}

		friend constexpr inline auto operator+(size_t lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(narrow_cast<double>(lhs) + rhs.mValue);
		}

		friend constexpr inline auto
		operator+(const Hertz& lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue + rhs.mValue);
		}

		constexpr inline auto operator+=(float x) noexcept -> Hertz& {
			mValue += x;
			return *this;
		}

		constexpr inline auto operator+=(double x) noexcept -> Hertz& {
			mValue += x;
			return *this;
		}

		constexpr inline auto operator+=(int64_t x) noexcept -> Hertz& {
			mValue += narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator+=(size_t x) noexcept -> Hertz& {
			mValue += narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator+=(const Hertz& rhs) noexcept -> Hertz& {
			mValue += rhs.mValue;
			return *this;
		}

		friend constexpr inline auto operator-(const Hertz& lhs, float rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue - rhs);
		}

		friend constexpr inline auto operator-(const Hertz& lhs, double rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue - rhs);
		}

		friend constexpr inline auto operator-(const Hertz& lhs, int64_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue - narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator-(const Hertz& lhs, size_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue - narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator-(float lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs - rhs.mValue);
		}

		friend constexpr inline auto operator-(double lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs - rhs.mValue);
		}

		friend constexpr inline auto operator-(int64_t lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(narrow_cast<double>(lhs) - rhs.mValue);
		}

		friend constexpr inline auto operator-(size_t lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(narrow_cast<double>(lhs) - rhs.mValue);
		}

		friend constexpr inline auto
		operator-(const Hertz& lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue - rhs.mValue);
		}

		constexpr inline auto operator-=(float x) noexcept -> Hertz& {
			mValue -= x;
			return *this;
		}

		constexpr inline auto operator-=(double x) noexcept -> Hertz& {
			mValue -= x;
			return *this;
		}

		constexpr inline auto operator-=(int64_t x) noexcept -> Hertz& {
			mValue -= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator-=(size_t x) noexcept -> Hertz& {
			mValue -= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator-=(const Hertz& rhs) noexcept -> Hertz& {
			mValue -= rhs.mValue;
			return *this;
		}

		friend constexpr inline auto operator-(const Hertz& lhs) noexcept -> Hertz {
			return Hertz(-lhs.mValue);
		}

		friend constexpr inline auto operator*(const Hertz& lhs, float rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue * rhs);
		}

		friend constexpr inline auto operator*(const Hertz& lhs, double rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue * rhs);
		}

		friend constexpr inline auto operator*(const Hertz& lhs, int64_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue * narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator*(const Hertz& lhs, size_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue * narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator*(float lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs * rhs.mValue);
		}

		friend constexpr inline auto operator*(double lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(lhs * rhs.mValue);
		}

		friend constexpr inline auto operator*(int64_t lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(narrow_cast<double>(lhs) * rhs.mValue);
		}

		friend constexpr inline auto operator*(size_t lhs, const Hertz& rhs) noexcept -> Hertz {
			return Hertz(narrow_cast<double>(lhs) * rhs.mValue);
		}

		constexpr inline auto operator*=(float x) noexcept -> Hertz& {
			mValue *= x;
			return *this;
		}

		constexpr inline auto operator*=(double x) noexcept -> Hertz& {
			mValue *= x;
			return *this;
		}

		constexpr inline auto operator*=(int64_t x) noexcept -> Hertz& {
			mValue *= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator*=(size_t x) noexcept -> Hertz& {
			mValue *= narrow_cast<double>(x);
			return *this;
		}

		friend constexpr inline auto operator/(const Hertz& lhs, float rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue / rhs);
		}

		friend constexpr inline auto operator/(const Hertz& lhs, double rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue / rhs);
		}

		friend constexpr inline auto operator/(const Hertz& lhs, int64_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue / narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator/(const Hertz& lhs, size_t rhs) noexcept -> Hertz {
			return Hertz(lhs.mValue / narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator/(float lhs, const Hertz& rhs) noexcept -> float {
			return narrow_cast<float>(lhs / rhs.mValue);
		}

		friend constexpr inline auto operator/(double lhs, const Hertz& rhs) noexcept -> double {
			return lhs / rhs.mValue;
		}

		friend constexpr inline auto operator/(int64_t lhs, const Hertz& rhs) noexcept -> int64_t {
			return static_cast<int64_t>(narrow_cast<double>(lhs) / rhs.mValue);
		}

		friend constexpr inline auto operator/(size_t lhs, const Hertz& rhs) noexcept -> size_t {
			return static_cast<size_t>(narrow_cast<double>(lhs) / rhs.mValue);
		}

		constexpr inline auto operator/=(float x) noexcept -> Hertz& {
			mValue /= x;
			return *this;
		}

		constexpr inline auto operator/=(double x) noexcept -> Hertz& {
			mValue /= x;
			return *this;
		}

		constexpr inline auto operator/=(int64_t x) noexcept -> Hertz& {
			mValue /= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator/=(size_t x) noexcept -> Hertz& {
			mValue /= narrow_cast<double>(x);
			return *this;
		}

		friend constexpr inline auto operator<(const Hertz& lhs, float rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Hertz& lhs, double rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Hertz& lhs, int64_t rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Hertz& lhs, size_t rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(float lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(double lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(int64_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(size_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto
		operator<(const Hertz& lhs, const Hertz& rhs) noexcept -> bool {
			return lhs.mValue < rhs.mValue;
		}

		friend constexpr inline auto operator>(const Hertz& lhs, float rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Hertz& lhs, double rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Hertz& lhs, int64_t rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Hertz& lhs, size_t rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(float lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(double lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(int64_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(size_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto
		operator>(const Hertz& lhs, const Hertz& rhs) noexcept -> bool {
			return lhs.mValue > rhs.mValue;
		}

		friend constexpr inline auto operator==(const Hertz& lhs, float rhs) noexcept -> bool {
			return lhs.mValue == rhs;
		}

		friend constexpr inline auto operator==(const Hertz& lhs, double rhs) noexcept -> bool {
			return lhs.mValue == rhs;
		}

		friend constexpr inline auto operator==(const Hertz& lhs, int64_t rhs) noexcept -> bool {
			return lhs.mValue == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator==(const Hertz& lhs, size_t rhs) noexcept -> bool {
			return lhs.mValue == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator==(float lhs, const Hertz& rhs) noexcept -> bool {
			return lhs == rhs.mValue;
		}

		friend constexpr inline auto operator==(double lhs, const Hertz& rhs) noexcept -> bool {
			return lhs == rhs.mValue;
		}

		friend constexpr inline auto operator==(int64_t lhs, const Hertz& rhs) noexcept -> bool {
			return narrow_cast<double>(lhs) == rhs.mValue;
		}

		friend constexpr inline auto operator==(size_t lhs, const Hertz& rhs) noexcept -> bool {
			return narrow_cast<double>(lhs) == rhs.mValue;
		}

		friend constexpr inline auto
		operator==(const Hertz& lhs, const Hertz& rhs) noexcept -> bool {
			return lhs.mValue == rhs.mValue;
		}

		friend constexpr inline auto operator<=(const Hertz& lhs, float rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(const Hertz& lhs, double rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(const Hertz& lhs, int64_t rhs) noexcept -> bool {
			return lhs < rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator<=(const Hertz& lhs, size_t rhs) noexcept -> bool {
			return lhs < rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator<=(float lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(double lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(int64_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto operator<=(size_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto
		operator<=(const Hertz& lhs, const Hertz& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Hertz& lhs, float rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Hertz& lhs, double rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Hertz& lhs, int64_t rhs) noexcept -> bool {
			return lhs > rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator>=(const Hertz& lhs, size_t rhs) noexcept -> bool {
			return lhs > rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator>=(float lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(double lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(int64_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto operator>=(size_t lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto
		operator>=(const Hertz& lhs, const Hertz& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		constexpr inline auto operator=(const float& x) noexcept -> Hertz& {
			mValue = x;
			return *this;
		}

		constexpr inline auto operator=(const double& x) noexcept -> Hertz& {
			mValue = x;
			return *this;
		}

		constexpr inline auto operator=(const int64_t& x) noexcept -> Hertz& {
			mValue = narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator=(const size_t& x) noexcept -> Hertz& {
			mValue = narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator=(const Hertz& rhs) noexcept -> Hertz& = default;
		constexpr inline auto operator=(Hertz&& rhs) noexcept -> Hertz& = default;

		explicit constexpr inline operator float() const noexcept {
			return narrow_cast<float>(mValue);
		}

		explicit constexpr inline operator double() const noexcept {
			return mValue;
		}

		explicit constexpr inline operator int() const noexcept {
			return narrow_cast<int>(mValue);
		}

		explicit constexpr inline operator int64_t() const noexcept {
			return narrow_cast<int64_t>(mValue);
		}

		explicit constexpr inline operator size_t() const noexcept {
			return narrow_cast<size_t>(mValue);
		}

		explicit constexpr inline operator Radians() const noexcept;

	  private:
		double mValue = 0.0;
	};

	/// @brief Basic class for working with frequency values in Radians and converting from Radians
	/// to Hertz and vice-versa
	class Radians {
	  public:
		/// @brief Constructs a `Radians` with a value of 0 Radians
		constexpr Radians() noexcept = default;

		/// @brief Constructs a `Radians` with the given frequency in Radians
		///
		/// @param radians - The frequency in Radians
		constexpr Radians(float radians) noexcept : mValue(radians) {
		}

		/// @brief Constructs a `Radians` with the given frequency in Radians
		///
		/// @param radians - The frequency in Radians
		constexpr Radians(double radians) noexcept : mValue(radians) {
		}

		/// @brief Copy constructs a `Radians` from the given one
		///
		/// @param radians - The `Radians` to copy
		constexpr Radians(const Radians& radians) noexcept = default;

		/// @brief Move contructs the given `Radians`
		///
		/// @param radians - The `Radians` to move
		constexpr Radians(Radians&& radians) noexcept = default;
		~Radians() noexcept = default;

		/// @brief Converts the given Radians value to Hertz
		///
		/// @param radians - The Radians value to convert
		///
		/// @return - The corresponding value in Hertz
		[[nodiscard]] static inline constexpr auto radiansToHertz(float radians) noexcept -> float {
			return radians * Constants<>::twoPi;
		}

		/// @brief Converts the given Radians value to Hertz
		///
		/// @param radians - The Radians value to convert
		///
		/// @return - The corresponding value in Hertz
		[[nodiscard]] static inline constexpr auto
		radiansToHertz(double radians) noexcept -> double {
			return radians * Constants<double>::twoPi;
		}

		/// @brief Converts the given Radians value to Hertz
		///
		/// @param radians - The Radians value to convert
		///
		/// @return - The corresponding value in Hertz
		[[nodiscard]] static inline constexpr auto toHertz(Radians radians) noexcept -> Hertz {
			return Hertz(Constants<double>::twoPi * static_cast<double>(radians));
		}

		/// @brief Converts the given Hertz value to Radians
		///
		/// @param hertz - The Hertz value to convert
		///
		/// @return - The corresponding Radians Value
		[[nodiscard]] static inline constexpr auto fromHertz(float hertz) noexcept -> Radians {
			return Radians(hertz / Constants<>::twoPi);
		}

		/// @brief Converts the given Hertz value to Radians
		///
		/// @param hertz - The Hertz value to convert
		///
		/// @return - The corresponding Radians Value
		[[nodiscard]] static inline constexpr auto fromHertz(double hertz) noexcept -> Radians {
			return Radians(hertz / Constants<double>::twoPi);
		}

		/// @brief Converts the given Hertz value to Radians
		///
		/// @param hertz - The Hertz value to convert
		///
		/// @return - The corresponding Radians Value
		[[nodiscard]] static inline constexpr auto fromHertz(Hertz hertz) noexcept -> Radians {
			return Radians(static_cast<double>(hertz) / Constants<double>::twoPi);
		}

		/// @brief Returns the Radians value of this
		///
		/// @return - The Radians value
		[[nodiscard]] constexpr inline auto getRadians() const noexcept -> double {
			return mValue;
		}

		/// @brief Sets the value of this to the given Radians value
		///
		/// @param radians - The new Radians value
		constexpr inline auto setFromRadians(float radians) noexcept -> void {
			mValue = radians;
		}

		/// @brief Sets the value of this to the given Radians value
		///
		/// @param radians - The new Radians value
		constexpr inline auto setFromRadians(double radians) noexcept -> void {
			mValue = radians;
		}

		/// @brief Sets the value of this to the given Radians value
		///
		/// @param radians - The new Radians value
		constexpr inline auto setFromRadians(Radians radians) noexcept -> void {
			mValue = radians.mValue;
		}

		/// @brief Returns the Hertz value of this
		///
		/// @return - The Hertz value
		[[nodiscard]] constexpr inline auto getHertz() const noexcept -> Hertz {
			return Hertz::fromRadians(mValue);
		}

		/// @brief Sets the value fo this to the given Hertz value
		///
		/// @param hertz - The new value, in Hertz
		constexpr inline auto setFromHertz(float hertz) noexcept -> void {
			mValue = hertz / Constants<>::twoPi;
		}

		/// @brief Sets the value fo this to the given Hertz value
		///
		/// @param hertz - The new value, in Hertz
		constexpr inline auto setFromHertz(double hertz) noexcept -> void {
			mValue = hertz / Constants<double>::twoPi;
		}

		/// @brief Sets the value fo this to the given Hertz value
		///
		/// @param hertz - The new value, in Hertz
		constexpr inline auto setFromHertz(Hertz hertz) noexcept -> void {
			mValue = static_cast<double>(hertz / Constants<double>::twoPi);
		}

		friend constexpr inline auto operator+(const Radians& lhs, float rhs) noexcept -> Radians {
			return Radians(lhs.mValue + rhs);
		}

		friend constexpr inline auto operator+(const Radians& lhs, double rhs) noexcept -> Radians {
			return Radians(lhs.mValue + rhs);
		}

		friend constexpr inline auto
		operator+(const Radians& lhs, int64_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue + narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator+(const Radians& lhs, size_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue + narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator+(float lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(lhs + rhs.mValue);
		}

		friend constexpr inline auto operator+(double lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(lhs + rhs.mValue);
		}

		friend constexpr inline auto
		operator+(int64_t lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(narrow_cast<double>(lhs) + rhs.mValue);
		}

		friend constexpr inline auto operator+(size_t lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(narrow_cast<double>(lhs) + rhs.mValue);
		}

		friend constexpr inline auto
		operator+(const Radians& lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(lhs.mValue + rhs.mValue);
		}

		constexpr inline auto operator+=(float x) noexcept -> Radians& {
			mValue += x;
			return *this;
		}

		constexpr inline auto operator+=(double x) noexcept -> Radians& {
			mValue += x;
			return *this;
		}

		constexpr inline auto operator+=(int64_t x) noexcept -> Radians& {
			mValue += narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator+=(size_t x) noexcept -> Radians& {
			mValue += narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator+=(const Radians& rhs) noexcept -> Radians& {
			mValue += rhs.mValue;
			return *this;
		}

		friend constexpr inline auto operator-(const Radians& lhs, float rhs) noexcept -> Radians {
			return Radians(lhs.mValue - rhs);
		}

		friend constexpr inline auto operator-(const Radians& lhs, double rhs) noexcept -> Radians {
			return Radians(lhs.mValue - rhs);
		}

		friend constexpr inline auto
		operator-(const Radians& lhs, int64_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue - narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator-(const Radians& lhs, size_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue - narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator-(float lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(lhs - rhs.mValue);
		}

		friend constexpr inline auto operator-(double lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(lhs - rhs.mValue);
		}

		friend constexpr inline auto
		operator-(int64_t lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(narrow_cast<double>(lhs) - rhs.mValue);
		}

		friend constexpr inline auto operator-(size_t lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(narrow_cast<double>(lhs) - rhs.mValue);
		}

		friend constexpr inline auto
		operator-(const Radians& lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(lhs.mValue - rhs.mValue);
		}

		constexpr inline auto operator-=(float x) noexcept -> Radians& {
			mValue -= x;
			return *this;
		}

		constexpr inline auto operator-=(double x) noexcept -> Radians& {
			mValue -= x;
			return *this;
		}

		constexpr inline auto operator-=(int64_t x) noexcept -> Radians& {
			mValue -= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator-=(size_t x) noexcept -> Radians& {
			mValue -= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator-=(const Radians& rhs) noexcept -> Radians& {
			mValue -= rhs.mValue;
			return *this;
		}

		friend constexpr inline auto operator-(const Radians& lhs) noexcept -> Radians {
			return Radians(-lhs.mValue);
		}

		friend constexpr inline auto operator*(const Radians& lhs, float rhs) noexcept -> Radians {
			return Radians(lhs.mValue * rhs);
		}

		friend constexpr inline auto operator*(const Radians& lhs, double rhs) noexcept -> Radians {
			return Radians(lhs.mValue * rhs);
		}

		friend constexpr inline auto
		operator*(const Radians& lhs, int64_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue * narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator*(const Radians& lhs, size_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue * narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator*(float lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(rhs.mValue * lhs);
		}

		friend constexpr inline auto operator*(double lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(rhs.mValue * lhs);
		}

		friend constexpr inline auto
		operator*(int64_t lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(rhs.mValue * narrow_cast<double>(lhs));
		}

		friend constexpr inline auto operator*(size_t lhs, const Radians& rhs) noexcept -> Radians {
			return Radians(rhs.mValue * narrow_cast<double>(lhs));
		}

		constexpr inline auto operator*=(float x) noexcept -> Radians& {
			mValue *= x;
			return *this;
		}

		constexpr inline auto operator*=(double x) noexcept -> Radians& {
			mValue *= x;
			return *this;
		}

		constexpr inline auto operator*=(int64_t x) noexcept -> Radians& {
			mValue *= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator*=(size_t x) noexcept -> Radians& {
			mValue *= narrow_cast<double>(x);
			return *this;
		}

		friend constexpr inline auto operator/(const Radians& lhs, float rhs) noexcept -> Radians {
			return Radians(lhs.mValue / rhs);
		}

		friend constexpr inline auto operator/(const Radians& lhs, double rhs) noexcept -> Radians {
			return Radians(lhs.mValue / rhs);
		}

		friend constexpr inline auto
		operator/(const Radians& lhs, int64_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue / narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator/(const Radians& lhs, size_t rhs) noexcept -> Radians {
			return Radians(lhs.mValue / narrow_cast<double>(rhs));
		}

		friend constexpr inline auto operator/(float lhs, const Radians& rhs) noexcept -> float {
			return narrow_cast<float>(lhs / rhs.mValue);
		}

		friend constexpr inline auto operator/(double lhs, const Radians& rhs) noexcept -> double {
			return lhs / rhs.mValue;
		}

		friend constexpr inline auto
		operator/(int64_t lhs, const Radians& rhs) noexcept -> int64_t {
			return static_cast<int64_t>(narrow_cast<double>(lhs) / rhs.mValue);
		}

		friend constexpr inline auto operator/(size_t lhs, const Radians& rhs) noexcept -> size_t {
			return static_cast<size_t>(narrow_cast<double>(lhs) / rhs.mValue);
		}

		constexpr inline auto operator/=(float x) noexcept -> Radians& {
			mValue /= x;
			return *this;
		}

		constexpr inline auto operator/=(double x) noexcept -> Radians& {
			mValue /= x;
			return *this;
		}

		constexpr inline auto operator/=(int64_t x) noexcept -> Radians& {
			mValue /= narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator/=(size_t x) noexcept -> Radians& {
			mValue /= narrow_cast<double>(x);
			return *this;
		}

		friend constexpr inline auto operator<(const Radians& lhs, float rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Radians& lhs, double rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Radians& lhs, int64_t rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(const Radians& lhs, size_t rhs) noexcept -> bool {
			return lhs.mValue < rhs;
		}

		friend constexpr inline auto operator<(float lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(double lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(int64_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto operator<(size_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs.mValue;
		}

		friend constexpr inline auto
		operator<(const Radians& lhs, const Radians& rhs) noexcept -> bool {
			return lhs.mValue < rhs.mValue;
		}

		friend constexpr inline auto operator>(const Radians& lhs, float rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Radians& lhs, double rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Radians& lhs, int64_t rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(const Radians& lhs, size_t rhs) noexcept -> bool {
			return lhs.mValue > rhs;
		}

		friend constexpr inline auto operator>(float lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(double lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(int64_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto operator>(size_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs.mValue;
		}

		friend constexpr inline auto
		operator>(const Radians& lhs, const Radians& rhs) noexcept -> bool {
			return lhs.mValue > rhs.mValue;
		}

		friend constexpr inline auto operator==(const Radians& lhs, float rhs) noexcept -> bool {
			return lhs.mValue == rhs;
		}

		friend constexpr inline auto operator==(const Radians& lhs, double rhs) noexcept -> bool {
			return lhs.mValue == rhs;
		}

		friend constexpr inline auto operator==(const Radians& lhs, int64_t rhs) noexcept -> bool {
			return lhs.mValue == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator==(const Radians& lhs, size_t rhs) noexcept -> bool {
			return lhs.mValue == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator==(float lhs, const Radians& rhs) noexcept -> bool {
			return lhs == rhs.mValue;
		}

		friend constexpr inline auto operator==(double lhs, const Radians& rhs) noexcept -> bool {
			return lhs == rhs.mValue;
		}

		friend constexpr inline auto operator==(int64_t lhs, const Radians& rhs) noexcept -> bool {
			return narrow_cast<double>(lhs) == rhs.mValue;
		}

		friend constexpr inline auto operator==(size_t lhs, const Radians& rhs) noexcept -> bool {
			return narrow_cast<double>(lhs) == rhs.mValue;
		}

		friend constexpr inline auto
		operator==(const Radians& lhs, const Radians& rhs) noexcept -> bool {
			return lhs.mValue == rhs.mValue;
		}

		friend constexpr inline auto operator<=(const Radians& lhs, float rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(const Radians& lhs, double rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(const Radians& lhs, int64_t rhs) noexcept -> bool {
			return lhs < rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator<=(const Radians& lhs, size_t rhs) noexcept -> bool {
			return lhs < rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator<=(float lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(double lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator<=(int64_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto operator<=(size_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto
		operator<=(const Radians& lhs, const Radians& rhs) noexcept -> bool {
			return lhs < rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Radians& lhs, float rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Radians& lhs, double rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(const Radians& lhs, int64_t rhs) noexcept -> bool {
			return lhs > rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator>=(const Radians& lhs, size_t rhs) noexcept -> bool {
			return lhs > rhs || lhs == narrow_cast<double>(rhs);
		}

		friend constexpr inline auto operator>=(float lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(double lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		friend constexpr inline auto operator>=(int64_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto operator>=(size_t lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs || narrow_cast<double>(lhs) == rhs;
		}

		friend constexpr inline auto
		operator>=(const Radians& lhs, const Radians& rhs) noexcept -> bool {
			return lhs > rhs || lhs == rhs;
		}

		constexpr inline auto operator=(const float& x) noexcept -> Radians& {
			mValue = x;
			return *this;
		}

		constexpr inline auto operator=(const double& x) noexcept -> Radians& {
			mValue = x;
			return *this;
		}

		constexpr inline auto operator=(const int64_t& x) noexcept -> Radians& {
			mValue = narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator=(const size_t& x) noexcept -> Radians& {
			mValue = narrow_cast<double>(x);
			return *this;
		}

		constexpr inline auto operator=(const Radians& rhs) noexcept -> Radians& = default;
		constexpr inline auto operator=(Radians&& rhs) noexcept -> Radians& = default;

		explicit constexpr inline operator float() const noexcept {
			return narrow_cast<float>(mValue);
		}

		explicit constexpr inline operator double() const noexcept {
			return mValue;
		}

		explicit constexpr inline operator int() const noexcept {
			return narrow_cast<int>(mValue);
		}

		explicit constexpr inline operator int64_t() const noexcept {
			return narrow_cast<int64_t>(mValue);
		}

		explicit constexpr inline operator size_t() const noexcept {
			return narrow_cast<size_t>(mValue);
		}

		explicit constexpr inline operator Hertz() const noexcept {
			return Hertz(mValue * Constants<double>::twoPi);
		}

	  private:
		double mValue = 0.0;
	};

	inline constexpr auto Hertz::toRadians(Hertz hertz) noexcept -> Radians {
		return Radians::fromHertz(hertz);
	}

	inline constexpr auto Hertz::fromRadians(Radians radians) noexcept -> Hertz {
		return Hertz(static_cast<double>(radians) * Constants<double>::twoPi);
	}

	inline constexpr auto Hertz::getRadians() const noexcept -> Radians {
		return Radians::fromHertz(mValue);
	}

	inline constexpr auto Hertz::setFromRadians(Radians radians) noexcept -> void {
		mValue = static_cast<double>(radians) * Constants<double>::twoPi;
	}

	inline constexpr Hertz::operator Radians() const noexcept {
		return Radians(mValue / Constants<double>::twoPi);
	}

	constexpr inline auto operator"" _Hz(long double x) noexcept -> Hertz {
		return Hertz(narrow_cast<double>(x));
	}

	constexpr inline auto operator""_Hz(unsigned long long x) noexcept -> Hertz {
		return Hertz(narrow_cast<double>(x));
	}

	constexpr inline auto operator""_kHz(long double x) noexcept -> Hertz {
		return Hertz(narrow_cast<double>(x) * 1000.0);
	}

	constexpr inline auto operator""_kHz(unsigned long long x) noexcept -> Hertz {
		return Hertz(narrow_cast<double>(x * 1000));
	}

	constexpr inline auto operator"" _rad(long double x) noexcept -> Radians {
		return Radians(narrow_cast<double>(x));
	}

	constexpr inline auto operator""_rad(unsigned long long x) noexcept -> Radians {
		return Radians(narrow_cast<double>(x));
	}
} // namespace apex::math
