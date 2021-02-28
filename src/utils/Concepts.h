#pragma once

#include <cmath>
#include <concepts>
#include <type_traits>

#include "TypeTraits.h"

namespace apex::utils::concepts {

	/// @brief Concept for `std::is_pointer_v<T>`
	///
	/// @tparam T
	template<typename T>
	concept Pointer = std::is_pointer_v<T>;

	/// @brief Concept for `! std::is_pointer_v<T>`
	///
	/// @tparam T
	template<typename T>
	concept NotPointer = !Pointer<T>;

	/// @brief Concept for `std::is_reference_v<T>`
	///
	/// @tparam T
	template<typename T>
	concept Reference = std::is_reference_v<T>;

	/// @brief Concept for `! std::is_reference_v<T>`
	///
	/// @tparam T
	template<typename T>
	concept NotReference = !Reference<T>;

	/// @brief Alias for `std::movable<T>`
	///
	/// @tparam T
	template<typename T>
	concept Movable = std::movable<T>;

	/// @brief Alias for `! std::movable<T>`
	///
	/// @tparam T
	template<typename T>
	concept NotMovable = !Movable<T>;

	/// @brief Alias for `std::copyable<T>`
	///
	/// @tparam T
	template<typename T>
	concept Copyable = std::copyable<T>;

	/// @brief Alias for `! std::copyable<T>`
	///
	/// @tparam T
	template<typename T>
	concept NotCopyable = !Copyable<T>;

	/// @brief Alias for `std::default_initializable<T>`
	///
	/// @tparam T
	template<typename T>
	concept DefaultConstructible = std::default_initializable<T>;

	/// @brief Alias for `! std::default_initializable<T>`
	///
	/// @tparam T
	template<typename T>
	concept NotDefaultConstructible = !DefaultConstructible<T>;

	/// @brief Concept requiring T is an error type (aka T derives from `apex::utils::Error`)
	///
	/// @tparam T
	template<typename T>
	concept ErrorType = utils::type_traits::is_error_type_v<T>;

	/// @brief Concept requiring T is **NOT** an error type (aka T derives from
	/// `apex::utils::Error`)
	///
	/// @tparam T
	template<typename T>
	concept NotErrorType = !ErrorType<T>;

	/// @brief Concept requiring T is copyable or movable
	///
	/// @tparam T
	template<typename T>
	concept CopyOrMovable = Copyable<T> || Movable<T>;

	/// @brief Concept requiring T is **NOT** copyable nor movable
	///
	/// @tparam T
	template<typename T>
	concept NotCopyOrMovable = !CopyOrMovable<T>;

	/// @brief Alias for `std::semiregular<T>`
	///
	/// @tparam T
	template<typename T>
	concept SemiRegular = std::semiregular<T>;

	/// @brief Alias for `! std::semiregular<T>`
	///
	/// @tparam T
	template<typename T>
	concept NotSemiRegular = !SemiRegular<T>;

	/// @brief Concept that is the disjunction of most of the requirements for `std::semiregular`
	/// Requires that that type be at least one of:
	/// * copyable
	/// * movable
	/// * a reference
	/// * a pointer
	///
	/// @tparam T
	template<typename T>
	concept Passable = CopyOrMovable<T> || Reference<T> || Pointer<T>;

	/// @brief Concept that is the negation of `Passable<T>`
	/// Requires that the type is **NONE** of:
	/// * copyable
	/// * movable
	/// * a reference
	/// * a pointer
	///
	/// @tparam T
	template<typename T>
	concept NotPassable = !Passable<T>;

	/// @brief Alias for `std::floating_point<T>`
	///
	/// @tparam T
	template<typename T>
	concept FloatingPoint = std::floating_point<T>;

	/// @brief Alias for `std::integral<T>`
	///
	/// @tparam T
	template<typename T>
	concept Integral = std::integral<T>;

} // namespace apex::utils::concepts
