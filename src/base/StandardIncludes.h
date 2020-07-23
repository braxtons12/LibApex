#pragma once

#include <gsl/gsl>

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "juce_dsp/juce_dsp.h"

#include "../utils/Option.h"
#include "../utils/Result.h"
#include "../utils/Error.h"

#include "../math/Decibels.h"
#include "../math/TrigFuncs.h"
#include "../math/Exponentials.h"
#include "../math/General.h"
#include "../math/Constants.h"
#include "../math/Random.h"

/// @brief All Apex Audio types/functions/etc
namespace apex {

	/// @brief Apex Audio Digital Signal Processing types and functions
	namespace dsp {
	} //namespace dsp

	/// @brief Apex Audio custom mathematics implementaitons
	namespace math {
	} //namespace math

	/// @brief Apex Audio UI components and systems
	namespace ui {
	} //namespace ui

	/// @brief Apex Audio utility types for indicating optional values, results, and errors, among others
	namespace utils {
	} //namespace utils

	using apex::utils::Option;
	using apex::utils::Result;
	using apex::utils::Error;
} //namespace apex
