#pragma once

#include <memory>

#include <juce_core/juce_core.h>

namespace apex {
	namespace utils {

		class Error {

			public:
				Error() = delete;
				~Error() = delete;

				virtual Error& source();
				virtual juce::String message();
				virtual juce::String toString();
			protected:
				std::unique_ptr<Error> mSource;
				juce::String mMessage;
		};
	}
}
