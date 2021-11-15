#ifndef _VEC3_H_
# define _VEC3_H_

#include "mft/vec.hpp"

namespace	mft
{
	template <typename T>
	struct	vec<4, T>
	{
		T	x;
		T	y;
		T	z;
		T	w;

		vec(): x(0), y(0), z(0), w(0)
		{
		}

		vec(vec<3, T> const & v): x(v.x), y(v.y), z(v.z), w(v.w)
		{
		}

		vec(T x, T y, T z): x(x), y(y), z(z), w(w)
		{
		}

		vec<4, T>	operator*(T scalar)
		{
			vec<4, T> ret = vec<4, T>(x * scalar, y * scalar, z * scalar, w * scalar);
			return ret;
		}

		int length()
		{
			return 3;
		}
	};


	template <typename T>
	std::ostream &		operator<<(std::ostream & o, vec<3, T> const & v)
	{
		std::cout << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
		return o;
	}
}

#endif
