#pragma once

#include <cstddef>
#include <gsl/gsl>
#include <iterator>
#include <limits>

#include "../math/General.h"
#include "Concepts.h"

namespace apex::utils {
	using concepts::DefaultConstructible, concepts::Integral, concepts::Copyable, concepts::Movable,
		concepts::NotMovable;
	/// @brief A simple Ring Buffer implementation.
	/// Supports resizing, writing, reading, erasing, and provides mutable and immutable
	/// random access iterators.
	///
	/// # Iterator Invalidation
	/// * Iterators are lazily evaluated, so will only ever be invalidated at their current state.
	/// Performing any mutating operation (mutating the iterator, not the underlying data) on them
	/// will re-sync them with their associated `RingBuffer`.
	/// The following operations will invalidate an iterator's current state:
	/// - Read-only operations: never
	/// - clear: always
	/// - reserve: only if the `RingBuffer` changed capacity
	/// - erase: Erased elements and all following elements
	/// - push_back, emplace_back: only `end()`
	/// - insert, emplace: only the element at the position inserted/emplaced
	/// - pop_back: the element removed and `end()`
	///
	/// @tparam T - The type to store in the `RingBuffer`; Must be Default Constructible
	template<DefaultConstructible T>
	class RingBuffer {
	  public:
		/// Default capacity of `RingBuffer`
		static const constexpr size_t DEFAULT_CAPACITY = 16;

		/// @brief Random-Access Bidirectional iterator for `RingBuffer`
		/// @note All navigation operators are checked such that any movement past `begin()` or
		/// `end()` is ignored.
		class Iterator {
		  public:
			using iterator_category = std::random_access_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

			constexpr explicit Iterator(pointer ptr,
										RingBuffer* containerPtr,
										size_t currentIndex) noexcept
				: mPtr(ptr), mContainerPtr(containerPtr), mCurrentIndex(currentIndex) {
			}
			constexpr Iterator(const Iterator& iter) noexcept = default;
			constexpr Iterator(Iterator&& iter) noexcept = default;
			~Iterator() noexcept = default;

			/// @brief Returns the index in the `RingBuffer` that corresponds
			/// to the element this iterator points to
			///
			/// @return The index corresponding with the element this points to
			[[nodiscard]] constexpr inline auto getIndex() const noexcept -> size_t {
				return mCurrentIndex;
			}

			constexpr auto operator=(const Iterator& iter) noexcept -> Iterator& = default;
			constexpr auto operator=(Iterator&& iter) noexcept -> Iterator& = default;

			constexpr inline auto operator==(const Iterator& rhs) const noexcept -> bool {
				return mPtr == rhs.mPtr;
			}

			constexpr inline auto operator!=(const Iterator& rhs) const noexcept -> bool {
				return mPtr != rhs.mPtr;
			}

			constexpr inline auto operator*() const noexcept -> reference {
				return *mPtr;
			}

			constexpr inline auto operator->() noexcept -> pointer {
				return mPtr;
			}

			constexpr inline auto operator++() noexcept -> Iterator& {
				mCurrentIndex++;
				if(mCurrentIndex >= mContainerPtr->capacity()) {
					mCurrentIndex = mContainerPtr->capacity();
					mPtr = mContainerPtr->end().mPtr;
				}
				else {
					mPtr = &(*mContainerPtr)[mCurrentIndex];
				}
				return *this;
			}

			constexpr inline auto operator++(int) noexcept -> Iterator {
				Iterator temp = *this;
				++(*this);
				return temp;
			}

			constexpr inline auto operator--() noexcept -> Iterator& {
				if(mCurrentIndex == 0) {
					return *this;
				}
				else {
					mCurrentIndex--;
					mPtr = &(*mContainerPtr)[mCurrentIndex];
				}
				return *this;
			}

			constexpr inline auto operator--(int) noexcept -> Iterator {
				Iterator temp = *this;
				--(*this);
				return temp;
			}

			constexpr inline auto operator+(Integral auto rhs) const noexcept -> Iterator {
				const auto diff = static_cast<size_t>(rhs);
				if(rhs < 0) {
					return std::move(*this - -rhs);
				}

				auto temp = *this;
				temp.mCurrentIndex += diff;
				if(temp.mCurrentIndex > temp.mContainerPtr->capacity()) {
					temp.mCurrentIndex = temp.mContainerPtr->capacity();
					temp.mPtr = temp.mContainerPtr->end().mPtr;
				}
				else {
					temp.mPtr = &(*temp.mContainerPtr)[temp.mCurrentIndex];
				}
				return temp;
			}

			constexpr inline auto operator+=(Integral auto rhs) noexcept -> Iterator& {
				*this = std::move(*this + rhs);
				return *this;
			}

			constexpr inline auto operator-(Integral auto rhs) const noexcept -> Iterator {
				const auto diff = static_cast<size_t>(rhs);
				if(rhs < 0) {
					return std::move(*this + -rhs);
				}

				auto temp = *this;
				if(diff > temp.mCurrentIndex) {
					temp.mPtr = temp.mContainerPtr->begin().mPtr;
					temp.mCurrentIndex = 0;
				}
				else {
					temp.mCurrentIndex -= diff;
					temp.mPtr = &(*temp.mContainerPtr)[temp.mCurrentIndex];
				}
				return temp;
			}

