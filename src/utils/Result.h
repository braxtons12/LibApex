#pragma once

#include <functional>
#include <juce_core/juce_core.h>
#include <type_traits>

#include "Error.h"

#ifndef RESULT
	#define RESULT

namespace apex::utils {
	template<typename T, bool copyable>
	class Option;

	/// @brief Represents the result of an operation that can fail.
	/// Every `Result` is either `Ok`, indicating success and containing a value
	/// or `Err`, indicating failure and containing an `Error` type
	///
	/// @tparam T - the type contained in the case of success
	/// @tparam E - the `Error` type contained in the case of failure
	template<typename T,
			 typename E,
			 typename Enable
			 = std::enable_if_t<!std::is_pointer_v<E> && std::is_base_of_v<Error, E>, E>>
	class [[nodiscard]] Result {
	  public:
		constexpr Result() noexcept = delete;
		constexpr Result(Result<T, E, Enable>& result) = delete;
		constexpr Result(const Result<T, E, Enable>& result) = delete;
		constexpr Result(Result<T, E, Enable>&& result) noexcept = default;
		~Result() noexcept;

		/// @brief Constructs a `Result` as the `Ok` variant, containing `ok`
		///
		/// @param ok - the success value
		///
		/// @return `Ok`
		[[nodiscard]] constexpr static inline auto Ok(T ok) noexcept -> Result<T, E> {
			if constexpr(std::is_trivially_copyable_v<T>) {
				return Result<T, E, Enable>(ok);
			}
			else {
				return Result<T, E, Enable>(std::move(ok));
			}
		}

		/// @brief Constructs a `Result` as the `Err` variant, containing `err`
		///
		/// @param err - the failure value
		///
		/// @return `Err`
		[[nodiscard]] constexpr static inline auto Err(E err) noexcept -> Result<T, E> {
			if constexpr(std::is_trivially_copyable_v<E>) {
				return Result<T, E, Enable>(err);
			}
			else {
				return Result<T, E, Enable>(std::move(err));
			}
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
		[[nodiscard]] constexpr auto
		ok() noexcept -> Option<T, std::is_copy_constructible_v<T> && !std::is_pointer_v<T>>;

		/// @brief Converts this `Result<T, E>` to an `Option<E>`,
		/// consuming this `Result` and discarding the success value, if any
		///
		/// @return `Option<E>`
		[[nodiscard]] constexpr auto
		err() noexcept -> Option<E, std::is_copy_constructible_v<E> && !std::is_pointer_v<E>>;

		/// @brief Maps this `Result<T, E>` to a `Result<U, E>`,
		/// returning `Ok(U)` (mapped by `mapFunc`) if this is an `Ok`,
		/// or `Err` if this is `Err`
		///
		/// @tparam U - The type to map to
		/// @param mapFunc - The function to perform the mapping
		///
		/// @return `Ok(U)` if this is `Ok`, or `Err`
		template<typename U>
		[[nodiscard]] auto
		map(std::function<U(T) const> mapFunc) const noexcept -> Result<U, E, Enable>;

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
		template<typename F,
				 typename FEnable
				 = std::enable_if_t<!std::is_pointer_v<F> && std::is_base_of_v<Error, F>>>
		[[nodiscard]] auto
		mapErr(std::function<F(E) const> mapFunc) const noexcept -> Result<T, F, FEnable>;

		auto operator=(Result<T, E, Enable> result) -> Result<T, E, Enable>& = delete;
		auto operator=(const Result<T, E, Enable>& result) -> Result<T, E, Enable>& = delete;
		constexpr auto
		operator=(Result<T, E, Enable>&& result) noexcept -> Result<T, E, Enable>& = default;

	  private:
		constexpr explicit Result(T ok) noexcept;
		constexpr explicit Result(E err) noexcept;

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
} // namespace apex::utils

#endif // RESULT

#ifndef RESULT_IMPL
	#define RESULT_IMPL

