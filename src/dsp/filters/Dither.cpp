#include "Dither.h"

#include "../../math/Exponentials.h"

namespace apex {
	namespace dsp {

		/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
		///
		/// @param numBits - The bit-depth to convert to
		/// @param noiseShaping - The noise shaping to use
		Dither<float>::Dither(size_t numBits, float noiseShaping)
			: mNumBits(numBits), mNoiseShaping(noiseShaping)
		{
			jassert(numBits <= 24);
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			size_t seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = 0.0f;
			mFeedbackTwo = 0.0f;
			mWordLength = math::pow2f(mNumBits - 1);
			mWordLengthInverse = 1.0f / mWordLength;

			mAmplitude = mWordLengthInverse / static_cast<float>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * 0.5f;
		}

		Dither<float>::~Dither() {

		}

		/// @brief Sets the bit-depth of this `Dither` to the given value
		///
		/// @param numBits - The new bit-depth to use
		void Dither<float>::setNumBits(size_t numBits) {
			jassert(numBits <= 24);
			mNumBits = numBits;
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			size_t seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = 0.0f;
			mFeedbackTwo = 0.0f;

			mWordLength = math::pow2f(mNumBits - 1);
			mWordLengthInverse = 1.0f / mWordLength;

			mAmplitude = mWordLengthInverse / static_cast<float>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * 0.5f;
		}

		/// @brief Sets the noise shaping to use to the given value
		///
		/// @param noiseShaping - The noise shaping to use
		void Dither<float>::setNoiseShaping(float noiseShaping) {
			mNoiseShaping = noiseShaping;
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			size_t seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = 0.0f;
			mFeedbackTwo = 0.0f;

			mWordLength = math::pow2f(mNumBits - 1);
			mWordLengthInverse = 1.0f / mWordLength;

			mAmplitude = mWordLengthInverse / static_cast<float>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * 0.5f;
		}

		/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
		///
		/// @param input - The input to dither
		///
		/// @return - The dithered, bit-depth reduced result
		float Dither<float>::dither(float input) {
			float output = input + mNoiseShaping * (mFeedbackOne + mFeedbackOne - mFeedbackTwo);
			float outputTemp = output + mDCOffset + mAmplitude
				* static_cast<float>(mRandomOne - mRandomTwo);
			int outputTruncated = static_cast<int>(mWordLength * outputTemp);

			if(outputTemp < 0.0f) --outputTruncated;
			mFeedbackTwo = mFeedbackOne;
			mFeedbackOne = output - mWordLengthInverse * static_cast<float>(outputTruncated);

			return output;
		}

		/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
		///
		/// @param numBits - The bit-depth to convert to
		/// @param noiseShaping - The noise shaping to use
		Dither<double>::Dither(size_t numBits, double noiseShaping)
			: mNumBits(numBits), mNoiseShaping(noiseShaping)
		{
			jassert(numBits <= 48);
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			size_t seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = 0.0;
			mFeedbackTwo = 0.0;

			mWordLength = math::pow2(mNumBits - 1);
			mWordLengthInverse = 1.0 / mWordLength;

			mAmplitude = mWordLengthInverse / static_cast<double>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * 0.5;
		}

		Dither<double>::~Dither() {

		}

		/// @brief Sets the bit-depth of this `Dither` to the given value
		///
		/// @param numBits - The new bit-depth to use
		void Dither<double>::setNumBits(size_t numBits) {
			jassert(numBits <= 48);
			mNumBits = numBits;
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			size_t seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = 0.0;
			mFeedbackTwo = 0.0;

			mWordLength = math::pow2(mNumBits - 1);
			mWordLengthInverse = 1.0 / mWordLength;

			mAmplitude = mWordLengthInverse / static_cast<double>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * 0.5;
		}

		/// @brief Sets the noise shaping to use to the given value
		///
		/// @param noiseShaping - The noise shaping to use
		void Dither<double>::setNoiseShaping(double noiseShaping) {
			mNoiseShaping = noiseShaping;
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			size_t seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = 0.0;
			mFeedbackTwo = 0.0;

			mWordLength = math::pow2(mNumBits - 1);
			mWordLengthInverse = 1.0 / mWordLength;

			mAmplitude = mWordLengthInverse / static_cast<double>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * 0.5;
		}

		/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
		///
		/// @param input - The input to dither
		///
		/// @return - The dithered, bit-depth reduced result
		double Dither<double>::dither(double input) {
			double output = input + mNoiseShaping * (mFeedbackOne + mFeedbackOne - mFeedbackTwo);
			double outputTemp = output + mDCOffset + mAmplitude
				* static_cast<double>(mRandomOne - mRandomTwo);
			int64_t outputTruncated = static_cast<int64_t>(mWordLength * outputTemp);

			if(outputTemp < 0.0) --outputTruncated;
			mFeedbackTwo = mFeedbackOne;
			mFeedbackOne = output - mWordLengthInverse * static_cast<double>(outputTruncated);

			return output;
		}
	}
}
