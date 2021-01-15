#pragma once

#include <type_traits>

#include "../math/Exponentials.h"
#include "../math/General.h"

namespace apex::dsp::waveshapers {
	using math::Exponentials;
	using math::General;

	/// @brief Calculates the resulting value from applying soft saturation to the input
	///
	/// @param input - The input to apply saturation to
	/// @param amount - The amount of saturation
	/// @param slope - The slope of the saturation
	/// @return - The saturated value
	template<typename FloatType = float,
			 typename Enable = std::enable_if_t<std::is_floating_point_v<FloatType>, bool>>
	[[nodiscard]] inline static constexpr auto
	softSaturation(FloatType input,
				   FloatType amount = static_cast<FloatType>(1.0),
				   FloatType slope = static_cast<FloatType>(0.4)) noexcept -> FloatType {
		return input * (static_cast<FloatType>(1.0) + amount)
			   / (static_cast<FloatType>(1.0)
				  + amount * Exponentials<FloatType>::pow(General<FloatType>::abs(input), slope));
	}

	/// @brief Calculates the resulting value from applying soft clipping to the input
	///
	/// @param input - The input to apply soft clipping to
	/// @param amount - The amount of clipping
	/// @return - The clipped value
	template<typename FloatType = float,
			 typename Enable = std::enable_if_t<std::is_floating_point_v<FloatType>, bool>>
	[[nodiscard]] inline static constexpr auto
	softClip(FloatType input, FloatType amount = static_cast<FloatType>(1.0)) noexcept
		-> FloatType {
		return input * (static_cast<FloatType>(1.0) + amount)
			   / (static_cast<FloatType>(1.0) + amount * General<FloatType>::abs(input));
	}

	/// @brief Calculates the resulting value from applying hard clipping to the input
	///
	/// @param input - The input to apply clipping to
	/// @param clipLevel - The hard clip level, in the linear domain
	/// @param amount  - The amount of clipping
	/// @return - The clipped value
	template<typename FloatType = float,
			 typename Enable = std::enable_if_t<std::is_floating_point_v<FloatType>, bool>>
	[[nodiscard]] inline static constexpr auto
	hardClip(FloatType input,
			 FloatType clipLevel,
			 FloatType amount = static_cast<FloatType>(1.0)) noexcept -> FloatType {
		auto x = (input * (static_cast<FloatType>(1.0) + amount)
				  / (static_cast<FloatType>(1.0) + amount * General<FloatType>::abs(input)));
		auto signX = x / General<FloatType>::abs(x);
		return (General<FloatType>::abs(x) > clipLevel ? clipLevel * signX : x);
	}
} // namespace apex::dsp::waveshapers