			constexpr inline auto operator-=(Integral auto rhs) noexcept -> Iterator& {
				*this = std::move(*this - rhs);
				return *this;
			}

			constexpr inline auto operator-(const Iterator& rhs) const noexcept -> difference_type {
				return mPtr - rhs.mPtr;
			}

			constexpr inline auto operator[](Integral auto index) noexcept -> Iterator {
				return std::move(*this + index);
			}

			constexpr inline auto operator>(const Iterator& rhs) const noexcept -> bool {
				return mCurrentIndex > rhs.mCurrentIndex;
			}

			constexpr inline auto operator<(const Iterator& rhs) const noexcept -> bool {
				return mCurrentIndex < rhs.mCurrentIndex;
			}

			constexpr inline auto operator>=(const Iterator& rhs) const noexcept -> bool {
				return mCurrentIndex >= rhs.mCurrentIndex;
			}

			constexpr inline auto operator<=(const Iterator& rhs) const noexcept -> bool {
				return mCurrentIndex <= rhs.mCurrentIndex;
			}

		  private:
			pointer mPtr;
			RingBuffer* mContainerPtr = nullptr;
			size_t mCurrentIndex = 0;
		};

		/// @brief Read-only Random-Access Bidirectional iterator for `RingBuffer`
		/// @note All navigation operators are checked such that any movement past `begin()` or
		/// `end()` is ignored.
		class ConstIterator {
		  public:
			using iterator_category = std::random_access_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = const value_type*;
			using reference = const value_type&;

			constexpr explicit ConstIterator(pointer ptr,
											 RingBuffer* containerPtr,
											 size_t currentIndex) noexcept
				: mPtr(ptr), mContainerPtr(containerPtr), mCurrentIndex(currentIndex) {
			}
			constexpr ConstIterator(const ConstIterator& iter) noexcept = default;
			constexpr ConstIterator(ConstIterator&& iter) noexcept = default;
			~ConstIterator() noexcept = default;

			/// @brief Returns the index in the `RingBuffer` that corresponds
			/// to the element this iterator points to
			///
			/// @return The index corresponding with the element this points to
			[[nodiscard]] constexpr inline auto getIndex() const noexcept -> size_t {
				return mCurrentIndex;
			}

			constexpr auto
			operator=(const ConstIterator& iter) noexcept -> ConstIterator& = default;
			constexpr auto operator=(ConstIterator&& iter) noexcept -> ConstIterator& = default;

			constexpr inline auto operator==(const ConstIterator& rhs) const noexcept -> bool {
				return mPtr == rhs.mPtr;
			}

			constexpr inline auto operator!=(const ConstIterator& rhs) const noexcept -> bool {
				return mPtr != rhs.mPtr;
			}

			constexpr inline auto operator*() const noexcept -> reference {
				return *mPtr;
			}

			constexpr inline auto operator->() const noexcept -> pointer {
				return mPtr;
			}

			constexpr inline auto operator++() noexcept -> ConstIterator& {
				mCurrentIndex++;
				if(mCurrentIndex >= mContainerPtr->capacity()) {
					mCurrentIndex = mContainerPtr->capacity();
					mPtr = mContainerPtr->end().mPtr;
				}
				else {
					mPtr = &(*mContainerPtr)[mCurrentIndex];
				}
				return *this;
			}

			constexpr inline auto operator++(int) noexcept -> ConstIterator {
				ConstIterator temp = *this;
				++(*this);
				return temp;
			}

			constexpr inline auto operator--() noexcept -> ConstIterator& {
				if(mCurrentIndex == 0) {
					return *this;
				}
				else {
					mCurrentIndex--;
					mPtr = &(*mContainerPtr)[mCurrentIndex];
				}
				return *this;
			}

			constexpr inline auto operator--(int) noexcept -> ConstIterator {
				ConstIterator temp = *this;
				--(*this);
				return temp;
			}

			constexpr inline auto operator+(Integral auto rhs) const noexcept -> ConstIterator {
				const auto diff = static_cast<size_t>(rhs);
				if(rhs < 0) {
					return std::move(*this - -rhs);
				}

				auto temp = *this;
				temp.mCurrentIndex += diff;
				if(temp.mCurrentIndex > temp.mContainerPtr->capacity()) {
					temp.mCurrentIndex = temp.mContainerPtr->capacity();
					temp.mPtr = temp.mContainerPtr->end().mPtr;
				}
				else {
					temp.mPtr = &(*temp.mContainerPtr)[temp.mCurrentIndex];
				}
				return temp;
			}

			constexpr inline auto operator+=(Integral auto rhs) noexcept -> ConstIterator& {
				*this = std::move(*this + rhs);
				return *this;
			}

			constexpr inline auto operator-(Integral auto rhs) const noexcept -> ConstIterator {
				const auto diff = static_cast<size_t>(rhs);
				if(rhs < 0) {
					return std::move(*this + -rhs);
				}

				auto temp = *this;
				if(diff > temp.mCurrentIndex) {
					temp.mPtr = temp.mContainerPtr->begin().mPtr;
					temp.mCurrentIndex = 0;
				}
				else {
					temp.mCurrentIndex -= diff;
					temp.mPtr = &(*temp.mContainerPtr)[temp.mCurrentIndex];
				}
				return temp;
			}

