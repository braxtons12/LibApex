#pragma once

#include <cstdlib>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

#include "Concepts.h"
#include "Error.h"
#include "MiscMacros.h"
#include "TypeTraits.h"

namespace apex::utils {

	using concepts::Passable, concepts::ErrorType, concepts::DefaultConstructible,
		concepts::Reference, concepts::NotReference, concepts::Pointer, concepts::NotPointer,
		concepts::Copyable, concepts::Movable, concepts::CopyOrMovable, concepts::NotMovable;

	template<Passable T, ErrorType E = Error>
	requires DefaultConstructible<T> && NotReference<T> && DefaultConstructible<E> && NotReference<
		E>
	class [[nodiscard]] Result;

#ifndef OPTION
	#define OPTION

	struct NoneType {
		explicit constexpr NoneType(int) { // NOLINT
		}
		constexpr NoneType(const NoneType& none) noexcept = default;
		constexpr NoneType(NoneType&& none) noexcept = default;
		constexpr auto operator=(const NoneType& none) noexcept -> NoneType& = default;
		constexpr auto operator=(NoneType&& none) noexcept -> NoneType& = default;
	};

	inline constexpr NoneType none_t{0};

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
	template<Passable T>
	requires NotReference<T>
	class [[nodiscard]] Option {
	  public:
		constexpr Option(NoneType none) noexcept { // NOLINT
			std::ignore = none;
		}
		constexpr Option(const Option& option) noexcept requires Copyable<T>
		= default;
		constexpr Option(Option&& option) noexcept requires Movable<T> {
			this->mIsSome = option.mIsSome;
			if(option.mIsSome) {
				option.mIsSome = false;
				if constexpr(std::is_pointer_v<T>) {
					this->mSome = option.mSome;
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					this->mSome = std::move(option.mSome);
				}
			}
		}

		~Option() noexcept = default;

		/// @brief Constructs an `Option<T>` containing `some`, aka a `Some` variant
		/// containing `some` aka `Some(some)`
		///
		/// @param some - The value to store in this `Option<T>`
		///
		/// @return `Some(some)`
		[[nodiscard]] constexpr static inline auto Some(const T& some) noexcept -> Option {
			return Option(some);
		}

		/// @brief Constructs an `Option<T>` containing `some`, aka a `Some` variant
		/// containing `some` aka `Some(some)`
		///
		/// @param some - The value to store in this `Option<T>`
		///
		/// @return `Some(some)`
		[[nodiscard]] constexpr static inline auto Some(T&& some) noexcept -> Option {
			return Option(std::forward<T>(some));
		}

