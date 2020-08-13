#pragma once
#include <functional>
#include <memory>

namespace apex::utils::synchronization {
	/// @brief Basic scoped lock guard that wraps a single value of the template type
	/// Returned by other synchronization mechanisms as a way of ensuring RAII managed locking and
	/// unlocking
	///
	/// @tparam T - The type of the value to wrap in the guard
	template<typename T>
	class [[nodiscard]] ScopedLockGuard {
	  public:
		ScopedLockGuard() = delete;
		ScopedLockGuard(const ScopedLockGuard<T>& guard) = delete;
		ScopedLockGuard(ScopedLockGuard<T>&& guard) noexcept = default;

		/// @brief Constructs a `ScopedLockGuard` with the given data and unlocker function
		///
		/// @param data - The data to be guarded
		/// @param unlocker - The unlocker to call upon destruction, updating the data in the Lock
		/// and unlocking it
		explicit ScopedLockGuard(std::shared_ptr<T> data,
								 std::function<void(void)> unlocker) noexcept
			: mData(std::move(data)), mUnlocker(std::move(unlocker)) {
		}

		~ScopedLockGuard() noexcept {
			mUnlocker();
		}

		/// @brief Writes the given value to the data
		///
		/// @param newValue - The new value to write to the data
		inline auto write(T newValue) noexcept -> void {
			*mData = newValue;
		}

		/// @brief Returns the current value of the data
		///
		/// @return - The current data
		[[nodiscard]] inline auto read() const noexcept -> T {
			return *mData;
		}

		auto operator=(const ScopedLockGuard<T>& guard) -> ScopedLockGuard<T>& = delete;
		auto operator=(ScopedLockGuard<T>&& guard) noexcept -> ScopedLockGuard<T>& = default;

	  private:
		std::shared_ptr<T> mData;
		std::function<void(void)> mUnlocker;
	};
} // namespace apex::utils::synchronization
