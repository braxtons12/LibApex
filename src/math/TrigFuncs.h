#pragma once

#include "Constants.h"
#include "General.h"

namespace apex::math {
	/// @brief Helper function for `cosf`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]]
		constexpr inline auto cos_helperf(float x) noexcept -> float {
			const float c1 = 0.99999999999925182F;
			const float c2 = -0.49999999997024012F;
			const float c3 = 0.041666666473384543F;
			const float c4 = -0.001388888418000423F;
			const float c5 = 0.0000248010406484558F;
			const float c6 = -0.0000002752469638432F;
			const float c7 = 0.0000000019907856854F;

			float x2 = x * x;							// The input argument squared

			return (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * (c6 + c7 * x2))))));
		}

	/// @brief Fast approximation calculation of cosine(angle)
	///
	/// @param angle - The angle to calculate the cosine of
	/// @return - The cosine of the angle
	[[nodiscard]]
		constexpr inline auto cosf(float angle) noexcept -> float {
			angle = fmodf(angle, twoPif);				// Get rid of values > 2* pi
			if(angle < 0.0F) {
				angle = -angle;					// cos(-x) = cos(x)
			}
			int quad = int(angle * twoOverPif);			// Get quadrant # (0 to 3) we're in
			switch(quad) {
				case 0: return  cos_helperf(angle);
				case 1: return -cos_helperf(pif - angle);
				case 2: return -cos_helperf(angle - pif);
				case 3: return  cos_helperf(twoPif - angle);
				default: return 0.0F;
			}
		}

	/// @brief Fast approximation calculation of sine(angle)
	///
	/// @param angle - The angle to calculate the sine of
	/// @return - The sine of the angle
	[[nodiscard]]
		constexpr inline auto sinf(float angle) noexcept -> float {
			return cosf(piOver2f - angle);
		}

	/// @brief Helper function for `tanf`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]]
		constexpr inline auto tan_helperf(float x) noexcept -> float {
			const float c1 = -34287.4662577359568109624F;
			const float c2 =  2566.7175462315050423295F;
			const float c3 = -26.5366371951731325438F;
			const float c4 = -43656.1579281292375769579F;
			const float c5 = 12244.4839556747426927793F;
			const float c6 = -336.611376245464339493F;

			float x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

	/// @brief Fast approximation calculation of tangent(angle)
	///
	/// @param angle - The angle to calculate the tangent of
	/// @return - The tangent of the angle
	[[nodiscard]]
		constexpr inline auto tanf(float angle) noexcept -> float {
			angle = fmodf(angle, twoPif);				// Get rid of values >2 *pi
			int octant = int(angle / fourOverPif);			// Get octant # (0 to 7)
			switch(octant) {
				case 0: return  tan_helperf(angle * fourOverPif);
				case 1: return  1.0F / tan_helperf((piOver2f - angle) * fourOverPif);
				case 2: return -1.0F / tan_helperf((angle - piOver2f) * fourOverPif);
				case 3: return -tan_helperf((pif - angle) * fourOverPif);
				case 4: return  tan_helperf((angle - pif) * fourOverPif);
				case 5: return  1.0F / tan_helperf((threePiOver2f - angle) * fourOverPif);
				case 6: return -1.0F / tan_helperf((angle - threePiOver2f) * fourOverPif);
				case 7: return -tan_helperf((twoPif - angle) * fourOverPif);
				default: return 0.0F;
			}
		}

	///tangent(pi / 6)
	static constexpr const float tanPiOver6f = tanf(piOver6f);
	///tangent(pi / 12)
	static constexpr const float tanPiOver12f = tanf(piOver12f);

	/// @brief Helper function for `atanf`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]]
		constexpr inline auto atan_helperf(float x) noexcept -> float {
			const float c1 = 48.70107004404898384F;
			const float c2 = 49.5326263772254345F;
			const float c3 =  9.40604244231624F;
			const float c4 = 48.70107004404996166F;
			const float c5 = 65.7663163908956299F;
			const float c6 = 21.587934067020262F;

			float x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

	/// @brief Fast approximation calculation of arctangent(angle)
	///
	/// @param angle - The angle to calculate the arctangent of
	/// @return - The arctangent of the angle
	[[nodiscard]]
		constexpr inline auto atanf(float angle) noexcept -> float {
			bool complement = false;				// true if arg was >1
			bool region = false;					// true depending on region arg is in
			bool sign = false;					// true if arg was < 0

			if (angle < 0.0F ){
				angle = -angle;
				sign = true;						// arctan(-x)=-arctan(x)
			}
			if (angle > 1.0F){
				angle = 1.0F / angle;						// keep arg between 0 and 1
				complement = true;
			}
			if (angle > tanPiOver12f){
				angle = (angle - tanPiOver6f) / (1.0F + tanPiOver6f * angle);	// reduce arg to under tan(pi/12)
				region = true;
			}

			float y = atan_helperf(angle);						// run the approximation
			if (region) {
				y += piOver6f;				// correct for region we're in
			}
			if (complement) {
				y = piOver2f - y;			// correct for 1/x if we did that
			}
			if (sign) {
				y = -y;						// correct for negative arg
			}
			return y;

		}

	/// @brief Fast approximation calculation of the hyperbolic tangent ("tanch") of the angle
	///
	/// @param angle - The angle to calculate the hyperbolic tanget of
	/// @return - The hyperbolic tangent of the angle
	[[nodiscard]]
		constexpr inline auto tanhf(float angle) noexcept -> float {
			float angle2 = angle * angle;
			return angle * ( 27.0F + angle2) / (27.0F + 9.0F * angle2);
		}

	/// @brief Helper function for `cos`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]]
		constexpr inline auto cos_helper(double x) noexcept -> double {
			const double c1 = 0.99999999999925182;
			const double c2 = -0.49999999997024012;
			const double c3 = 0.041666666473384543;
			const double c4 = -0.001388888418000423;
			const double c5 = 0.0000248010406484558;
			const double c6 = -0.0000002752469638432;
			const double c7 = 0.0000000019907856854;

			double x2 = x * x;							// The input argument squared

			return (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * (c6 + c7 * x2))))));
		}

	/// @brief Fast approximation calculation of cosine(angle)
	///
	/// @param angle - The angle to calculate the cosine of
	/// @return - The cosine of the angle
	[[nodiscard]]
		constexpr inline auto cos(double angle) noexcept -> double {
			angle = fmod(angle, twoPi);				// Get rid of values > 2* pi
			if(angle < 0.0) {
				angle = -angle;					// cos(-x) = cos(x)
			}
			int quad = int(angle * twoOverPi);			// Get quadrant # (0 to 3) we're in
			switch(quad) {
				case 0: return  cos_helper(angle);
				case 1: return -cos_helper(pi - angle);
				case 2: return -cos_helper(angle - pi);
				case 3: return  cos_helper(twoPi - angle);
				default: return 0.0;
			}
		}

	/// @brief Fast approximation calculation of sine(angle)
	///
	/// @param angle - The angle to calculate the sine of
	/// @return - The sine of the angle
	[[nodiscard]]
		constexpr inline auto sin(double angle) noexcept -> double {
			return cos(piOver2 - angle);
		}

	/// @brief Helper function for `tan`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]]
		constexpr inline auto tan_helper(double x) noexcept -> double {
			const double c1 = -34287.4662577359568109624;
			const double c2 =  2566.7175462315050423295;
			const double c3 = -26.5366371951731325438;
			const double c4 = -43656.1579281292375769579;
			const double c5 = 12244.4839556747426927793;
			const double c6 = -336.611376245464339493;

			double x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

	/// @brief Fast approximation calculation of tangent(angle)
	///
	/// @param angle - The angle to calculate the tangent of
	/// @return - The tangent of the angle
	[[nodiscard]]
		constexpr inline auto tan(double angle) noexcept -> double {
			angle = fmod(angle, twoPi);				// Get rid of values >2 *pi
			int octant = int(angle / fourOverPi);			// Get octant # (0 to 7)
			switch(octant) {
				case 0: return  tan_helper(angle * fourOverPi);
				case 1: return  1.0 / tan_helper((piOver2 - angle) * fourOverPi);
				case 2: return -1.0 / tan_helper((angle - piOver2) * fourOverPi);
				case 3: return -tan_helper((pi - angle) * fourOverPi);
				case 4: return  tan_helper((angle - pi) * fourOverPi);
				case 5: return  1.0 / tan_helper((threePiOver2 - angle) * fourOverPi);
				case 6: return -1.0 / tan_helper((angle - threePiOver2) * fourOverPi);
				case 7: return -tan_helper((twoPi - angle) * fourOverPi);
				default: return 0.0;
			}
		}

	///tangent(pi / 6)
	static const constexpr double tanPiOver6 = tan(piOver6);
	///tangent(pi / 12)
	static const constexpr double tanPiOver12 = tan(piOver12);

	/// @brief Helper function for `atanf`; Don't use on its own
	///
	/// @param x
	/// @return
	[[nodiscard]]
		constexpr inline auto atan_helper(double x) noexcept -> double {
			const double c1 = 48.70107004404898384;
			const double c2 = 49.5326263772254345;
			const double c3 =  9.40604244231624;
			const double c4 = 48.70107004404996166;
			const double c5 = 65.7663163908956299;
			const double c6 = 21.587934067020262;

			double x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

	/// @brief Fast approximation calculation of arctangent(angle)
	///
	/// @param angle - The angle to calculate the arctangent of
	/// @return - The arctangent of the angle
	[[nodiscard]]
		constexpr inline auto atan(double angle) noexcept -> double {
			bool complement = false;				// true if arg was >1
			bool region = false;					// true depending on region arg is in
			bool sign = false;					// true if arg was < 0

			if (angle < 0.0 ){
				angle = -angle;
				sign = true;						// arctan(-x)=-arctan(x)
			}
			if (angle > 1.0){
				angle = 1.0 / angle;						// keep arg between 0 and 1
				complement = true;
			}
			if (angle > tanPiOver12){
				angle = (angle - tanPiOver6) / (1.0 + tanPiOver6 * angle);	// reduce arg to under tan(pi/12)
				region = true;
			}

			double y = atan_helper(angle);						// run the approximation
			if (region) {
				y += piOver6;				// correct for region we're in
			}
			if (complement) {
				y = piOver2 - y;			// correct for 1/x if we did that
			}
			if (sign) {
				y = -y;						// correct for negative arg
			}
			return y;

		}

	/// @brief Fast approximation calculation of the hyperbolic tangent ("tanch") of the angle
	///
	/// @param angle - The angle to calculate the hyperbolic tanget of
	/// @return - The hyperbolic tangent of the angle
	[[nodiscard]]
		constexpr inline auto tanh(double angle) noexcept -> double {
			double angle2 = angle * angle;
			return angle * ( 27.0 + angle2) / (27.0 + 9.0 * angle2);
		}
} //namespace apex::math
