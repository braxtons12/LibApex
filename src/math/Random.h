#pragma once

#include <cstdint>
#include <limits>
using std::size_t;

namespace apex {
	namespace math {

		/// @brief Pseudo-Random number generator
		class Random {
			public:
				/// @brief Seeds this random number generator with the given value
				///
				/// @param seed - The seed to use
				void srand(size_t seed);

				/// @brief Generates a random number
				///
				/// @return - The random number
				size_t rand();

				///The maximum value returnable by `Random::rand()`
				static const constexpr size_t MAX = std::numeric_limits<size_t>::max();
			private:
				size_t seed1 = 12345;
				size_t seed2 = 12345;
				size_t seed3 = 12345;
				size_t seed4 = 12345;
		};
	}
}
