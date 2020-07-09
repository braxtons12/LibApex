#pragma once

#include <juce_core/juce_core.h>

namespace apex {
	namespace utils {

		template<typename T>
			class Option;

		/// @brief Base error interface.
		/// Used to implement custom error types used as the `E` in `Result<T, E>`
		/// to represent and communicate failure of a function
		///
		/// @see `Result<T, E>`
		/// @tags {`utils`}
		class Error {

			public:
				Error() = delete;
				virtual ~Error();

				/// @brief Returns the source/cause `Error` of this error aka `Some(sourceError)`,
				/// if applicable, or `None` otherwise
				///
				/// @return `Some(sourceError)` if applicable, or `None`
				virtual Option<Error*> source() const;

				/// @brief Returns the error message for this `Error`
				///
				/// @return The error message
				virtual juce::String message() const;

				/// @brief Converts this `Error` to a `juce::String`.
				/// Generally implemented by combining the `source`'s `toString` and
				/// this `Error`'s `message`
				///
				/// @return this `Error` formatted as a `juce::String`
				virtual juce::String toString() const;
			protected:
				///whether this `Error` has a source `Error`
				bool mHasSource = false;
				///the source `Error` of this one
				std::unique_ptr<Error> mSource = nullptr;
				///the error message
				juce::String mMessage;
		};
	}
}
