#include "mft/mft.hpp"
#include <cmath>

namespace mft
{
	template<typename T>
	mat<4, 4, T> ortho(T left, T right, T bottom, T top)
	{
		mat<4, 4, T>	res();

		res[0][0] = 2 / (right - left);
		res[3][0] = -(right + left) / (right - left);
		res[1][1] = 2 / (top - bottom);
		res[3][1] = -(top + bottom) / (top - bottom);
		res[3][3] = 1;
		return res;
	}

	template<typename T>
	mat<4, 4, T> ortho(T left, T right, T bottom, T top, T near, T far)
	{
		mat<4, 4, T>	res = mat<4, 4, T>(1);

		res[0][0] = 2.0f / (right - left);
		res[3][0] = -(right + left) / (right - left);
		res[1][1] = 2.0f / (top - bottom);
		res[3][1] = -(top + bottom) / (top - bottom);
		res[2][2] = -2.0f / (far - near);
		res[3][2] = -(far + near) / (far - near);
		res[3][3] = 1.0f;
		return res;
	}

	template<typename T>
	mat<4, 4, T> perspective(T fovy, T aspect, T near, T far)
	{
		T const	tang = tan(fovy * 0.5f);
		mat<4, 4, T>	res(0);

		res[0][0] = 1 / (aspect * tang);
		res[1][1] = 1 / tang;
		res[2][2] = -far / (far - near);
		res[2][3] = -1;
		res[3][2] = -(far * near) / (far - near);
		return res;
	}
}