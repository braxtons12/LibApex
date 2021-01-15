#pragma once

namespace apex::test {
	// this is worst case. average case is ~3 orders of magnitude better, or more
	static const constexpr double DOUBLE_ACCEPTED_ERROR = 0.0005;
	static const constexpr double FLOAT_ACCEPTED_ERROR = 0.0005;
} // namespace apex::test