			constexpr inline auto operator-=(Integral auto rhs) noexcept -> ConstIterator& {
				*this = std::move(*this - rhs);
				return *this;
			}

			constexpr inline auto
			operator-(const ConstIterator& rhs) const noexcept -> difference_type {
				return mPtr - rhs.mPtr;
			}

			constexpr inline auto operator[](Integral auto index) const noexcept -> ConstIterator {
				return std::move(*this + index);
			}

			constexpr inline auto operator>(const ConstIterator& rhs) const noexcept -> bool {
				return mCurrentIndex > rhs.mCurrentIndex;
			}

			constexpr inline auto operator<(const ConstIterator& rhs) const noexcept -> bool {
				return mCurrentIndex < rhs.mCurrentIndex;
			}

			constexpr inline auto operator>=(const ConstIterator& rhs) const noexcept -> bool {
				return mCurrentIndex >= rhs.mCurrentIndex;
			}

			constexpr inline auto operator<=(const ConstIterator& rhs) const noexcept -> bool {
				return mCurrentIndex <= rhs.mCurrentIndex;
			}

		  private:
			pointer mPtr;
			RingBuffer* mContainerPtr = nullptr;
			size_t mCurrentIndex = 0;
		};

		/// @brief Creates a `RingBuffer` with default capacity
		RingBuffer() noexcept : mBuffer(new T[DEFAULT_CAPACITY_INTERNAL]) {
		}

		/// @brief Creates a `RingBuffer` with (at least) the given initial capacity
		///
		/// @param initialCapacity - The initial capacity of the `RingBuffer`
		constexpr explicit RingBuffer(size_t initialCapacity) noexcept
			: mBuffer(new T[initialCapacity + 1]), mLoopIndex(initialCapacity),
			  mCapacity(initialCapacity) {
		}

		/// @brief Constructs a new `RingBuffer` with the given initial capacity and
		/// fills it with `defaultValue`
		///
		/// @param initialCapacity - The initial capacity of the `RingBuffer`
		/// @param defaultValue - The value to fill the `RingBuffer` with
		constexpr RingBuffer(size_t initialCapacity,
							 const T& defaultValue) noexcept requires Copyable<T>
			: mBuffer(new T[initialCapacity + 1]),
			  mLoopIndex(initialCapacity),
			  mCapacity(initialCapacity) {
			for(auto i = 0ULL; i < mCapacity; ++i) {
				mBuffer[i] // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					= defaultValue;
				incrementIndices();
			}
		}

		constexpr RingBuffer(const RingBuffer& buffer) noexcept requires Copyable<T>
			: mBuffer(new T[buffer.mCapacity + 1]),
			  mWriteIndex(0ULL), // NOLINT
			  mStartIndex(0ULL), // NOLINT
			  mLoopIndex(buffer.mLoopIndex),
			  mCapacity(buffer.mCapacity) {
			for(auto i = 0ULL; i < mCapacity; ++i) {
				push_back(buffer[i]);
			}
			mStartIndex = buffer.mStartIndex; // NOLINT(cppcoreguidelines-prefer-member-initializer)
			mWriteIndex = buffer.mWriteIndex; // NOLINT(cppcoreguidelines-prefer-member-initializer)
			mSize = buffer.mSize;			  // NOLINT(cppcoreguidelines-prefer-member-initializer)
		}

		constexpr RingBuffer(RingBuffer&& buffer) noexcept
			: mBuffer(std::move(buffer.mBuffer)), mCapacity(buffer.mCapacity),
			  mLoopIndex(buffer.mLoopIndex), mStartIndex(buffer.mStartIndex), mSize(buffer.mSize) {
			buffer.mBuffer = nullptr;
			buffer.mCapacity = 0ULL;
			buffer.mLoopIndex = 0ULL;
			buffer.mSize = 0ULL;
			buffer.mWriteIndex = 0ULL;
			buffer.mStartIndex = 0ULL;
		}

		~RingBuffer() noexcept = default;

