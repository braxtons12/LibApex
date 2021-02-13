#pragma once

#include <functional>
#include <juce_core/juce_core.h>
#include <type_traits>
#include <utility>

#include "Details.h"
#include "Error.h"

#ifndef RESULT
	#define RESULT

namespace apex::utils {
	template<typename T>
	requires(utils_details::is_copy_move_or_pointer_v<T>) class Option;

	/// @brief Represents the result of an operation that can fail.
	/// Every `Result` is either `Ok`, indicating success and containing a value
	/// or `Err`, indicating failure and containing an `Error` type
	///
	/// @tparam T - the type contained in the case of success
	/// @tparam E - the `Error` type contained in the case of failure
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 utils_details::is_error_type_v<E>) class [[nodiscard]] Result {
	  public:
		constexpr Result() noexcept = delete;
		constexpr Result(Result& result) = delete;
		constexpr Result(const Result& result) = delete;
		constexpr Result(Result&& result) noexcept = default;
		~Result() noexcept;

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto
		Ok(T ok) noexcept -> Result<T, E> requires(std::is_reference_v<T>) {
			return Result(std::forward<T>(ok));
		}

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T ok) noexcept
			-> Result<T, E> requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
			return Result(std::move(ok));
		}

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T ok) noexcept
			-> Result<T, E> requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
			return Result(ok);
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto
		Err(E err) noexcept -> Result<T, E> requires(std::is_reference_v<E>) {
			return Result(std::forward<E>(err));
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E err) noexcept
			-> Result<T, E> requires(!std::is_reference_v<E> && std::is_move_constructible_v<E>) {
			return Result(std::move(err));
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E err) noexcept
			-> Result<T, E> requires(!std::is_reference_v<E> && !std::is_move_constructible_v<E>) {
			return Result(err);
		}

		/// @brief Returns `true` if this is `Ok`, `false` if this is `Err`
		///
		/// @return Whether this is `Ok`
		[[nodiscard]] constexpr auto isOk() const noexcept -> bool;

		/// @brief Returns `true` if this is `Err`, `false` if this is `Ok`
		///
		/// @return Whether this is `Err`
		[[nodiscard]] constexpr auto isErr() const noexcept -> bool;

		/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
		/// Returns a pointer to the mutable `T` if this is `Ok`, otherwise calls
		/// `std::terminate`
		///
		/// @return A pointer to the contained `T`
		[[nodiscard]] constexpr auto getMut() noexcept -> T*;

		/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
		/// Returns a pointer to the const `T` if this is `Ok`, otherwise calls
		/// `std::terminate`
		///
		/// @return An immutable pointer to the contained `T`
		[[nodiscard]] constexpr auto getConst() const noexcept -> const T*;

		/// @brief Returns the contained `T`, consuming this `Result`.
		/// If this is not `Ok`, then `std::terminate` is called
		///
		/// @return The contained `T`
		[[nodiscard]] constexpr auto unwrap() noexcept -> T;

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result`
		/// If this is not `Ok`, then returns `defaultValue`
		///
		/// @param defaultValue - The value to return if this is `Err`
		///
		/// @return The contained `T` if this is `Ok`, or `defaultValue`
		[[nodiscard]] constexpr auto unwrapOr(T defaultValue) noexcept -> T;

		/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result`.
		/// If this is not `Ok`, then returns the value generated by `defaultGenerator`
		///
		/// @param defaultGenerator - The function to generate the value returned if this is
		/// `Err`
		///
		/// @return  The contained `T` if this is `Ok`, or the value generated by
		/// `defaultGenerator`
		[[nodiscard]] auto unwrapOrElse(std::function<T() const> defaultGenerator) noexcept -> T;

		/// @brief Returns the contained `E` if this is `Err`, consuming this `Result`.
		/// If this is not `Err`, then `std::terminate` is called
		///
		/// @return The contained `E`
		[[nodiscard]] constexpr auto unwrapErr() noexcept -> E;

		/// @brief Converts this `Result<T, E>` to an `Option<T>`,
		/// consuming this `Result` and discarding the error, if any
		///
		/// @return `Option<T>`
		[[nodiscard]] constexpr auto ok() noexcept -> Option<T>;

		/// @brief Converts this `Result<T, E>` to an `Option<E>`,
		/// consuming this `Result` and discarding the success value, if any
		///
		/// @return `Option<E>`
		[[nodiscard]] constexpr auto err() noexcept -> Option<E>;

		/// @brief Maps this `Result<T, E>` to a `Result<U, E>`,
		/// returning `Ok(U)` (mapped by `mapFunc`) if this is an `Ok`,
		/// or `Err` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Ok(U)` if this is `Ok`, or `Err`
		template<typename U>
		[[nodiscard]] auto map(std::function<U(T) const> mapFunc) const noexcept -> Result<U, E>;

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
		[[nodiscard]] auto
		mapOr(std::function<U(T) const> mapFunc, U defaultValue) const noexcept -> U;

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
		[[nodiscard]] auto mapOrElse(std::function<U(T) const> mapFunc,
									 std::function<U() const> defaultGenerator) const noexcept -> U;

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
		[[nodiscard]] auto mapErr(std::function<F(E) const> mapFunc) const noexcept -> Result<T, F>;

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
	inline static constexpr auto
	Ok(T ok) noexcept -> Result<T, E> requires(std::is_reference_v<T>) {
		return Result<T, E>::Ok(std::forward<T>(ok));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result` representing success
	template<typename T, typename E>
	inline static constexpr auto Ok(T ok) noexcept
		-> Result<T, E> requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
		return Result<T, E>::Ok(std::move(ok));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Ok`
	///
	/// @param `ok` - The value to store in the `Result` representing success
	template<typename T, typename E>
	inline static constexpr auto Ok(T ok) noexcept
		-> Result<T, E> requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
		return Result<T, E>::Ok(ok);
	}

	/// @brief Convenience shorthand for `Result<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result` representing failure
	template<typename T, typename E>
	inline static constexpr auto
	Err(E err) noexcept -> Result<T, E> requires(std::is_reference_v<T>) {
		return Result<T, E>::Err(std::forward<E>(err));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result` representing failure
	template<typename T, typename E>
	inline static constexpr auto Err(E err) noexcept
		-> Result<T, E> requires(!std::is_reference_v<T> && std::is_move_constructible_v<T>) {
		return Result<T, E>::Err(std::move(err));
	}

	/// @brief Convenience shorthand for `Result<T, E>::Err`
	///
	/// @param `err` - The value to store in the `Result` representing failure
	template<typename T, typename E>
	inline static constexpr auto Err(E err) noexcept
		-> Result<T, E> requires(!std::is_reference_v<T> && !std::is_move_constructible_v<T>) {
		return Result<T, E>::Err(err);
	}
} // namespace apex::utils

#endif // RESULT

#ifndef RESULT_IMPL
	#define RESULT_IMPL

	#include "Option.h"

namespace apex::utils {
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<E>)
		Result<T, E>::~Result() noexcept {
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

	/// @brief Returns `true` if this is `Ok`, `false` if this is `Err`
	///
	/// @return Whether this is `Ok`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::isOk() const noexcept -> bool {
		mHandled = true;
		return mIsOk;
	}

	/// @brief Returns `true` if this is `Err`, `false` if this is `Ok`
	///
	/// @return Whether this is `Err`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::isErr() const noexcept -> bool {
		mHandled = true;
		return !mIsOk;
	}

	/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
	/// Returns a pointer to the mutable `T` if this is `Ok`, otherwise calls
	/// `std::terminate`
	///
	/// @return A pointer to the contained `T`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::getMut() noexcept -> T* {
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
			std::terminate();
		}
	}

	/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
	/// Returns a pointer to the const `T` if this is `Ok`, otherwise calls
	/// `std::terminate`
	///
	/// @return An immutable pointer to the contained `T`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::getConst() const noexcept -> const T* {
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
			std::terminate();
		}
	}

	/// @brief Returns the contained `T`, consuming this `Result`.
	/// If this is not `Ok`, then `std::terminate` is called
	///
	/// @return The contained `T`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::unwrap() noexcept -> T {
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
			std::terminate();
		}
	}

	/// @brief Returns the contained `T` if this is `Ok`, consuming this `Result`
	/// If this is not `Ok`, then returns `defaultValue`
	///
	/// @param defaultValue - The value to return if this is `Err`
	///
	/// @return The contained `T` if this is `Ok`, or `defaultValue`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::unwrapOr(T defaultValue) noexcept -> T {
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
	/// `Err(E`)
	///
	/// @return  The contained `T` if this is `Ok`, or the value generated by
	/// `defaultGenerator`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) inline auto Result<T, E>::unwrapOrElse(std::function<T() const>
															defaultGenerator) noexcept -> T {
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

	/// @brief Converts this `Result<T, E>` to an `Option<T>`,
	/// consuming this `Result` and discarding the error, if any
	///
	/// @return The contained `E`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::ok() noexcept -> Option<T> {
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
			return None();
		}
	}

	/// @brief Converts this `Result<T, E>` to an `Option<E>`,
	/// consuming this `Result` and discarding the success value, if any
	///
	/// @return `Option<E>`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&& utils_details::is_error_type_v<
			 E>) constexpr inline auto Result<T, E>::err() noexcept -> Option<E> {
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
			return None();
		}
	}

	/// @brief Maps this `Result` to a `Result<U, E>`,
	/// returning `Ok(U)` (mapped by `mapFunc`) if this is an `Ok`,
	/// or `Err` if this is `Err`
	///
	/// @tparam U - The type to map to
	/// @param mapFunc - The function to perform the mapping
	///
	/// @return `Ok(U)` if this is `Ok`, or `Err`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 utils_details::is_error_type_v<E>) template<typename U>
	inline auto Result<T, E>::map(std::function<U(T) const> mapFunc) const noexcept
		-> Result<U, E> {
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
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 utils_details::is_error_type_v<E>) template<typename U>
	inline auto Result<T, E>::mapOr(std::function<U(T) const> mapFunc,
									U defaultValue) const noexcept -> U {
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
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 utils_details::is_error_type_v<E>) template<typename U>
	inline auto Result<T, E>::mapOrElse(std::function<U(T) const> mapFunc,
										std::function<U() const> defaultGenerator) const noexcept
		-> U {
		mHandled = true;
		if(mIsOk) {
			return mapFunc(mOk);
		}
		else {
			return defaultGenerator();
		}
	}

	/// @brief Maps this `Result` to a `Result<T, F>`,
	/// returning `Ok` if this is `Ok`,
	/// or `Err(F)` (mapped by `mapFunc`) if this is `Err`
	///
	/// @tparam F - The type to map to
	///				Must be an `Error` type (`apex::utils::Error`)
	/// @param mapFunc - The function to perform the mapping
	///
	/// @return `Ok` if this is `Ok`, or `Err(F)` if this is `Err`
	template<typename T, typename E>
	requires(utils_details::is_copy_move_or_pointer_v<T>&&
				 utils_details::is_error_type_v<E>) template<typename F>
	inline auto Result<T, E>::mapErr(std::function<F(E) const> mapFunc) const noexcept
		-> Result<T, F> {
		mHandled = true;
		if(!mIsOk) {
			return Err<T, F>(mapFunc(mErr));
		}
		else {
			return Ok<T, F>(mOk);
		}
	}
} // namespace apex::utils

#endif // RESULT_IMPL
