#pragma once

#include <array>
#include <cstdint>
#include <gsl/gsl>
#include <gsl/span>

#ifndef APEX_SPAN
	#define APEX_SPAN
namespace apex::utils {
	#ifndef _MSC_VER
	using std::size_t;
	#endif // _MSC_VER

	/// @brief Thin wrapper around `gsl::span`
	///
	/// @tparam T - The type contained in the `Span`
	/// @tparam Size - The number of elements for constexpr sizes, or gsl::dynamic_extent (default)
	/// for run-time sizes
	template<typename T, size_t Size = gsl::dynamic_extent>
	class Span {
	  public:
		using Iterator = typename gsl::span<T, Size>::iterator;
		using ReverseIterator = typename gsl::span<T, Size>::reverse_iterator;
		template<size_t Offset, size_t Count>
		using SubSpan = typename gsl::details::calculate_subspan_type<T, Size, Offset, Count>::type;

		Span() noexcept = default;

		/// @brief Constructs a `Span` from a `gsl::span`
		///
		/// @param span - The `gsl::span` to wrap
		explicit constexpr Span(gsl::span<T, Size> span) noexcept : mSpanInternal(span) {
		}

		/// @brief Copy constructs a `Span` from the given one
		///
		/// @param span - The `Span` to copy
		constexpr Span(const Span<T, Size>& span) noexcept = default;

		/// @brief Move constructs the given `Span`
		///
		/// @param span - The `Span` to move
		constexpr Span(Span<T, Size>&& span) noexcept = default;
		~Span() noexcept = default;

		/// @brief Returns the element at the given `index`
		///
		/// @param index - The index of the desired element
		/// @return - The element at `index`
		[[nodiscard]] constexpr inline auto at(size_t index) noexcept -> T& {
			return gsl::at(mSpanInternal, static_cast<gsl::index>(index));
		}

		/// @brief Returns the first `Count` elements in the `Span`
		///
		/// @tparam Count - The number of elements to get
		/// @tparam std::enable_if_t<Size != gsl::dynamic_extent>
		/// @return - The first `Count` elements in the `Span`, as a `Span`
		template<size_t Count,
				 size_t size = Size,
				 std::enable_if_t<size != gsl::dynamic_extent, int> = 0>
		[[nodiscard]] constexpr inline auto first() const noexcept -> Span<T, Count> {
			return Span(mSpanInternal.template first<Count>());
		}

		/// @brief Returns the first `count` elements in the `Span`
		///
		/// @tparam size - The size of the `Span`, this should always be left defaulted
		/// @tparam std::enable_if_t<Size == gsl::dynamic_extent>
		/// @param count - The number of elements to get
		/// @return - The first `count` elements in the `Span`, as a `Span`
		template<size_t size = Size, std::enable_if_t<size == gsl::dynamic_extent, int> = 0>
		[[nodiscard]] constexpr inline auto first(size_t count) const noexcept -> Span<T, Size> {
			return Span(mSpanInternal.first(count));
		}

		/// @brief Returns the last `Count` elements in the `Span`
		///
		/// @tparam Count - The number of elements to get
		/// @tparam std::enable_if_t<Size != gsl::dynamic_extent>
		/// @return - The last `Count` elements in the `Span`, as a `Span`
		template<size_t Count,
				 size_t size = Size,
				 std::enable_if_t<size != gsl::dynamic_extent, int> = 0>
		[[nodiscard]] constexpr inline auto last() const noexcept -> Span<T, Count> {
			return Span(mSpanInternal.template last<Count>());
		}

		/// @brief Returns the last `count` elements in the `Span`
		///
		/// @tparam Size - The size of the `Span`, this should always be left defaulted
		/// @tparam std::enable_if_t<Size == gsl::dynamic_extent>
		/// @param count - The number of elements to get
		/// @return- The last `count` elements in the `Span` as a `Span`
		template<size_t size = Size, std::enable_if_t<size == gsl::dynamic_extent, int> = 0>
		[[nodiscard]] constexpr inline auto last(size_t count) const noexcept -> Span<T, Size> {
			return Span(mSpanInternal.last(count));
		}

		/// @brief Returns a subspan starting at the given `Offset` with `Count` elements
		///
		/// @tparam Offset - The offset to start the subspan at
		/// @tparam Count - The number of elements to get in the subspan
		/// @return - The subspan starting at `Offset` of size `Count`
		template<size_t Offset, size_t Count = gsl::dynamic_extent>
		[[nodiscard]] constexpr inline auto subspan() const noexcept -> SubSpan<Offset, Count> {
			return Span(mSpanInternal.template subspan<Offset, Count>());
		}

		/// @brief Returns a subspan starting at the given `offset` with `count` elements
		///
		/// @param offset - The offset to start the subspan at
		/// @param count - The number of elements to get in the subspan
		/// @return - The subspan starting at `offset` of size `count`
		[[nodiscard]] constexpr inline auto
		subspan(size_t offset, size_t count = gsl::dynamic_extent) const noexcept
			-> Span<T, gsl::dynamic_extent> {
			return Span(mSpanInternal.subspan(offset, count));
		}

