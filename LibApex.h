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
#include "src/dsp/WaveShaper.h"
#include "src/dsp/dynamics/DynamicsState.h"
#include "src/dsp/dynamics/compressors/Compressor.h"
#include "src/dsp/dynamics/compressors/CompressorFET.h"
#include "src/dsp/dynamics/compressors/CompressorOpto.h"
#include "src/dsp/dynamics/compressors/CompressorVCA.h"
#include "src/dsp/dynamics/expanders/Expander.h"
#include "src/dsp/dynamics/expanders/ExpanderVCA.h"
#include "src/dsp/dynamics/gaincomputers/GainComputer.h"
#include "src/dsp/dynamics/gaincomputers/GainComputerCompressor.h"
#include "src/dsp/dynamics/gaincomputers/GainComputerExpander.h"
#include "src/dsp/dynamics/gainreductions/GainReduction.h"
#include "src/dsp/dynamics/gainreductions/GainReductionFET.h"
#include "src/dsp/dynamics/gainreductions/GainReductionOpto.h"
#include "src/dsp/dynamics/gainreductions/GainReductionVCA.h"
#include "src/dsp/dynamics/leveldetectors/LevelDetector.h"
#include "src/dsp/dynamics/leveldetectors/LevelDetector1176.h"
#include "src/dsp/dynamics/leveldetectors/LevelDetectorModernBus.h"
#include "src/dsp/dynamics/leveldetectors/LevelDetectorRMS.h"
#include "src/dsp/dynamics/leveldetectors/LevelDetectorSSL.h"
#include "src/dsp/dynamics/sidechains/Sidechain.h"
#include "src/dsp/dynamics/sidechains/Sidechain1176.h"
#include "src/dsp/dynamics/sidechains/SidechainModernBus.h"
#include "src/dsp/dynamics/sidechains/SidechainSSL.h"
#include "src/dsp/filters/BiQuadFilter.h"
#include "src/dsp/gainstages/GainStage.h"
#include "src/dsp/gainstages/GainStageFET.h"
#include "src/dsp/gainstages/GainStageVCA.h"
#include "src/dsp/processors/EQBand.h"
#include "src/dsp/processors/Gain.h"
#include "src/dsp/processors/MultiChannelProcessor.h"
#include "src/dsp/processors/OverSampler.h"
#include "src/dsp/processors/ParallelEQBand.h"
#include "src/dsp/processors/Processor.h"
#include "src/ui/ApexComboBox.h"
#include "src/ui/ApexFader.h"
#include "src/ui/ApexFilmStrip.h"
#include "src/ui/ApexLookAndFeel.h"
#include "src/ui/ApexMeter.h"
#include "src/ui/ApexSlider.h"
#include "src/ui/ApexThumbSlider.h"
#include "src/ui/ApexToggleButton.h"
#include "src/utils/synchronization/ReadWriteLock.h"
#include "src/utils/synchronization/ScopedLockGuard.h"
