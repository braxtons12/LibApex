#pragma once

#include <gsl/gsl>
#include <memory>

#include "../math/Constants.h"
#include "../math/Decibels.h"
#include "../math/Exponentials.h"
#include "../math/Frequency.h"
#include "../math/General.h"
#include "../math/Random.h"
#include "../math/TrigFuncs.h"
#include "../utils/Error.h"
#include "../utils/LockFreeQueue.h"
#include "../utils/Logger.h"
#include "../utils/MiscMacros.h"
#include "../utils/OptionAndResult.h"
#include "../utils/Span.h"
#include "../utils/synchronization/ReadWriteLock.h"
#ifndef TEST_HARNESS
	#include "juce_dsp/juce_dsp.h"
	#include "juce_gui_basics/juce_gui_basics.h"
	#include "juce_gui_extra/juce_gui_extra.h"
#endif

/// @brief All Apex Audio types/functions/etc
namespace apex {

	/// @brief Apex Audio Digital Signal Processing types and functions
	namespace dsp { } // namespace dsp

	/// @brief Apex Audio custom mathematics implementaitons
	namespace math { } // namespace math

	/// @brief Apex Audio UI components and systems
	namespace ui { } // namespace ui

	/// @brief Apex Audio utility types for indicating optional values, results, and errors, among
	/// others
	namespace utils { } // namespace utils

	using apex::utils::Err;
	using apex::utils::Error;
	using apex::utils::LockFreeQueue;
	using apex::utils::Logger;
	using apex::utils::None;
	using apex::utils::Ok;
	using apex::utils::Option;
	using apex::utils::Result;
	using apex::utils::Some;
	using apex::utils::Span;

	using apex::math::Constants;
	using apex::math::Decibels;
	using apex::math::Exponentials;
	using apex::math::General;
	using apex::math::Hertz;
	using apex::math::Radians;
	using apex::math::Trig;
	using apex::math::operator""_dB;
	using apex::math::operator""_Hz;
	using apex::math::operator""_kHz;
	using apex::math::operator""_rad;

	using gsl::narrow_cast;
} // namespace apex

#ifdef TESTING_PROCESSOR
	#define TESTING_SIDECHAIN
	#define TESTING_SIDECHAIN_1176
	#define TESTING_SIDECHAIN_SSL
	#define TESTING_SIDECHAIN_MODERN_BUS
	#define TESTING_GAIN
	#define TESTING_GAIN_REDUCTION
	#define TESTING_GAIN_REDUCTION_FET
	#define TESTING_GAIN_REDUCTION_OPTO
	#define TESTING_GAIN_REDUCTION_VCA
	#define TESTING_GAINSTAGE
	#define TESTING_GAINSTAGE_FET
	#define TESTING_GAINSTAGE_VCA
	#define TESTING_LEVELDETECTOR
	#define TESTING_LEVELDETECTOR_1176
	#define TESTING_LEVELDETECTOR_MODERN_BUS
	#define TESTING_LEVELDETECTOR_RMS
	#define TESTING_LEVELDETECTOR_SSL
	#define TESTING_GAIN_COMPUTER
	#define TESTING_GAIN_COMPUTER_COMPRESSOR
	#define TESTING_GAIN_COMPUTER_EXPANDER
	#define TESTING_DYNAMICS_STATE
	#define TESTING_BIQUAD_FILTER
	#define TESTING_DITHER
	#define TESTING_PROCESSOR_INTERFACE
	#define TESTING_BASE_COMPRESSOR
	#define TESTING_COMPRESSOR_1176
	#define TESTING_COMPRESSOR_MODERN_BUS
	#define TESTING_COMPRESSOR_SSL
	#define TESTING_EQBAND
	#define TESTING_PARALLEL_EQBAND
	#define TESTING_OVERSAMPLER
	#define TESTING_MULTICHANNEL_PROCESSOR
	#define TESTING_WAVESHAPER
#endif
