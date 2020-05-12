#pragma once

#include "juce_analytics/juce_analytics.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_audio_plugin_client/juce_audio_plugin_client.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_core/juce_core.h"
#include "juce_cryptography/juce_cryptography.h"
#include "juce_data_structures/juce_data_structures.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "juce_opengl/juce_opengl.h"
#include "juce_product_unlocking/juce_product_unlocking.h"

#include "utils/Option.h"
#include "utils/Result.h"

#include "math/Decibels.h"
#include "math/TrigFuncs.h"
#include "math/Exponentials.h"
#include "math/General.h"
#include "math/Constants.h"

using namespace juce;

using apex::utils::Option;
using apex::utils::Result;

using namespace apex::math;

using jDecibels = juce::Decibels;

using Decibels = apex::math::Decibels;

