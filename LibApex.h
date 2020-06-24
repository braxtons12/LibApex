/*******************************************************************************
  The block below describes the properties of this module, and is read by
  the Projucer to automatically generate project code that uses it.
  For details about the syntax and how to create or use a module, see the
  JUCE Module Format.txt file.


  BEGIN_JUCE_MODULE_DECLARATION

ID:               LibApex
vendor:           Apex Audio
version:          0.1.0
name:             Apex Audio Library
description:      A library for Apex Audio products.
website:          https://www.apex-audio.com
license:          Commercial

dependencies:
END_JUCE_MODULE_DECLARATION

 *******************************************************************************/
#pragma once

#include "src/base/StandardIncludes.h"

#include "src/ui/ApexComboBox.h"
#include "src/ui/ApexFader.h"
#include "src/ui/ApexFilmStrip.h"
#include "src/ui/ApexLookAndFeel.h"
#include "src/ui/ApexMeter.h"
#include "src/ui/ApexSlider.h"
#include "src/ui/ApexThumbSlider.h"
#include "src/ui/ApexToggleButton.h"

#include "src/dsp/EQ.h"
#include "src/dsp/Gain.h"
#include "src/dsp/GainStage.h"
#include "src/dsp/WaveShaper.h"
#include "src/dsp/compressors/Compressor.h"
#include "src/dsp/compressors/CompressorFET.h"
#include "src/dsp/compressors/CompressorOpto.h"
#include "src/dsp/compressors/CompressorVCA.h"
#include "src/dsp/expanders/Expander.h"
#include "src/dsp/expanders/ExpanderVCA.h"
