#pragma once

#include <atomic>
#include <gsl/gsl>
#include <iostream>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <thread>

#include "LockFreeQueue.h"

namespace apex::utils {
	class Logger {
	  public:
		static auto LogMessage(juce::String message) noexcept -> void {
			auto local = std::move(message);
			if(INITIALIZED.load()) {
				auto messages = GetMessagesInstance();
				if(messages != nullptr) {
					auto res = messages->push(local);
					if(res.isErr()) {
						std::cerr << "Failed to log Message: " << local << "\n";
					}
				}
			}
		}

		static auto InitLogger(juce::String rootName) noexcept -> void {
			if(!INITIALIZED.load()) {
				auto lock = juce::ScopedLock(MESSAGES_LOCK);
				EXIT_THREAD.store(false);
				NAME_ROOT = std::move(rootName);
				if(MESSAGE_THREAD == nullptr) {
					MESSAGE_THREAD = std::make_unique<std::thread>([&]() {
						while(!EXIT_THREAD.load()) {
							auto messages = GetMessagesInstance();
							if(!messages->isEmpty()) {
								auto logger = GetLoggerInstance();
								if(logger != nullptr) {
									auto res = messages->read();
									if(res.isOk()) {
										logger->logMessage(res.unwrap());
									}
								}
							}
						}
					});
				}
				INITIALIZED.store(true);
			}
		}

		static auto CloseLogger() noexcept -> void {
			if(INITIALIZED.load()) {
				auto lock = juce::ScopedLock(MESSAGES_LOCK);
				EXIT_THREAD.store(true);
				MESSAGE_THREAD->join();
				INITIALIZED.store(false);
			}
		}

		static auto LoggerInitialized() noexcept -> bool {
			return INITIALIZED.load();
		}

	  private:
		static std::atomic_bool INITIALIZED;
		static std::atomic_bool EXIT_THREAD;
		static juce::String NAME_ROOT;
		static std::shared_ptr<juce::FileLogger> LOGGER;
		static std::shared_ptr<LockFreeQueue<juce::String>> MESSAGES;
		static std::unique_ptr<std::thread> MESSAGE_THREAD;
		static juce::CriticalSection MESSAGES_LOCK;

		static auto GetLoggerInstance() noexcept -> std::shared_ptr<juce::FileLogger> {
			if(LOGGER == nullptr) {
				LOGGER = std::shared_ptr<juce::FileLogger>(
					juce::FileLogger::createDateStampedLogger("ApexAudio",
															  NAME_ROOT,
															  ".apexlog",
															  "Starting Apex Audio Logger"));
			}
			return LOGGER;
		}

		static auto GetMessagesInstance() noexcept -> std::shared_ptr<LockFreeQueue<juce::String>> {
			if(MESSAGES == nullptr) {
				MESSAGES = std::make_shared<LockFreeQueue<juce::String>>();
			}

			return MESSAGES;
		}
	};
} // namespace apex::utils
