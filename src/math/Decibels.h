#pragma once

#include "Exponentials.h"
namespace apex::math {
	class Decibels {
		public:
			/// @brief Converts the given linear value to the corresponding Decibel value 
			/// 
			/// @param linear - The linear value to convert
			/// @return - The Decibel value
			static inline constexpr auto linearToDecibels(float linear) noexcept -> float {
				return 20.0F * log10f(linear);
			}

			/// @brief Converts the given Decibel value to the corresponding linear value
			/// 
			/// @param decibels - The Decibel value to convert
			/// @return - The linear value
			static inline constexpr auto decibelsToLinear(float decibels) noexcept -> float {
				return pow10f(decibels / 20.0F);
			}

			/// @brief Converts the given linear value to the corresponding Decibel value 
			/// 
			/// @param linear - The linear value to convert
			/// @return - The Decibel value
			static inline constexpr auto linearToDecibels(double linear) noexcept -> double {
				return 20.0 * log10(linear);
			}

			/// @brief Converts the given Decibel value to the corresponding linear value
			/// 
			/// @param decibels - The Decibel value to convert
			/// @return - The linear value
			static inline constexpr auto decibelsToLinear(double decibels) noexcept -> double {
				return pow10(decibels / 20.0);
			}
	};
} //namespace apex::math
