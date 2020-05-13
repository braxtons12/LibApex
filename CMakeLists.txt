cmake_minimum_required(VERSION 3.15)

project(Apex VERSION 0.1.0)
set(COVERAGE_COMPILE_FLAGS "-std=c++17")

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_package(JUCE CONFIG REQUIRED)

juce_set_vst2_sdk_path("/home/braxton/audio-dev/sdks/VST_SDK/VST2_SDK")

add_library(Apex STATIC)

target_include_directories(Apex INTERFACE
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/src/ui"
	"${CMAKE_SOURCE_DIR}/src/dsp"
	"${CMAKE_SOURCE_DIR}/src/dsp/compressors"
	"${CMAKE_SOURCE_DIR}/src/dsp/expanders"
	"${CMAKE_SOURCE_DIR}/src/math"
	"${CMAKE_SOURCE_DIR}/src/utils"
	)

set(BASE
	"${CMAKE_SOURCE_DIR}/src/StandardIncludes.h"
	)

set(UI
	"${CMAKE_SOURCE_DIR}/src/ui/ApexComboBox.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexFader.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexFilmStrip.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexLookAndFeel.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexMeter.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexSlider.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexThumbSlider.cpp"
	"${CMAKE_SOURCE_DIR}/src/ui/ApexToggleButton.cpp"
	)

set(DSP
	"${CMAKE_SOURCE_DIR}/src/dsp/EQ.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/compressors/Compressor.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/compressors/CompressorFET.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/compressors/CompressorOpto.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/compressors/CompressorVCA.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/expanders/Expander.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/expanders/ExpanderVCA.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/Gain.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/GainStage.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/WaveShaper.cpp"
	)

set(MATH
	"${CMAKE_SOURCE_DIR}/src/math/General.cpp"
	"${CMAKE_SOURCE_DIR}/src/math/Decibels.cpp"
	"${CMAKE_SOURCE_DIR}/src/math/Exponentials.cpp"
	"${CMAKE_SOURCE_DIR}/src/math/TrigFuncs.cpp"
	)

set(UTILS
	"${CMAKE_SOURCE_DIR}/src/utils/Option.cpp"
	"${CMAKE_SOURCE_DIR}/src/utils/Result.cpp"
	)

target_sources(Apex PUBLIC
	${BASE}
	${UI}
	${DSP}
	${MATH}
	${UTILS}
	)

target_compile_definitions(Apex PRIVATE
	JUCE_WEB_BROWSER=0
	JUCE_USE_CURL=0
	JUCE_VST3_CAN_REPLACE_VST2=0
	)

target_link_libraries(Apex PRIVATE
	juce::juce_recommended_warning_flags
	juce::juce_recommended_config_flags
	juce::juce_recommended_lto_flags
	juce::juce_audio_basics
	juce::juce_audio_devices
	juce::juce_audio_formats
	juce::juce_audio_processors
	juce::juce_audio_utils
	juce::juce_core
	juce::juce_cryptography
	juce::juce_data_structures
	juce::juce_events
	juce::juce_graphics
	juce::juce_gui_basics
	juce::juce_gui_extra
	juce::juce_opengl
	)

install( TARGETS Apex EXPORT LibApex)
export(TARGETS Apex 
	juce_audio_basics
	juce_audio_devices
	juce_audio_formats
	juce_audio_processors
	juce_audio_utils
	juce_core
	juce_cryptography
	juce_data_structures
	juce_events
	juce_graphics
	juce_gui_basics
	juce_gui_extra
	juce_opengl
	juce_vst3_headers
	FILE ${CMAKE_BINARY_DIR}/LibApexConfig.cmake
	)
export(PACKAGE LibApex)