		/// @brief Returns the number of elements in the `Span`
		///
		/// @return - The number of elements in the `Span`
		[[nodiscard]] constexpr inline auto size() const noexcept -> size_t {
			return mSpanInternal.size();
		}

		/// @brief Returns the size of the `Span` in bytes
		///
		/// @return - The size of the `Span` in bytes
		[[nodiscard]] constexpr inline auto sizeBytes() const noexcept -> size_t {
			return mSpanInternal.size_bytes();
		}

		/// @brief Returns whether the `Span` is empty
		///
		/// @return - If the `Span` is empty
		[[nodiscard]] constexpr inline auto isEmpty() const noexcept -> bool {
			return size() == 0;
		}

		/// @brief A pointer to the data contained in the `Span`
		///
		/// @return - A pointer to the data
		[[nodiscard]] constexpr inline auto data() const noexcept -> T* {
			return mSpanInternal.data();
		}

		/// @brief Returns the first element in the `Span`
		///
		/// @return - The first element
		[[nodiscard]] constexpr inline auto front() const noexcept -> T& {
			return mSpanInternal.front();
		}

		/// @brief Returns the last element in the `Span`
		///
		/// @return - The last element
		[[nodiscard]] constexpr inline auto back() const noexcept -> T& {
			return mSpanInternal.back();
		}

		[[nodiscard]] constexpr inline auto begin() noexcept -> Iterator {
			return mSpanInternal.begin();
		}

		[[nodiscard]] constexpr inline auto begin() const noexcept -> Iterator {
			return mSpanInternal.begin();
		}

		[[nodiscard]] constexpr inline auto end() noexcept -> Iterator {
			return mSpanInternal.end();
		}

		[[nodiscard]] constexpr inline auto end() const noexcept -> Iterator {
			return mSpanInternal.end();
		}

		[[nodiscard]] constexpr inline auto beginReverse() noexcept -> ReverseIterator {
			return mSpanInternal.rbegin();
		}

		[[nodiscard]] constexpr inline auto beginReverse() const noexcept -> ReverseIterator {
			return mSpanInternal.rbegin();
		}

		[[nodiscard]] constexpr inline auto endReverse() noexcept -> ReverseIterator {
			return mSpanInternal.rend();
		}

		[[nodiscard]] constexpr inline auto endReverse() const noexcept -> ReverseIterator {
			return mSpanInternal.rend();
		}

	#ifdef _MSC_VER
		[[nodiscard]] constexpr inline auto _Unchecked_begin() const noexcept -> T* {
			return mSpanInternal._Unchecked_begin();
		}

		[[nodiscard]] constexpr inline auto _Unchecked_end() const noexcept -> T* {
			return mSpanInternal._Unchecked_end();
		}
	#endif // _MSC_VER

		[[nodiscard]] constexpr static inline auto
		MakeSpan(T* ptr, typename gsl::span<T>::size_type size) noexcept -> Span<T> {
			return Span(gsl::make_span(ptr, size));
		}

		[[nodiscard]] constexpr static inline auto MakeSpan(T* first, T* last) noexcept -> Span<T> {
			return Span(gsl::make_span(first, last));
		}

		template<size_t Count>
		[[nodiscard]] constexpr static inline auto
			MakeSpan(T (&array)[Count]) noexcept -> Span<T, Count> { // NOLINT
			return Span(gsl::make_span(array));
		}

		template<typename Container>
		[[nodiscard]] constexpr static inline auto
		MakeSpan(Container& container) noexcept -> Span<typename Container::value_type> {
			return Span(gsl::make_span(container));
		}

		template<typename Container>
		[[nodiscard]] constexpr static inline auto MakeSpan(const Container& container) noexcept
			-> Span<const typename Container::value_type> {
			return Span(gsl::make_span(container));
		}

		template<typename Tptr>
		[[nodiscard]] constexpr static inline auto
		MakeSpan(Tptr& container, size_t size) noexcept -> Span<typename Tptr::element_type> {
			return Span(gsl::make_span(container, size));
		}

		template<typename Tptr>
		[[nodiscard]] constexpr static inline auto
		MakeSpan(Tptr& container) noexcept -> Span<typename Tptr::element_type> {
			return Span(gsl::make_span(container));
		}

		[[nodiscard]] constexpr static inline auto
		MakeSpan(std::array<T, Size> array) noexcept -> Span<T, Size> {
			return Span(gsl::make_span(array));
		}

		[[nodiscard]] constexpr inline auto operator[](size_t index) noexcept -> T& {
			return this->at(index);
		}

		constexpr auto operator=(const Span<T, Size>& span) noexcept -> Span<T, Size>& = default;
		constexpr auto operator=(Span<T, Size>&& span) noexcept -> Span<T, Size>& = default;

	  private:
		gsl::span<T, Size> mSpanInternal = gsl::span<T, Size>();
	};
} // namespace apex::utils
#endif // APEX_SPAN
