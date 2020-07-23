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
			explicit Random(size_t seed) noexcept;

			/// @brief Seeds this random number generator with the given value
			///
			/// @param seed - The seed to use
			void srand(size_t seed) noexcept;

			/// @brief Generates a random number
			///
			/// @return - The random number
			auto rand() noexcept -> size_t;

			///The maximum value returnable by `Random::rand()`
			static const constexpr size_t MAX = std::numeric_limits<size_t>::max();
		private:
			size_t seed1 = 12345;
			size_t seed2 = 12345;
			size_t seed3 = 12345;
			size_t seed4 = 12345;
	};
} //namespace apex::math
