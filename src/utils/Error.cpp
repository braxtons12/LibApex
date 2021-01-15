#include "Error.h"

namespace apex::utils {
	constexpr Error::Error(const char* message) noexcept : mMessage(message) {
	}

	Error::~Error() noexcept {
		if(mHasSource) {
			delete mSource;
			mSource = nullptr;
		}
	}

	/// @brief Constructs an `Error` with the given message and source.
	/// Takes ownership of `source`
	///
	/// @param message - The error message
	/// @param source - The source/cause `Error`
	constexpr Error::Error(const char* message, gsl::owner<Error*> source) noexcept
		: mHasSource(true), mSource(source), mMessage(message) {
	}

	/// @brief Returns the source/cause `Error` of this error if there is one,
	/// passing ownership to the containing `Option`.
	///
	/// @return sourceError, if there is one, or nullptr
	constexpr auto Error::source() const noexcept -> const Error* {
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
	constexpr auto Error::message() const noexcept -> const char* {
		return mMessage;
	}

	/// @brief Converts this `Error` to a `const char*`.
	/// Generally implemented by combining the `source`'s `toString` and
	/// this `Error`'s `message`
	///
	/// @return this `Error` formatted as a `const char*`
	constexpr auto Error::toString() const noexcept -> const char* {
		if(mHasSource) {
			return (new std::string("Error: " + std::string(mMessage) + "\n"
									+ "Source: " + mSource->toString() + "\n"))
				->c_str();
		}
		else {
			return (new std::string("Error: " + std::string(mMessage) + "\n"))->c_str();
		}
	}
} // namespace apex::utils