		/// @brief Constructs an empty `Option<T>`, aka a `None`
		///
		/// @return `None`
		[[nodiscard]] constexpr static inline auto None(NoneType none) noexcept -> Option {
			return Option(none);
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
		template<Passable U>
		[[nodiscard]] inline auto
		map(std::function<U(const T)> mapFunc) const noexcept -> Option<U> {
			if(mIsSome) {
				return Option<U>::Some(mapFunc(mSome));
			}
			else {
				return Option<U>::None(none_t);
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
		template<Passable U>
		[[nodiscard]] inline auto
		mapOr(std::function<U(const T)> mapFunc, const U& defaultValue) const noexcept -> U {
			if(mIsSome) {
				return mapFunc(mSome);
			}
			else {
				return defaultValue;
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
		template<Passable U>
		[[nodiscard]] inline auto
		mapOr(std::function<U(const T)> mapFunc, U&& defaultValue) const noexcept -> U {
			if(mIsSome) {
				return mapFunc(mSome);
			}
			else {
				return std::forward<U>(defaultValue);
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
		template<Passable U>
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

		/// @brief Converts this `Option<T>` to a `Result()`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
		/// `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param error - The `Error` to return if this `Option<T>` is `None`
		///
		/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
		[[nodiscard]] constexpr inline auto
		okOr(const ErrorType auto& error) noexcept -> Result<T, decltype(error)>
		requires NotReference<std::remove_reference_t<decltype(error)>>;

		/// @brief Converts this `Option<T>` to a `Result()`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
		/// `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param error - The `Error` to return if this `Option<T>` is `None`
		///
		/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
		[[nodiscard]] constexpr inline auto okOr(ErrorType auto&& error) noexcept
			-> Result<T, std::remove_reference_t<decltype(error)>>;

		/// @brief Converts this `Option<T>` to a `Result()<T, E>`, consuming this
		/// `Option<T>`. Returns `Ok(T)` if this is `Some`, or `Err(E)` (where E is
		/// generated by `errorGenerator`) if this is `None`
		///
		/// @tparam E - The type to return if this is `None`.
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param errorGenerator - The function to generate the `Error` value
		///
		/// @return `Ok(T)` if this is `Some`, `Err(E)` if this is `None`
		template<ErrorType E = Error>
		requires NotReference<E>
		[[nodiscard]] inline auto
		okOrElse(std::function<E()> errorGenerator) noexcept -> Result<T, E>;

		/// @brief Returns the contained `T`, consuming this `Option`.
		/// If this is not `Some`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto
		unwrap() noexcept -> T requires Movable<T> && NotPointer<T> {
			if(mIsSome) {
				auto some = std::move(mSome);
				mIsSome = false;
				return std::move(some);
			}
			else {
				std::cerr << "unwrap called on a None, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T`, consuming this `Option`.
		/// If this is not `Some`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto unwrap() noexcept -> T requires Pointer<T> {
			if(mIsSome) {
				auto some = mSome;
				mIsSome = false;
				mSome = nullptr;
				return some;
			}
			else {
				std::cerr << "unwrap called on a None, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T`, consuming this `Option`.
		/// If this is not `Some`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto
		unwrap() noexcept -> T requires NotMovable<T> && NotPointer<T> {
			if(mIsSome) {
				auto some = mSome;
				mIsSome = false;
				return some;
			}
			else {
				std::cerr << "unwrap called on a None, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T` if this is `Some`, consuming this
		/// `Option`. If this is not `Some`, then returns `defaultValue`.
		///
		/// @param defaultValue - The value to return if this is `None`
		///
		/// @return The contained `T` if this is `Some`, or `defaultValue`
		[[nodiscard]] constexpr inline auto unwrapOr(const T& defaultValue) noexcept -> T {
			if(mIsSome) {
				return unwrap();
			}
			else {
				return defaultValue;
			}
		}

		/// @brief Returns the contained `T` if this is `Some`, consuming this
		/// `Option`. If this is not `Some`, then returns `defaultValue`.
		///
		/// @param defaultValue - The value to return if this is `None`
		///
		/// @return The contained `T` if this is `Some`, or `defaultValue`
		[[nodiscard]] constexpr inline auto unwrapOr(T&& defaultValue) noexcept -> T {
			if(mIsSome) {
				return unwrap();
			}
			else {
				return std::forward<T>(defaultValue);
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
		[[nodiscard]] inline auto unwrapOrElse(std::function<T()> defaultGenerator) noexcept -> T {
			if(mIsSome) {
				return unwrap();
			}
			else {
				return defaultGenerator();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Option`.
		/// Returns a pointer to the mutable `T` if this is `Some`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer (or reference if `T` is a reference) to `T`
		[[nodiscard]] constexpr inline auto getMut() noexcept {
			if(mIsSome) {
				if constexpr(std::is_pointer_v<T>) {
					return mSome;
				}
				else {
					return &mSome;
				}
			}
			else {
				std::cerr << "getMut called on a None, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Similar to `unwrap`, except doesn't consume this `Option`.
		/// Returns a pointer to the const `T` if this is `Some`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer (or reference if `T` is a reference) to const `T`
		[[nodiscard]] constexpr inline auto getConst() const noexcept {
			if(mIsSome) {
				if constexpr(std::is_pointer_v<T>) {
					return mSome;
				}
				else {
					return &mSome;
				};
			}
			else {
				std::cerr << "getConst called on a None, terminating" << std::endl;
				std::terminate();
			}
		}

		explicit constexpr operator bool() const noexcept {
			return mIsSome;
		}

		constexpr auto operator=(const Option& option) -> Option& requires Copyable<T>
		= default;
		constexpr auto operator=(Option&& option) noexcept -> Option& requires Movable<T> {
			this->mIsSome = option.mIsSome;
			if(option.mIsSome) {
				option.mIsSome = false;
				if constexpr(std::is_pointer_v<T>) {
					this->mSome = option.mSome;
				}
				else if constexpr(std::is_move_constructible_v<T>) {
					this->mSome = std::move(option.mSome);
				}
			}
			return *this;
		}

	  private :
		  /// value type constructor
		  constexpr explicit Option(const T& some) noexcept requires Movable<T> : mSome(some),
																				  mIsSome(true) {
		}

		/// rvalue constructor
		constexpr explicit Option(T&& some) noexcept : mSome(some), mIsSome(true) {
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
	template<Passable T>
	requires DefaultConstructible<T> && NotReference<T>
	inline constexpr auto Some(const T& some) noexcept -> Option<T> {
		return Option<T>::Some(some);
	}

	/// @brief Convenience shorthand for `Option<T>::Some`
	///
	/// @param `some` - The value to store in the `Option`
	template<Passable T>
	requires DefaultConstructible<T> && NotReference<T>
	inline constexpr auto Some(T&& some) noexcept -> Option<T> {
		return Option<T>::Some(std::forward<T>(some));
	}

	/// @brief Convenience shorthand for `Option<T>::None`
	inline constexpr auto None() noexcept -> NoneType {
		return none_t;
	}
#endif // OPTION

#ifndef RESULT
	#define RESULT

	template<ErrorType E = Error>
	requires NotReference<E>
	struct ErrorWrapper {
		explicit ErrorWrapper(const E& error) : mError(error) {
		}
		explicit ErrorWrapper(E&& error) : mError(std::forward<E>(error)) {
		}
		ErrorWrapper(const ErrorWrapper& error) noexcept requires Copyable<E>
		= default;
		ErrorWrapper(ErrorWrapper&& error) noexcept requires Movable<E>
		= default;
		auto operator=(const ErrorWrapper& error) noexcept -> ErrorWrapper& requires Copyable<E>
		= default;
		auto operator=(ErrorWrapper&& error) noexcept -> ErrorWrapper& requires Movable<E>
		= default;

		E mError;
	};

	template<Passable T>
	requires NotReference<T> && DefaultConstructible<T>
	struct OkWrapper {
		explicit constexpr OkWrapper(const T& ok) : mOk(ok) {
		}
		explicit constexpr OkWrapper(T&& ok) : mOk(std::forward<T>(ok)) {
		}
		constexpr OkWrapper(const OkWrapper& ok) noexcept requires Copyable<T>
		= default;
		constexpr OkWrapper(OkWrapper&& ok) noexcept requires Movable<T>
		= default;
		constexpr auto operator=(const OkWrapper& ok) noexcept -> OkWrapper& requires Copyable<T>
		= default;
		constexpr auto operator=(OkWrapper&& ok) noexcept -> OkWrapper& requires Movable<T>
		= default;

		T mOk;
	};

	/// @brief Represents the result of an operation that can fail.
	/// Every `Result()` is either `Ok`, indicating success and containing a value
	/// or `Err`, indicating failure and containing an `Error` type
	///
	/// @tparam T - the type contained in the case of success
	/// @tparam E - the `Error` type contained in the case of failure
	template<Passable T, ErrorType E>
	requires DefaultConstructible<T> && NotReference<T> && DefaultConstructible<E> && NotReference<
		E>
	class [[nodiscard]] Result {
	  public:
		Result(const ErrorWrapper<E>& error) noexcept : mErr(error.mError) { // NOLINT
		}
		Result(ErrorWrapper<E>&& error) noexcept : mErr(error.mError) { // NOLINT
		}
		constexpr Result(const OkWrapper<T>& ok) noexcept : mIsOk(true), mOk(ok.mOk) { // NOLINT
		}
		constexpr Result(OkWrapper<T>&& ok) noexcept : mIsOk(true), mOk(ok.mOk) { // NOLINT
		}
		constexpr Result() noexcept = delete;
		constexpr Result(Result& result) = delete;
		constexpr Result(const Result& result) = delete;
		constexpr Result(Result&& result) noexcept requires Movable<T> && Movable<E> {
			this->mHandled = result.mHandled;
			result.mHandled = true;
			this->mIsOk = result.mIsOk;
			if(result.mIsOk) {
				if constexpr(std::is_move_constructible_v<T>) {
					this->mOk = std::move(result.mOk);
				}
				else {
					this->mOk = result.mOk;
				}
			}
			else {
				if constexpr(std::is_move_constructible_v<E>) {
					this->mErr = std::move(result.mErr);
				}
				else {
					this->mErr = result.mErr;
				}
			}
		}

		~Result() noexcept {
			if constexpr(std::is_pointer_v<T>) {
				if(!mHandled) {
					if(mIsOk) {
						mOk = nullptr;
					}
					std::cerr
						<< "Unhandled Result() that must be handled being destroyed, terminating"
						<< std::endl;
					std::terminate();
				}
				else if(mIsOk && mOk != nullptr) {
					mOk = nullptr;
				}
			}
			else if(!mHandled) {
				std::cerr << "Unhandled Result() that must be handled being destroyed, terminating"
						  << std::endl;
				std::terminate();
			}
		}

		/// @brief Constructs a `Result()` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(const T& ok) noexcept -> Result<T, E> {
			return Result(ok);
		}

		/// @brief Constructs a `Result()` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T&& ok) noexcept -> Result<T, E> {
			return Result(std::forward<T>(ok));
		}

		/// @brief Constructs a `Result()` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(OkWrapper<T>&& ok) noexcept -> Result<T, E> {
			return Result(std::forward<T>(ok));
		}

		/// @brief Constructs a `Result()` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto
		Ok(const OkWrapper<T>& ok) noexcept -> Result<T, E> {
			return Result(ok);
		}

		/// @brief Constructs a `Result()` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(const E& err) noexcept -> Result<T, E> {
			return Result(err);
		}

		/// @brief Constructs a `Result()` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E&& err) noexcept -> Result<T, E> {
			return Result(std::forward<E>(err));
		}

		/// @brief Constructs a `Result()` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto
		Err(const ErrorWrapper<E>& err) noexcept -> Result<T, E> {
			return Result(err);
		}

		/// @brief Constructs a `Result()` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto
		Err(ErrorWrapper<E>&& err) noexcept -> Result<T, E> {
			return Result(std::forward<ErrorWrapper<E>>(err));
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

		/// @brief Similar to `unwrap`, except doesn't consume this `Result()`.
		/// Returns a pointer to the mutable `T` if this is `Ok`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer (or reference if `T` is a reference) to `T`
		[[nodiscard]] constexpr inline auto getMut() noexcept {
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

		/// @brief Similar to `unwrap`, except doesn't consume this `Result()`.
		/// Returns a pointer to the const `T` if this is `Ok`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer (or reference if `T` is a reference) to const `T`
		[[nodiscard]] constexpr inline auto getConst() const noexcept {
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

		/// @brief Returns the contained `T`, consuming this `Result()`.
		/// If this is not `Ok`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto
		unwrap() noexcept -> T requires Movable<T> && NotPointer<T> {
			mHandled = true;
			if(mIsOk) {
				auto _ok = std::move(mOk);
				mIsOk = false;
				return std::move(_ok);
			}
			else {
				std::cerr << "unwrap called on an Error result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T`, consuming this `Result()`.
		/// If this is not `Ok`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto unwrap() noexcept -> T requires Pointer<T> {
			mHandled = true;
			if(mIsOk) {
				auto _ok = mOk;
				mIsOk = false;
				mOk = nullptr;
				return _ok;
			}
			else {
				std::cerr << "unwrap called on an Error result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T`, consuming this `Result()`.
		/// If this is not `Ok`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr inline auto
		unwrap() noexcept -> T requires NotMovable<T> && NotPointer<T> {
			mHandled = true;
			if(mIsOk) {
				auto _ok = mOk;
				mIsOk = false;
				return _ok;
			}
			else {
				std::cerr << "unwrap called on an Error result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result()`
		/// If this is not `Ok`, then returns `defaultValue`
		///
		/// @param defaultValue - The value to return if this is `Err`
		///
		/// @return The contained `T` if this is `Ok`, or `defaultValue`
		[[nodiscard]] constexpr inline auto unwrapOr(const T& defaultValue) noexcept -> T {
			mHandled = true;
			if(mIsOk) {
				return unwrap();
			}
			else {
				return defaultValue;
			}
		}

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result()`
		/// If this is not `Ok`, then returns `defaultValue`
		///
		/// @param defaultValue - The value to return if this is `Err`
		///
		/// @return The contained `T` if this is `Ok`, or `defaultValue`
		[[nodiscard]] constexpr inline auto unwrapOr(T&& defaultValue) noexcept -> T {
			mHandled = true;
			if(mIsOk) {
				return unwrap();
			}
			else {
				return std::forward<T>(defaultValue);
			}
		}

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result()`.
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
				return unwrap();
			}
			else {
				return defaultGenerator();
			}
		}

		/// @brief Returns the contained `E` if this is `Err`, consuming this `Result()`.
		/// If this is not `Err`, then `std::terminate` is called
		///
		/// @return The contained `E`
		[[nodiscard]] constexpr inline auto
		unwrapErr() noexcept -> E requires Movable<E> && NotPointer<E> {
			mHandled = true;
			if(!mIsOk) {
				auto _err = std::move(mErr);
				return std::move(_err);
			}
			else {
				std::cerr << "unwrapErr called on an Ok result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `E` if this is `Err`, consuming this `Result()`.
		/// If this is not `Err`, then `std::terminate` is called
		///
		/// @return The contained `E`
		[[nodiscard]] constexpr inline auto unwrapErr() noexcept -> E requires Pointer<E> {
			mHandled = true;
			if(!mIsOk) {
				auto _err = mErr;
				mErr = nullptr;
				return _err;
			}
			else {
				std::cerr << "unwrapErr called on an Ok result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Returns the contained `E` if this is `Err`, consuming this `Result()`.
		/// If this is not `Err`, then `std::terminate` is called
		///
		/// @return The contained `E`
		[[nodiscard]] constexpr inline auto
		unwrapErr() noexcept -> E requires NotMovable<E> && NotPointer<E> {
			mHandled = true;
			if(!mIsOk) {
				auto _err = mErr;
				return _err;
			}
			else {
				std::cerr << "unwrapErr called on an Ok result, terminating" << std::endl;
				std::terminate();
			}
		}

		/// @brief Converts this `Result()<T, E>` to an `Option<T>`,
		/// consuming this `Result()` and discarding the error, if any
		///
		/// @return `Option<T>`
		[[nodiscard]] constexpr inline auto ok() noexcept -> Option<T>
		requires Movable<T> && NotPointer<T> {
			mHandled = true;
			if(mIsOk) {
				auto _ok = std::move(mOk);
				mIsOk = false;
				return Some(std::move(_ok));
			}
			else {
				return None();
			}
		}

		/// @brief Converts this `Result()<T, E>` to an `Option<T>`,
		/// consuming this `Result()` and discarding the error, if any
		///
		/// @return `Option<T>`
		[[nodiscard]] constexpr inline auto ok() noexcept -> Option<T>
		requires Pointer<T> {
			mHandled = true;
			if(mIsOk) {
				auto _ok = mOk;
				mIsOk = false;
				mOk = nullptr;
				return Some(_ok);
			}
			else {
				return None();
			}
		}

		/// @brief Converts this `Result()<T, E>` to an `Option<T>`,
		/// consuming this `Result()` and discarding the error, if any
		///
		/// @return `Option<T>`
		[[nodiscard]] constexpr inline auto ok() noexcept -> Option<T>
		requires NotMovable<T> && NotPointer<T> {
			mHandled = true;
			if(mIsOk) {
				auto _ok = mOk;
				mIsOk = false;
				return Some(_ok);
			}
			else {
				return None();
			}
		}

		/// @brief Converts this `Result()<T, E>` to an `Option<E>`,
		/// consuming this `Result()` and discarding the success value, if any
		///
		/// @return `Option<E>`
		[[nodiscard]] constexpr inline auto err() noexcept -> Option<E>
		requires Movable<E> && NotPointer<E> {
			mHandled = true;
			if(!mIsOk) {
				auto _err = std::move(mErr);
				return Some(std::move(_err));
			}
			else {
				mIsOk = false;
				return None();
			}
		}

		/// @brief Converts this `Result()<T, E>` to an `Option<E>`,
		/// consuming this `Result()` and discarding the success value, if any
		///
		/// @return `Option<E>`
		[[nodiscard]] constexpr inline auto err() noexcept -> Option<E>
		requires Pointer<E> {
			mHandled = true;
			if(!mIsOk) {
				auto _err = mErr;
				mErr = nullptr;
				return Some(_err);
			}
			else {
				mIsOk = false;
				return None();
			}
		}

		/// @brief Converts this `Result()<T, E>` to an `Option<E>`,
		/// consuming this `Result()` and discarding the success value, if any
		///
		/// @return `Option<E>`
		[[nodiscard]] constexpr inline auto err() noexcept -> Option<E>
		requires NotMovable<E> && NotPointer<E> {
			mHandled = true;
			if(!mIsOk) {
				auto _err = mErr;
				return Some(_err);
			}
			else {
				mIsOk = false;
				return None();
			}
		}

		/// @brief Maps this `Result()<T, E>` to a `Result()<U, E>`,
		/// returning `Ok(U)` (mapped by `mapFunc`) if this is an `Ok`,
		/// or `Err` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Ok(U)` if this is `Ok`, or `Err`
		template<Passable U>
		requires NotReference<U>
		[[nodiscard]] inline auto
		map(std::function<U(const T)> mapFunc) const noexcept -> Result<U, E> {
			mHandled = true;
			if(mIsOk) {
				return Result<U, E>::Ok(mapFunc(mOk));
			}
			else {
				return Result<U, E>::Err(mErr);
			}
		}

		/// @brief Maps this `Result()` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Ok`,
		/// or `defaultValue` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultValue - The default value
		///
		/// @return The result of the mapping if this is `Ok`, or `defaultValue`
		template<Passable U>
		[[nodiscard]] inline auto
		mapOr(std::function<U(const T)> mapFunc, const U& defaultValue) const noexcept -> U {
			mHandled = true;
			if(mIsOk) {
				return mapFunc(mOk);
			}
			else {
				return defaultValue;
			}
		}

		/// @brief Maps this `Result()` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Ok`,
		/// or `defaultValue` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultValue - The default value
		///
		/// @return The result of the mapping if this is `Ok`, or `defaultValue`
		template<Passable U>
		[[nodiscard]] inline auto
		mapOr(std::function<U(const T)> mapFunc, U&& defaultValue) const noexcept -> U {
			mHandled = true;
			if(mIsOk) {
				return mapFunc(mOk);
			}
			else {
				return std::forward<U>(defaultValue);
			}
		}

		/// @brief Maps this `Result()` to a `U`,
		/// returning `U` (mapped by `mapFunc`) if this is `Ok`,
		/// or the value returned by `defaultGenerator` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		/// @param defaultGenerator - The function to generate the default value
		///
		/// @return The result of the mapping if this is `Ok`,
		///			or the value returned by `defaultGenerator`
		template<Passable U>
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

		/// @brief Maps this `Result()<T, E>` to a `Result()<T, F>`,
		/// returning `Ok` if this is `Ok`,
		/// or `Err(F)` (mapped by `mapFunc`) if this is `Err`
		///
		/// @tparam F - The type to map to
		///				Must be an `Error` type (`apex::utils::Error`)
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Ok` if this is `Ok`, or `Err(F)` if this is `Err`
		template<ErrorType F = Error>
		requires NotReference<F>
		[[nodiscard]] inline auto
		mapErr(std::function<F(const E)> mapFunc) const noexcept -> Result<T, F> {
			mHandled = true;
			if(!mIsOk) {
				return Result<T, F>::Err(mapFunc(mErr));
			}
			else {
				return Result<T, F>::Ok(mOk);
			}
		}

		explicit constexpr operator bool() const noexcept {
			return mIsOk;
		}

		auto operator=(const Result& result) -> Result& = delete;
		constexpr auto
		operator=(Result&& result) noexcept -> Result& requires Movable<T> && Movable<E> {
			this->mHandled = result.mHandled;
			result.mHandled = true;
			this->mIsOk = result.mIsOk;
			if(result.mIsOk) {
				result.mIsOk = false;
				if constexpr(std::is_move_constructible_v<T>) {
					this->mOk = std::move(result.mOk);
				}
				else {
					this->mOk = result.mOk;
				}
			}
			else {
				if constexpr(std::is_move_constructible_v<E>) {
					this->mErr = std::move(result.mErr);
				}
				else {
					this->mErr = result.mErr;
				}
			}
			return *this;
		}

	  private :
		  /// value type constructor
		  constexpr explicit Result(const T& ok) noexcept
		  : mIsOk(true), mOk(ok) {
		}

		/// rvalue contructor
		constexpr explicit Result(T&& ok) noexcept : mIsOk(true), mOk(ok) {
		}

		/// value type constructor
		constexpr explicit Result(const E& err) noexcept : mErr(err) {
		}

		/// rvalue contructor
		constexpr explicit Result(E&& err) noexcept : mErr(err) {
		}

		/// whether this is `Ok` (`true`) or `Err` (`false`)
		bool mIsOk = false;
		/// whether this `Result()` has been handled
		mutable bool mHandled = false;
		/// the `Ok` value
		T mOk;
		/// The `Err` value
		E mErr;

		APEX_DECLARE_NON_HEAP_ALLOCATABLE()
	};

	/// @brief Convenience shorthand for `Result()<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result()` representing success
	template<Passable T>
	requires NotReference<T> && DefaultConstructible<T>
	inline static constexpr auto Ok(const T& ok) noexcept -> OkWrapper<T> {
		return OkWrapper<T>(ok);
	}

	/// @brief Convenience shorthand for `Result()<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result()` representing success
	template<Passable T>
	requires NotReference<T> && DefaultConstructible<T>
	inline static constexpr auto Ok(T&& ok) noexcept -> OkWrapper<T> {
		return OkWrapper<T>(std::forward<T>(ok));
	}

	/// @brief Convenience shorthand for `Result()<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result()` representing failure
	template<ErrorType E = Error>
	requires NotReference<E> && DefaultConstructible<E>
	inline static constexpr auto Err(const E& err) noexcept -> ErrorWrapper<E> {
		return ErrorWrapper<E>(err);
	}

	/// @brief Convenience shorthand for `Result()<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result()` representing failure
	template<ErrorType E = Error>
	requires NotReference<E> && DefaultConstructible<E>
	inline static constexpr auto Err(E&& err) noexcept -> ErrorWrapper<E> {
		return ErrorWrapper<E>(std::forward<E>(err));
	}

#endif // RESULT

#ifndef OPTION_IMPL
	#define OPTION_IMPL
	/// @brief Converts this `Option<T>` to a `Result()`, consuming this
	/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
	/// `None`
	///
	/// @tparam E - The type to return if this is `None`.
	///				Must be an `Error` type (`apex::utils::Error`)
	/// @param error - The `Error` to return if this `Option<T>` is `None`
	///
	/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
	template<Passable T>
	requires NotReference<T>
	[[nodiscard]] constexpr inline auto
	Option<T>::okOr(const ErrorType auto& error) noexcept -> Result<T, decltype(error)>
	requires NotReference<std::remove_reference_t<decltype(error)>> {
		if(mIsSome) {
			return Ok(unwrap());
		}
		else {
			return Err(error);
		}
	}

	/// @brief Converts this `Option<T>` to a `Result()`, consuming this
	/// `Option<T>`. Returns `Ok(T)` if this is `Some` or `Err(error)` if this is
	/// `None`
	///
	/// @tparam E - The type to return if this is `None`.
	///				Must be an `Error` type (`apex::utils::Error`)
	/// @param error - The `Error` to return if this `Option<T>` is `None`
	///
	/// @return `Ok(T)` if this is `Some`, `Err(error)` if this is `None`
	template<Passable T>
	requires NotReference<T>
	[[nodiscard]] constexpr inline auto Option<T>::okOr(ErrorType auto&& error) noexcept
		-> Result<T, std::remove_reference_t<decltype(error)>> {
		using E = std::remove_reference_t<decltype(error)>;
		if(mIsSome) {
			return Ok(unwrap());
		}
		else {
			return Err(std::forward<E>(error));
		}
	}

	/// @brief Converts this `Option<T>` to a `Result()<T, E>`, consuming this
	/// `Option<T>`. Returns `Ok(T)` if this is `Some`, or `Err(E)` (where E is
	/// generated by `errorGenerator`) if this is `None`
	///
	/// @tparam E - The type to return if this is `None`.
	///				Must be an `Error` type (`apex::utils::Error`)
	/// @param errorGenerator - The function to generate the `Error` value
	///
	/// @return `Ok(T)` if this is `Some`, `Err(E)` if this is `None`
	template<Passable T>
	requires NotReference<T>
	template<ErrorType E>
	requires NotReference<E>
	[[nodiscard]] inline auto
	Option<T>::okOrElse(std::function<E()> errorGenerator) noexcept -> Result<T, E> {
		if(mIsSome) {
			return Ok(unwrap());
		}
		else {
			return Err(errorGenerator());
		}
	}
#endif // OPTION_IMPL
} // namespace apex::utils
