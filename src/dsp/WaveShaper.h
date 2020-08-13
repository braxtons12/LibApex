#pragma once

#include "../math/Exponentials.h"
#include "../math/General.h"

namespace apex::dsp::waveshapers {
	/// @brief Calculates the resulting value from applying soft saturation to the input
	///
	/// @param input - The input to apply saturation to
	/// @param amount - The amount of saturation
	/// @param slope - The slope of the saturation
	/// @return - The saturated value
	[[nodiscard]] inline auto
	softSaturation(float input, float amount = 1.0F, float slope = 0.4F) noexcept -> float {
		return input * (1.0F + amount) / (1.0F + amount * math::powf(math::fabsf(input), slope));
	}

	/// @brief Calculates the resulting value from applying soft saturation to the input
	///
	/// @param input - The input to apply saturation to
	/// @param amount - The amount of saturation
	/// @param slope - The slope of the saturation
	/// @return - The saturated value
	[[nodiscard]] inline auto
	softSaturation(double input, double amount = 1.0, double slope = 0.4) noexcept -> double {
		return input * (1.0 + amount) / (1.0 + amount * math::pow(math::fabs(input), slope));
	}

	/// @brief Calculates the resulting value from applying soft clipping to the input
	///
	/// @param input - The input to apply soft clipping to
	/// @param amount - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]] inline auto softClip(float input, float amount = 1.0F) noexcept -> float {
		return input * (1.0F + amount) / (1.0F + amount * math::fabsf(input));
	}

	/// @brief Calculates the resulting value from applying soft clipping to the input
	///
	/// @param input - The input to apply soft clipping to
	/// @param amount - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]] inline auto softClip(double input, double amount = 1.0) noexcept -> double {
		return input * (1.0 + amount) / (1.0 + amount * math::fabs(input));
	}

	/// @brief Calculates the resulting value from applying hard clipping to the input
	///
	/// @param input - The input to apply clipping to
	/// @param clipLevel - The hard clip level, in the linear domain
	/// @param amount  - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]] inline auto
	hardClip(float input, float clipLevel, float amount = 1.0F) noexcept -> float {
		float x = (input * (1.0F + amount) / (1.0F + amount * math::fabsf(input)));
		float signX = x / math::fabsf(x);
		return (math::fabsf(x) > clipLevel ? clipLevel * signX : x);
	}

	/// @brief Calculates the resulting value from applying hard clipping to the input
	///
	/// @param input - The input to apply clipping to
	/// @param clipLevel - The hard clip level, in the linear domain
	/// @param amount  - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]] inline auto
	hardClip(double input, double clipLevel, double amount = 1.0) noexcept -> double {
		double x = (input * (1.0 + amount) / (1.0 + amount * math::fabs(input)));
		double signX = x / math::fabs(x);
		return (math::fabs(x) > clipLevel ? clipLevel * signX : x);
	}
} // namespace apex::dsp::waveshapers
