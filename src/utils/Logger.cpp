
#include "Logger.h"

namespace apex::utils {
	std::atomic_bool Logger::INITIALIZED = std::atomic_bool(false);
	std::atomic_bool Logger::EXIT_THREAD = std::atomic_bool(false);
	juce::String Logger::NAME_ROOT = juce::String();
	std::shared_ptr<juce::FileLogger> Logger::LOGGER = nullptr;
	std::shared_ptr<LockFreeQueue<juce::String>> Logger::MESSAGES = nullptr;
	std::unique_ptr<std::thread> Logger::MESSAGE_THREAD = nullptr;
	juce::CriticalSection Logger::MESSAGES_LOCK = juce::CriticalSection();

} // namespace apex::utils
