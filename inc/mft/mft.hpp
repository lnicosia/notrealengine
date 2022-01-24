#ifndef	_MFT_H_
# define _MFT_H_

# include "mft/math_utils.hpp"
# include "mft/templates/mat.template.hpp"
# include "mft/templates/vec.template.hpp"

namespace	mft
{
	using mat2 = mat<float, float>;
	using mat3 = mat<float, float, float>;
	using mat4 = mat<float, float, float, float>;

	using vec2 = vec<float, float>;
	using vec3 = vec<float, float, float>;
	using vec4 = vec<float, float, float, float>;

	using vec2i = vec<int, int>;
} // namespace mft

#endif
