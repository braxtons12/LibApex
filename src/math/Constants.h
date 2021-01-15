#pragma once

#include <type_traits>

namespace apex::math {
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Constants {
	  public:
		static const constexpr FloatType pi = static_cast<FloatType>(3.1415926535897932384626433);
		static const constexpr FloatType twoPi = static_cast<FloatType>(2.0) * pi;
		static const constexpr FloatType twoOverPi = static_cast<FloatType>(2.0) / pi;
		static const constexpr FloatType piOver2 = pi / static_cast<FloatType>(2.0);
		static const constexpr FloatType threePiOver2 = static_cast<FloatType>(3.0) * piOver2;
		static const constexpr FloatType fourOverPi = static_cast<FloatType>(4.0) / pi;
		static const constexpr FloatType piOver4 = pi / static_cast<FloatType>(4.0);
		static const constexpr FloatType piOver6 = pi / static_cast<FloatType>(6.0);
		static const constexpr FloatType piOver12 = pi / static_cast<FloatType>(12.0);
	};
} // namespace apex::math
