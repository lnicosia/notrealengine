#ifndef	_MFT_H_
# define _MFT_H_

# include "mft/math_utils.hpp"
# include "mft/templates/mat.template.hpp"
# include "mft/templates/vec.template.hpp"

namespace	mft
{
	typedef mat<float,float>				mat2;
	typedef mat<float,float,float>			mat3;
	typedef mat<float,float,float,float>	mat4;

	typedef vec<float,float>				vec2;
	typedef vec<float,float,float>			vec3;
	typedef vec<float,float,float,float>	vec4;

	typedef vec<int,int>					vec2i;
}

#endif
