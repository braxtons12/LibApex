cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(Apex VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#############################################################################
# Import GoogleTest
#############################################################################
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL)
#############################################################################
#############################################################################

#############################################################################
# Configure The Project
#############################################################################

find_package(JUCE CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)

add_library(Apex STATIC)

if(MSVC)
	target_compile_options(Apex PRIVATE /WX /W4 /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang")
	if(NOT WIN32)
		target_compile_options(Apex PRIVATE
			-std=c++20
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
			-Wno-suggest-destructor-override
			-Wno-suggest-override
			-Wno-deprecated-this-capture
			)
	else()
		target_compile_options(Apex PRIVATE
			-std=c++20
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
			-Wno-deprecated-this-capture
			)
	endif()
else()
	target_compile_options(Apex PRIVATE
		-std=c++20
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
		-Wno-deprecated-this-capture
		)
endif()

target_include_directories(Apex BEFORE INTERFACE
	#"${CMAKE_SOURCE_DIR}/../dependencies/GSL_CMAKE/include"
	"${CMAKE_SOURCE_DIR}/src/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/filters/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/expanders/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/meters/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/test"
	"${CMAKE_SOURCE_DIR}/src/math/test"
	"${CMAKE_SOURCE_DIR}/src/utils/test"
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
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReduction.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReductionFET.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReductionOpto.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/GainReductionVCA.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetector.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetectorRMS.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetector1176.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetectorSSL.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/LevelDetectorModernBus.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/GainComputer.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/GainComputerCompressor.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/GainComputerExpander.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/Sidechain.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/Sidechain1176.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/SidechainModernBus.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/SidechainSSL.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/filters/BiQuadFilter.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/filters/Dither.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages/GainStageFET.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages/GainStageVCA.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/meters/Meter.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/meters/RMSMeter.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/meters/PeakMeter.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/EQBand.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/ParallelEQBand.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/Gain.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/OverSampler.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/BaseCompressor.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/Compressor1176.h"
	"${CMAKE_SOURCE_DIR}/src/dsp/WaveShaper.h"
	)

set(MATH
	"${CMAKE_SOURCE_DIR}/src/math/General.h"
	"${CMAKE_SOURCE_DIR}/src/math/Decibels.h"
	"${CMAKE_SOURCE_DIR}/src/math/Frequency.h"
	"${CMAKE_SOURCE_DIR}/src/math/Exponentials.h"
	"${CMAKE_SOURCE_DIR}/src/math/TrigFuncs.h"
	"${CMAKE_SOURCE_DIR}/src/math/Random.h"
	)

set(UTILS
	"${CMAKE_SOURCE_DIR}/src/utils/ChangeDetector.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Concepts.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Error.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Interpolator.h"
	"${CMAKE_SOURCE_DIR}/src/utils/LockFreeQueue.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Logger.cpp"
	"${CMAKE_SOURCE_DIR}/src/utils/MiscMacros.h"
	"${CMAKE_SOURCE_DIR}/src/utils/OptionAndResult.h"
	"${CMAKE_SOURCE_DIR}/src/utils/RingBuffer.h"
	"${CMAKE_SOURCE_DIR}/src/utils/Span.h"
	"${CMAKE_SOURCE_DIR}/src/utils/TypeTraits.h"
	"${CMAKE_SOURCE_DIR}/src/utils/synchronization/ScopedLockGuard.h"
	"${CMAKE_SOURCE_DIR}/src/utils/synchronization/ReadWriteLock.h"
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

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang" OR APPLE)
	set_target_properties(Apex PROPERTIES CXX_CLANG_TIDY clang-tidy)
endif()

###############################################################################
###############################################################################

###############################################################################
# Configure Tests
###############################################################################

add_executable(ApexTest src/test/ApexTest.cpp)

if(MSVC)
	target_compile_options(ApexTest PRIVATE /WX /W4 /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "clang")
	if(NOT WIN32)
		target_compile_options(ApexTest PRIVATE
			-std=c++20
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
			-Wno-suggest-destructor-override
			-Wno-suggest-override
			-Wno-deprecated-this-capture
			-Wno-used-but-marked-unused
			)
	else()
		target_compile_options(ApexTest PRIVATE
			-std=c++20
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
			-Wno-deprecated-this-capture
			-Wno-used-but-marked-unused
			)
	endif()
else()
	target_compile_options(ApexTest PRIVATE
		-std=c++20
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
		-Wno-deprecated-this-capture
		-Wno-used-but-marked-unused
		)
endif()

target_include_directories(ApexTest BEFORE INTERFACE
	#"${CMAKE_SOURCE_DIR}/../dependencies/GSL_CMAKE/include"
	"${CMAKE_SOURCE_DIR}/src/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/filters/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/compressors/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/expanders/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gainreductions/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/leveldetectors/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/gaincomputers/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/dynamics/sidechains/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/gainstages/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/meters/test"
	"${CMAKE_SOURCE_DIR}/src/dsp/processors/test"
	"${CMAKE_SOURCE_DIR}/src/math/test"
	"${CMAKE_SOURCE_DIR}/src/utils/test"
	)

if(UNIX AND NOT APPLE)
	target_link_libraries(ApexTest PRIVATE
		curl
		juce::juce_dsp
		Microsoft.GSL::GSL
		gtest)
else()
	target_link_libraries(ApexTest PRIVATE
		juce::juce_dsp
		Microsoft.GSL::GSL
		gtest)
endif()
