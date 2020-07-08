#include "Random.h"

namespace apex {
	namespace math {

		/// @brief Seeds this random number generator with the given value
		///
		/// @param seed - The seed to use
		inline void Random::srand(size_t seed) {
			seed1 = seed + 1;
			seed2 = seed + 7;
			seed3 = seed + 15;
			seed4 = seed + 127;
		}

		/// @brief Generates a random number
		///
		/// @return - The random number
		inline size_t Random::rand() {
			size_t b;
			b = ((seed1 << 6) ^ seed1) >> 13;
			seed1 = ((seed1 & 4294967294U) << 18) ^ b;
			b = ((seed2 << 2) ^ seed2) >> 27;
			seed2 = ((seed2 & 4294967288U) << 2) ^ b;
			b = ((seed3 << 13) ^ seed3) >> 21;
			seed3 = ((seed3 & 4294967280U) << 7) ^ b;
			b = ((seed4 << 3) ^ seed4) >> 12;
			seed4 = ((seed4 & 4294967168U) << 13) ^ b;
			return (seed1 ^ seed2 ^ seed3 ^ seed4);
		}
	}
}
