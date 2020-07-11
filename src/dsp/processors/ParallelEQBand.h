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
					ParallelEQBand(const ParallelEQBand<float>&& band) noexcept;
					~ParallelEQBand() noexcept override;

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

					ParallelEQBand<float> operator=(const ParallelEQBand<float>&& band) noexcept;

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
					ParallelEQBand(const ParallelEQBand<double>&& band) noexcept;
					~ParallelEQBand() noexcept override;

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

					ParallelEQBand<double> operator=(const ParallelEQBand<double>&& band) noexcept;

				protected:
					///the actual gain, before acounting for changes necessary for EQing in parallel
					double mGainActual = 0.0;

					/// @brief Creates necessary filter(s) for this EQ band
					virtual void createFilters() noexcept override;
			};
	}
}
