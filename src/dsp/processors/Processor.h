#pragma once

#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Interface specifying necessary behaviors of an audio processor
		///
		/// @tparam T - The floating point type backing operations
		template<typename T>
			class Processor {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
					Processor() {

					}

					virtual ~Processor() {

					}

					/// @brief Processes the input value
					///
					/// @param input - The input value to process
					///
					/// @return - The processed value
					virtual T process(T input) = 0;

					/// @brief Processes the array of input values, in place
					///
					/// @param input - The input values to process
					/// @param numSamples - The number of samples in the array
					virtual void process(T* input, size_t numSamples) = 0;

					/// @brief Resets the processor to an initial state
					virtual void reset() = 0;

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
			};
	}
}