		/// @brief Returns the element at the given index.
		/// @note This is not checked in the same manner as STL containers:
		/// if index >= capacity, the element at capacity - 1 is returned.
		///
		/// @param index - The index of the desired element
		///
		/// @return The element at the given index, or at capacity - 1 if index >= capacity
		[[nodiscard]] constexpr inline auto at(Integral auto index) noexcept -> T& {
			auto i = getAdjustedInternalIndex(index);

			if(i > mLoopIndex) {
				i = mLoopIndex;
			}

			return mBuffer[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		/// @brief Returns the first element in the `RingBuffer`
		///
		/// @return The first element
		[[nodiscard]] constexpr inline auto front() noexcept -> T& {
			return mBuffer[mStartIndex]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		/// @brief Returns the last element in the `RingBuffer`
		/// @note If <= 1 elements are in the `RingBuffer`, this will be the same as `front`
		///
		/// @return The last element
		[[nodiscard]] constexpr inline auto back() noexcept -> T& {
			auto index = mWriteIndex - 1;
			if(mWriteIndex == 0) {
				if(mStartIndex == 0) {
					index = 0;
				}
				else {
					index = mLoopIndex;
				}
			}
			return mBuffer[index]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		/// @brief Returns a pointer to the underlying data in the `RingBuffer`.
		/// @note This is not sorted in any way to match the representation used by the `RingBuffer`
		///
		/// @return A pointer to the underlying data
		[[nodiscard]] constexpr inline auto data() noexcept -> T* {
			return mBuffer;
		}

		/// @brief Returns whether the `RingBuffer` is empty
		///
		/// @return `true` if the `RingBuffer` is empty, `false` otherwise
		[[nodiscard]] constexpr inline auto empty() noexcept -> bool {
			return mSize == 0;
		}

		/// @brief Returns the current number of elements in the `RingBuffer`
		///
		/// @return The current number of elements
		[[nodiscard]] constexpr inline auto size() noexcept -> size_t {
			return mSize;
		}

		/// @brief Returns the maximum possible number of elements this `RingBuffer` could store
		/// if grown to maximum possible capacity
		///
		/// @return The maximum possible number of storable elements
		[[nodiscard]] constexpr inline auto max_size() noexcept -> size_t {
			return std::numeric_limits<size_t>::max();
		}

		/// @brief Returns the current capacity of the `RingBuffer`;
		/// the number of elements it can currently store
		///
		/// @return The current capacity
		[[nodiscard]] constexpr inline auto capacity() noexcept -> size_t {
			return mCapacity;
		}

		/// @brief Reserves more storage for the `RingBuffer`. If `newCapacity` is > capacity,
		/// then the capacity of the `RingBuffer` will be extended until at least `newCapacity`
		/// elements can be stored.
		/// @note Memory contiguity is maintained, so no **elements** will be lost or invalidated.
		/// However, all iterators and references to elements will be invalidated.
		///
		/// @param newCapacity - The new capacity of the `RingBuffer`
		constexpr inline auto reserve(size_t newCapacity) noexcept -> void {
			// we only need to do anything if `newCapacity` is actually larger than `mCapacity`
			if(newCapacity > mCapacity) {
				gsl::owner<T*> temp = new T[newCapacity]; // NOLINT
				auto span = gsl::make_span(temp, newCapacity);
				std::copy(begin(), end(), span.begin());
				mBuffer.reset(temp);
				// if there are elements between the start of the vector and
				// the "start" of the `RingBuffer`, we need to move as many as
				// possible to the new space between the old end of the vector and the
				// new end of the vector. We also need to move whatever elements are remaining
				// afterwards to the beginning of the vector.
				//
				// Effectively, we need to rotate elements from the beginning of the vector
				// to the end until contiguity is restored.
				auto oldLoopIndex = mLoopIndex;
				mLoopIndex = newCapacity;
				mCapacity = newCapacity - 1;
				if(mStartIndex > 0 && mWriteIndex < mStartIndex && mWriteIndex != 0) {
					auto index = mWriteIndex - 1;
					mWriteIndex = oldLoopIndex + 1;
					for(auto i = static_cast<int>(index); i >= 0; --i) {
						// clang-format off
						emplace_back(mBuffer[index- static_cast<size_t>(i)]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
						// clang-format on
					}
				}
			}
		}

		/// @brief Erases all elements from the `RingBuffer`
		constexpr inline auto clear() noexcept -> void {
			mStartIndex = 0;
			mWriteIndex = 0;
			mSize = 0;
		}

		/// @brief Inserts the given element at the end of the `RingBuffer`
		/// @note if `size() == capacity()` then this loops and overwrites `front()`
		///
		/// @param value - the element to insert
		constexpr inline auto push_back(const T& value) noexcept -> void requires Copyable<T> {
			// clang-format off
		mBuffer[mWriteIndex] = value; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			// clang-format on

			incrementIndices();
		}

		/// @brief Inserts the given element at the end of the `RingBuffer`
		/// @note if `size() == capacity()` then this loops and overwrites `front()`
		///
		/// @param value - the element to insert
		constexpr inline auto push_back(T&& value) noexcept -> void {
			// clang-format off
			mBuffer[mWriteIndex] = std::forward<T>(value); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			// clang-format on

			incrementIndices();
		}

		/// @brief Constructs the given element in place at the end of the `RingBuffer`
		/// @note if `size() == capacity()` then this loops and overwrites `front()`
		///
		/// @tparam Args - The types of the element's constructor arguments
		/// @param args - The constructor arguments for the element
		///
		/// @return A reference to the element constructed at the end of the `RingBuffer`
		template<typename... Args>
		constexpr inline auto emplace_back(Args&&... args) noexcept -> T& requires Movable<T> {
			// clang-format off
			new (&mBuffer[mWriteIndex]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			// clang-format on

			incrementIndices();

			auto index = mWriteIndex == 0 ? mLoopIndex : mWriteIndex - 1;
			return mBuffer[index]; // NOLINT
		}

		/// @brief Constructs the given element in place at the location
		/// indicated by the `Iterator` `position`
		///
		/// @tparam Args - The types of the element's constructor arguments
		/// @param position - `Iterator` indicating where in the `RingBuffer` to construct the
		/// element
		/// @param args - The constructor arguments for the element
		///
		/// @return A reference to the element constructed at the location indicated by `position`
		template<typename... Args>
		constexpr inline auto emplace(const Iterator& position, Args&&... args) noexcept -> T& {
			auto index = getAdjustedInternalIndex(position.getIndex());

			// clang-format off
			new (&mBuffer[index]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			// clang-format on

			return mBuffer[index]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		/// @brief Constructs the given element in place at the location
		/// indicated by the `ConstIterator` `position`
		///
		/// @tparam Args - The types of the element's constructor arguments
		/// @param position - `ConstIterator` indicating where in the `RingBuffer` to construct the
		/// element
		/// @param args - The constructor arguments for the element
		///
		/// @return A reference to the element constructed at the location indicated by `position`
		template<typename... Args>
		constexpr inline auto
		emplace(const ConstIterator& position, Args&&... args) noexcept -> T& {
			auto index = getAdjustedInternalIndex(position.getIndex());

			// clang-format off
			new (&mBuffer[index]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			// clang-format on

			return mBuffer[index]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		/// @brief Assigns the given element to the position indicated
		/// by the `Iterator` `position`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param position - `Iterator` indicating where in the `RingBuffer` to place the element
		/// @param element - The element to store in the `RingBuffer`
		constexpr inline auto
		insert(const Iterator& position, const T& element) noexcept -> void requires Copyable<T> {
			insert_internal(position.getIndex(), element);
		}

		/// @brief Assigns the given element to the position indicated
		/// by the `Iterator` `position`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param position - `Iterator` indicating where in the `RingBuffer` to place the element
		/// @param element - The element to store in the `RingBuffer`
		constexpr inline auto insert(const Iterator& position, T&& element) noexcept -> void {
			insert_internal(position.getIndex(), std::forward<T>(element));
		}

		/// @brief Assigns the given element to the position indicated
		/// by the `ConstIterator` `position`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param position - `ConstIterator` indicating where in the `RingBuffer` to place the
		/// element
		/// @param element - The element to store in the `RingBuffer`
		constexpr inline auto insert(const ConstIterator& position, const T& element) noexcept
			-> void requires Copyable<T> {
			insert_internal(position.getIndex(), element);
		}

		/// @brief Assigns the given element to the position indicated
		/// by the `ConstIterator` `position`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param position - `ConstIterator` indicating where in the `RingBuffer` to place the
		/// element
		/// @param element - The element to store in the `RingBuffer`
		constexpr inline auto insert(const ConstIterator& position, T&& element) noexcept -> void {
			insert_internal(position.getIndex(), std::forward<T>(element));
		}

		/// @brief Constructs the given element at the insertion position indicated
		/// by the `ConstIterator` `position`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param position - `ConstIterator` indicating where in the `RingBuffer` to place the
		/// element
		/// @param element - The element to store in the `RingBuffer`
		template<typename... Args>
		constexpr inline auto
		insert_emplace(const Iterator& position, Args&&... args) noexcept -> T& {
			return insert_emplace_internal(position.getIndex(), args...);
		}

		/// @brief Constructs the given element at the insertion position indicated
		/// by the `ConstIterator` `position`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param position - `ConstIterator` indicating where in the `RingBuffer` to place the
		/// element
		/// @param element - The element to store in the `RingBuffer`
		template<typename... Args>
		constexpr inline auto
		insert_emplace(const ConstIterator& position, Args&&... args) noexcept -> T& {
			return insert_emplace_internal(position.getIndex(), args...);
		}

		/// @brief Erases the element at the given `position`, moving other elements backward
		/// in the buffer to maintain contiguity
		///
		/// @param position - The element to erase
		///
		/// @return `Iterator` pointing to the element after the one erased
		constexpr inline auto erase(const Iterator& position) noexcept -> Iterator {
			return erase_internal(position.getIndex());
		}

		/// @brief Erases the element at the given `position`, moving other elements backward
		/// in the buffer to maintain contiguity
		///
		/// @param position - The element to erase
		///
		/// @return `Iterator` pointing to the element after the one erased
		constexpr inline auto erase(const ConstIterator& position) noexcept -> Iterator {
			return erase_internal(position.getIndex());
		}

		/// @brief Erases the range of elements in [`first`, `last`)
		/// Returns an `Iterator` to the element after the last one erased
		/// @note In the case `first` >= `last`, no elements are erased and `last` is returned;
		///
		/// @param first - The first element in the range to erase
		/// @param last - The last element in the range
		///
		/// @return `Iterator` pointing to the element after the last one erased
		constexpr inline auto
		erase(const Iterator& first, const Iterator& last) noexcept -> Iterator {
			if(first >= last) {
				return last;
			}

			return erase_internal(first.getIndex(), last.getIndex());
		}

		/// @brief Erases the range of elements in [`first`, `last`)
		/// Returns an `Iterator` to the element after the last one erased
		/// @note In the case `first` >= `last`, no elements are erased and `last` is returned;
		///
		/// @param first - The first element in the range to erase
		/// @param last - The last element in the range
		///
		/// @return `Iterator` pointing to the element after the last one erased
		constexpr inline auto
		erase(const ConstIterator& first, const ConstIterator& last) noexcept -> Iterator {
			if(first >= last) {
				return last;
			}

			return erase_internal(first.getIndex(), last.getIndex());
		}

		/// @brief Removes the last element in the `RingBuffer` and returns it
		///
		/// @return The last element in the `RingBuffer`
		[[nodiscard]] constexpr inline auto pop_back() noexcept -> T requires Copyable<T> {
			T _back = back();
			erase(--end());
			return _back;
		}

		/// @brief Returns a Random Access Bidirectional iterator over the `RingBuffer`,
		/// at the beginning
		///
		/// @return The iterator, at the beginning
		[[nodiscard]] constexpr inline auto begin() -> Iterator {
			T* p = &mBuffer[mStartIndex]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

			return Iterator(p, this, 0ULL);
		}

		/// @brief Returns a Random Access Bidirectional iterator over the `RingBuffer`,
		/// at the end
		///
		/// @return The iterator, at the end
		[[nodiscard]] constexpr inline auto end() -> Iterator {
			T* p = &mBuffer[mWriteIndex]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

			return Iterator(p, this, mSize);
		}

		/// @brief Returns a Random Access Bidirectional read-only iterator over the `RingBuffer`,
		/// at the beginning
		///
		/// @return The iterator, at the beginning
		[[nodiscard]] constexpr inline auto cbegin() -> ConstIterator {
			T* p = &mBuffer[mStartIndex]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

			return ConstIterator(p, this, 0ULL);
		}

		/// @brief Returns a Random Access Bidirectional read-only iterator over the `RingBuffer`,
		/// at the end
		///
		/// @return The iterator, at the end
		[[nodiscard]] constexpr inline auto cend() -> ConstIterator {
			T* p = &mBuffer[mWriteIndex]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			return ConstIterator(p, this, mSize);
		}

		/// @brief Unchecked access-by-index operator
		///
		/// @param index - The index to get the corresponding element for
		///
		/// @return - The element at index
		[[nodiscard]] constexpr inline auto operator[](Integral auto index) noexcept -> T& {
			auto i = getAdjustedInternalIndex(index);

			return mBuffer[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		constexpr auto
		operator=(const RingBuffer& buffer) noexcept -> RingBuffer& requires Copyable<T> {
			if(this == &buffer) {
				return *this;
			}
			mBuffer = new T[buffer.mCapacity + 1]; // NOLINT
			mStartIndex = 0;
			mWriteIndex = 0;
			mCapacity = buffer.mCapacity;
			for(auto i = 0; i < mCapacity; ++i) {
				push_back(buffer[i]);
			}
			mStartIndex = buffer.mStartIndex;
			mWriteIndex = buffer.mWriteIndex;
			mLoopIndex = buffer.mLoopIndex;
			mSize = buffer.mSize;
			return *this;
		}
		constexpr auto operator=(RingBuffer&& buffer) noexcept -> RingBuffer& {
			mBuffer = std::move(buffer.mBuffer);
			mWriteIndex = buffer.mWriteIndex;
			mStartIndex = buffer.mStartIndex;
			mLoopIndex = buffer.mLoopIndex;
			mCapacity = buffer.mCapacity;
			mSize = buffer.mSize;
			buffer.mBuffer = nullptr;
			buffer.mWriteIndex = 0ULL;
			buffer.mStartIndex = 0ULL;
			buffer.mCapacity = 0ULL;
			buffer.mSize = 0ULL;
			return *this;
		}

	  private:
		static const constexpr size_t DEFAULT_CAPACITY_INTERNAL = DEFAULT_CAPACITY + 1;
		std::unique_ptr<T[]> mBuffer = new T[DEFAULT_CAPACITY_INTERNAL]; // NOLINT
		size_t mWriteIndex = 0;
		size_t mStartIndex = 0;
		size_t mLoopIndex = DEFAULT_CAPACITY;
		size_t mCapacity = DEFAULT_CAPACITY;
		size_t mSize = 0;

		/// @brief Converts the given `RingBuffer` index into the corresponding index into then
		/// underlying `T` array
		///
		/// @param index - The `RingBuffer` index to convert
		///
		/// @return The corresponding index into the underlying `T` array
		[[nodiscard]] constexpr inline auto
		getAdjustedInternalIndex(Integral auto index) const noexcept -> size_t {
			auto i = static_cast<size_t>(index);
			if(mStartIndex + i > mLoopIndex) {
				i = (mStartIndex + i) - (mLoopIndex + 1);
			}
			else {
				i += mStartIndex;
			}

			return i;
		}

		/// @brief Converts the given index into the underlying `T` array into
		/// a using facing index into the `RingBuffer`
		///
		/// @param index - The internal index
		///
		/// @return The corresponding user-facing index
		[[nodiscard]] constexpr inline auto
		getExternalIndexFromInternal(Integral auto index) const noexcept -> size_t {
			auto i = static_cast<size_t>(index);
			if(i > mStartIndex && i <= mLoopIndex) {
				return i - mStartIndex;
			}
			else if(i < mStartIndex) {
				return (mLoopIndex - mStartIndex) + i + 1;
			}
			else if(i == mStartIndex) {
				return 0;
			}
			else {
				return mSize;
			}
		}

		/// @brief Increments the start and write indices into the underlying `T` array,
		/// and the size property, maintaining the logical `RingBuffer` structure
		constexpr inline auto incrementIndices() noexcept -> void {
			mWriteIndex++;
			mSize = math::General<size_t>::min(mSize + 1, mCapacity);

			// if write index is at start - 1, we need to push start forward to maintain
			// the "invalid" spacer element for this.end()
			if(mWriteIndex > mLoopIndex && mStartIndex == 0) {
				mWriteIndex = 0;
				mStartIndex = 1;
			}
			else if(mWriteIndex == mStartIndex) {
				mStartIndex++;
				if(mStartIndex > mLoopIndex) {
					mStartIndex = 0;
				}
			}
		}

		/// @brief Inserts the given element at the position indicated
		/// by the `externalIndex`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param externalIndex - The user-facing index into the `RingBuffer` to insert the element
		/// at
		/// @param elem - The element to store in the `RingBuffer`
		constexpr inline auto
		insert_internal(size_t externalIndex, const T& elem) noexcept -> void requires Movable<T> {
			const auto index = getAdjustedInternalIndex(externalIndex);

			if(index == mWriteIndex) {
				emplace_back(elem);
			}
			else {
				auto numToMove = mSize - externalIndex;
				auto iter = begin() + externalIndex;
				if(mSize == mCapacity) [[likely]] { // NOLINT
					for(size_t i = 1ULL, j = numToMove - 2; i < numToMove; ++i, --j) {
						*((end() - i) + 1) = std::move(*(iter + j));
					}
				}
				else {
					for(size_t i = 0ULL, j = numToMove - 1; i < numToMove; ++i, --j) {
						*(end() - i) = std::move(*(iter + j));
					}
				}
				incrementIndices();
				if(externalIndex == 0) {
					// clang-format off
					mBuffer[mStartIndex] = elem; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
				else {
					// clang-format off
					mBuffer[index] = elem; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
			}
		}

		/// @brief Inserts the given element at the position indicated
		/// by the `externalIndex`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param externalIndex - The user-facing index into the `RingBuffer` to insert the element
		/// at
		/// @param elem - The element to store in the `RingBuffer`
		constexpr inline auto insert_internal(size_t externalIndex, const T& elem) noexcept
			-> void requires NotMovable<T> {
			const auto index = getAdjustedInternalIndex(externalIndex);

			if(index == mWriteIndex) {
				emplace_back(elem);
			}
			else {
				auto numToMove = mSize - externalIndex;
				auto iter = begin() + externalIndex;
				if(mSize == mCapacity) [[likely]] { // NOLINT
					for(size_t i = 1ULL, j = numToMove - 2; i < numToMove; ++i, --j) {
						*((end() - i) + 1) = *(iter + j);
					}
				}
				else {
					for(size_t i = 0ULL, j = numToMove - 1; i < numToMove; ++i, --j) {
						*(end() - i) = *(iter + j);
					}
				}
				incrementIndices();
				if(externalIndex == 0) {
					// clang-format off
					mBuffer[mStartIndex] = elem; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
				else {
					// clang-format off
					mBuffer[index] = elem; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
			}
		}

		/// @brief Inserts the given element at the position indicated
		/// by the `externalIndex`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param externalIndex - The user-facing index into the `RingBuffer` to insert the element
		/// at
		/// @param elem - The element to store in the `RingBuffer`
		constexpr inline auto
		insert_internal(size_t externalIndex, T&& elem) noexcept -> void requires Movable<T> {
			const auto index = getAdjustedInternalIndex(externalIndex);

			if(index == mWriteIndex) {
				emplace_back(elem);
			}
			else {
				auto numToMove = mSize - externalIndex;
				auto iter = begin() + externalIndex;
				if(mSize == mCapacity) [[likely]] { // NOLINT
					for(size_t i = 1ULL, j = numToMove - 2; i < numToMove; ++i, --j) {
						*((end() - i) + 1) = std::move(*(iter + j));
					}
				}
				else {
					for(size_t i = 0ULL, j = numToMove - 1; i < numToMove; ++i, --j) {
						*(end() - i) = std::move(*(iter + j));
					}
				}
				incrementIndices();
				if(externalIndex == 0) {
					// clang-format off
					mBuffer[mStartIndex] = std::move(elem); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
				else {
					// clang-format off
					mBuffer[index] = std::move(elem); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
			}
		}

		/// @brief Inserts the given element at the position indicated
		/// by the `externalIndex`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param externalIndex - The user-facing index into the `RingBuffer` to insert the element
		/// at
		/// @param elem - The element to store in the `RingBuffer`
		constexpr inline auto
		insert_internal(size_t externalIndex, T&& elem) noexcept -> void requires NotMovable<T> {
			const auto index = getAdjustedInternalIndex(externalIndex);

			if(index == mWriteIndex) {
				emplace_back(elem);
			}
			else {
				auto numToMove = mSize - externalIndex;
				auto iter = begin() + externalIndex;
				if(mSize == mCapacity) [[likely]] { // NOLINT
					for(size_t i = 1ULL, j = numToMove - 2; i < numToMove; ++i, --j) {
						*((end() - i) + 1) = *(iter + j);
					}
				}
				else {
					for(size_t i = 0ULL, j = numToMove - 1; i < numToMove; ++i, --j) {
						*(end() - i) = *(iter + j);
					}
				}
				incrementIndices();
				if(externalIndex == 0) {
					// clang-format off
					mBuffer[mStartIndex] = elem; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
				else {
					// clang-format off
					mBuffer[index] = elem; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
			}
		}

		/// @brief Constructs the given element at the insertion position indicated
		/// by the `externalIndex`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param externalIndex - The user-facing index into the `RingBuffer` to insert the element
		/// at
		/// @param elem - The element to store in the `RingBuffer`
		template<typename... Args>
		constexpr inline auto insert_emplace_internal(size_t externalIndex, Args&&... args) noexcept
			-> T& requires Movable<T> {
			const auto index = getAdjustedInternalIndex(externalIndex);

			if(index == mWriteIndex) {
				return emplace_back(args...);
			}
			else {
				auto numToMove = mSize - externalIndex;
				auto iter = begin() + externalIndex;
				if(mSize == mCapacity) [[likely]] { // NOLINT
					for(size_t i = 1ULL, j = numToMove - 2; i < numToMove; ++i, --j) {
						*((end() - i) + 1) = std::move(*(iter + j));
					}
				}
				else {
					for(size_t i = 0ULL, j = numToMove - 1; i < numToMove; ++i, --j) {
						*(end() - i) = std::move(*(iter + j));
					}
				}
				incrementIndices();
				if(externalIndex == 0) {
					// clang-format off
					new (&mBuffer[mStartIndex]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					return mBuffer[mStartIndex];// NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
				else {
					// clang-format off
					new(&mBuffer[index]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					return mBuffer[index];// NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
			}
		}

		/// @brief Constructs the given element at the insertion position indicated
		/// by the `externalIndex`
		/// @note if `size() == capacity()` this drops the last element out of the `RingBuffer`
		///
		/// @param externalIndex - The user-facing index into the `RingBuffer` to insert the element
		/// at
		/// @param elem - The element to store in the `RingBuffer`
		template<typename... Args>
		constexpr inline auto insert_emplace_internal(size_t externalIndex, Args&&... args) noexcept
			-> T& requires NotMovable<T> {
			const auto index = getAdjustedInternalIndex(externalIndex);

			if(index == mWriteIndex) {
				return emplace_back(args...);
			}
			else {
				auto numToMove = mSize - externalIndex;
				auto iter = begin() + externalIndex;
				if(mSize == mCapacity) [[likely]] { // NOLINT
					for(size_t i = 1ULL, j = numToMove - 2; i < numToMove; ++i, --j) {
						*((end() - i) + 1) = *(iter + j);
					}
				}
				else {
					for(size_t i = 0ULL, j = numToMove - 1; i < numToMove; ++i, --j) {
						*(end() - i) = *(iter + j);
					}
				}
				incrementIndices();
				if(externalIndex == 0) {
					// clang-format off
					new (&mBuffer[mStartIndex]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					return mBuffer[mStartIndex];// NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
				else {
					// clang-format off
					new(&mBuffer[index]) T(args...); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					return mBuffer[index];// NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}
			}
		}

		/// @brief Erases the element at the given index, returning an `Iterator` to the element
		/// after the removed one
		///
		/// @param index - The index to the element to remove. This should be a `RingBuffer` index:
		/// IE, not an interal one into the `T` array
		///
		/// @return `Iterator` pointing to the element after the one removed
		[[nodiscard]] constexpr inline auto erase_internal(size_t index) noexcept -> Iterator {
			auto indexInternal = getAdjustedInternalIndex(index);
			if(indexInternal == mWriteIndex) [[unlikely]] { // NOLINT
				return end();
			}
			else {
				auto numToMove = (mSize - 1) - index;
				mWriteIndex = indexInternal;
				mSize -= numToMove + 1;
				auto posToMove = index + 1;
				for(auto i = 0ULL; i < numToMove; ++i) {
					// clang-format off
					emplace_back(mBuffer[getAdjustedInternalIndex(posToMove + i)]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}

				auto iter = begin() + getExternalIndexFromInternal(mWriteIndex);
				return iter;
			}
		}

		/// @brief Erases the range of elements in [`first`, `last`)
		/// Returns an `Iterator` to the element after the last one erased
		///
		/// @param first - The first index in the range to erase. This should be a `RingBuffer`
		/// index: IE, not an internal one into the `T` array
		/// @param last - The last index` in the range to erase. This should be a `RingBuffer`
		/// index: IE, not an internal one into the `T` array
		///
		/// @return `Iterator` pointing to the element after the last one erased
		[[nodiscard]] constexpr inline auto
		erase_internal(size_t first, size_t last) noexcept -> Iterator {
			auto firstInternal = getAdjustedInternalIndex(first);
			auto lastInternal = getAdjustedInternalIndex(last);
			if(lastInternal == mWriteIndex) {
				if(mWriteIndex == mLoopIndex) {
					mStartIndex--;
					mWriteIndex -= last - first;
				}
				else if(mWriteIndex < mStartIndex) {
					auto numToRemove = (last - first);
					auto numBeforeZero = numToRemove - mWriteIndex;
					auto numAfterZero = numToRemove - numBeforeZero;
					mStartIndex -= numBeforeZero;
					if(numAfterZero > 0) {
						mWriteIndex = mLoopIndex;
						numAfterZero--;
						mWriteIndex -= numAfterZero;
					}
					else {
						mWriteIndex -= numBeforeZero;
					}
				}
				else if(mWriteIndex > mStartIndex) {
					auto numToRemove = (last - first);
					mWriteIndex -= numToRemove;
				}
				mSize -= last - first;
				return end();
			}
			else {
				auto numToMove = mSize - last;
				mWriteIndex = firstInternal;
				mSize -= numToMove + (last - first);
				auto posToMove = last;
				for(auto i = 0ULL; i < numToMove; ++i) {
					// clang-format off
					emplace_back(mBuffer[getAdjustedInternalIndex(posToMove + i)]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
					// clang-format on
				}

				auto iter = begin() + first;
				return iter;
			}
		}
	};

} // namespace apex::utils
