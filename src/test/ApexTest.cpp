#define TEST_HARNESS

#include "../dsp/test/WaveShaperTest.h"
#include "../math/test/DecibelsTest.h"
#include "../math/test/ExponentialsTestDouble.h"
#include "../math/test/ExponentialsTestFloat.h"
#include "../math/test/FrequencyTest.h"
#include "../math/test/GeneralTestDouble.h"
#include "../math/test/GeneralTestFloat.h"
#include "../math/test/TrigFuncsTestDouble.h"
#include "../math/test/TrigFuncsTestFloat.h"
#include "../utils/test/OptionTest.h"
#include "../utils/test/ResultTest.h"
#include "../utils/test/ChangeDetectorTest.h"
#include "gtest/gtest.h"

auto main(int argc, char** argv) -> int {
	testing::InitGoogleTest(&argc, argv);
	testing::FLAGS_gtest_death_test_style = "fast";
	return RUN_ALL_TESTS();
}
