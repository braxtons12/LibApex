#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "../../base/StandardIncludes.h"
#include "EQBand.h"

namespace apex::dsp {
	/// @brief Basic Parallel Equalizer band
	///
	/// @tparam T - The floating point type to back operations
	template<typename T>
	class ParallelEQBand : public EQBand<T> {
	  public:
		static_assert(std::is_floating_point<T>::value,
					  "T must be a floating point type (float or double)");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParallelEQBand)
	};

	/// @brief Basic Parallel Equalizer band
	template<>
	class ParallelEQBand<float> : public EQBand<float> {
	  public:
		/// @brief Creates a default `ParallelEQBand`
		ParallelEQBand() noexcept;

		/// @brief Creates a `ParallelEQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The band type to use
		ParallelEQBand(float frequency,
					   float q,
					   float gainDB,
					   size_t sampleRate,
					   BandType type) noexcept;

		/// @brief Move constructs a `ParallelEQBand` from the given one
		///
		/// @param band - The `ParallelEQBand` to move
		ParallelEQBand(ParallelEQBand<float>&& band) noexcept = default;
		~ParallelEQBand() noexcept override = default;

		/// @brief Sets the gain of this `ParallelEQBand` to the given value
		///
		/// @param gainDB - The new gain, in Decibels
		auto setGainDB(float gainDB) noexcept -> void override;

		/// @brief Returns the gain of this `ParallelEQBand`
		///
		/// @return - The current gain, in Decibels
		[[nodiscard]] auto getGainDB() const noexcept -> float override;

		/// @brief Applies this `ParallelEQBand` to the given input value
		///
		/// @param input - The input value to apply EQ to
		///
		/// @return - The processed value
		[[nodiscard]] auto process(float input) noexcept -> float override;

		/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
		///
		/// @param input - The array of input values to apply EQ to
		auto process(gsl::span<float, gsl::dynamic_extent> input) noexcept -> void override;

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto
		getMagnitudeForFrequency(float frequency) const noexcept -> float override;

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto
		getDecibelMagnitudeForFrequency(float frequency) const noexcept -> float override;

		/// @brief Calculates the linear magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		auto
		getMagnitudesForFrequencies(gsl::span<float, gsl::dynamic_extent> frequencies,
									gsl::span<float, gsl::dynamic_extent> magnitudes) const noexcept
			-> void override;

		/// @brief Calculates the decibel magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		auto getDecibelMagnitudesForFrequencies(
			gsl::span<float, gsl::dynamic_extent> frequencies,
			gsl::span<float, gsl::dynamic_extent> magnitudes) const noexcept -> void override;

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in radians, at the given frequency
		[[nodiscard]] auto getPhaseForFrequency(float frequency) const noexcept -> float override;

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		[[nodiscard]] auto
		getDegreesPhaseForFrequency(float frequency) const noexcept -> float override;

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		auto getPhasesForFrequencies(gsl::span<float, gsl::dynamic_extent> frequencies,
									 gsl::span<float, gsl::dynamic_extent> phases) const noexcept
			-> void override;

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		auto
		getDegreesPhasesForFrequencies(gsl::span<float, gsl::dynamic_extent> frequencies,
									   gsl::span<float, gsl::dynamic_extent> phases) const noexcept
			-> void override;

		auto operator=(ParallelEQBand<float>&& band) noexcept -> ParallelEQBand<float>& = default;

	  protected:
		/// the actual gain, before acounting for changes necessary for EQing in parallel
		float mGainActual = 0.0F;

		/// @brief Creates necessary filter(s) for this EQ band
		auto createFilters() noexcept -> void override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParallelEQBand)
	};

	/// @brief Basic Parallel Equalizer band
	template<>
	class ParallelEQBand<double> : public EQBand<double> {
	  public:
		/// @brief Creates a default `ParallelEQBand`
		ParallelEQBand() noexcept;

		/// @brief Creates a `ParallelEQBand` with the given parameters
		///
		/// @param frequency - The frequency to use, in Hertz
		/// @param q - The q to use
		/// @param gainDB - The gain to use, in Decibels
		/// @param sampleRate - The sample rate to use, in Hertz
		/// @param type - The band type to use
		ParallelEQBand(double frequency,
					   double q,
					   double gainDB,
					   size_t sampleRate,
					   BandType type) noexcept;

		/// @brief Move constructs a `ParallelEQBand` from the given one
		///
		/// @param band - The `ParallelEQBand` to move
		ParallelEQBand(ParallelEQBand<double>&& band) noexcept = default;
		~ParallelEQBand() noexcept override = default;

		/// @brief Sets the gain of this `ParallelEQBand` to the given value
		///
		/// @param gainDB - The new gain, in Decibels
		auto setGainDB(double gainDB) noexcept -> void override;

		/// @brief Returns the gain of this `ParallelEQBand`
		///
		/// @return - The current gain, in Decibels
		[[nodiscard]] auto getGainDB() const noexcept -> double override;

		/// @brief Applies this `ParallelEQBand` to the given input value
		///
		/// @param input - The input value to apply EQ to
		///
		/// @return - The processed value
		[[nodiscard]] auto process(double input) noexcept -> double override;

		/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
		///
		/// @param input - The array of input values to apply EQ to
		auto process(gsl::span<double, gsl::dynamic_extent> input) noexcept -> void override;

		/// @brief Calculates the linear magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto
		getMagnitudeForFrequency(double frequency) const noexcept -> double override;

		/// @brief Calculates the decibel magnitude response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
		///
		/// @return - The magnitude response at the given frequency
		[[nodiscard]] auto
		getDecibelMagnitudeForFrequency(double frequency) const noexcept -> double override;

		/// @brief Calculates the linear magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		auto getMagnitudesForFrequencies(
			gsl::span<double, gsl::dynamic_extent> frequencies,
			gsl::span<double, gsl::dynamic_extent> magnitudes) const noexcept -> void override;

		/// @brief Calculates the decibel magnitude response of this filter for the given array of
		/// frequencies and stores them in `magnitudes`
		///
		/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
		/// @param magnitudes - The array to store the magnitudes in
		auto getDecibelMagnitudesForFrequencies(
			gsl::span<double, gsl::dynamic_extent> frequencies,
			gsl::span<double, gsl::dynamic_extent> magnitudes) const noexcept -> void override;

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in radians, at the given frequency
		[[nodiscard]] auto getPhaseForFrequency(double frequency) const noexcept -> double override;

		/// @brief Calculates the phase response of this filter for the given frequency
		///
		/// @param frequency - The frequency to calculate the phase response for, in Hertz
		///
		/// @return - The phase response, in degrees, at the given frequency
		[[nodiscard]] auto
		getDegreesPhaseForFrequency(double frequency) const noexcept -> double override;

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in radians) in
		auto getPhasesForFrequencies(gsl::span<double, gsl::dynamic_extent> frequencies,
									 gsl::span<double, gsl::dynamic_extent> phases) const noexcept
			-> void override;

		/// @brief Calculates the phase response of this filter for the given array of frequencies
		/// and stores it in `phases`
		///
		/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
		/// @param phases - The array to store the phases (in degrees) in
		auto
		getDegreesPhasesForFrequencies(gsl::span<double, gsl::dynamic_extent> frequencies,
									   gsl::span<double, gsl::dynamic_extent> phases) const noexcept
			-> void override;

		auto operator=(ParallelEQBand<double>&& band) noexcept -> ParallelEQBand<double>& = default;

	  protected:
		/// the actual gain, before acounting for changes necessary for EQing in parallel
		double mGainActual = 0.0;

		/// @brief Creates necessary filter(s) for this EQ band
		auto createFilters() noexcept -> void override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParallelEQBand)
	};
} // namespace apex::dsp
