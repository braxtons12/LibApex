#pragma once

#include <gsl/gsl>
#include <memory>
#include <string>
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
		constexpr Error() noexcept = default;

		/// @brief Constructs an `Error` with the given message
		///
		/// @param message - The error message
		constexpr explicit Error(const char* message) noexcept : mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message
		///
		/// @param message - The error message
		explicit Error(const std::string& message) noexcept : mMessage(message.c_str()) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		/// Takes ownership of `source`
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		constexpr Error(const char* message, gsl::owner<Error*> source) noexcept
			: mHasSource(true), mSource(source), mMessage(message) {
		}

		/// @brief Constructs an `Error` with the given message and source.
		/// Takes ownership of `source`
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		constexpr Error(const std::string& message, gsl::owner<Error*> source) noexcept
			: mHasSource(true), mSource(source), mMessage(message.c_str()) {
		}

		constexpr Error(const Error& error) = delete;
		constexpr Error(Error&& error) noexcept = default;

		virtual ~Error() noexcept {
			if(mHasSource) {
				delete mSource;
				mSource = nullptr;
			}
		}

		/// @brief Returns the source/cause `Error` of this error if there is one,
		/// passing ownership to the containing `Option`.
		///
		/// @return sourceError, if there is one, or nullptr
		[[nodiscard]] constexpr auto source() const noexcept -> const Error* {
			if(mHasSource) {
				return mSource;
			}
			else {
				return nullptr;
			}
		}

		/// @brief Returns the error message for this `Error`
		///
		/// @return The error message
		[[nodiscard]] constexpr auto message() const noexcept -> const char* {
			return mMessage;
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

		auto operator=(const Error& error) -> Error& = delete;
		auto operator=(Error&& error) noexcept -> Error& = default;

		auto operator new(std::size_t size) noexcept -> gsl::owner<void*> {
			// clang-format off
			// NOLINTNEXTLINE(hicpp-no-malloc, cppcoreguidelines-no-malloc): We're writing new, we have to use malloc
			gsl::owner<void*> pointer = std::malloc(size);
			//clang-format on
			if(pointer != nullptr) {
				return pointer;
			}
			else {
				return nullptr;
			}
		}
		auto operator delete(void* pointer) noexcept -> void {
			// clang-format off
			// NOLINTNEXTLINE(hicpp-no-malloc, cppcoreguidelines-no-malloc): We're writing new, we have to use malloc
			std::free(gsl::owner<void*>(pointer));
			//clang-format on
		}

	  protected:
		/// whether this `Error` has a source `Error`
		bool mHasSource = false;
		/// the source `Error` of this one
		/// We use a raw pointer so we can be constexpr
		gsl::owner<Error*> mSource = nullptr;
		/// the error message.
		/// We use a c string instead of `std::string` internally so we can be constexpr
		const char* mMessage = "";
	};
} // namespace apex::utils
