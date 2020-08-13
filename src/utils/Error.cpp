#include "Error.h"

namespace apex::utils {
	Error::Error(juce::String message) noexcept : mMessage(std::move(message)) {
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
	Error::Error(juce::String message, gsl::owner<Error*> source) noexcept
		: mHasSource(true), mSource(source), mMessage(std::move(message)) {
	}

	/// @brief Returns the source/cause `Error` of this error if there is one,
	/// passing ownership to the containing `Option`.
	///
	/// @return sourceError, if there is one, or nullptr
	auto Error::source() const noexcept -> const Error* {
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
	auto Error::message() const noexcept -> juce::String {
		return mMessage;
	}

	/// @brief Converts this `Error` to a `juce::String`.
	/// Generally implemented by combining the `source`'s `toString` and
	/// this `Error`'s `message`
	///
	/// @return this `Error` formatted as a `juce::String`
	auto Error::toString() const noexcept -> juce::String {
		if(mHasSource) {
			return TRANS("Error: ") + mMessage + juce::String("\n") + TRANS("Source: ")
				   + mSource->toString() + juce::String("\n");
		}
		else {
			return TRANS("Error: ") + mMessage + juce::String("\n");
		}
	}
} // namespace apex::utils
