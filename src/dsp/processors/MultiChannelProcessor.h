#include <functional>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"
#include "Processor.h"

namespace apex::dsp {
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

				using MultiChannelArray = gsl::span<gsl::span<float, gsl::dynamic_extent>, gsl::dynamic_extent>;

				MultiChannelProcessor() = delete;

				/// @brief Creates a `MultiChannelProcessor` with the given number of channels
				///
				/// @param numChannels - The number of channels in this processor
				explicit MultiChannelProcessor(size_t numChannels) noexcept {
					mProcessors.resize(numChannels);
				}

				/// @brief Processes the array of input values, in place
				///
				/// @param input - The array of input values to process
				auto process(MultiChannelArray input) noexcept -> void {
					jassert(mProcessors.size() == input.size());

					auto size = static_cast<gsl::index>(mProcessors.size());
					for(gsl::index channel = 0; channel < size; ++channel) {
						mProcessors.at(channel).process(gsl::at(input, channel));
					}
				}

				/// @brief Resets the processor to an initial state
				auto reset() noexcept -> void {
					std::for_each(mProcessors.begin(), mProcessors.end(), [](F& proc) { proc.reset(); });
				}

				/// @brief Sets the number of channels in this processor
				///
				/// @param numChannels - The new number of channels
				auto setNumChannels(size_t numChannels) noexcept -> void {
					if(mProcessors.size() != numChannels) {
						mProcessors.resize(numChannels);
					}
				}

				/// @brief Returns the number of channels in this processor
				///
				/// @return - The number of channels in this processor
				[[nodiscard]]
					auto getNumChannels() const noexcept -> size_t {
						return mProcessors.size();
					}

				/// @brief Updates the state of the internal processors via the given function.
				///
				/// @param stateChanger - The function used to update the processors's state
				auto changeState(std::function<void(F&)> stateChanger) noexcept -> void {
					std::for_each(mProcessors.begin(), mProcessors.end(), stateChanger);
				}

			private:
				std::vector<F> mProcessors = std::vector<F>();

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiChannelProcessor)
		};

	/// @brief Multichannel `Processor` class to duplicate a single processor type across multiple channels
	///
	/// @tparam F - The `Processor` type
	template<typename F>
		class MultiChannelProcessor<double, F> {
			public:
				static_assert(std::is_base_of<Processor<double>, F>::value, "F must be a Processor type");

				using MultiChannelArray = gsl::span<gsl::span<double, gsl::dynamic_extent>, gsl::dynamic_extent>;

				MultiChannelProcessor() = delete;

				/// @brief Creates a `MultiChannelProcessor` with the given number of channels
				///
				/// @param numChannels - The number of channels in this processor
				explicit MultiChannelProcessor(size_t numChannels) noexcept {
					mProcessors.resize(numChannels);
				}

				/// @brief Processes the array of input values, in place
				///
				/// @param input - The array of input values to process
				auto process(MultiChannelArray input) noexcept -> void {
					jassert(mProcessors.size() == input.size());

					auto size = static_cast<gsl::index>(mProcessors.size());
					for(gsl::index channel = 0; channel < size; ++channel) {
						mProcessors.at(channel).process(gsl::at(input, channel));
					}
				}

				/// @brief Resets the processor to an initial state
				auto reset() noexcept -> void {
					std::for_each(mProcessors.begin(), mProcessors.end(), [](F& proc) { proc.reset(); });
				}

				/// @brief Sets the number of channels in this processor
				///
				/// @param numChannels - The new number of channels
				auto setNumChannels(size_t numChannels) noexcept -> void {
					if(mProcessors.size() != numChannels) {
						mProcessors.resize(numChannels);
					}
				}

				/// @brief Returns the number of channels in this processor
				///
				/// @return - The number of channels in this processor
				[[nodiscard]]
					auto getNumChannels() const noexcept -> size_t {
						return mProcessors.size();
					}

				/// @brief Updates the state of the internal processors via the given function.
				///
				/// @param stateChanger - The function used to update the processors's state
				auto changeState(std::function<void(F&)> stateChanger) noexcept -> void {
					std::for_each(mProcessors.begin(), mProcessors.end(), stateChanger);
				}

			private:
				std::vector<F> mProcessors = std::vector<F>();
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiChannelProcessor)
		};
} //namespace apex::dsp
