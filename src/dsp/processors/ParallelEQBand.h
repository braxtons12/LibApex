#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "EQBand.h"
#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Basic Parallel Equalizer band
		///
		/// @tparam T - The floating point type to back operations
		template<typename T>
			class ParallelEQBand : public EQBand<T> {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
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
					ParallelEQBand(float frequency, float q, float gainDB, size_t sampleRate, BandType type) noexcept;

					/// @brief Move constructs a `ParallelEQBand` from the given one
					///
					/// @param band - The `ParallelEQBand` to move
					ParallelEQBand(ParallelEQBand<float>&& band) noexcept = default;
					~ParallelEQBand() noexcept override = default;

					/// @brief Sets the gain of this `ParallelEQBand` to the given value
					///
					/// @param gainDB - The new gain, in Decibels
					virtual void setGainDB(float gainDB) noexcept override;

					/// @brief Returns the gain of this `ParallelEQBand`
					///
					/// @return - The current gain, in Decibels
					virtual float getGainDB() const noexcept override;

					/// @brief Applies this `ParallelEQBand` to the given input value
					///
					/// @param input - The input value to apply EQ to
					///
					/// @return - The processed value
					virtual float process(float input) noexcept override;

					/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
					///
					/// @param input - The array of input values to apply EQ to
					/// @param numSamples - The number of samples in the array
					virtual void process(float* input, size_t numSamples) noexcept override;

					/// @brief Calculates the linear magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					virtual float getMagnitudeForFrequency(float frequency) const noexcept override;

					/// @brief Calculates the decibel magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					virtual float getDecibelMagnitudeForFrequency(float frequency) const noexcept override;

					/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getMagnitudesForFrequencies(float* frequencies, float* magnitudes,
							size_t numFrequencies) const noexcept override;

					/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getDecibelMagnitudesForFrequencies(float* frequencies, float* magnitudes,
							size_t numFrequencies) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response, in radians, at the given frequency
					virtual float getPhaseForFrequency(float frequency) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response, in degrees, at the given frequency
					virtual float getDegreesPhaseForFrequency(float frequency) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases (in radians) in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getPhasesForFrequencies(float* frequencies, float* phases,
							size_t numFrequencies) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases (in degrees) in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getDegreesPhasesForFrequencies(float* frequencies, float* phases,
							size_t numFrequencies) const noexcept override;

					ParallelEQBand<float>& operator=(ParallelEQBand<float>&& band) noexcept = default;

				protected:
					///the actual gain, before acounting for changes necessary for EQing in parallel
					float mGainActual = 0.0f;

					/// @brief Creates necessary filter(s) for this EQ band
					virtual void createFilters() noexcept override;
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
					ParallelEQBand(double frequency, double q, double gainDB, size_t sampleRate, BandType type) noexcept;

					/// @brief Move constructs a `ParallelEQBand` from the given one
					///
					/// @param band - The `ParallelEQBand` to move
					ParallelEQBand(ParallelEQBand<double>&& band) noexcept = default;
					~ParallelEQBand() noexcept override = default;

					/// @brief Sets the gain of this `ParallelEQBand` to the given value
					///
					/// @param gainDB - The new gain, in Decibels
					virtual void setGainDB(double gainDB) noexcept override;

					/// @brief Returns the gain of this `ParallelEQBand`
					///
					/// @return - The current gain, in Decibels
					virtual double getGainDB() const noexcept override;

					/// @brief Applies this `ParallelEQBand` to the given input value
					///
					/// @param input - The input value to apply EQ to
					///
					/// @return - The processed value
					virtual double process(double input) noexcept override;

					/// @brief Applies this `ParallelEQBand` to the array of given input values, in place
					///
					/// @param input - The array of input values to apply EQ to
					/// @param numSamples - The number of samples in the array
					virtual void process(double* input, size_t numSamples) noexcept override;

					/// @brief Calculates the linear magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					virtual double getMagnitudeForFrequency(double frequency) const noexcept override;

					/// @brief Calculates the decibel magnitude response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calcualte the magnitude response for, in Hertz
					///
					/// @return - The magnitude response at the given frequency
					virtual double getDecibelMagnitudeForFrequency(double frequency) const noexcept override;

					/// @brief Calculates the linear magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getMagnitudesForFrequencies(double* frequencies, double* magnitudes,
							size_t numFrequencies) const noexcept override;

					/// @brief Calculates the decibel magnitude response of this filter for the given array of frequencies and stores them in `magnitudes`
					///
					/// @param frequencies - The frequencies to calcualte the magnitude response for, in Hertz
					/// @param magnitudes - The array to store the magnitudes in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getDecibelMagnitudesForFrequencies(double* frequencies, double* magnitudes,
							size_t numFrequencies) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response, in radians, at the given frequency
					virtual double getPhaseForFrequency(double frequency) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given frequency
					///
					/// @param frequency - The frequency to calculate the phase response for, in Hertz
					///
					/// @return - The phase response, in degrees, at the given frequency
					virtual double getDegreesPhaseForFrequency(double frequency) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases (in radians) in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getPhasesForFrequencies(double* frequencies, double* phases,
							size_t numFrequencies) const noexcept override;

					/// @brief Calculates the phase response of this filter for the given array of frequencies and stores it in `phases`
					///
					/// @param frequencies - The frequencies to calculate the phase response for, in Hertz
					/// @param phases - The array to store the phases (in degrees) in
					/// @param numFrequencies - The number of frequencies in the `frequencies` array
					virtual void getDegreesPhasesForFrequencies(double* frequencies, double* phases,
							size_t numFrequencies) const noexcept override;

					ParallelEQBand<double>& operator=(ParallelEQBand<double>&& band) noexcept = default;

				protected:
					///the actual gain, before acounting for changes necessary for EQing in parallel
					double mGainActual = 0.0;

					/// @brief Creates necessary filter(s) for this EQ band
					virtual void createFilters() noexcept override;
			};
	}
}
