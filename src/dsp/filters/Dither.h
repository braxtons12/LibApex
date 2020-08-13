#pragma once

//(http://www.musicdsp.org/showone.php?id=77)
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex::dsp {
	/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
	/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
	///
	/// @tparam T - The floating point type used to back operations, either float or double
	template<typename T>
	class Dither {
	  public:
		static_assert(std::is_floating_point<T>::value,
					  "T must be a floating point type (float or double)");

	  private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dither)
	};

	/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
	/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
	template<>
	class Dither<float> {
	  public:
		Dither() noexcept;

		/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
		///
		/// @param numBits - The bit-depth to convert to
		/// @param noiseShaping - The noise shaping to use
		explicit Dither(size_t numBits, float noiseShaping = 0.5F) noexcept;
		~Dither() noexcept = default;

		/// @brief Sets the bit-depth of this `Dither` to the given value
		///
		/// @param numBits - The new bit-depth to use
		inline auto setNumBits(size_t numBits) noexcept -> void {
			jassert(numBits <= 24);
			mNumBits = numBits;
			updateState();
		}

		/// @brief Sets the noise shaping to use to the given value
		///
		/// @param noiseShaping - The noise shaping to use
		inline auto setNoiseShaping(float noiseShaping) noexcept -> void {
			mNoiseShaping = noiseShaping;
			updateState();
		}

		/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
		///
		/// @param input - The input to dither
		///
		/// @return - The dithered, bit-depth reduced result
		[[nodiscard]] auto dither(float input) noexcept -> float;

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
		float mFeedbackOne = 0.0F;
		/// The second running feedback error value
		float mFeedbackTwo = 0.0F;
		/// Noise shaping amount
		float mNoiseShaping = 0.5F;
		/// Resulting word length
		float mWordLength = math::pow2f(static_cast<float>(mNumBits - 1));
		/// Inverse of the word length
		float mWordLengthInverse = 1.0F / mWordLength;
		/// Dither amplitude
		float mAmplitude = mWordLengthInverse / static_cast<float>(math::Random::MAX);
		/// DC Offset adjustment
		float mDCOffset = mWordLengthInverse * 0.5F;

		auto updateState() noexcept -> void;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dither)
	};

	/// @brief Class used to apply dither along with bit-depth reduction (eg 32bit to 24 bit)
	/// See http://www.musicdsp.org/showone.php?id=77 for more details on the algorithm
	template<>
	class Dither<double> {
	  public:
		Dither() noexcept;

		/// @brief Constructs a `Dither` targeting the given bit-depth with the given noise shaping
		///
		/// @param numBits - The bit-depth to convert to
		/// @param noiseShaping - The noise shaping to use
		explicit Dither(size_t numBits, double noiseShaping = 0.5) noexcept;
		~Dither() noexcept = default;

		/// @brief Sets the bit-depth of this `Dither` to the given value
		///
		/// @param numBits - The new bit-depth to use
		inline auto setNumBits(size_t numBits) noexcept -> void {
			jassert(numBits <= 48);
			mNumBits = numBits;
			updateState();
		}

		/// @brief Sets the noise shaping to use to the given value
		///
		/// @param noiseShaping - The noise shaping to use
		inline auto setNoiseShaping(double noiseShaping) noexcept -> void {
			mNoiseShaping = noiseShaping;
			updateState();
		}

		/// @brief Dithers and bit-depth reduces the input based on this `Dither`'s parameters
		///
		/// @param input - The input to dither
		///
		/// @return - The dithered, bit-depth reduced result
		[[nodiscard]] auto dither(double input) noexcept -> double;

	  private:
		/// Random number generator
		math::Random mRandom = math::Random();
		/// Runnning Random number one
		size_t mRandomOne = 0;
		/// Running Random number two
		size_t mRandomTwo = 0;
		/// The bit depth to use
		size_t mNumBits = 48;
		/// The first running feedback error value
		double mFeedbackOne = 0.0;
		/// The second running feedback error value
		double mFeedbackTwo = 0.0;
		/// Noise shaping amount
		double mNoiseShaping = 0.5;
		/// Resulting word length
		double mWordLength = math::pow2(static_cast<double>(mNumBits - 1));
		/// Inverse of the word length
		double mWordLengthInverse = 1.0 / mWordLength;
		/// Dither amplitude
		double mAmplitude = mWordLengthInverse / static_cast<double>(math::Random::MAX);
		/// DC Offset adjustment
		double mDCOffset = mWordLengthInverse * 0.5;

		auto updateState() noexcept -> void;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dither)
	};
} // namespace apex::dsp
