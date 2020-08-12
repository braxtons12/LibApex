#pragma once

#include <cstdint>
#include <limits>

namespace apex::math {
#ifndef _MSC_VER
	using std::size_t;
#endif //_MSC_VER

	/// @brief Pseudo-Random number generator
	class Random {
	  public:
		Random() noexcept = default;

		/// @brief Constructs a `Random` pre-seeded with the given value
		///
		/// @param seed - The seed to use
		explicit Random(size_t seed) noexcept {
			srand(seed);
		}

		/// @brief Seeds this random number generator with the given value
		///
		/// @param seed - The seed to use
		inline auto srand(size_t seed) noexcept -> void {
			seed1 = seed + 1;
			seed2 = seed + 7;
			seed3 = seed + 15;
			seed4 = seed + 127;
		}

		/// @brief Generates a random number
		///
		/// @return - The random number
		inline auto rand() noexcept -> size_t {
			size_t b = 0;
			b = ((seed1 << 6U) ^ seed1) >> 13U;
			seed1 = ((seed1 & 4294967294U) << 18U) ^ b;
			b = ((seed2 << 2U) ^ seed2) >> 27U;
			seed2 = ((seed2 & 4294967288U) << 2U) ^ b;
			b = ((seed3 << 13U) ^ seed3) >> 21U;
			seed3 = ((seed3 & 4294967280U) << 7U) ^ b;
			b = ((seed4 << 3U) ^ seed4) >> 12U;
			seed4 = ((seed4 & 4294967168U) << 13U) ^ b;
			return (seed1 ^ seed2 ^ seed3 ^ seed4);
		}

		/// The maximum value returnable by `Random::rand()`
		static const constexpr size_t MAX = std::numeric_limits<size_t>::max();

	  private:
		size_t seed1 = 12345;
		size_t seed2 = 12345;
		size_t seed3 = 12345;
		size_t seed4 = 12345;
	};
} // namespace apex::math
