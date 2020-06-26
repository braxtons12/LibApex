#include "Error.h"

namespace apex {
	namespace utils {
		Error& Error::source() {
			return *mSource.get();
		}

		juce::String Error::message() {
			return mMessage;
		}

		juce::String Error::toString() {
			return juce::String(TRANS("Source: ")) + mSource->toString() + juce::String("\n")
				+ juce::String(TRANS("Message: ")) + mMessage;
		}
	}
}
