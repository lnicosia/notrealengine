#include "mft/mft.hpp"

namespace mft
{
	template<typename T>
	mat<4, 4, T> translate(mat<4, 4, T> const& m, vec<3, T> const& v)
	{
		mat<4, 4, T>	res(m);
		res[3][0] += v.x;
		res[3][1] += v.y;
		res[3][2] += v.z;
		return res;
	}

	template<typename T>
	mat<4, 4, T> scale(mat<4, 4, T> const& m, vec<3, T> const& v)
	{
		mat<4, 4, T>	res(m);
		res[0][0] *= v.x;
		res[1][1] *= v.y;
		res[2][2] *= v.z;
		return res;
	}
}
