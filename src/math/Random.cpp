#include "Random.h"

namespace apex::math {

	/// @brief Constructs a `Random` pre-seeded with the given value
	///
	/// @param seed - The seed to use
	Random::Random(size_t seed) noexcept {
		srand(seed);
	}

	/// @brief Seeds this random number generator with the given value
	///
	/// @param seed - The seed to use
	inline void Random::srand(size_t seed) noexcept {
		seed1 = seed + 1;
		seed2 = seed + 7;
		seed3 = seed + 15;
		seed4 = seed + 127;
	}

	/// @brief Generates a random number
	///
	/// @return - The random number
	inline auto Random::rand() noexcept -> size_t {
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
} // namespace apex::math
