cmake_minimum_required(VERSION 3.15)

project(Apex VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_package(JUCE CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)

add_library(Apex STATIC)

if(MSVC)
	target_compile_options(Apex PRIVATE /WX /W4 /std:c++17)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang")
	if(NOT WIN32)
		target_compile_options(Apex PRIVATE
			-std=c++17
			-Wall
			-Wextra
			-Wpedantic
			-Weverything
			-Werror
			#Everything below this is to allow warnings that come from the JUCE library's own code
			-Wno-undef
			-Wno-c++98-compat-pedantic
			-Wno-documentation-unknown-command
			-Wno-documentation-deprecated-sync
			-Wno-old-style-cast
			-Wno-packed
			-Wno-padded
			-Wno-documentation
			-Wno-global-constructors
			-Wno-double-promotion
			-Wno-extra-semi-stmt
			-Wno-covered-switch-default
			-Wno-cast-qual
			-Wno-gnu-zero-variadic-macro-arguments
			-Wno-float-equal
			-Wno-implicit-int-float-conversion
			-Wno-weak-vtables
			-Wno-unused-template
			-Wno-unused-macros
			-Wno-exit-time-destructors
			-Wno-deprecated-copy-dtor
			-Wno-alloca
			-Wno-missing-variable-declarations
			-Wno-weak-template-vtables
			-Wno-reserved-id-macro
			-Wno-missing-noreturn
			-Wno-missing-prototypes
			-Wno-undefined-reinterpret-cast
			-Wno-misleading-indentation
			-Wno-range-loop-construct
			-Wno-implicit-fallthrough
			-Wno-date-time
			-Wno-disabled-macro-expansion
			)
	else()
		target_compile_options(Apex PRIVATE
			-std=c++17
			-Wall
			-Wextra
			-Wpedantic
			-Weverything
			-Werror
			#Everything below this is to allow warnings that come from the JUCE library's own code
			-Wno-undef
			-Wno-c++98-compat-pedantic
			-Wno-documentation-unknown-command
			-Wno-documentation-deprecated-sync
			-Wno-old-style-cast
			-Wno-packed
			-Wno-padded
			-Wno-documentation
			-Wno-global-constructors
			-Wno-double-promotion
			-Wno-extra-semi-stmt
			-Wno-covered-switch-default
			-Wno-cast-qual
			-Wno-gnu-zero-variadic-macro-arguments
			-Wno-float-equal
			-Wno-implicit-int-float-conversion
			-Wno-weak-vtables
			-Wno-unused-template
			-Wno-unused-macros
			-Wno-exit-time-destructors
			-Wno-deprecated-copy-dtor
			-Wno-alloca
			-Wno-missing-variable-declarations
			-Wno-weak-template-vtables
			-Wno-reserved-id-macro
			-Wno-missing-noreturn
			-Wno-missing-prototypes
			-Wno-undefined-reinterpret-cast
			-Wno-misleading-indentation
			-Wno-range-loop-construct
			-Wno-implicit-fallthrough
			-Wno-date-time
			-Wno-disabled-macro-expansion
			-Wno-inconsistent-missing-destructor-override
			-Wno-suggest-destructor-override
			-Wno-nonportable-system-include-path
			-Wno-language-extension-token
			-Wno-suggest-override
			-Wno-non-virtual-dtor
			-Wno-zero-as-null-pointer-constant
			-Wno-four-char-constants
			-Wno-cast-align
			-Wno-sign-conversion
			-Wno-shadow-field-in-constructor
			-Wno-shift-sign-overflow
			-Wno-nested-anon-types
			-Wno-shorten-64-to-32
			)
	endif()
else()
	target_compile_options(Apex PRIVATE
		-std=c++17
		-Wall
		-Wextra
		-Wpedantic
		-Werror
		#Everything below this is to allow warnings that come from the JUCE library's own code
		-Wno-undef
		-Wno-old-style-cast
		-Wno-packed
		-Wno-padded
		-Wno-cast-qual
		-Wno-float-equal
		-Wno-unused-macros
		-Wno-deprecated-copy-dtor
		-Wno-alloca
		-Wno-missing-noreturn
		-Wno-misleading-indentation
		-Wno-implicit-fallthrough
		-Wno-date-time
		-Wno-ignored-qualifiers
		)
endif()

target_include_directories(Apex INTERFACE
	"${CMAKE_SOURCE_DIR}/../dependencies/GSL_CMAKE/include"
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/src/base"
	"${CMAKE_SOURCE_DIR}/src/ui"
	"${CMAKE_SOURCE_DIR}/src/dsp"
	"${CMAKE_SOURCE_DIR}/src/dsp/filters"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/expanders"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors"
	"${CMAKE_SOURCE_DIR}/src/math"
	"${CMAKE_SOURCE_DIR}/src/utils"
	)

set(BASE
	"${CMAKE_SOURCE_DIR}/src/base/StandardIncludes.h"
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
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/DynamicsState.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors/Compressor.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors/CompressorFET.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors/CompressorOpto.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors/CompressorVCA.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/expanders/Expander.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/expanders/ExpanderVCA.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReduction.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReductionFET.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReductionOpto.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReductionVCA.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetector.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetectorRMS.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetector1176.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetectorSSL.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetectorModernBus.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/GainComputer.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/GainComputerCompressor.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/GainComputerExpander.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/Sidechain.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/Sidechain1176.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/SidechainModernBus.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/SidechainSSL.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/filters/BiQuadFilter.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages/GainStageFET.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages/GainStageVCA.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/MultiChannelProcessor.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/EQBand.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/ParallelEQBand.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/Gain.cpp"
	"${CMAKE_SOURCE_DIR}/src/dsp/WaveShaper.cpp"
	)

set(MATH
	"${CMAKE_SOURCE_DIR}/src/math/General.h"
	"${CMAKE_SOURCE_DIR}/src/math/Decibels.h"
	"${CMAKE_SOURCE_DIR}/src/math/Exponentials.h"
	"${CMAKE_SOURCE_DIR}/src/math/TrigFuncs.h"
	"${CMAKE_SOURCE_DIR}/src/math/Random.cpp"
	)

set(UTILS
	"${CMAKE_SOURCE_DIR}/src/utils/Option.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Result.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Error.cpp"
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
	#juce::juce_recommended_warning_flags
	#juce::juce_recommended_config_flags
	#juce::juce_recommended_lto_flags
	juce::juce_gui_basics
	juce::juce_gui_extra
	juce::juce_dsp
	Microsoft.GSL::GSL
	)
