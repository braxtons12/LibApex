#pragma once

#include <functional>
#include <juce_core/juce_core.h>
#include <type_traits>
#include <utility>

#include "Details.h"
#include "Error.h"
#include "MiscMacros.h"
#include "Result.h"

#ifndef OPTION
	#define OPTION

namespace apex::utils {

	/// @brief Represents an optional value.
	/// Every `Option` is either `Some` and contains a value, or `None`, and does
	/// not. Useful for things such as:
	/// * * Optional members/fields
	/// * * Members/fields that can be loaned or "taken"
	/// * * Optional function arguments
	/// * * Nullable types/pointers
	/// * * Return values for functions not defined over their entire input range
	///
	/// @tparam T - The type of the potentially-contained value
	///
	/// @tags {`utils`}
	template<typename T>
	requires(utils_details::is_copy_move_or_pointer_v<T>) class [[nodiscard]] Option {
	  public:
		constexpr Option(const Option& option) noexcept requires(std::is_copy_constructible_v<T>)
			= default;
		constexpr Option(Option&& option) noexcept requires(std::is_move_constructible_v<T>)
			= default;
		~Option() noexcept requires(!std::is_pointer_v<T>) = default;

		~Option() noexcept requires(std::is_pointer_v<T>) {
			if(mIsSome) {
				delete mSome;
				mSome = gsl::owner<T*>(nullptr);
			}
		}

		/// @brief Constructs an `Option<T>` containing `some`, aka a `Some` variant
		/// containing `some` aka `Some(some)`
		///
		/// @param some - The value to store in this `Option<T>`
		///
		/// @return `Some(some)`
		[[nodiscard]] constexpr static inline auto
		Some(T some) noexcept -> Option requires(std::is_reference_v<T>) {
			return Option(std::forward<T>(some));
		}

		/// @brief Constructs an `Option<T>` containing `some`, aka a `Some` variant
		/// containing `some` aka `Some(some)`
		///
		/// @param some - The value to store in this `Option<T>`
		///
		/// @return `Some(some)`
		[[nodiscard]] constexpr static inline auto Some(T some) noexcept -> Option
			requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
			return Option(std::move(some));
		}

