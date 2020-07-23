#pragma once

namespace apex::dsp::waveshapers {
	/// @brief Calculates the resulting value from applying soft saturation to the input
	///
	/// @param input - The input to apply saturation to
	/// @param amount - The amount of saturation
	/// @param slope - The slope of the saturation
	/// @return - The saturated value
	[[nodiscard]]
		auto softSaturation(float input, float amount = 1.0F, float slope = 0.4F) noexcept -> float;

	/// @brief Calculates the resulting value from applying soft saturation to the input
	///
	/// @param input - The input to apply saturation to
	/// @param amount - The amount of saturation
	/// @param slope - The slope of the saturation
	/// @return - The saturated value
	[[nodiscard]]
		auto softSaturation(double input, double amount = 1.0, double slope = 0.4) noexcept -> double;

	/// @brief Calculates the resulting value from applying soft clipping to the input
	///
	/// @param input - The input to apply soft clipping to
	/// @param amount - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]]
		auto softClip(float input, float amount = 1.0F) noexcept -> float;

	/// @brief Calculates the resulting value from applying soft clipping to the input
	///
	/// @param input - The input to apply soft clipping to
	/// @param amount - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]]
		auto softClip(double input, double amount = 1.0) noexcept -> double;

	/// @brief Calculates the resulting value from applying hard clipping to the input
	///
	/// @param input - The input to apply clipping to
	/// @param clipLevel - The hard clip level, in the linear domain
	/// @param amount  - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]]
		auto hardClip(float input , float clipLevel, float amount = 1.0F) noexcept -> float;

	/// @brief Calculates the resulting value from applying hard clipping to the input
	///
	/// @param input - The input to apply clipping to
	/// @param clipLevel - The hard clip level, in the linear domain
	/// @param amount  - The amount of clipping
	/// @return - The clipped value
	[[nodiscard]]
		auto hardClip(double input, double clipLevel, double amount = 1.0) noexcept -> double;
} //namespace apex::dsp::waveshapers
