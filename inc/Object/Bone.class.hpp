#ifndef _BONE_CLASS_H_
# define _BONE_CLASS_H_

#include "mft/mft.hpp"

#include <string>

namespace notrealengine
{
	class Bone
	{
	public:
		Bone();
		~Bone();

	private:
		int			id;
		std::string name;
		mft::mat4	model;
		mft::mat4	local;
	};

}

#endif