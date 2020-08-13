#pragma once

#include <gsl/gsl>
#include <juce_core/juce_core.h>
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
		/// @brief Constructs an `Error` with the given message
		///
		/// @param message - The error message
		explicit Error(juce::String message) noexcept;

		/// @brief Constructs an `Error` with the given message and source.
		/// Takes ownership of `source`
		///
		/// @param message - The error message
		/// @param source - The source/cause `Error`
		Error(juce::String message, gsl::owner<Error*> source) noexcept;
		Error(const Error& error) = default;
		Error(Error&& error) noexcept = default;
		virtual ~Error() noexcept;

		/// @brief Returns the source/cause `Error` of this error if there is one,
		/// passing ownership to the containing `Option`.
		///
		/// @return sourceError, if there is one, or nullptr
		[[nodiscard]] virtual auto source() const noexcept -> const Error*;

		/// @brief Returns the error message for this `Error`
		///
		/// @return The error message
		[[nodiscard]] virtual auto message() const noexcept -> juce::String;

		/// @brief Converts this `Error` to a `juce::String`.
		/// Generally implemented by combining the `source`'s `toString` and
		/// this `Error`'s `message`
		///
		/// @return this `Error` formatted as a `juce::String`
		[[nodiscard]] virtual auto toString() const noexcept -> juce::String;

		auto operator=(const Error& error) -> Error& = default;
		auto operator=(Error&& error) noexcept -> Error& = default;
		auto operator new(std::size_t size) noexcept -> gsl::owner<void*> {
			gsl::owner<void*> pointer = std::malloc(size);
			if(pointer != nullptr) {
				return pointer;
			}
			else {
				return nullptr;
			}
		}
		auto operator delete(void* pointer) noexcept -> void {
			std::free(gsl::owner<void*>(pointer));
		}

	  protected:
		Error() noexcept = default;
		/// whether this `Error` has a source `Error`
		bool mHasSource = false;
		/// the source `Error` of this one
		gsl::owner<Error*> mSource = nullptr;
		/// the error message
		juce::String mMessage = "";
	};
} // namespace apex::utils
