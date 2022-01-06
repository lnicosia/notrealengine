#include "mft/math_utils.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

namespace mft
{
	float		radians(float degrees) {
		return (degrees * (M_PI / 180.));
	}
	float		degrees(float radians) {
		return (radians * (180. / M_PI));
	}

	float	rsqrt(float nb)
	{
		unsigned int		i;
		float		x2, y;

		x2 = nb * 0.5f;
		unsigned int* p1 = reinterpret_cast<unsigned int*>(&nb);
		i = 0x5f3759df - (*p1 >> 1);
		float* p2 = reinterpret_cast<float*>(&i);
		float tmp = *p2;
		y = tmp * (1.5f - (x2 * tmp * tmp));
		return y;
	}
}
