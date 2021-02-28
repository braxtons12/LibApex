#pragma once

#include <gsl/gsl>
#include <memory>
#include <string>
#include <system_error>
#include <type_traits>

#include "MiscMacros.h"

namespace apex::utils {
	/// @brief Base error interface.
	/// Used to implement custom error types used as the `E` in `Result<T, E>`
	/// to represent and communicate failure of a function
	///
	/// @see `Result<T, E>`
	/// @tags {`utils`, "Error handling"}
	class [[nodiscard]] Error {
	  public:
		/// Constructs a default `Error` with no message
		Error() noexcept = default;

		/// @brief Constructs an `Error` from the given `std::error_code`
		///
		/// @param code
		Error(const std::error_code& code) noexcept // NOLINT
			: mHasErrorCode(true), mErrorCode(code) {
		}

		/// @brief Constructs an `Error` from the given `std::error_code`
		///
		/// @param code
		Error(std::error_code&& code) noexcept // NOLINT
			: mHasErrorCode(true), mErrorCode(code) {
		}

		/// @brief Constructs an `Error` with the given message
		///
		/// @param message - The error message
		explicit Error(const char* message) noexcept : mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message
		///
		/// @param message - The error message
		explicit Error(const std::string& message) noexcept : mMessage(message) { // NOLINT
		}

		/// @brief Constructs an `Error` with the given message
		///
		/// @param message - The error message
		explicit Error(std::string&& message) noexcept : mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		/// Takes ownership of `source`
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(const char* message, gsl::owner<Error*> source) noexcept
			: mHasSource(true), mSource(source), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		/// Takes ownership of `source`
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(const std::string& message, gsl::owner<Error*> source) noexcept // NOLINT
			: mHasSource(true), mSource(source), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		/// Takes ownership of `source`
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(std::string&& message, gsl::owner<Error*> source) noexcept
			: mHasSource(true), mSource(source), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(const char* message, const Error& source) noexcept
			: mHasSource(true), mSource(std::make_shared<Error>(source)), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(const std::string& message, const Error& source) noexcept // NOLINT
			: mHasSource(true), mSource(std::make_shared<Error>(source)), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(std::string&& message, const Error& source) noexcept
			: mHasSource(true), mSource(std::make_shared<Error>(source)), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(const char* message, Error&& source) noexcept
			: mHasSource(true), mSource(std::make_shared<Error>(source)), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(const std::string& message, Error&& source) noexcept // NOLINT
			: mHasSource(true), mSource(std::make_shared<Error>(source)), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(std::string&& message, Error&& source) noexcept
			: mHasSource(true), mSource(std::make_shared<Error>(source)), mMessage(message) {
		}

		Error(const Error& error) = default;
		Error(Error&& error) noexcept = default;

		virtual ~Error() noexcept = default;

		/// @brief Returns the source/cause `Error` of this error if there is one,
		/// passing ownership to the containing `Option`.
		///
		/// @return sourceError, if there is one, or nullptr
		[[nodiscard]] auto source() const noexcept -> const std::weak_ptr<Error> {
			return mSource;
		}

		[[nodiscard]] constexpr auto hasStdErrorCode() const noexcept -> bool {
			return mHasErrorCode;
		}

		[[nodiscard]] auto errorCode() const noexcept -> const std::error_code {
			return mErrorCode;
		}

		/// @brief Returns the error message for this `Error`
		///
		/// @return The error message
		[[nodiscard]] constexpr auto message() const noexcept -> const char* {
			return mMessage.c_str();
		}

		[[nodiscard]] inline auto messageAsStdString() const noexcept -> std::string {
			return {mMessage};
		}

		/// @brief Converts this `Error` to a `const char*`.
		/// Generally implemented by combining the `source`'s `toString` and
		/// this `Error`'s `message`
		///
		/// @return this `Error` formatted as a `const char*`
		[[nodiscard]] constexpr auto toString() const noexcept -> gsl::owner<const char*> {
			if(mHasSource) {
				return (new std::string(
							"Error: " + std::string(mMessage)
							+ "\n"
							// NOLINTNEXTLINE(cppcoreguidelines-owning-memory): this is fine
							+ "Source: " + mSource->toString() + "\n"))
					->c_str();
			}
			else {
				return (new std::string("Error: " + std::string(mMessage) + "\n"))->c_str();
			}
		}

		/// @brief Converts this `Error` to a `std::string`
		/// Generally implemented by combining the `source`'s `toStdString`
		/// and this `Error`'s `message`
		///
		/// @return this `Error` formatted as a `std::string`
		[[nodiscard]] auto toStdString() const noexcept -> std::string {
			if(mHasSource) {
				return std::string("Error: " + std::string(mMessage) + "\n"
								   + "Source: " + mSource->toStdString() + "\n");
			}
			else {
				return std::string("Error: " + std::string(mMessage) + "\n");
			}
		}

		auto operator=(const Error& error) -> Error& = default;
		auto operator=(Error&& error) noexcept -> Error& = default;

	  protected:
		/// whether this `Error` originated from a `std::error_code`
		bool mHasErrorCode = false;
		/// whether this `Error` has a source `Error`
		bool mHasSource = false;
		/// error code
		std::error_code mErrorCode = std::error_code();
		/// the source `Error` of this one
		/// We use `std::shared_ptr` instead of `std::unique_ptr` so we can be copyable
		std::shared_ptr<Error> mSource = nullptr;
		/// the error message.
		std::string mMessage;
	};
} // namespace apex::utils
