#ifndef _VERTEX_HPP_
# define _VERTEX_HPP_

#include "mft/mft.hpp"

struct Vertex
{
	mft::vec3	pos;
	mft::vec3	norm;
	mft::vec2	uv;
};

#endif