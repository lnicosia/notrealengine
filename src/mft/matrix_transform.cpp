#include "mft/mft.hpp"
#include <cmath>

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

	template<typename T>
	mat<4, 4, T> rotate(mat<4, 4, T> const& m, float angle, vec<3, T> const& v)
	{
		float	c = cos(angle);
		float	s = sin(angle);
		float	minc = 1 - c;

		vec<3, T>	unit = normalize(v);
		mat<4, 4, T>	rot(
			c + unit.x * unit.x * minc, unit.x * unit.y * minc - unit.z * s, unit.x * unit.z * minc + unit.y * s, 0,
			unit.y * unit.x * minc + unit.z * s, c + unit.y * unit.y * minc, unit.y * unit.z * minc - unit.x * s, 0,
			unit.z * unit.x * minc - unit.y * s, unit.z * unit.y * minc - unit.x * s, c + unit.z * unit.z * minc, 0,
			0, 0, 0, 1
		);
		return (m * rot);
	}

	template<typename T>
	mat<4, 4, T> look_at(vec<3, T> const& pos, vec<3, T> const& target, vec<3, T> const& up)
	{
		vec<3, T>	right = normalize(cross(normalize(up), target));
		mat<4, 4, T>	res(
			right.x,	right.y,	right.z,	0,
			up.x,		up.y,		up.z,		0,
			target.x,	target.y,	target.z,	0,
			0, 0, 0, 1
		);
		return translate(res, pos);
	}
}
