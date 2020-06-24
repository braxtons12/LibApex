#include "TrigFuncs.h"

//for explanations of these approximations, refer to:
//trigs:
//http://www.ganssle.com/approx/sincos.cpp
//tanh:
//https://www.musicdsp.org/en/latest/Other/238-rational-tanh-approximation.html

#include "Constants.h"
#include "General.h"

namespace apex {
	namespace math {

		inline float cos_helperf(float x) {
			const float c1 = 0.99999999999925182f;
			const float c2 = -0.49999999997024012f;
			const float c3 = 0.041666666473384543f;
			const float c4 = -0.001388888418000423f;
			const float c5 = 0.0000248010406484558f;
			const float c6 = -0.0000002752469638432f;
			const float c7 = 0.0000000019907856854f;

			float x2 = x * x;							// The input argument squared

			return (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * (c6 + c7 * x2))))));
		}

		inline float cosf(float angle) {
			angle = fmodf(angle, twoPif);				// Get rid of values > 2* pi
			if(angle < 0) angle = -angle;					// cos(-x) = cos(x)
			int quad = int(angle * twoOverPif);			// Get quadrant # (0 to 3) we're in
			switch(quad) {
				case 0: return  cos_helperf(angle);
				case 1: return -cos_helperf(pif - angle);
				case 2: return -cos_helperf(angle - pif);
				case 3: return  cos_helperf(twoPif - angle);
			}
			return 0.0f;
		}

		inline float sinf(float angle) {
			return cosf(piOver2f - angle);
		}

		inline float tan_helperf(float x) {
			const float c1 = -34287.4662577359568109624f;
			const float c2 =  2566.7175462315050423295f;
			const float c3 = -26.5366371951731325438f;
			const float c4 = -43656.1579281292375769579f;
			const float c5 = 12244.4839556747426927793f;
			const float c6 = -336.611376245464339493f;

			float x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

		inline float tanf(float angle) {
			angle = fmodf(angle, twoPif);				// Get rid of values >2 *pi
			int octant = int(angle / fourOverPif);			// Get octant # (0 to 7)
			switch(octant) {
				case 0: return  tan_helperf(angle * fourOverPif);
				case 1: return  1.0f / tan_helperf((piOver2f - angle) * fourOverPif);
				case 2: return -1.0f / tan_helperf((angle - piOver2f) * fourOverPif);
				case 3: return -tan_helperf((pif - angle) * fourOverPif);
				case 4: return  tan_helperf((angle - pif) * fourOverPif);
				case 5: return  1.0f / tan_helperf((threePiOver2f - angle) * fourOverPif);
				case 6: return -1.0f / tan_helperf((angle - threePiOver2f) * fourOverPif);
				case 7: return -tan_helperf((twoPif - angle) * fourOverPif);
			}
			return 0.0f;
		}

		const float tanPiOver6f = tanf(piOver6f);
		const float tanPiOver12f = tanf(piOver12f);

		inline float atan_helperf(float x) {
			const float c1 = 48.70107004404898384f;
			const float c2 = 49.5326263772254345f;
			const float c3 =  9.40604244231624f;
			const float c4 = 48.70107004404996166f;
			const float c5 = 65.7663163908956299f;
			const float c6 = 21.587934067020262f;

			float x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

		inline float atanf(float angle) {
			bool complement = false;				// true if arg was >1
			bool region = false;					// true depending on region arg is in
			bool sign = false;					// true if arg was < 0

			if (angle < 0.0f ){
				angle = -angle;
				sign = true;						// arctan(-x)=-arctan(x)
			}
			if (angle > 1.0f){
				angle = 1.0f / angle;						// keep arg between 0 and 1
				complement = true;
			}
			if (angle > tanPiOver12f){
				angle = (angle - tanPiOver6f) / (1.0f + tanPiOver6f * angle);	// reduce arg to under tan(pi/12)
				region = true;
			}

			float y = atan_helperf(angle);						// run the approximation
			if (region) y += piOver6f;				// correct for region we're in
			if (complement) y = piOver2f - y;			// correct for 1/x if we did that
			if (sign) y = -y;						// correct for negative arg
			return y;

		}

		inline float tanhf(float angle) {
			float angle2 = angle * angle;
			return angle * ( 27.0f + angle2) / (27.0f + 9.0f * angle2);
		}

		inline double cos_helper(double x) {
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

		inline double cos(double angle) {
			angle = fmod(angle, twoPi);				// Get rid of values > 2* pi
			if(angle < 0) angle = -angle;					// cos(-x) = cos(x)
			int quad = int(angle * twoOverPi);			// Get quadrant # (0 to 3) we're in
			switch(quad) {
				case 0: return  cos_helper(angle);
				case 1: return -cos_helper(pi - angle);
				case 2: return -cos_helper(angle - pi);
				case 3: return  cos_helper(twoPi - angle);
			}
			return 0.0;
		}

		inline double sin(double angle) {
			return cos(piOver2 - angle);
		}

		inline double tan_helper(double x) {
			const double c1 = -34287.4662577359568109624;
			const double c2 =  2566.7175462315050423295;
			const double c3 = -26.5366371951731325438;
			const double c4 = -43656.1579281292375769579;
			const double c5 = 12244.4839556747426927793;
			const double c6 = -336.611376245464339493;

			double x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

		inline double tan(double angle) {
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
			}
			return 0.0;
		}

		const double tanPiOver6 = tan(piOver6);
		const double tanPiOver12 = tan(piOver12);

		inline double atan_helper(double x) {
			const double c1 = 48.70107004404898384;
			const double c2 = 49.5326263772254345;
			const double c3 =  9.40604244231624;
			const double c4 = 48.70107004404996166;
			const double c5 = 65.7663163908956299;
			const double c6 = 21.587934067020262;

			double x2 = x * x;							// The input argument squared

			return (x * (c1 + x2 * (c2 + x2 * c3)) / (c4 + x2 * (c5 + x2 * (c6 + x2))));
		}

		inline double atan(double angle) {
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
			if (region) y += piOver6;				// correct for region we're in
			if (complement) y = piOver2 - y;			// correct for 1/x if we did that
			if (sign) y = -y;						// correct for negative arg
			return y;

		}

		inline double tanh(double angle) {
			double angle2 = angle * angle;
			return angle * ( 27.0 + angle2) / (27.0 + 9.0 * angle2);
		}
	}
}
