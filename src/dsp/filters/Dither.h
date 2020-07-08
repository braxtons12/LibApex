#pragma once

//(http://www.musicdsp.org/showone.php?id=77)
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
		/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
		///
		/// @tparam T - The floating point type used to back operations, either float or double
		template<typename T>
			class Dither {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type (float or double)");
			};

		/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
		/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
		template<>
			class Dither<float> {
				public:
					/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
					///
					/// @param numBits - The bit-depth to convert to
					/// @param noiseShaping - The noise shaping to use
					Dither(size_t numBits, float noiseShaping = 0.5f);
					~Dither();

					/// @brief Sets the bit-depth of this `Dither` to the given value
					///
					/// @param numBits - The new bit-depth to use
					void setNumBits(size_t numBits);

					/// @brief Sets the noise shaping to use to the given value
					///
					/// @param noiseShaping - The noise shaping to use
					void setNoiseShaping(float noiseShaping);

					/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
					///
					/// @param input - The input to dither
					///
					/// @return - The dithered, bit-depth reduced result
					float dither(float input);

				private:
					///Random number generator
					math::Random mRandom;
					///Runnning Random number one
					size_t mRandomOne;
					///Running Random number two
					size_t mRandomTwo;
					///The bit depth to use
					size_t mNumBits;
					///The first running feedback error value
					float mFeedbackOne;
					///The second running feedback error value
					float mFeedbackTwo;
					///DC Offset adjustment
					float mDCOffset;
					///Dither amplitude
					float mAmplitude;
					///Noise shaping amount
					float mNoiseShaping;
					///Resulting word length
					float mWordLength;
					///Inverse of the word length
					float mWordLengthInverse;
			};

		/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
		/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
		template<>
			class Dither<double> {
				public:
					/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
					///
					/// @param numBits - The bit-depth to convert to
					/// @param noiseShaping - The noise shaping to use
					Dither(size_t numBits, double noiseShaping = 0.5);
					~Dither();

					/// @brief Sets the bit-depth of this `Dither` to the given value
					///
					/// @param numBits - The new bit-depth to use
					void setNumBits(size_t numBits);

					/// @brief Sets the noise shaping to use to the given value
					///
					/// @param noiseShaping - The noise shaping to use
					void setNoiseShaping(double noiseShaping);

					/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
					///
					/// @param input - The input to dither
					///
					/// @return - The dithered, bit-depth reduced result
					double dither(double input);

				private:
					///Random number generator
					math::Random mRandom;
					///Runnning Random number one
					size_t mRandomOne;
					///Running Random number two
					size_t mRandomTwo;
					///The bit depth to use
					size_t mNumBits;
					///The first running feedback error value
					double mFeedbackOne;
					///The second running feedback error value
					double mFeedbackTwo;
					///DC Offset adjustment
					double mDCOffset;
					///Dither amplitude
					double mAmplitude;
					///Noise shaping amount
					double mNoiseShaping;
					///Resulting word length
					double mWordLength;
					///Inverse of the word length
					double mWordLengthInverse;
			};
	}
}
