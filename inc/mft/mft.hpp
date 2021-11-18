#ifndef	_MFT_HPP_
# define _MFT_HPP_

#include "mft/mat/mat3.hpp"
#include "mft/mat/mat4.hpp"
#include "mft/vec/vec3.hpp"

namespace	mft
{
	typedef mat<3 ,3, float>	mat3;
	typedef mat<4 ,4, float>	mat4;
	typedef vec<3, float>		vec3;
	typedef vec<4, float>		vec4;

	// Math utils

	float		rsqrt(float nb);
	template <int l, typename T>
	vec<l, T>	rsqrt(vec<l, T> const& v);

	float		radians(float degrees);
	float		degrees(float radians);

	// Vertex utils

	template<typename T>
	vec<3, T>	cross(vec<3, T> const& v1, vec<3, T> const& v2);

	template<int l, typename T>
	T	dot(vec<l, T> const& v1, vec<3, T> const& v2);

	template <int l, typename T>
	vec<l, T>	normalize(vec<l, T> const & v);

	// Transformation functions

	template<typename T>
	mat<4, 4, T> translate(mat<4, 4, T> const& m, vec<3, T> const & v);

	template<typename T>
	mat<4, 4, T> scale(mat<4, 4, T> const& m, vec<3, T> const & v);

	template<typename T>
	mat<4, 4, T> rotate(mat<4, 4, T> const& m, float angle, vec<3, T> const & v);

	template<typename T>
	mat<4, 4, T> look_at(vec<3, T> const& pos, vec<3, T> const& target, vec<3, T> const& up);

	// Projection functions

	template<typename T>
	mat<4, 4, T> ortho(T left, T right, T bottom, T top);

	template<typename T>
	mat<4, 4, T> ortho(T left, T right, T bottom, T top, T near, T far);

	template<typename T>
	mat<4, 4, T> perspective(T fovy, T aspect, T near, T far);
}

#endif
