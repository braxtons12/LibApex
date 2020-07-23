#include "Dither.h"

#include "../../math/Exponentials.h"

namespace apex::dsp {
	Dither<float>::Dither() noexcept {
		updateState();
	}

	/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
	///
	/// @param numBits - The bit-depth to convert to
	/// @param noiseShaping - The noise shaping to use
	Dither<float>::Dither(size_t numBits, float noiseShaping) noexcept
		: mNumBits(numBits), mNoiseShaping(noiseShaping)
		{
			jassert(numBits <= 24);
			updateState();
		}

	/// @brief Sets the bit-depth of this `Dither` to the given value
	///
	/// @param numBits - The new bit-depth to use
	auto Dither<float>::setNumBits(size_t numBits) noexcept -> void {
		jassert(numBits <= 24);
		mNumBits = numBits;
		updateState();
	}

	/// @brief Sets the noise shaping to use to the given value
	///
	/// @param noiseShaping - The noise shaping to use
	auto Dither<float>::setNoiseShaping(float noiseShaping) noexcept -> void {
		mNoiseShaping = noiseShaping;
		updateState();
	}

	/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
	///
	/// @param input - The input to dither
	///
	/// @return - The dithered, bit-depth reduced result
	auto Dither<float>::dither(float input) noexcept -> float {
		float output = input + mNoiseShaping * (mFeedbackOne + mFeedbackOne - mFeedbackTwo);
		float outputTemp = output + mDCOffset + mAmplitude
			* static_cast<float>(mRandomOne - mRandomTwo);
		int outputTruncated = static_cast<int>(mWordLength * outputTemp);

		if(outputTemp < 0.0F) {
			--outputTruncated;
		}
		mFeedbackTwo = mFeedbackOne;
		mFeedbackOne = output - mWordLengthInverse * static_cast<float>(outputTruncated);

		return output;
	}

	auto Dither<float>::updateState() noexcept -> void {
		mRandom.srand(10956489098);
		size_t rand = mRandom.rand();
		auto seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
		size_t seed = rand / seedTemp;
		mRandom.srand(seed);
		mRandomOne = mRandom.rand();
		mRandom.srand(mRandomOne);
		mRandomTwo = mRandom.rand();

		mFeedbackOne = 0.0F;
		mFeedbackTwo = 0.0F;

		mWordLength = math::pow2f(static_cast<float>(mNumBits - 1));
		mWordLengthInverse = 1.0F / mWordLength;

		mAmplitude = mWordLengthInverse / static_cast<float>(math::Random::MAX);

		mDCOffset = mWordLengthInverse * 0.5F;
	}

	Dither<double>::Dither() noexcept {
		updateState();
	}

	/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
	///
	/// @param numBits - The bit-depth to convert to
	/// @param noiseShaping - The noise shaping to use
	Dither<double>::Dither(size_t numBits, double noiseShaping) noexcept
		: mNumBits(numBits), mNoiseShaping(noiseShaping)
		{
			jassert(numBits <= 48);
			updateState();
		}

	/// @brief Sets the bit-depth of this `Dither` to the given value
	///
	/// @param numBits - The new bit-depth to use
	auto Dither<double>::setNumBits(size_t numBits) noexcept -> void {
		jassert(numBits <= 48);
		mNumBits = numBits;
		updateState();
	}

	/// @brief Sets the noise shaping to use to the given value
	///
	/// @param noiseShaping - The noise shaping to use
	auto Dither<double>::setNoiseShaping(double noiseShaping) noexcept -> void {
		mNoiseShaping = noiseShaping;
		updateState();
	}

	/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
	///
	/// @param input - The input to dither
	///
	/// @return - The dithered, bit-depth reduced result
	auto Dither<double>::dither(double input) noexcept -> double {
		double output = input + mNoiseShaping * (mFeedbackOne + mFeedbackOne - mFeedbackTwo);
		double outputTemp = output + mDCOffset + mAmplitude
			* static_cast<double>(mRandomOne - mRandomTwo);
		int outputTruncated = static_cast<int>(mWordLength * outputTemp);

		if(outputTemp < 0.0) {
			--outputTruncated;
		}

		mFeedbackTwo = mFeedbackOne;
		mFeedbackOne = output - mWordLengthInverse * static_cast<double>(outputTruncated);

		return output;
	}

	auto Dither<double>::updateState() noexcept -> void {
		mRandom.srand(10956489098);
		size_t rand = mRandom.rand();
		auto seedTemp = static_cast<size_t>(mNumBits * mNoiseShaping);
		size_t seed = rand / seedTemp;
		mRandom.srand(seed);
		mRandomOne = mRandom.rand();
		mRandom.srand(mRandomOne);
		mRandomTwo = mRandom.rand();

		mFeedbackOne = 0.0;
		mFeedbackTwo = 0.0;

		mWordLength = math::pow2(static_cast<double>(mNumBits - 1));
		mWordLengthInverse = 1.0 / mWordLength;

		mAmplitude = mWordLengthInverse / static_cast<double>(math::Random::MAX);

		mDCOffset = mWordLengthInverse * 0.5;
	}
} //namespace apex::dsp
