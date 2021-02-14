#pragma once

#include <functional>
#include <iostream>
#include <juce_core/juce_core.h>
#include <type_traits>
#include <utility>

#include "Details.h"
#include "Error.h"
#include "MiscMacros.h"

namespace apex::utils {

	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& std::is_default_constructible_v<T>&&
				 utils_details::is_error_type_v<E>&&
					 std::is_default_constructible_v<E>) class [[nodiscard]] Result;

#ifndef OPTION
	#define OPTION
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
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 std::is_default_constructible_v<T>) class [[nodiscard]] Option {
	  public:
		constexpr Option(const Option& option) noexcept requires(std::is_copy_constructible_v<T>)
			= default;
		constexpr Option(Option&& option) noexcept requires(std::is_move_constructible_v<T>)
			= default;
		~Option() noexcept {
			if constexpr(std::is_pointer_v<T>) {
				if(mIsSome) {
					delete mSome;
					mSome = gsl::owner<T>(nullptr);
				}
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
		map(std::function<U(const T)> mapFunc) const noexcept -> Option<U> {
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
		mapOr(std::function<U(const T)> mapFunc, U defaultValue) const noexcept -> U {
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
		mapOrElse(std::function<U(const T)> mapFunc,
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
		[[nodiscard]] constexpr inline auto okOr(E error) noexcept -> Result<T, E>
		requires(utils_details::is_error_type_v<E>&&
					 utils_details::is_copy_or_move_v<T> && !std::is_pointer_v<T>);

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
		[[nodiscard]] constexpr inline auto okOr(E error) noexcept -> Result<T, E>
		requires(utils_details::is_error_type_v<E>&& std::is_pointer_v<T>);

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
		okOrElse(std::function<E() const> errorGenerator) noexcept -> Result<T, E>
		requires(utils_details::is_error_type_v<E>&&
					 utils_details::is_copy_or_move_v<T> && !std::is_pointer_v<T>);

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
		okOrElse(std::function<E() const> errorGenerator) noexcept -> Result<T, E>
		requires(utils_details::is_error_type_v<E>&& std::is_pointer_v<T>);

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
	inline constexpr auto Some(T some) noexcept -> Option<T>
	requires(std::is_reference_v<T>) {
		return Option<T>::Some(std::forward<T>(some));
	}

	/// @brief Convenience shorthand for `Option<T>::Some`
	///
	/// @param `some` - The value to store in the `Option`
	template<typename T>
	inline constexpr auto Some(T some) noexcept -> Option<T>
	requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
		return Option<T>::Some(std::move(some));
	}

	/// @brief Convenience shorthand for `Option<T>::Some`
	///
	/// @param `some` - The value to store in the `Option`
	template<typename T>
	inline constexpr auto Some(T some) noexcept -> Option<T>
	requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
		return Option<T>::Some(some);
	}

	/// @brief Convenience shorthand for `Option<T>::None`
	template<typename T>
	inline constexpr auto None() noexcept -> Option<T> {
		return Option<T>::None();
	}
#endif // OPTION

#ifndef RESULT
	#define RESULT

	/// @brief Represents the result of an operation that can fail.
	/// Every `Result` is either `Ok`, indicating success and containing a value
	/// or `Err`, indicating failure and containing an `Error` type
	///
	/// @tparam T - the type contained in the case of success
	/// @tparam E - the `Error` type contained in the case of failure
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& std::is_default_constructible_v<T>&&
				 utils_details::is_error_type_v<E>&&
					 std::is_default_constructible_v<E>) class [[nodiscard]] Result {
	  public:
		constexpr Result() noexcept = delete;
		constexpr Result(Result& result) = delete;
		constexpr Result(const Result& result) = delete;
		constexpr Result(Result&& result) noexcept = default;
		~Result() noexcept {
			if constexpr(std::is_pointer_v<E>) {
				if(!mIsOk) {
					delete mErr;
					mErr = nullptr;
				}
			}

			if constexpr(std::is_pointer_v<T>) {
				if(!mHandled) {
					if(mIsOk) {
						delete mOk;
						mOk = nullptr;
					}
					// std::terminate();
				}
				else if(mIsOk && mOk != nullptr) {
					delete mOk;
					mOk = nullptr;
				}
			}
			else if(!mHandled) {
				// std::terminate();
			}
		}

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T ok) noexcept -> Result<T, E>
		requires(std::is_reference_v<T>) {
			return Result(std::forward<T>(ok));
		}

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T ok) noexcept -> Result<T, E>
		requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
			return Result(std::move(ok));
		}

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T ok) noexcept -> Result<T, E>
		requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
			return Result(ok);
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E err) noexcept -> Result<T, E>
		requires(std::is_reference_v<E>) {
			return Result(std::forward<E>(err));
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E err) noexcept -> Result<T, E>
		requires(!std::is_reference_v<E> && std::is_move_constructible_v<E>) {
			return Result(std::move(err));
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E err) noexcept -> Result<T, E>
		requires(!std::is_reference_v<E> && !std::is_move_constructible_v<E>) {
			return Result(err);
		}

		/// @brief Returns `true` if this is `Ok`, `false` if this is `Err`
		///
		/// @return Whether this is `Ok`
		[[nodiscard]] constexpr auto isOk() const noexcept -> bool {
			mHandled = true;
			return mIsOk;
		}

		/// @brief Returns `true` if this is `Err`, `false` if this is `Ok`
		///
		/// @return Whether this is `Err`
		[[nodiscard]] constexpr inline auto isErr() const noexcept -> bool {
			mHandled = true;
			return !mIsOk;
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
		/// Returns a pointer to the mutable `T` if this is `Ok`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer to the contained `T`
		[[nodiscard]] constexpr inline auto getMut() noexcept -> T* {
			mHandled = true;
			if(mIsOk) {
				if constexpr(std::is_pointer_v<T>) {
					return mOk;
				}
				else {
					return &mOk;
				}
			}
			else {
				std::cerr << "getMut called on an Error result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
		/// Returns a pointer to the const `T` if this is `Ok`, otherwise calls
		/// `std::terminate`
		///
		/// @return An immutable pointer to the contained `T`
		[[nodiscard]] constexpr inline auto getConst() const noexcept -> const T* {
			mHandled = true;
			if(mIsOk) {
				if constexpr(std::is_pointer_v<T>) {
					return mOk;
				}
				else {
					return &mOk;
				}
			}
			else {
				std::cerr << "getConst called on an Error result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T`, consuming this `Result`.
		/// If this is not `Ok`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto unwrap() noexcept -> T {
			mHandled = true;
			if(mIsOk) {
				if constexpr(std::is_pointer_v<T>) {
					auto ok = mOk;
					mIsOk = false;
					mOk = nullptr;
					this->~Result();
					return ok;
				}
				else if constexpr(std::is_reference_v<T>) {
					auto ok = std::forward<T>(mOk);
					mIsOk = false;
					this->~Result();
					return std::forward<T>(ok);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto ok = std::move(mOk);
					mIsOk = false;
					this->~Result();
					return std::move(ok);
				}
				else {
					auto ok = mOk;
					mIsOk = false;
					this->~Result();
					return ok;
				}
			}
			else {
				std::cerr << "unwrap called on an Error result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result`
		/// If this is not `Ok`, then returns `defaultValue`
		///
		/// @param defaultValue - The value to return if this is `Err`
		///
		/// @return The contained `T` if this is `Ok`, or `defaultValue`
		[[nodiscard]] constexpr inline auto unwrapOr(T defaultValue) noexcept -> T {
			mHandled = true;
			if(mIsOk) {
				if constexpr(std::is_pointer_v<T>) {
					auto ok = mOk;
					mIsOk = false;
					mOk = nullptr;
					this->~Result();
					return ok;
				}
				else if constexpr(std::is_reference_v<T>) {
					auto ok = std::forward<T>(mOk);
					mIsOk = false;
					this->~Result();
					return std::forward<T>(ok);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto ok = std::move(mOk);
					mIsOk = false;
					this->~Result();
					return std::move(ok);
				}
				else {
					auto ok = mOk;
					mIsOk = false;
					this->~Result();
					return ok;
				}
			}
			else if constexpr(std::is_reference_v<T>) {
				return std::forward<T>(defaultValue);
			}
			else if constexpr(std::is_move_constructible_v<T>) {
				return std::move(defaultValue);
			}
			else {
				return defaultValue;
			}
		}

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result`.
		/// If this is not `Ok`, then returns the value generated by `defaultGenerator`
		///
		/// @param defaultGenerator - The function to generate the value returned if this is
		/// `Err`
		///
		/// @return  The contained `T` if this is `Ok`, or the value generated by
		/// `defaultGenerator`
		[[nodiscard]] inline auto unwrapOrElse(std::function<T()> defaultGenerator) noexcept -> T {
			mHandled = true;
			if(mIsOk) {
				if constexpr(std::is_pointer_v<T>) {
					auto ok = mOk;
					mIsOk = false;
					mOk = nullptr;
					this->~Result();
					return ok;
				}
				else if constexpr(std::is_reference_v<T>) {
					auto ok = std::forward<T>(mOk);
					mIsOk = false;
					this->~Result();
					return std::forward<T>(ok);
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto ok = std::move(mOk);
					mIsOk = false;
					this->~Result();
					return std::move(ok);
				}
				else {
					auto ok = mOk;
					mIsOk = false;
					this->~Result();
					return ok;
				}
			}
			else {
				return defaultGenerator();
			}
		}

		/// @brief Returns the contained `E` if this is `Err`, consuming this `Result`.
		/// If this is not `Err`, then `std::terminate` is called
		///
		/// @return The contained `E`
		[[nodiscard]] constexpr inline auto unwrapErr() noexcept -> E {
			mHandled = true;
			if(!mIsOk) {
				if constexpr(std::is_reference_v<E>) {
					auto err = std::forward<E>(mErr);
					this->~Result();
					return std::forward<E>(err);
				}
				else if constexpr(std::is_move_constructible_v<E>) {
					auto err = std::move(mErr);
					this->~Result();
					return std::move(err);
				}
				else {
					auto err = mErr;
					this->~Result();
					return err;
				}
			}
			else {
				std::cerr << "unwrapErr called on an Ok result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Converts this `Result<T, E>` to an `Option<T>`,
		/// consuming this `Result` and discarding the error, if any
		///
		/// @return `Option<T>`
		[[nodiscard]] constexpr inline auto ok() noexcept -> Option<T> {
			mHandled = true;
			if(mIsOk) {
				if constexpr(std::is_pointer_v<T>) {
					auto ok = mOk;
					mIsOk = false;
					mOk = nullptr;
					this->~Result();
					return Some(ok);
				}
				else if constexpr(std::is_reference_v<T>) {
					auto ok = std::forward<T>(mOk);
					mIsOk = false;
					this->~Result();
					return Some(std::forward<T>(mOk));
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					auto ok = std::move(mOk);
					mIsOk = false;
					this->~Result();
					return Some(std::move(ok));
				}
				else {
					auto ok = mOk;
					mIsOk = false;
					this->~Result();
					return Some(ok);
				}
			}
			else {
				this->~Result();
				return None<T>();
			}
		}

		/// @brief Converts this `Result<T, E>` to an `Option<E>`,
		/// consuming this `Result` and discarding the success value, if any
		///
		/// @return `Option<E>`
		[[nodiscard]] constexpr inline auto err() noexcept -> Option<E> {
			mHandled = true;
			if(!mIsOk) {
				if constexpr(std::is_reference_v<T>) {
					auto err = std::forward<E>(mErr);
					this->~Result();
					return Some(std::forward<E>(err));
				}
				else if constexpr(std::is_move_constructible_v<E>) {
					auto err = std::move(mErr);
					this->~Result();
					return Some(std::move(err));
				}
				else {
					auto err = mErr;
					this->~Result();
					return Some(err);
				}
			}
			else {
				this->~Result();
				return None<E>();
			}
		}

		/// @brief Maps this `Result<T, E>` to a `Result<U, E>`,
		/// returning `Ok(U)` (mapped by `mapFunc`) if this is an `Ok`,
		/// or `Err` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Ok(U)` if this is `Ok`, or `Err`
		template<typename U>
		[[nodiscard]] inline auto
		map(std::function<U(const T)> mapFunc) const noexcept -> Result<U, E> {
			mHandled = true;
			if(mIsOk) {
				return Ok<U, E>(mapFunc(mOk));
			}
			else {
				return Err<U, E>(mErr);
			}
		}

		/// @brief Maps this `Result` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Ok`,
		/// or `defaultValue` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultValue - The default value
		///
		/// @return The result of the mapping if this is `Ok`, or `defaultValue`
		template<typename U>
		[[nodiscard]] inline auto
		mapOr(std::function<U(const T)> mapFunc, U defaultValue) const noexcept -> U {
			mHandled = true;
			if(mIsOk) {
				return mapFunc(mOk);
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

		/// @brief Maps this `Result` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Ok`,
		/// or the value returned by `defaultGenerator` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultGenerator - The function to generate the default value
		///
		/// @return The result of the mapping if this is `Ok`,
		///			or the value returned by `defaultGenerator`
		template<typename U>
		[[nodiscard]] inline auto
		mapOrElse(std::function<U(const T)> mapFunc,
				  std::function<U()> defaultGenerator) const noexcept -> U {
			mHandled = true;
			if(mIsOk) {
				return mapFunc(mOk);
			}
			else {
				return defaultGenerator();
			}
		}

		/// @brief Maps this `Result<T, E>` to a `Result<T, F>`,
		/// returning `Ok` if this is `Ok`,
		/// or `Err(F)` (mapped by `mapFunc`) if this is `Err`
		///
		/// @tparam F - The type to map to
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Ok` if this is `Ok`, or `Err(F)` if this is `Err`
		template<typename F>
		[[nodiscard]] inline auto
		mapErr(std::function<F(const E)> mapFunc) const noexcept -> Result<T, F> {
			mHandled = true;
			if(!mIsOk) {
				return Err<T, F>(mapFunc(mErr));
			}
			else {
				return Ok<T, F>(mOk);
			}
		}

		auto operator=(Result result) -> Result& = delete;
		auto operator=(const Result& result) -> Result& = delete;
		constexpr auto operator=(Result&& result) noexcept -> Result& = default;

	  private:
		constexpr explicit Result(T ok) noexcept requires(std::is_reference_v<T>)
			: mOk(std::forward<T>(ok)), mIsOk(true) {
		}

		constexpr explicit Result(T ok) noexcept
			requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>)
			: mOk(std::move(ok)), mIsOk(true) {
		}

		constexpr explicit Result(T ok) noexcept
			requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>)
			: mOk(ok), mIsOk(true) {
		}

		constexpr explicit Result(E err) noexcept requires(std::is_reference_v<E>)
			: mErr(std::forward<E>(err)) {
		}

		constexpr explicit Result(E err) noexcept
			requires(!std::is_reference_v<E> && std::is_move_constructible_v<E>)
			: mErr(std::move(err)) {
		}

		constexpr explicit Result(E err) noexcept
			requires(!std::is_reference_v<E> && !std::is_move_constructible_v<E>)
			: mErr(err) {
		}

		/// the value representing success
		T mOk;
		/// the value representing failure
		E mErr;
		/// whether this is `Ok` (`true`) or `Err` (`false`)
		bool mIsOk = false;
		/// whether this `Result` has been handled
		mutable bool mHandled = false;

		APEX_DECLARE_NON_HEAP_ALLOCATABLE()
	};

	/// @brief Convenience shorthand for `Result<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result` representing success
	template<typename T, typename E>
	inline static constexpr auto Ok(T ok) noexcept -> Result<T, E>
	requires(std::is_reference_v<T>) {
		return Result<T, E>::Ok(std::forward<T>(ok));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result` representing success
	template<typename T, typename E>
	inline static constexpr auto Ok(T ok) noexcept -> Result<T, E>
	requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
		return Result<T, E>::Ok(std::move(ok));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result` representing success
	template<typename T, typename E>
	inline static constexpr auto Ok(T ok) noexcept -> Result<T, E>
	requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
		return Result<T, E>::Ok(ok);
	}

	/// @brief Convenience shorthand for `Result<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result` representing failure
	template<typename T, typename E>
	inline static constexpr auto Err(E err) noexcept -> Result<T, E>
	requires(std::is_reference_v<T>) {
		return Result<T, E>::Err(std::forward<E>(err));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result` representing failure
	template<typename T, typename E>
	inline static constexpr auto Err(E err) noexcept -> Result<T, E>
	requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
		return Result<T, E>::Err(std::move(err));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result` representing failure
	template<typename T, typename E>
	inline static constexpr auto Err(E err) noexcept -> Result<T, E>
	requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
		return Result<T, E>::Err(err);
	}

#endif // RESULT

#ifndef OPTION_IMPL
	#define OPTION_IMPL
	/// @brief Converts this `Option<T>` to a `Result<T, E>`, consuming this
	/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
	/// `None`
	///
	/// @tparam E - The type to return if this is `None`.
	///				Must be an `Error` type (`apex::utils::Error`)
	/// @param error - The `Error` to return if this `Option<T>` is `None`
	///
	/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
	template<typename T>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 std::is_default_constructible_v<T>) template<typename E>
	[[nodiscard]] constexpr inline auto Option<T>::okOr(E error) noexcept -> Result<T, E>
	requires(utils_details::is_error_type_v<E>&&
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
				return Err<T, E>(std::move(error));
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
	template<typename T>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 std::is_default_constructible_v<T>) template<typename E>
	[[nodiscard]] constexpr inline auto Option<T>::okOr(E error) noexcept -> Result<T, E>
	requires(utils_details::is_error_type_v<E>&& std::is_pointer_v<T>) {
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
				return Err<T, E>(std::move(error));
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
	template<typename T>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 std::is_default_constructible_v<T>) template<typename E>
	[[nodiscard]] inline auto Option<T>::okOrElse(std::function<E() const> errorGenerator) noexcept
		-> Result<T, E>
	requires(utils_details::is_error_type_v<E>&&
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
	template<typename T>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 std::is_default_constructible_v<T>) template<typename E>
	[[nodiscard]] inline auto Option<T>::okOrElse(std::function<E() const> errorGenerator) noexcept
		-> Result<T, E>
	requires(utils_details::is_error_type_v<E>&& std::is_pointer_v<T>) {
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
#endif // OPTION_IMPL
} // namespace apex::utils
