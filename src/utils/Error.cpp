#include "Error.h"
#include "Option.h"

namespace apex {
	namespace utils {

		Error::~Error() noexcept {

		}

		/// @brief Returns the source/cause `Error` of this error aka `Some(sourceError)`,
		/// if applicable, or `None` otherwise
		///
		/// @return `Some(sourceError)` if applicable, or `None`
		Option<Error*> Error::source() const noexcept {
			if(mHasSource) {
				return Option<Error*>::Some(mSource.get());
			}
			else return Option<Error*>::None();
		}

		/// @brief Returns the error message for this `Error`
		///
		/// @return The error message
		juce::String Error::message() const noexcept {
			return mMessage;
		}

		/// @brief Converts this `Error` to a `juce::String`.
		/// Generally implemented by combining the `source`'s `toString` and
		/// this `Error`'s `message`
		///
		/// @return this `Error` formatted as a `juce::String`
		juce::String Error::toString() const noexcept {
			return juce::String(TRANS("Source: ")) + mSource->toString() + juce::String("\n")
				+ juce::String(TRANS("Message: ")) + mMessage;
		}
	}
}

