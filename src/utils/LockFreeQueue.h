#pragma once

#include <array>
#include <cstdint>

#include "Result.h"

namespace apex::utils {
#ifndef _MSVC_VER
	using std::size_t;
#endif

	enum class LockFreeQueueErrorTypes
	{
		QueueIsFull,
		QueueIsEmpty
	};

	template<LockFreeQueueErrorTypes Type>
	class LockFreeQueueError final : public Error {
	  public:
		constexpr LockFreeQueueError() noexcept {
			if constexpr(Type == LockFreeQueueErrorTypes::QueueIsFull) {
				this->mMessage = "Failed to push entry into LockFreeQueue: LockFreeQueue Is Full";
			}
			else {
				this->mMessage = "Failed to read entry from LockFreeQueue: LockFreeQueue Is Empty";
			}
		}
		constexpr LockFreeQueueError(const LockFreeQueueError<Type>& error) noexcept = default;
		constexpr LockFreeQueueError(LockFreeQueueError<Type>&& error) noexcept = default;
		~LockFreeQueueError() noexcept final = default;

		constexpr auto operator=(const LockFreeQueueError<Type>& error) noexcept
			-> LockFreeQueueError<Type>& = default;
		constexpr auto
		operator=(LockFreeQueueError<Type>&& error) noexcept -> LockFreeQueueError<Type>& = default;
	};

	template<typename T, size_t Capacity = 512>
	class LockFreeQueue {
	  public:
		using PushError = LockFreeQueueError<LockFreeQueueErrorTypes::QueueIsFull>;
		using ReadError = LockFreeQueueError<LockFreeQueueErrorTypes::QueueIsEmpty>;

		constexpr LockFreeQueue() noexcept = default;
		constexpr LockFreeQueue(const LockFreeQueue<T, Capacity>& queue) noexcept = default;
		constexpr LockFreeQueue(LockFreeQueue<T, Capacity>&& queue) noexcept = default;

		[[nodiscard]] inline auto push(T entry) noexcept -> Result<bool, PushError> {
			if(mWriteIndex == mReadIndex && mWriteGeneration > mReadGeneration) {
				return Result<bool, PushError>::Err(PushError());
			}

			mData.at(mWriteIndex) = entry;
			mWriteIndex++;
			if(mWriteIndex == mCapacity - 1) {
				mWriteIndex = 0;
				mWriteGeneration++;
			}
			return Result<bool, PushError>::Ok(true);
		}

		[[nodiscard]] inline auto read() noexcept -> Result<T, ReadError> {
			if(isEmpty()) {
				return Result<T, ReadError>::Err(ReadError());
			}
			else {
				mReadIndex++;
				if(mReadIndex == mCapacity - 1) {
					mReadIndex = 0;
					mReadGeneration++;
				}
				return Result<T, ReadError>::Ok(mData.at(mReadIndex));
			}
		}

		[[nodiscard]] inline auto isEmpty() const noexcept -> bool {
			if(mWriteGeneration == mReadGeneration) {
				return mWriteIndex == mReadIndex;
			}
			else {
				return !(mWriteGeneration > mReadGeneration);
			}
		}

	  private:
		static const constexpr size_t mCapacity = Capacity;
		size_t mReadIndex = 0;
		size_t mReadGeneration = 0;
		size_t mWriteIndex = 0;
		size_t mWriteGeneration = 0;
		std::array<T, mCapacity> mData = std::array<T, mCapacity>();
	};
} // namespace apex::utils