	#include "Option.h"

namespace apex::utils {
	template<typename T, typename E, typename Enable>
	Result<T, E, Enable>::~Result() noexcept {
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
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::isOk() const noexcept -> bool {
		mHandled = true;
		return mIsOk;
	}

	/// @brief Returns `true` if this is `Err`, `false` if this is `Ok`
	///
	/// @return Whether this is `Err`
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::isErr() const noexcept -> bool {
		mHandled = true;
		return !mIsOk;
	}

	/// @brief Similar to `unwrap`, except doesn't consume this `Result`.
	/// Returns a pointer to the mutable `T` if this is `Ok`, otherwise calls
	/// `std::terminate`
	///
	/// @return A pointer to the contained `T`
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::getMut() noexcept -> T* {
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
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::getConst() const noexcept -> const T* {
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
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::unwrap() noexcept -> T {
		mHandled = true;
		if(mIsOk) {
			if constexpr(std::is_pointer_v<T>) {
				auto ok = mOk;
				mIsOk = false;
				this->~Result();
				return ok;
			}
			else {
				auto ok = std::move(mOk);
				mIsOk = false;
				this->~Result();
				return std::move(ok);
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
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::unwrapOr(T defaultValue) noexcept -> T {
		mHandled = true;
		if(mIsOk) {
			if constexpr(std::is_pointer_v<T>) {
				auto ok = mOk;
				mIsOk = false;
				this->~Result();
				return ok;
			}
			else {
				auto ok = std::move(mOk);
				mIsOk = false;
				this->~Result();
				return std::move(ok);
			}
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
	template<typename T, typename E, typename Enable>
	inline auto
	Result<T, E, Enable>::unwrapOrElse(std::function<T() const> defaultGenerator) noexcept -> T {
		mHandled = true;
		if(mIsOk) {
			if constexpr(std::is_pointer_v<T>) {
				auto ok = mOk;
				mIsOk = false;
				this->~Result();
				return ok;
			}
			else {
				auto ok = std::move(mOk);
				mIsOk = false;
				this->~Result();
				return std::move(ok);
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
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::ok() noexcept
		-> Option<T, std::is_copy_constructible_v<T> && !std::is_pointer_v<T>> {
		mHandled = true;
		if(mIsOk) {
			if constexpr(std::is_pointer_v<T>) {
				auto ok = mOk;
				mIsOk = false;
				this->~Result();
				return Option < T,
					   std::is_copy_constructible_v<T> && !std::is_pointer_v<T> > ::Some(ok);
			}
			else {
				auto ok = std::move(mOk);
				mIsOk = false;
				this->~Result();
				return Option < T, std::is_copy_constructible_v<
									   T> && !std::is_pointer_v<T> > ::Some(std::move(ok));
			}
		}
		else {
			this->~Result();
			return Option < T, std::is_copy_constructible_v<T> && !std::is_pointer_v<T> > ::None();
		}
	}

	/// @brief Converts this `Result<T, E>` to an `Option<E>`,
	/// consuming this `Result` and discarding the success value, if any
	///
	/// @return `Option<E>`
	template<typename T, typename E, typename Enable>
	constexpr inline auto Result<T, E, Enable>::err() noexcept
		-> Option<E, std::is_copy_constructible_v<E> && !std::is_pointer_v<E>> {
		mHandled = true;
		if(!mIsOk) {
			auto err = mErr;
			this->~Result();
			return Option < E, std::is_copy_constructible_v<
								   E> && !std::is_pointer_v<E> > ::Some(std::move(err));
		}
		else {
			this->~Result();
			return Option < E, std::is_copy_constructible_v<E> && !std::is_pointer_v<E> > ::None();
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
	template<typename T, typename E, typename Enable>
	template<typename U>
	inline auto Result<T, E, Enable>::map(std::function<U(T) const> mapFunc) const noexcept
		-> Result<U, E, Enable> {
		mHandled = true;
		if(mIsOk) {
			return Result<U, E>::Ok(mapFunc(mOk));
		}
		else {
			return Result<U, E>::Err(mErr);
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
	template<typename T, typename E, typename Enable>
	template<typename U>
	inline auto
	Result<T, E, Enable>::mapOr(std::function<U(T) const> mapFunc, U defaultValue) const noexcept
		-> U {
		mHandled = true;
		if(mIsOk) {
			return mapFunc(mOk);
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
	template<typename T, typename E, typename Enable>
	template<typename U>
	inline auto
	Result<T, E, Enable>::mapOrElse(std::function<U(T) const> mapFunc,
									std::function<U() const> defaultGenerator) const noexcept -> U {
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
	template<typename T, typename E, typename Enable>
	template<typename F, typename FEnable>
	inline auto Result<T, E, Enable>::mapErr(std::function<F(E) const> mapFunc) const noexcept
		-> Result<T, F, FEnable> {
		mHandled = true;
		if(!mIsOk) {
			return Result<T, F>::Err(mapFunc(mErr));
		}
		else {
			return Result<T, F>::Ok(mOk);
		}
	}

	template<typename T, typename E, typename Enable>
	constexpr Result<T, E, Enable>::Result(T ok) noexcept : mIsOk(true) {
		if constexpr(std::is_trivially_copyable_v<T>) {
			mOk = ok;
		}
		else {
			mOk = std::move(ok);
		}
	}

	template<typename T, typename E, typename Enable>
	constexpr Result<T, E, Enable>::Result(E err) noexcept {
		if constexpr(std::is_trivially_copyable_v<E>) {
			mErr = err;
		}
		else {
			mErr = std::move(err);
		}
	}
} // namespace apex::utils

#endif // RESULT_IMPL
