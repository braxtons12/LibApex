#pragma once

#include <type_traits>

#include "Error.h"

namespace apex::utils::type_traits {
	/// @brief Type Trait to determine if `T` is copyable or movable
	///
	/// @tparam T
	template<typename T>
	struct is_copy_or_move
		: std::bool_constant<std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>> {
	};

	/// @brief value of Type Trait  `is_copy_or_move`
	///
	/// @tparam T
	template<typename T>
	constexpr auto is_copy_or_move_v = is_copy_or_move<T>::value;

	/// @brief Type Trait to determine if `T` is copyable, movable, or a pointer type
	///
	/// @tparam T
	template<typename T>
	struct is_copy_move_or_pointer
		: std::bool_constant<is_copy_or_move_v<T> || std::is_pointer_v<T>> { };

	/// @brief value of Type Trait `is_copy_move_or_pointer`
	///
	/// @tparam T
	template<typename T>
	constexpr auto is_copy_move_or_pointer_v = is_copy_move_or_pointer<T>::value;

	/// @brief Type Trait to determine if `T` is an `apex::utils::Error` type
	///
	/// @tparam E
	template<typename E>
	struct is_error_type
		: std::bool_constant<
			  std::is_base_of_v<Error, std::remove_reference_t<std::remove_pointer_t<E>>>> { };

	/// @brief value of Type Trait `is_error_type`
	///
	/// @tparam E
	template<typename E>
	constexpr auto is_error_type_v = is_error_type<E>::value;

	/// @brief Type Trait to determine if `T` has the `!=` operator comparing to type `U`,
	/// where `U` defaults to `T`
	///
	/// @tparam T - The LHS type
	/// @tparam U - The RHS type
	template<typename T, typename U = T, typename = std::void_t<>>
	struct has_not_equal : std::false_type { };

	/// @brief Type Trait to determine if `T` has the `!=` operator comparing to type `U`,
	/// where `U` defaults to `T`
	///
	/// @tparam T - The LHS type
	/// @tparam U - The RHS type
	template<typename T, typename U>
	struct has_not_equal<T, U, std::void_t<decltype(std::declval<T>() != std::declval<U>())>>
		: std::true_type { };

	/// @brief Value of Type Trait `has_not_equal`
	///
	/// @tparam T - The LHS type
	/// @tparam U - The RHS type
	template<typename T, typename U = T>
	static inline constexpr auto has_not_equal_v = has_not_equal<T, U>::value;
	;
} // namespace apex::utils::type_traits
