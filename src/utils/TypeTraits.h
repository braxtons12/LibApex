#pragma once

#include <type_traits>

#include "Error.h"

namespace apex::utils::type_traits {
	template<typename T>
	struct is_copy_move_or_pointer
		: std::bool_constant<std::is_copy_constructible_v<
								 T> || std::is_move_constructible_v<T> || std::is_pointer_v<T>> { };

	template<typename T>
	constexpr auto is_copy_move_or_pointer_v = is_copy_move_or_pointer<T>::value;

	template<typename T>
	struct is_copy_or_move
		: std::bool_constant<std::is_copy_assignable_v<T> || std::is_move_constructible_v<T>> { };

	template<typename T>
	constexpr auto is_copy_or_move_v = is_copy_or_move<T>::value;

	template<typename E>
	struct is_error_type
		: std::bool_constant<
			  std::is_base_of_v<Error, std::remove_reference_t<std::remove_pointer_t<E>>>> { };

	template<typename E>
	constexpr auto is_error_type_v = is_error_type<E>::value;

} // namespace apex::utils::type_traits
