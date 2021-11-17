#ifndef		_MFT_HPP_
# define	_MFT_HPP_

#include "mft/mat/mat3.hpp"
#include "mft/mat/mat4.hpp"
#include "mft/vec/vec3.hpp"

namespace	mft
{
	typedef mat<3 ,3, float>	mat3;
	typedef mat<4 ,4, float>	mat4;
	typedef vec<3, float>		vec3;
	typedef vec<4, float>		vec4;

	// Vertex utils

	float		rsqrt(float nb);

	template <int l, typename T>
	vec<l, T>	rsqrt(vec<l, T> const & v);

	template<int l, typename T>
	T	dot(vec<l, T> const& v1, vec<3, T> const& v2);

	template <int l, typename T>
	vec<l, T>	normalize(vec<l, T> const & v);

	// Transformation function

	template<typename T>
	mat<4, 4, T> translate(mat<4, 4, T> const& m, vec<3, T> const & v);

	template<typename T>
	mat<4, 4, T> scale(mat<4, 4, T> const& m, vec<3, T> const & v);

	template<typename T>
	mat<4, 4, T> rotate(mat<4, 4, T> const& m, vec<3, T> const & v);
	
	// Transformation function
	/*template<typename T>
	mat<4, 4, T> translate(mat<4, 4, T> const & m, vec<3, T> const & v)
	{
		mat<4, 4, T>	res(m);
		res[3][0] += v.x;
		res[3][1] += v.y;
		res[3][2] += v.z;
		return res;
	}

	template<typename T>
	mat<4, 4, T> scale(mat<4, 4, T> const & m, vec<3, T> const & v)
	{
		mat<4, 4, T>	res(m);
		res[0][0] *= v.x;
		res[1][1] *= v.y;
		res[2][2] *= v.z;
		return res;
	}*/
}

#endif
