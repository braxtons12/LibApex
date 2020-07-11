#include <functional>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Processor.h"

namespace apex {
	namespace dsp {

		/// @brief Multichannel `Processor` class to duplicate a single processor type across multiple channels
		///
		/// @tparam T - The floating point type to back operations
		/// @tparam F - The `Processor` type
		template<typename T, typename F>
			class MultiChannelProcessor {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
					static_assert(std::is_base_of<Processor<T>, F>::value, "F must be a Processor type");

				private:
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiChannelProcessor)
			};

		/// @brief Multichannel `Processor` class to duplicate a single processor type across multiple channels
		///
		/// @tparam F - The `Processor` type
		template<typename F>
			class MultiChannelProcessor<float, F> {
				public:
					static_assert(std::is_base_of<Processor<float>, F>::value, "F must be a Processor type");

					MultiChannelProcessor() = delete;

					/// @brief Creates a `MultiChannelProcessor` with the given number of channels
					///
					/// @param numChannels - The number of channels in this processor
					MultiChannelProcessor(size_t numChannels) noexcept {
						mProcessors.resize(numChannels);
					}

					/// @brief Processes the array of input values, in place
					///
					/// @param input - The array of input values to process
					/// @param numSamples - The number of samples in each channel
					void process(float** input, size_t numSamples) noexcept {
						for(size_t channel = 0; channel < mProcessors.size(); ++channel) {
							mProcessors[channel].process(input[channel], numSamples);
						}
					}

					/// @brief Resets the processor to an initial state
					void reset() noexcept {
						std::for_each(mProcessors.begin(), mProcessors.end(), [](F& proc) { proc.reset(); });
					}

					/// @brief Sets the number of channels in this processor
					///
					/// @param numChannels - The new number of channels
					void setNumChannels(size_t numChannels) noexcept {
						if(mProcessors.size() != numChannels) mProcessors.resize(numChannels);
					}

					/// @brief Returns the number of channels in this processor
					///
					/// @return - The number of channels in this processor
					size_t getNumChannels() const noexcept {
						return mProcessors.size();
					}

					/// @brief Updates the state of the internal processors via the given function.
					///
					/// @param stateChanger - The function used to update the processors's state
					void changeState(std::function<void(F&)> stateChanger) noexcept {
						std::for_each(mProcessors.begin(), mProcessors.end(), stateChanger);
					}

				private:
					std::vector<F> mProcessors;
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiChannelProcessor)
			};

		/// @brief Multichannel `Processor` class to duplicate a single processor type across multiple channels
		///
		/// @tparam F - The `Processor` type
		template<typename F>
			class MultiChannelProcessor<double, F> {
				public:
					static_assert(std::is_base_of<Processor<double>, F>::value, "F must be a Processor type");

					MultiChannelProcessor() = delete;

					/// @brief Creates a `MultiChannelProcessor` with the given number of channels
					///
					/// @param numChannels - The number of channels in this processor
					MultiChannelProcessor(size_t numChannels) noexcept {
						mProcessors.resize(numChannels);
					}

					/// @brief Processes the array of input values, in place
					///
					/// @param input - The array of input values to process
					/// @param numSamples - The number of samples in each channel
					void process(double** input, size_t numSamples) noexcept {
						for(size_t channel = 0; channel < mProcessors.size(); ++channel) {
							mProcessors[channel].process(input[channel], numSamples);
						}
					}

					/// @brief Resets the processor to an initial state
					void reset() noexcept {
						std::for_each(mProcessors.begin(), mProcessors.end(), [](F& proc) { proc.reset(); });
					}

					/// @brief Sets the number of channels in this processor
					///
					/// @param numChannels - The new number of channels
					void setNumChannels(size_t numChannels) noexcept {
						if(mProcessors.size() != numChannels) mProcessors.resize(numChannels);
					}

					/// @brief Returns the number of channels in this processor
					///
					/// @return - The number of channels in this processor
					size_t getNumChannels() const noexcept {
						return mProcessors.size();
					}

					/// @brief Updates the state of the internal processors via the given function.
					///
					/// @param stateChanger - The function used to update the processors's state
					void changeState(std::function<void(F&)> stateChanger) noexcept {
						std::for_each(mProcessors.begin(), mProcessors.end(), stateChanger);
					}

				private:
					std::vector<F> mProcessors;
					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiChannelProcessor)
			};
	}
}