		/// @brief Constructs an `Option<T>` containing `some`, aka a `Some` variant
		/// containing `some` aka `Some(some)`
		///
		/// @param some - The value to store in this `Option<T>`
		///
		/// @return `Some(some)`
		[[nodiscard]] constexpr static inline auto Some(T some) noexcept -> Option
			requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
			return Option(some);
		}

		/// @brief Constructs an empty `Option<T>`, aka a `None`
		///
		/// @return `None`
		[[nodiscard]] constexpr static inline auto None() noexcept -> Option {
			return Option();
		}

		/// @brief Returns `true` if this is `Some`, `false` if this is `None`
		///
		/// @return Whether this is `Some`
		[[nodiscard]] constexpr inline auto isSome() const noexcept -> bool {
			return mIsSome;
		}

		/// @brief Returns `true` if this is `None`, `false` if this is `Some`
		///
		/// @return Whether this is `None`
		[[nodiscard]] constexpr inline auto isNone() const noexcept -> bool {
			return !mIsSome;
		}

		/// @brief Maps this `Option<T>` to an `Option<U>`,
		/// returning `Some(U)` if this is Some, or `None` if this is `None`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Some(U)` if this is Some, or `None` if this is `None`
		template<typename U>
		[[nodiscard]] inline auto
		map(std::function<U(T) const> mapFunc) const noexcept -> Option<U> {
			if(mIsSome) {
				return Option<U>::Some(mapFunc(mSome));
			}
			else {
				return Option<U>::None();
			}
		}

		/// @brief Maps this `Option<T>` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Some`,
		/// or `defaultValue` if this is `None`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultValue - The default value
		///
		/// @return The result of the mapping if this is `Some`, or `defaultValue`
		template<typename U>
		[[nodiscard]] inline auto
		mapOr(std::function<U(T) const> mapFunc, U defaultValue) const noexcept -> U {
			if(mIsSome) {
				return mapFunc(mSome);
			}
			else if constexpr(std::is_reference_v<U>) {
				return std::forward<U>(defaultValue);
			}
			else if constexpr(std::is_move_constructible_v<U>) {
				return std::move(defaultValue);
			}
			else {
				return defaultValue;
			}
		}

		/// @brief Maps this `Option<T>` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Some`,
		/// or `U` (the default value returned by `defaultGenerator`)
		/// if this is `None`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultGenerator - The function to generate the default value
		///
		/// @return The result of the mapping if this is `Some`,
		///			or the value returned by `defaultGenerator` if this is
		///`None`
		template<typename U>
		[[nodiscard]] inline auto
		mapOrElse(std::function<U(T) const> mapFunc,
				  std::function<U()> defaultGenerator) const noexcept -> U {
			if(mIsSome) {
				return mapFunc(mSome);
			}
			else {
				return defaultGenerator();
			}
		}

		/// @brief Converts this `Option<T>` to a `Result<T, E>`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
		/// `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param error - The `Error` to return if this `Option<T>` is `None`
		///
		/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
		template<typename E>
		[[nodiscard]] constexpr inline auto okOr(E error) noexcept -> Result<T, E> requires(
			utils_details::is_error_type_v<E>&&
				utils_details::is_copy_or_move_v<T> && !std::is_pointer_v<T>) {
			if(mIsSome) {
				if constexpr(std::is_reference_v<T>) {
					auto res = Ok<T, E>(std::forward<T>(mSome));
					mIsSome = false;
					this->~Option();
					return std::move(res);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto res = Ok<T, E>(std::move(mSome));
					mIsSome = false;
					this->~Option();
					return std::move(res);
				}
				else {
					auto res = Ok<T, E>(mSome);
					mIsSome = false;
					this->~Option();
					return res;
				}
			}
			else {
				this->~Option();
				if constexpr(std::is_reference_v<E>) {
					return Err<T, E>(std::forward<E>(error));
				}
				else if constexpr(std::is_move_constructible_v<E>) {
					return Err<T, E>(std::move<E>(error));
				}
				else {
					return Err<T, E>(error);
				}
			}
		}

		/// @brief Converts this `Option<T>` to a `Result<T, E>`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
		/// `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param error - The `Error` to return if this `Option<T>` is `None`
		///
		/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
		template<typename E>
		[[nodiscard]] constexpr inline auto okOr(E error) noexcept
			-> Result<T, E> requires(utils_details::is_error_type_v<E>&& std::is_pointer_v<T>) {
			if(mIsSome) {
				auto res = Ok<T, E>(mSome);
				mIsSome = false;
				mSome = nullptr;
				this->~Option();
				return std::move(res);
			}
			else {
				this->~Option();
				if constexpr(std::is_reference_v<E>) {
					return Err<T, E>(std::forward<E>(error));
				}
				else if constexpr(std::is_move_constructible_v<E>) {
					return Err<T, E>(std::move<E>(error));
				}
				else {
					return Err<T, E>(error);
				}
			}
		}

		/// @brief Converts this `Option<T>` to a `Result<T, E>`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some`, or `Err(E)` (where E is
		/// generated by `errorGenerator`) if this is `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param errorGenerator - The function to generate the `Error` value
		///
		/// @return `Ok(T)` if this is `Some`, `Err(E)` if this is `None`
		template<typename E>
		[[nodiscard]] inline auto
		okOrElse(std::function<E() const> errorGenerator) noexcept -> Result<T, E> requires(
			utils_details::is_error_type_v<E>&&
				utils_details::is_copy_or_move_v<T> && !std::is_pointer_v<T>) {
			if(mIsSome) {
				if constexpr(std::is_reference_v<T>) {
					auto res = Ok<T, E>(std::forward<T>(mSome));
					mIsSome = false;
					this->~Option();
					return std::move(res);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto res = Ok<T, E>(std::move(mSome));
					mIsSome = false;
					this->~Option();
					return std::move(res);
				}
				else {
					auto res = Ok<T, E>(mSome);
					mIsSome = false;
					this->~Option();
					return res;
				}
			}
			else {
				this->~Option();
				return Err<T, E>(errorGenerator());
			}
		}

		/// @brief Converts this `Option<T>` to a `Result<T, E>`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some`, or `Err(E)` (where E is
		/// generated by `errorGenerator`) if this is `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param errorGenerator - The function to generate the `Error` value
		///
		/// @return `Ok(T)` if this is `Some`, `Err(E)` if this is `None`
		template<typename E>
		[[nodiscard]] inline auto okOrElse(std::function<E() const> errorGenerator) noexcept
			-> Result<T, E> requires(utils_details::is_error_type_v<E>&& std::is_pointer_v<T>) {
			if(mIsSome) {
				auto res = Ok<T, E>(mSome);
				mIsSome = false;
				mSome = nullptr;
				this->~Option();
				return std::move(res);
			}
			else {
				this->~Option();
				return Err<T, E>(errorGenerator());
			}
		}

		/// @brief Returns the contained `T`, consuming this `Option`.
		/// If this is not `Some`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto unwrap() noexcept -> T requires(!std::is_pointer_v<T>) {
			if(mIsSome) {
				if constexpr(std::is_reference_v<T>) {
					auto some = std::forward<T>(mSome);
					mIsSome = false;
					this->~Option();
					return std::forward<T>(some);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto some = std::move(mSome);
					mIsSome = false;
					this->~Option();
					return std::move(some);
				}
				else {
					auto some = mSome;
					mIsSome = false;
					this->~Option();
					return some;
				}
			}
			else {
				std::terminate();
			}
		}

		/// @brief Returns the contained `T`, consuming this `Option`.
		/// If this is not `Some`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto unwrap() noexcept -> T requires(std::is_pointer_v<T>) {
			if(mIsSome) {
				auto some = mSome;
				mIsSome = false;
				mSome = nullptr;
				this->~Option();
				return some;
			}
			else {
				std::terminate();
			}
		}

		/// @brief Returns the contained `T` if this is `Some`, consuming this
		/// `Option`. If this is not `Some`, then returns `defaultValue`.
		///
		/// @param defaultValue - The value to return if this is `None`
		///
		/// @return The contained `T` if this is `Some`, or `defaultValue`
		[[nodiscard]] constexpr inline auto
		unwrapOr(T defaultValue) noexcept -> T requires(!std::is_pointer_v<T>) {
			if(mIsSome) {
				if constexpr(std::is_reference_v<T>) {
					auto some = std::forward<T>(mSome);
					mIsSome = false;
					this->~Option();
					return std::forward<T>(some);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto some = std::move(mSome);
					mIsSome = false;
					this->~Option();
					return std::move(some);
				}
				else {
					auto some = mSome;
					mIsSome = false;
					this->~Option();
					return some;
				}
			}
			else {
				this->~Option();
				if constexpr(std::is_reference_v<T>) {
					return std::forward<T>(defaultValue);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					return std::move(defaultValue);
				}
				else {
					return defaultValue;
				}
			}
		}

		/// @brief Returns the contained `T` if this is `Some`, consuming this
		/// `Option`. If this is not `Some`, then returns `defaultValue`.
		///
		/// @param defaultValue - The value to return if this is `None`
		///
		/// @return The contained `T` if this is `Some`, or `defaultValue`
		[[nodiscard]] constexpr inline auto
		unwrapOr(T defaultValue) noexcept -> T requires(std::is_pointer_v<T>) {
			if(mIsSome) {
				auto some = mSome;
				mIsSome = false;
				mSome = nullptr;
				return some;
			}
			else {
				this->~Option();
				return defaultValue;
			}
		}

		/// @brief Returns the contained `T` if this is `Some`, consuming this
		/// `Option<T>`. If this is not `Some`, then returns the value generated by
		/// `defaultGenerator`.
		///
		/// @param defaultGenerator - The function to generate the value returned if
		/// this is `None`
		///
		/// @return The contained `T` if this is `Some`, or the value generated by
		/// `defaultGenerator`
		[[nodiscard]] inline auto unwrapOrElse(std::function<T() const> defaultGenerator) noexcept
			-> T requires(!std::is_pointer_v<T>) {
			if(mIsSome) {
				if constexpr(std::is_reference_v<T>) {
					auto some = std::forward<T>(mSome);
					mIsSome = false;
					this->~Option();
					return std::forward<T>(some);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto some = std::move(mSome);
					mIsSome = false;
					this->~Option();
					return std::move(some);
				}
				else {
					auto some = mSome;
					mIsSome = false;
					this->~Option();
					return some;
				}
			}
			else {
				this->~Option();
				return defaultGenerator();
			}
		}

		/// @brief Returns the contained `T` if this is `Some`, consuming this
		/// `Option<T>`. If this is not `Some`, then returns the value generated by
		/// `defaultGenerator`.
		///
		/// @param defaultGenerator - The function to generate the value returned if
		/// this is `None`
		///
		/// @return The contained `T` if this is `Some`, or the value generated by
		/// `defaultGenerator`
		[[nodiscard]] inline auto unwrapOrElse(std::function<T() const> defaultGenerator) noexcept
			-> T requires(std::is_pointer_v<T>) {
			if(mIsSome) {
				auto some = mSome;
				mIsSome = false;
				mSome = nullptr;
				return some;
			}
			else {
				this->~Option();
				return defaultGenerator();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Option`.
		/// Returns a pointer to the mutable `T` if this is `Some`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer to the contained `T`
		[[nodiscard]] constexpr inline auto
		getMut() noexcept -> T* requires(!std::is_pointer_v<T>) {
			if(mIsSome) {
				return &mSome;
			}
			else {
				std::terminate();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Option`.
		/// Returns a pointer to the mutable `T` if this is `Some`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer to the contained `T`
		[[nodiscard]] constexpr inline auto getMut() noexcept -> T requires(std::is_pointer_v<T>) {
			if(mIsSome) {
				return mSome;
			}
			else {
				std::terminate();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Option`.
		/// Returns a pointer to the const `T` if this is `Some`, otherwise calls
		/// `std::terminate`
		///
		/// @return An immutable pointer to the contained `T`
		[[nodiscard]] constexpr inline auto
		getConst() const noexcept -> const T* requires(!std::is_pointer_v<T>) {
			if(mIsSome) {
				return &mSome;
			}
			else {
				std::terminate();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Option`.
		/// Returns a pointer to the const `T` if this is `Some`, otherwise calls
		/// `std::terminate`
		///
		/// @return An immutable pointer to the contained `T`
		[[nodiscard]] constexpr inline auto
		getConst() const noexcept -> const T requires(std::is_pointer_v<T>) {
			if(mIsSome) {
				return mSome;
			}
			else {
				std::terminate();
			}
		}

		constexpr auto
		operator=(const Option& option) -> Option& requires(std::is_copy_constructible_v<T>)
			= default;
		constexpr auto
		operator=(Option&& option) noexcept -> Option& requires(std::is_move_constructible_v<T>)
			= default;

	  private:
		constexpr explicit Option(T some) noexcept requires(std::is_reference_v<T>)
			: mSome(std::forward<T>(some)), mIsSome(true) {
		}

		constexpr explicit Option(T some) noexcept
			requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>)
			: mSome(std::move(some)), mIsSome(true) {
		}

		constexpr explicit Option(T some) noexcept
			requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>)
			: mSome(some), mIsSome(true) {
		}

		constexpr Option() = default;

		/// The contained value
		T mSome;
		/// Whether this is `Some`
		bool mIsSome = false;

		APEX_DECLARE_NON_HEAP_ALLOCATABLE()
	};

	/// @brief Convenience shorthand for `Option<T>::Some`
	///
	/// @param `some` - The value to store in the `Option`
	template<typename T>
	inline static constexpr auto
	Some(T some) noexcept -> Option<T> requires(std::is_reference_v<T>) {
		return Option<T>::Some(std::forward<T>(some));
	}

	/// @brief Convenience shorthand for `Option<T>::Some`
	///
	/// @param `some` - The value to store in the `Option`
	template<typename T>
	inline static constexpr auto Some(T some) noexcept
		-> Option<T> requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
		return Option<T>::Some(std::move(some));
	}

	/// @brief Convenience shorthand for `Option<T>::Some`
	///
	/// @param `some` - The value to store in the `Option`
	template<typename T>
	inline static constexpr auto Some(T some) noexcept
		-> Option<T> requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
		return Option<T>::Some(some);
	}

	/// @brief Convenience shorthand for `Option<T>::None`
	template<typename T>
	inline static constexpr auto None() noexcept -> Option<T> {
		return Option<T>::None();
	}
} // namespace apex::utils

#endif // OPTION
