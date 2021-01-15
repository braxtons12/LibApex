#pragma once

//(http://www.musicdsp.org/showone.php?id=77)
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
	/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
	///
	/// @tparam FloatType - The FloatTypeing point type used to back operations, either FloatType or
	/// double
	template<typename FloatType = float,
			 std::enable_if_t<std::is_floating_point_v<FloatType>, bool> = true>
	class Dither {
	  public:
		Dither() noexcept {
			updateState();
		}

		/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
		///
		/// @param numBits - The bit-depth to convert to
		/// @param noiseShaping - The noise shaping to use
		explicit Dither(size_t numBits,
						FloatType noiseShaping = narrow_cast<FloatType>(0.5)) noexcept
			: mNumBits(numBits), mNoiseShaping(noiseShaping) {
			if constexpr(std::is_same_v<FloatType, FloatType>) {
				jassert(numBits <= 24);
			}
			else {
				jassert(numBits <= 48);
			}

			updateState();
		}
		~Dither() noexcept = default;

		/// @brief Sets the bit-depth of this `Dither` to the given value
		///
		/// @param numBits - The new bit-depth to use
		inline auto setNumBits(size_t numBits) noexcept -> void {
			if constexpr(std::is_same_v<FloatType, FloatType>) {
				jassert(numBits <= 24);
			}
			else {
				jassert(numBits <= 48);
			}
			mNumBits = numBits;
			updateState();
		}

		/// @brief Sets the noise shaping to use to the given value
		///
		/// @param noiseShaping - The noise shaping to use
		inline auto setNoiseShaping(FloatType noiseShaping) noexcept -> void {
			mNoiseShaping = noiseShaping;
			updateState();
		}

		/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
		///
		/// @param input - The input to dither
		///
		/// @return - The dithered, bit-depth reduced result
		[[nodiscard]] inline auto dither(FloatType input) noexcept -> FloatType {
			auto output = input + mNoiseShaping * (mFeedbackOne + mFeedbackOne - mFeedbackTwo);
			auto outputTemp
				= output + mDCOffset + mAmplitude * narrow_cast<FloatType>(mRandomOne - mRandomTwo);
			auto outputTruncated = narrow_cast<int>(mWordLength * outputTemp);

			if(outputTemp < narrow_cast<FloatType>(0.0)) {
				--outputTruncated;
			}
			mFeedbackTwo = mFeedbackOne;
			mFeedbackOne = output - mWordLengthInverse * narrow_cast<FloatType>(outputTruncated);

			return output;
		}

	  private:
		/// Random number generator
		math::Random mRandom = math::Random();
		/// Runnning Random number one
		size_t mRandomOne = 0;
		/// Running Random number two
		size_t mRandomTwo = 0;
		/// The bit depth to use
		size_t mNumBits = 24;
		/// The first running feedback error value
		FloatType mFeedbackOne = narrow_cast<FloatType>(0.0);
		/// The second running feedback error value
		FloatType mFeedbackTwo = narrow_cast<FloatType>(0.0);
		/// Noise shaping amount
		FloatType mNoiseShaping = narrow_cast<FloatType>(0.5);
		/// Resulting word length
		FloatType mWordLength = Exponentials<FloatType>::pow2(static_cast<FloatType>(mNumBits - 1));
		/// Inverse of the word length
		FloatType mWordLengthInverse = narrow_cast<FloatType>(1.0) / mWordLength;
		/// Dither amplitude
		FloatType mAmplitude = mWordLengthInverse / static_cast<FloatType>(math::Random::MAX);
		/// DC Offset adjustment
		FloatType mDCOffset = mWordLengthInverse * narrow_cast<FloatType>(0.5);

		inline auto updateState() noexcept -> void {
			mRandom.srand(10956489098);
			size_t rand = mRandom.rand();
			auto seedTemp = narrow_cast<size_t>(mNumBits * mNoiseShaping);
			size_t seed = rand / seedTemp;
			mRandom.srand(seed);
			mRandomOne = mRandom.rand();
			mRandom.srand(mRandomOne);
			mRandomTwo = mRandom.rand();

			mFeedbackOne = narrow_cast<FloatType>(0.0);
			mFeedbackTwo = narrow_cast<FloatType>(0.0);

			mWordLength = Exponentials<FloatType>::pow2(narrow_cast<FloatType>(mNumBits - 1));
			mWordLengthInverse = narrow_cast<FloatType>(1.0) / mWordLength;

			mAmplitude = mWordLengthInverse / narrow_cast<FloatType>(math::Random::MAX);

			mDCOffset = mWordLengthInverse * narrow_cast<FloatType>(0.5);
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dither)
	};
} // namespace apex::dsp
